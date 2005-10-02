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
	TRACE( "Algorithm::~Algorithm - Sprzatam %i modulow\n", GetModuleCount() );

	ModuleIdIterator mit;
	for(mit = ModuleIdIteratorBegin(); mit != ModuleIdIteratorEnd(); mit++) {
		delete GetModule(*mit);
	}

	TRACE( "Algorithm::~Algorithm - Bye!\n" );
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

	TRACE( "Algorithm::InitAudioPorts - Dodano moduly AudioPortIn i AudioPortOut\n" );
}

/** Dodanie modulu.
 * @param type Rozpoznawany prez system typ modulu.
 * @param name Oczekwiana nazwa modulu.
 */
ModuleId Algorithm::AddModule( string type, string name ) {
	Module* mod = moduleFactory.CreateModule(type);

    if( mod == NULL )
        throw RTSError( "Algorithm::AddModulee - Nie ma modulu typu '" + type + "'" );
	
   	mod->SetName(name);
   	mod->Init();
   	ModuleId moduleId;
   	moduleId = add_vertex(graph);
   	graph[moduleId].module = mod;

   	moduleName2IdMap.insert( make_pair(name, moduleId) );
   	TRACE( "Algorithm::AddModule - Dodano modul '%s' typu '%s'\n",
  		mod->GetName().c_str(), mod->GetType().c_str() );
   		
   	return moduleId;
}

void Algorithm::PrintInfo(void) {
	cout << "Algorithm::PrintInfo(): Informacje o algorytmie... " << endl;

	ModuleIdIterator mit;
	for(mit = ModuleIdIteratorBegin(); mit != ModuleIdIteratorEnd(); mit++) {
		cout << "    Modul: " << GetModule(*mit)->GetName() <<
			"(" << GetModule(*mit)->GetType() << ")" << endl;
	}
	
	PrintEdges();
}

/**
 Utworzenie polaczenia miedzy modulami. Tworzy polaczenie miedzy wybranym
 wyjsciem i wejsciem dwoch roznych modulow.
 @param moduleId1 Identyfikator modulu zrodlowego
 @param outputId Identyfiaktor wyjscia modulu zrodlowego
 @param moduleId2 Identyfikator modulu docelowego
 @param inputId Identyfiaktor wejscia modulu docelowego
*/
bool Algorithm::ConnectModules( ModuleId moduleId1, int outputId,
			ModuleId moduleId2, int inputId, ConnectionId& connId )
{
	//Algorithm::ConnectModules\n
	TRACE( "    '%s'.'%s' -> '%s'.'%s'\n",
		GetModule(moduleId1)->GetName().c_str(),
		GetModule(moduleId1)->GetOutput(outputId)->GetName().c_str(),
		GetModule(moduleId2)->GetName().c_str(),
		GetModule(moduleId2)->GetInput(inputId)->GetName().c_str() );

	ConnectionDescription c;
	c = boost::add_edge(moduleId1, moduleId2, graph); // utworzenie krawedzi w grafie
	connId = c.first; // zwracany identyfikator polaczenia w grafie
	
	if( IsGraphAcyclic() ) {
 		// graf jest acykliczny - polacz wejscie do wyjscia
		GetModule( moduleId2 )->GetInput( inputId )->ConnectTo(
			GetModule( moduleId1 )->GetOutput( outputId ) );
		graph[connId].sourceModule = GetModule( moduleId1 );
		graph[connId].sourceOutputId = outputId;
		graph[connId].destinationModule = GetModule( moduleId2 );
        graph[connId].destinationInputId = inputId;
		return true;
	}
	else {
		boost::remove_edge( connId, graph );
	}

	return false;
}

