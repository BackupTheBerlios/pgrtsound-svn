#include "algorithm.h"

extern NullModuleSingleton& nullModule;

/** Konstruktor.
 * Utworznie obiektu algorytmu wymaga podania dlugosci bloku probek (przetwarznie
 * odbywa sie blokowo).
 * @param fpb Dlugosc bloku probek (frames per block).
 */
Algorithm::Algorithm(unsigned long fpb) {
	framesPerBlock = fpb;
	Module::framesPerBlock = fpb;
	name = "New algorithm";
	InitAudioPorts();
}

Algorithm::~Algorithm() {
	TRACE3("Algorithm", "Sprzatanie algorytmu (", GetModuleCount(), " modulow)...");

	ModuleIdIterator mit;
	for(mit = ModuleIdIteratorBegin(); mit != ModuleIdIteratorEnd(); mit++) {
		delete GetModule(*mit);
	}

	TRACE("Algorithm", "Bye!");
}

void Algorithm::InitAudioPorts() {
   	Module* module;
	ModuleId moduleId;
	
	module = new AudioPortIn;
	moduleId = add_vertex(graph);   // dodajemy do grafu
	graph[moduleId].module = module;
	//put(boost::vertex_name, graph, moduleId, module);
	moduleName2IdMap.insert( make_pair("AudioPortIn", moduleId) );
	inputPort = module;

	module = new AudioPortOut;
	moduleId = add_vertex(graph);
	graph[moduleId].module = module;
	//put(boost::vertex_name, graph, moduleId, module);
	moduleName2IdMap.insert( make_pair("AudioPortOut", moduleId) );
	outputPort = module;

	TRACE("Algorithm::InitAudioPorts()", "Dodano moduly AudioPortIn i AudioPortOut");
}

/** Dodanie modulu.
 * @param type Rozpoznawany prez system typ modulu.
 * @param name Oczekwiana nazwa modulu.
 */
ModuleId Algorithm::AddModule(string type, string name) {
	Module* mod = moduleFactory.CreateModule(type);
	mod->SetName(name);
	ModuleId moduleId;
	moduleId = add_vertex(graph);
	//graph[*gm] = *(moduleFactory.CreateModule(type));
	graph[moduleId].module = mod;
	//put(boost::vertex_name, graph, moduleId, mod);

	moduleName2IdMap.insert( make_pair(name, moduleId) );
	TRACE4("Algorithm", "Dodano modul '", mod->GetName(), "' typu ", mod->GetType());
	return moduleId;
}

void Algorithm::PrintInfo(void) {
	cout << "Algorithm::PrintInfo(): Informacje o algorytmie... " << endl;

	ModuleIdIterator mit;
	for(mit = ModuleIdIteratorBegin(); mit != ModuleIdIteratorEnd(); mit++) {
		cout << "    Modul: " << GetModule(*mit)->GetName() <<
			"(" << GetModule(*mit)->GetType() << ")" << endl;
	}
}

/**
 * Utworzenie polaczenia miedzy modulami. Tworzy polaczenie miedzy wybranym
 * wyjsciem i wejsciem dwoch roznych modulow.
 * @param moduleId1 Identyfikator modulu zrodlowego
 * @param outputId Identyfiaktor wyjscia modulu zrodlowego
 * @param moduleId2 Identyfikator modulu docelowego
 * @param inputId Identyfiaktor wejscia modulu docelowego
 */
ConnectionId Algorithm::ConnectModules(ModuleId moduleId1, int outputId,
	ModuleId moduleId2, int inputId)
{
	#ifndef NDEBUG
		cout << "    Lacze '" <<
			GetModule(moduleId1)->GetName() << "'." << GetModule(moduleId1)->GetOutput(outputId)->GetName() << " -> '" <<
			GetModule(moduleId2)->GetName() << "'." << GetModule(moduleId2)->GetInput(inputId)->GetName() << endl;
	#endif

	// polacz wejscie do wyjscia
	GetModule(moduleId2)->GetInput(inputId)->ConnectTo(
		GetModule(moduleId1)->GetOutput(outputId) );

	ConnectionDescription c;
	ConnectionId cId;
	// utworzenie polaczenia w grafie
	c = add_edge(moduleId1, moduleId2, graph);
	// identyfikator polaczenia w grafie
	cId = c.first;
	graph[cId].sourceModule = GetModule(moduleId1);
	graph[cId].sourceOutputId = outputId;
	graph[cId].destinationModule = GetModule(moduleId2);
	graph[cId].destinationInputId = inputId;

//	graph[cId].sourceId = outputId;
//	graph[cId].destinationId = inputId;
//	graph[cId].source = GetModule(moduleId1)->GetOutput(outputId);
//	graph[cId].destination = GetModule(moduleId2)->GetInput(inputId);

	return cId;
}

