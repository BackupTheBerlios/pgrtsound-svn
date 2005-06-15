#include "module.h"

int	Module::framesPerBlock = 256;
float	Module::sampleRate = 44100;

// uzyteczny, globabalny bufor zerowy
float* nullBuffer;
static bool allocateNullBuffer = true;

void SetNullBuffer(unsigned long size) {
	if(allocateNullBuffer) {
		TRACE2("module.cpp", "Alokuje bufor zerowy rozmiaru ", size);
		float* buff;
		nullBuffer = new float[size];
		buff = nullBuffer;

	 	for(unsigned long i = 0; i < size; i++) {
			*buff++ = 0.0f;
		}
		allocateNullBuffer = false;

		TRACE2("module.cpp", "Bufor zerowy zaalokowany ", nullBuffer);
	}
}

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
int Module::AddParameter(Parameter *param) {
	parameters.push_back(param);
	return param->GetID();
}

/**
 * Dodanie wejscia do modulu.
 * Funkcja po dodaniu wejscia, zwraca numer idnetyfikujacy to wejscie w module.
 * @param input Wskaznik do obiektu wejscia, ktory chcemy dodac
 */
int Module::AddInput(Input* input) {
	input->SetID( inputs.size() );
	inputs.push_back(input);
	return input->GetID();
}

/**
 * Dodanie wyjscia do modulu.
 * Po dodania wyjscia zwraca numer identyfikujacy je w module.
 * @param output Wskaznik do obiektu wyjscia, ktory chcemy dodac
 */
int Module::AddOutput(Output* output) {
	float* outBuff;

	// stworzenie bufora wyjsciowego o dlugosci framesPerBlock
	outBuff = new float[Module::framesPerBlock];
	if(outBuff == NULL) {
        throw RTSError("Module::AddOutput(): Nie mozna zaalokowac pamieci na bufor wyjsciowy");
	}
	else {
        output->SetID( outputs.size() );
		output->SetSignal(outBuff);
 		outputs.push_back(output);
		return output->GetID();
	}
}

/**
 *	Laczy wejscie biezacego modulu z wyjsciem innego.
 * @param numInput Numer wejscia beirzacego modulu
 * @param sourceSignal Wskaznik do bufora wyjciowego wysjcia, do ktorege sie podlaczamy
 */
void Module::ConnectInputTo(int numInput, float *sourceSignal) {
	if(numInput > inputs.size()) {
		TRACE3("Module::ConnectInputTo()", "Wejscie o id = ", numInput, " nie istnieje");
	}
	else {
		GetInput(numInput)->SetSignal(sourceSignal);
	}
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
