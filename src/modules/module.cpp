#include "module.h"

// domyslne
int	Module::framesPerBlock = 256;
float Module::sampleRate = 44100;

// gloablny modul wysylajacy zera
NullModuleSingleton NullModuleSingleton::NullModule;
NullModuleSingleton& nullModule = NullModuleSingleton::Instance();

//------------------------------------------------------------------------------
/**
 * Konstruktor.
 * Poprawne utworzenie obiektu modulu wymaga podania jego typu oraz nazwy.
 * @param type_ Rozpoznawany przez system typ modulu
 * @param name_ Oczekiwana nazwa modulu
 */
Module::Module(string type_, string name_) {
	type = type_;
	name = name_;
}

Module::~Module() {
}

/**
 * Dodanie parametru do modulu.
 * Po dodaniu parametru, zwraca numer identyfikujacy je w module.
 * @param param Wskaznik do obiektu parametru, ktory chcemy dodac
 */
int Module::AddParameter(Parameter& param) {
	param.SetID( parameters.size() );
	parameters.push_back(&param);
	return param.GetID();
}

/**
 * Dodanie wejscia do modulu.
 * Funkcja po dodaniu wejscia, zwraca numer idnetyfikujacy to wejscie w module.
 * @param input Wskaznik do obiektu wejscia, ktory chcemy dodac
 */
int Module::AddInput(Input& input) {
	input.SetID( inputs.size() );
	inputs.push_back(&input);
	return input.GetID();
}

/**
 * Dodanie wyjscia do modulu.
 * Po dodania wyjscia zwraca numer identyfikujacy je w module.
 * @param output Wskaznik do obiektu wyjscia, ktory chcemy dodac
 */
int Module::AddOutput(Output& output) {
	output.SetID( outputs.size() );
	outputs.push_back(&output);
	return output.GetID();
}


/**
 * Funckja przetwarzania.
 * Definiuje realizowana przez modul funckje DSP. Funkcja ta wywolywana jest przez
 * algorytm raz dla pojedynczego kroku (dlugosci bloku probek) syntezy.
 */
void Module::Process() {
	// nic sie nie dzieje
}

/**
 * Funckja inicjalizujaca. Redefiniujac te funckje mozna zdefiniowac czynnosci
 * wykonywane tuz po dodaniu modulu do algorytmu.
 */
void Module::Init() {
}

/**
 * Ustalenie nazwy modulu.
 * Nazwa modulu musi byc jest jednoznaczna w skali calego systemu.
 */
void Module::SetName(string newName) {
	name = newName;
}

/**
 * Zwraca nazwe obiektu.
 */
string Module::GetName() const {
	return name;
}

/**
 * Zwraca typ modulu.
 */
string Module::GetType() const {
	return type;
}

/**
 * Zwraca liczbe parametrow modulu.
 */
int Module::GetParameterCount() const {
	return parameters.size();
}

/**
 * Zwraca liczbe wyjsc modulu.
 */
int Module::GetOutputCount() {
    return outputs.size();   
}

/**
 * Zwraca liczbe wejsc modulu.
 */
int Module::GetInputCount() {
    return inputs.size();
}

/**
 Powiadomienie modulu o zmianie czest. probkowania.
 Funcka jest wywolywana gdy nastapi zmiana czest. probkowania.
 Redefinicja omawianej funkcji wirtualnej w module pozwala na jego odpowiednie
 dostosowanie do nowej czest. probkowania, jesli realizowany algorytmy tego wymaga.
*/
void Module::SampleRateChanged() {
	// nadpisywac w modulach
}

/**
 Powiadomienie modulu zmianie dlugosci bloku przetwarzania.
 Funcka jest wywolywana gdy nastapi zmiana dlugosci bloku przetwarzania.
 Redefinicja omawianej funkcji wirtualnej w module  pozwala na jego odpowiednie
 dostosowanie do nowej czest. probkowania, jesli realizowany algorytmy tego wymaga.
*/
void Module::BlockSizeChanged() {
	// nadpisywac w modulach
}

/**
 Aktualziacja rozmiaru bloku.
 Funckja wykonuje czynnosci potrzbne po zmianie dlugosci bloku (Module::framesPerBlock).
 Nanlezy wywolac ja dla wszsytkich modulow, m.in. w celu aktualizacji dlugosci
 buforow wyjsc.
*/
void Module::UpdateBlockSize() {
	//zmiana rozmiaru buforow wyjsc
	for(int i = 0; i < outputs.size(); i++) {
        TRACE6("Output::SetBufferSize()", "Wyjscie [", name, "].[", outputs[i]->GetName(),
			"] zmienia rozmiar bufora na ", Module::framesPerBlock);
		outputs[i]->SetBufferSize(Module::framesPerBlock);
	}
	
	// niech modul obsluzy fakt zmiany rozmiaru bloku
	BlockSizeChanged();
}