/**
 * Utworzenie polaczenia miedzy modulami. Tworzy polaczenie miedzy wybranym
 * wyjsciem i wejsciem dwoch roznych modulow o podanych nazwach.
 * @param moduleName1 Nazwa modulu zrodlowego
 * @param outputId Identyfiaktor wyjscia modulu zrodlowego
 * @param moduleName2 Nazwa modulu docelowego
 * @param inputId Identyfiaktor wejscia modulu docelowego
*/
ConnectionId Algorithm::ConnectModules(string moduleName1, int outputId,
	string moduleName2, int inputId)
{
	ModuleId moduleId1 = ( *moduleName2IdMap.find(moduleName1) ).second;
	ModuleId moduleId2 = ( *moduleName2IdMap.find(moduleName2) ).second;

	return ConnectModules(moduleId1, outputId, moduleId2, inputId);
}

/**
 * Ustawienie czestotliwosci probkowania.
 */
void Algorithm::SetSampleRate(float sRate) {
	sampleRate = sRate;
	Module::sampleRate = sRate; // wartos widoczna we wszystkich modulach

	TRACE2("Algorithm", "Module::sampleRate = ", Module::sampleRate);
}

/**
 * Tworzy kolejke modulow dla przetwarznia. Struktura grafu algorytmu zostaje
 * przetworzona na odpowiadajaca mu szeregowa sekwencje modulow, zgodnie z ktora
 * wywolane zostac musza ich funkcje przetwarzania aby zachowac zaleznosci
 * wejscie/wyjscie medzy modulami w grafie.
 */
void Algorithm::CreateQueue() {
	TRACE("Algorithm", "Tworzenie kolejki modulow...");

	using namespace boost;

	typedef vector< ModuleId > container;
	container c;
	property_map<Graph, vertex_index_t>::type index = get(vertex_index, graph);
//	property_map<Graph, vertex_name_t>::type name = get(vertex_name, graph);
	property_map<Graph, Module* GraphModule::*>::type mods = get(&GraphModule::module, graph);

	// initialize the vertex_index property values
	graph_traits<Graph>::vertex_iterator vi, vend;
	graph_traits<Graph>::vertices_size_type cnt = 0;
	for(tie(vi,vend) = vertices(graph); vi != vend; ++vi)
	    put(index, *vi, cnt++);

	modulesQueue.clear();
	topological_sort(graph, std::back_inserter(c));

	std::cout << "A topological ordering: ";
	for (container::reverse_iterator ii = c.rbegin(); ii != c.rend(); ++ii) {
//        cout << "'" << name[*ii]->GetName() << "' ";
        cout << "'" << mods[*ii]->GetName() << "' ";
        modulesQueue.push_back(mods[*ii]);
	}
    cout << endl;
}

/**
 * Zwraca ilosc modulow w grafie algorytmu.
 */
int Algorithm::GetModuleCount() const {
	return num_vertices(graph);
}

int Algorithm::GetConnectionCount() {
	return num_edges(graph);
}

/**
 * Czysczenie algorytmu. Powoduje wyczyszczenie wszystkich elementow
 * przechowujacych strukture algorytmu. Po wycyzsczeniu algorytm nie spelnia
 * zadnej funckji przetwarznia, ale gotowy jest do konfiguracji.
 */
void  Algorithm::Clear() {
	TRACE("Algorithm::Clear()", "Czyszcze algorytm...");

	// usuniecie modulow
	ModuleIdIterator mit;
	for(mit = ModuleIdIteratorBegin(); mit != ModuleIdIteratorEnd(); mit++) {
		delete GetModule(*mit);
	}

	modulesQueue.clear();
	graph.clear();
	moduleName2IdMap.clear();
	InitAudioPorts();
	
	TRACE("Algorithm::Clear()", "Algorytm wyczyszczony");
}

/**
 * Inicjalizacjia algorytmu. Wywoluje funckje inicjalizujace wszytkich modulow
 * w grafie.
 */
void Algorithm::Init() {
    TRACE("Algorithm::Init()", "Inicjalizacja modulow...");
    
	ModuleIdIterator mit;
	for(mit = ModuleIdIteratorBegin(); mit != ModuleIdIteratorEnd(); mit++) {
		GetModule(*mit)->Init();
	}

    TRACE("Algorithm::Init()", "Inicjalizacja modulow zakonczona");
}

/**
 * Dostep do modulu. Zwraca wskaznik do modulu o podanej nazwie.
 * @param moduleName Nazwa dociekanego modulu
 */
Module* Algorithm::GetModule(string moduleName) const {
    if (moduleName2IdMap.find(moduleName) != moduleName2IdMap.end()) {
        //TRACE("Algorithm::GetModule(string moduleName)", "Znaleziono");
    	ModuleId moduleId = ( *moduleName2IdMap.find(moduleName) ).second;
		return GetModule(moduleId);
    } else {
        //TRACE("Algorithm::GetModule(string moduleName)", "Nie znaleziono");
        return NULL;   
    }
}

/**
 * Zwraca identyfikator modulu o podanej nazwie.
 * @param moduleName Nazwa dociekanego modulu
 */