/**
 Utworzenie polaczenia miedzy modulami. Tworzy polaczenie miedzy wybranym
 wyjsciem i wejsciem dwoch roznych modulow o podanych nazwach.
 @param moduleName1 Nazwa modulu zrodlowego
 @param outputId Identyfiaktor wyjscia modulu zrodlowego
 @param moduleName2 Nazwa modulu docelowego
 @param inputId Identyfiaktor wejscia modulu docelowego
*/
bool Algorithm::ConnectModules( string moduleName1, int outputId,
	string moduleName2, int inputId, ConnectionId& connId)
{
	ModuleId moduleId1 = ( *moduleName2IdMap.find(moduleName1) ).second;
	ModuleId moduleId2 = ( *moduleName2IdMap.find(moduleName2) ).second;

	return ConnectModules( moduleId1, outputId, moduleId2, inputId, connId );
}

/**
 * Ustawienie czestotliwosci probkowania.
 */
void Algorithm::SetSampleRate(float sRate) {
	sampleRate = sRate;
	Module::sampleRate = sRate; // wartos widoczna we wszystkich modulach

	TRACE( "Algorithm::SetSampleRate - Module::sampleRate = %i\n", Module::sampleRate );
}

/**
 * Tworzy kolejke modulow dla przetwarznia. Struktura grafu algorytmu zostaje
 * przetworzona na odpowiadajaca mu szeregowa sekwencje modulow, zgodnie z ktora
 * wywolane zostac musza ich funkcje przetwarzania aby zachowac zaleznosci
 * wejscie/wyjscie medzy modulami w grafie.
 */
void Algorithm::CreateQueue() {
	TRACE( "Algorithm::CreateQueue - Tworzenie kolejki modulow...\n" );

	using namespace boost;

	typedef vector<ModuleId> container;
	container c;
	property_map<Graph, vertex_index_t>::type index = get(vertex_index, graph);
	property_map<Graph, Module* GraphModule::*>::type mods = get(&GraphModule::module, graph);

	// numerownaie wszystkich wezlow (inicjalizacja wlasciwosci vertex_index)
	graph_traits<Graph>::vertex_iterator vi, vend;
	graph_traits<Graph>::vertices_size_type cnt = 0;
	for( tie(vi,vend) = vertices(graph); vi != vend; ++vi )
	    boost::put( index, *vi, cnt++ );

	modulesQueue.clear();
	boost::topological_sort(graph, std::back_inserter(c));

	for (container::reverse_iterator ii = c.rbegin(); ii != c.rend(); ++ii) {
        TRACE( "    %s\n", mods[*ii]->GetName().c_str() );
        modulesQueue.push_back(mods[*ii]);
	}
}

/**
 Zwraca ilosc modulow w grafie algorytmu.
*/
int Algorithm::GetModuleCount() const {
	return num_vertices(graph);
}

int Algorithm::GetConnectionCount() {
	return num_edges(graph);
}

/**
 Czysczenie algorytmu. Powoduje wyczyszczenie wszystkich elementow
 przechowujacych strukture algorytmu. Po wycyzsczeniu algorytm nie spelnia
 zadnej funckji przetwarznia, ale gotowy jest do konfiguracji.
*/
void  Algorithm::Clear() {
	TRACE( "Algorithm::Clear - Czyszczenie algorytm...\n" );

	// usuniecie modulow
	ModuleIdIterator mit;
	for(mit = ModuleIdIteratorBegin(); mit != ModuleIdIteratorEnd(); mit++) {
		delete GetModule(*mit);
	}

	modulesQueue.clear();
	graph.clear();
	moduleName2IdMap.clear();
	
	InitAudioPorts();
	
	TRACE( "Algorithm::Clear - Algorytm wyczyszczony\n" );
}

/**
 * Inicjalizacjia algorytmu. Wywoluje funckje inicjalizujace wszytkich modulow
 * w grafie.
 */
void Algorithm::Init() {
    TRACE( "Algorithm::Init - Inicjalizacja modulow...\n" );
    
	ModuleIdIterator mit;
	for(mit = ModuleIdIteratorBegin(); mit != ModuleIdIteratorEnd(); mit++) {
		GetModule(*mit)->Init();
	}

    TRACE( "Algorithm::Init - Inicjalizacja modulow zakonczona\n" );
}

