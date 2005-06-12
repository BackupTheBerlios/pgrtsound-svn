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
Input::Input(string name_) {
   	name = name_;
   	// niepodlaczone wejscie bedzie pobierac dane z bufora nullBuffer
   	signal = nullBuffer;
}

Input::~Input() {
}

float* Input::GetSignal() {
	return signal;
}

void Input::SetID(int newID) {
	id = newID;
}

void Input::SetSignal(float* sig) {
	signal = sig;
}

int Input::GetID() const {
	return id;
}

string Input::GetName() const {
	return name;
}



//------------------------------------------------------------------------------
Output::Output(string name_) {
	name = name_;
}

Output::~Output() {
}

void  Output::SetSignal(float* sig) {
	signal = sig;
}

void Output::SetID(int newID) {
	id = newID;
}

int Output::GetID() const {
	return id;
}

string Output::GetName() const {
	return name;
}

float* Output::GetSignal() const {
	return signal;
}

//------------------------------------------------------------------------------
Module::Module(string type_, string name_) {
	type = type_;
	name = name_;
}

Module::~Module() {
	int i;
	float* temp;

	TRACE2("Module::~Module", "Sprzatam modul ", name);
/*	// czy to naprawde usuwa bufory?
	for(unsigned i = 0; i < outputs.size(); i++) {
        temp = outputs[i]->GetSignal();
		delete []temp;
	}
*/	
    for (int i = 0; i < GetInputCount(); i++) {
        delete inputs[i];        
    }
    inputs.clear();
    
    for (int i = 0; i < GetOutputCount(); i++) {
        delete outputs[i];
    }
    outputs.clear();
    
	TRACE3("Module::~Module", "Modul ", name, " sprzatniety");
}

/**
 * Dodanie parametru
 */
int Module::AddParameter(Parameter *param) {
	parameters.push_back(param);
	return param->GetID();
}

/**
 * Dodanie wejscia
 * Funkcja po dodaniu wejscia zwraca numer idnetyfikujacy to wejscie w module.
 */
int Module::AddInput(Input* input) {
	input->SetID( inputs.size() );
	inputs.push_back(input);
	return input->GetID();
}

/**
 * Dodanie wyjscia
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
 *	Laczy wejscie bierzacego modulu z wyjsciem innego
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
 * Funckja przetwarzania. Musi zostac zredefiniowana w kazdym module.
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

/*void Module::SetID(int newID) {
	id = newID;
}*/

void Module::SetName(string newName) {
	name = newName;
}

/*int Module::GetID() const {
	return id;
}*/

string Module::GetName() const {
	return name;
}

string Module::GetType() const {
	return type;
}

int Module::GetParameterCount() const {
	return parameters.size();
}

int Module::GetOutputCount() {
    return outputs.size();   
}

int Module::GetInputCount() {
    return inputs.size();
}
