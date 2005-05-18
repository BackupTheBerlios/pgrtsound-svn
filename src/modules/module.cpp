#include "module.h"

int Module::framesPerBlock = 256;
int Module::sampleRate = 44100;

//------------------------------------------------------------------------------
Input::Input() {
}

Input::~Input() {
}

void Input::SetID(int newID) {
	id = newID;
}

void Input::SetName(string newName) {
	name = newName;
}

void  Input::SetSignal(float* sig) {
	signal = sig;
}

int Input::GetID() const {
	return id;
}

string Input::GetName() const {
	return name;
}

//------------------------------------------------------------------------------
Output::Output() {
}

Output::~Output() {
}

void  Output::SetSignal(float* sig) {
	signal = sig;
}


void Output::SetID(int newID) {
	id = newID;
}

void Output::SetName(string newName) {
	name = newName;
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
Module::Module() {
	type = "no type";
}

Module::~Module() {
	int i;
	float* temp;

	TRACE2("Module::~Module", "Sprzatam modul ", id);

	// czy to naprawde usuwa bufory?
	for(i = 0; i < outputs.size(); i++) {
		temp = outputs[i]->GetSignal();
		delete []temp;
	}
	
	for(i = 0; i < parameters.size(); i++) {
		delete parameters[i];
	}

	TRACE3("Module::~Module", "Modul ", id, " sprzatniety");
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
	input->SetSignal(NULL);
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
        output->SetID(outputs.size());
		output->SetSignal(outBuff);
 		outputs.push_back(output);
		return output->GetID();
	}
}

/**
 *	Laczy wejscie bierzacego modulu z wyjsciem innego
 */
void Module::ConnectInputTo(int numInput, float *sourceSignal) {
	inputs[numInput]->SetSignal(sourceSignal);
}

/**
 * Funckja przetwarzania. Musi zostac zredefiniowana w kazdym module.
 */
void Module::Process() {
	// nic sie nie dzieje
}

void Module:: SetID(int newID) {
	id = newID;
}

void Module::SetName(string newName) {
	name = newName;
}

int Module::GetID() const {
	return id;
}

string Module::GetName() const {
	return name;
}

string Module::GetType() const {
	return type;
}