/**
 Dostep do modulu. Zwraca wskaznik do modulu o podanej nazwie.
 @param moduleName Nazwa dociekanego modulu
*/
Module* Algorithm::GetModule(string moduleName) const {
    if (moduleName2IdMap.find(moduleName) != moduleName2IdMap.end()) {
        ModuleId moduleId = ( *moduleName2IdMap.find(moduleName) ).second;
		return GetModule(moduleId);
    } else {
		return NULL;
    }
}

/**
 Zwraca identyfikator modulu o podanej nazwie.
 @param moduleName Nazwa dociekanego modulu
*/
ModuleId Algorithm::GetModuleId( string moduleName ) const {
    if ( moduleName2IdMap.find(moduleName) != moduleName2IdMap.end() ) {
    	ModuleId moduleId = ( *moduleName2IdMap.find(moduleName) ).second;
		return moduleId;
    } else {
        return NULL;
    }    
}

/**
 Dostep do modulu. Zwraca wskaznik do modulu o zadanym identyfikatorze.
 @param moduleId Identyfikator modulu
*/
Module* Algorithm::GetModule(ModuleId moduleId) const {
    Module* mod;
    
	try {
		mod = graph[moduleId].module;
	}
	catch ( boost::bad_graph ) {
		TRACE( "!!!! Algorithm::GetModule - bad_graph przechwycony\n" );
		return NULL;
	}
	
    return mod;
}

Module* Algorithm::GetInputPort() const {
	return inputPort;
}

Module* Algorithm::GetOutputPort() const {
	return outputPort;
}

/**
 Usuwanie modulu z grafu.
 @param moduleId Identyfikator modulu
*/
bool Algorithm::DeleteModule( ModuleId moduleId ) {
	TRACE( "Algorithm::DeleteModule - Usuwam modul '%s'\n",
		GetModule( moduleId )->GetName().c_str() );
	
	if( moduleName2IdMap.count( GetModule( moduleId )->GetName() ) == 0 ) {
		TRACE( "!!!! Algorithm::DeleteModule - Nie ma modulu o nazwie '%s'\n",
			GetModule( moduleId )->GetName().c_str() );
		return false;
	}
	
	using namespace boost;
	
	property_map<Graph, vertex_index_t>::type index = get(vertex_index, graph);
	// initialize the vertex_index property values
	graph_traits<Graph>::vertex_iterator vi, vend, vi_end;
	graph_traits<Graph>::vertices_size_type cnt = 0;
	for(tie(vi,vend) = vertices(graph); vi != vend; ++vi)
	    put(index, *vi, cnt++);

	// odlaczenie wszytkich wejsci do ktorych jest usuwnay modul podlaczony
	TRACE( "    Rozlaczam wejscia:\n" );

	boost::graph_traits<Graph>::out_edge_iterator ei, eend;
	for( tie(ei, eend) = out_edges( moduleId, graph ); ei != eend; ++ei ) {
		TRACE( "        [%s].[%s]\n",
			graph[*ei].destinationModule->GetName().c_str(),
			graph[*ei].destinationModule->GetInput( graph[*ei].destinationInputId )
				->GetName().c_str() );

		graph[*ei].destinationModule->GetInput( graph[*ei].destinationInputId )
			->Disconnect();
	}

    //usuwanie z mapy
	moduleName2IdMap.erase( GetModule(moduleId)->GetName() );
	//usuwanie grafu
	boost::clear_vertex( moduleId, graph ); // usuniecie polaczen z tym wezlem zwiazanych
	boost::remove_vertex( moduleId, graph ); // usuniecia wezla

	//usuwanie z pamieci
	delete GetModule( moduleId );

	CreateQueue();
	
	PrintInfo(); // debug

	TRACE( "Algorithm::DeleteModule - Modul usuniety\n" );
	
	return true;
}