ModuleId Algorithm::GetModuleId(string moduleName) const {
    //poprawiono b³¹d gdy modu³u nie by³o :)
    if (moduleName2IdMap.find(moduleName) != moduleName2IdMap.end()) {
        //TRACE("Algorithm::GetModuleId(string moduleName)", "Znaleziono");
    	ModuleId moduleId = ( *moduleName2IdMap.find(moduleName) ).second;
		return moduleId;
    } else {
        //TRACE("Algorithm::GetModuleId(string moduleName)", "Nie znaleziono");
        return NULL;   
    }    
}

/**
 * Dostep do modulu. Zwraca wskaznik do modulu o zadanym identyfikatorze.
 * @param moduleId Identyfikator modulu
 */
Module* Algorithm::GetModule(ModuleId moduleId) const {
//	return boost::get(boost::vertex_name, graph, moduleId);
    return graph[moduleId].module;
}

Module* Algorithm::GetInputPort() const {
	return inputPort;
}

Module* Algorithm::GetOutputPort() const {
	return outputPort;
}

//ConnectionId  Algorithm::GetFirstConnectionId() {
//	boost::tie(connectionIterator, connectionIteratorLat) = boost::edges(graph);
//    return *connectionIterator;
//}
//
//ConnectionId  Algorithm::GetNextConnectionId() {
//	if(++moduleIterator != moduleIteratorLast)
//		//return boost::get(boost::vertex_name, graph, *moduleIterator);
//	    return graph[*moduleIterator].module;
//	else
//		return --moduleIterator;
//}

/**
 * Usuwanie modulu z grafu.
 * @param moduleId Identyfikator modulu
*/
void Algorithm::DeleteModule(ModuleId moduleId) {
    //usuwanie z mapy
	moduleName2IdMap.erase(GetModule(moduleId)->GetName());
	//usuwanie z pamieci modulu
	delete GetModule(moduleId);
	//usuwanie grafu
	boost::clear_vertex(moduleId, graph);
}

/**
 Usuwanie polaczenia miedzy modulami.
 @param connectionId identyfikator polaczenia do usuniecia
*/
void Algorithm::DeleteConnection(ConnectionId connectionId) {
	//graph[connectionId].sink->SetSignal(nullBuffer); // rozlaczamy wejscie modulu 2 ???
	//graph[connectionId].destinationModule->GetInput( graph[connectionId].destinationInputId )
	//	->ConnectTo( nullModule.GetOutput(0) ); // rozlaczamy wejscie modulu 2 ???
	graph[connectionId].destinationModule->GetInput( graph[connectionId].destinationInputId )
		->Disconnect(); // rozlaczamy wejscie modulu 2 ???
	boost::remove_edge(connectionId, graph);
}

/**
 Ustawienie nazwy algorytmu.
 Nazwa w zamysle ma w zwiezly sposob opisywac dzialanie algorytmu.
*/
void Algorithm::SetName(string newName) {
	name = newName;
}

/**
 Zwraca nazwe algorytmu.
*/
string Algorithm::GetName() {
	return name;
}

/**
 Zmiana romiaru bloku przetwarzania.
 @param newBlockSize Nowy rozmiar bloku wyrazony w ilosci probek.
*/
void Algorithm::SetBlockSize(unsigned long newBlockSize) {
	TRACE("Algorithm::SetBlockSize()", "Zmieniam rozmiar bloku...");
	Module::framesPerBlock = newBlockSize;
	nullModule.UpdateBlockSize();
	ModuleIdIterator mit;
	for(mit = ModuleIdIteratorBegin(); mit != ModuleIdIteratorEnd(); mit++) {
		GetModule(*mit)->UpdateBlockSize();
	}
	
	TRACE("Algorithm::SetBlockSize()", "Rozmiar bloku zmieniony");
}

void Algorithm::PrintEdges() {
	ConnectionIdIterator ei;
	std::cout << "Lista polaczen:" << endl;
	for(ei = ConnectionIdIteratorBegin(); ei != ConnectionIdIteratorEnd(); ei++) {
		std::cout << "    (" << GetConnection(*ei)->sourceModule->GetName()
			<< ", " << GetConnection(*ei)->destinationModule->GetName() << ") " << std::endl;
	}
}

ModuleIdIterator Algorithm::ModuleIdIteratorBegin() {
	boost::tie(moduleIterator, moduleIteratorLast) = boost::vertices(graph);
	return moduleIterator;
}

ModuleIdIterator Algorithm::ModuleIdIteratorEnd() {
	boost::tie(moduleIterator, moduleIteratorLast) = boost::vertices(graph);
	return moduleIteratorLast;
}

ConnectionIdIterator Algorithm::ConnectionIdIteratorBegin() {
	boost::tie(connectionIterator, connectionIteratorLast) = boost::edges(graph);
	return connectionIterator;
}

ConnectionIdIterator Algorithm::ConnectionIdIteratorEnd() {
	boost::tie(connectionIterator, connectionIteratorLast) = boost::edges(graph);
	return connectionIteratorLast;
}

Connection* Algorithm::GetConnection(ConnectionId connectionId) {
	// TODO: idiotodpornosc i inline
	return &graph[connectionId];
}

std::vector<string>& Algorithm::ListModuleTypes() {
	return moduleFactory.ListModuleTypes();
}