/**
 Usuwanie polaczenia miedzy modulami.
 @param connectionId identyfikator polaczenia do usuniecia
*/
bool Algorithm::DeleteConnection(ConnectionId connectionId) {
	TRACE( "Algorithm::DeleteConnection\n" );
	
	Input* input;
	
	try {
		input = graph[connectionId].destinationModule
			->GetInput( graph[connectionId].destinationInputId );
		boost::remove_edge( connectionId, graph );
	}
	catch ( boost::bad_graph ) {
		TRACE( "!!!! Algorithm::DeleteConnection - bad_graph przechwycony\n" );
		return false;
	}
	
	input->Disconnect();
	
	// rozlaczamy wejscie modulu 2 ???
	//cout << "Is connected: " << graph[connectionId].destinationModule
	//	->GetInput( graph[connectionId].destinationInputId )->IsConnected() << endl;
	TRACE( "Algorithm::DeleteConnection - Polaczenie usuniete\n" );
	PrintInfo();
	
	return true;
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
	TRACE( "Algorithm::SetBlockSize - Zmieniam rozmiar bloku...\n" );
	Module::framesPerBlock = newBlockSize;
	nullModule.UpdateBlockSize();
	ModuleIdIterator mit;
	for(mit = ModuleIdIteratorBegin(); mit != ModuleIdIteratorEnd(); mit++) {
		GetModule(*mit)->UpdateBlockSize();
	}
	
	TRACE( "Algorithm::SetBlockSize - Rozmiar bloku zmieniony\n" );
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

Connection* Algorithm::GetConnection( ConnectionId connectionId ) {
	// TODO: idiotodpornosc i inline
	return &graph[connectionId];
}

std::vector<string>& Algorithm::ListModuleTypes() {
	return moduleFactory.ListModuleTypes();
}

/*
 Detekcja polaczen cyklicznych w grafie. Wartosc zwrtona funckji mowi o tym czy
 w obecnej strukturze grafu algorytmu wyktyro polaczenia cykliczne (petle
 zwrotne).
*/
bool Algorithm::IsGraphAcyclic() {
	// inicjalizacja indeksow wierzcholkow grafu
	boost::property_map<Graph, boost::vertex_index_t>::type index =
		get( boost::vertex_index, graph );
	boost::graph_traits<Graph>::vertex_iterator vi, vend, vi_end;
	boost::graph_traits<Graph>::vertices_size_type cnt = 0;
	for( boost::tie(vi,vend) = vertices(graph); vi != vend; ++vi )
	    put(index, *vi, cnt++);

	// sprawdznie cyklicznosci
	bool has_cycle = false;
	cycle_detector vis( has_cycle );
	boost::depth_first_search( graph, visitor(vis) );
	
	return !has_cycle;
}

/*
 Zmiana nazwy modulu. Nastapi zmiana nazwy modulu o podanym identyfikatorze o
 ile podana nazwa nie je juz uzywana przez inny modul. Wartosc zwracana prze
 funckje informuje o powodzeniu operacji.
 @param moduleId Identyfikator modulu, ktorgo nazwe zmieniamy
 @param newName Nowa nazwa modulu
*/
bool Algorithm::ChangeModuleName( ModuleId moduleId, string newName ) {
	/* TODO (#1#): Implement Algorithm::ChangeModuleName() */
	if( moduleName2IdMap.count( newName ) > 0 ) {
		return false;
	}

	Module* mod = GetModule( moduleId );
	moduleName2IdMap.erase( mod->GetName() );
	mod->SetName( newName );
	moduleName2IdMap.insert( make_pair( newName, moduleId ) );
	
	TRACE( "Algorithm::ChangeModuleName - Zmieniono znawe na '%s'\n",
		GetModule( moduleId )->GetName().c_str() );
		
	PrintInfo();
	
	return true;
}
