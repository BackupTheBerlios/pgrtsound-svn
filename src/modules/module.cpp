#include "module.h"

int Module::framesPerBlock = 64;
int Module::sampleRate = 44100;

Module::Module() {
	type = "no_type_given";
}

Module::~Module() {
	TRACE2("Module", "Sprzatam modul ", id);

	int i;	

	for(i = 0; i < inputs.size(); i++) {
		delete inputs[i];
	}
	
	// czy to naprawde usuwa bufory?
	for(i = 0; i < outputs.size(); i++) {
		delete outputs[i]->signal;
		delete outputs[i];
	}
	
	for(i = 0; i < params.size(); i++) {
		delete params[i];
	}

	TRACE3("Module", "Modul ", id, " sprzatniety");
}

/**
 * Dodanie parametru o nazwie name
 */
int Module::AddParam(string name) {
	Param* param = new Param;
	
	param->name = name;
	param->id = params.size();
	param->value = 0;

	params.push_back(param);
	
	return param->id;
}

/**
 * Dodanie wejscia o nazwie name.
 * Funkcja po dodaniu wejscia zwraca numer idnetyfikujacy to wejscie w module.
 * Nazwa moze byc dowolna, byc moze bedzie miala wieksze zastosowanie w GUI.
 */
int Module::AddInput(string name) {
	Input* input = new Input;
	
	input->name = name;
	input->id = inputs.size();
	input->signal = NULL;

	inputs.push_back(input);
	
	return input->id;
}

/**
 * Dodanie wyjscia o nazwie name
 */
int Module::AddOutput(string name) {
	Output* output = new Output;
	
	output->name = name;
	output->id = outputs.size();
	// stowrzenie bufora o dlugosci framesPerBlock
	output->signal = new float[Module::framesPerBlock];
 
	outputs.push_back(output);
	
	return output->id;
}

void Module::ConnectInputTo(int numInput, float *sourceSignal) {
	inputs[numInput]->signal = sourceSignal;
}

/**
 * Funckja przetwarzania. Musi zostac zredefiniowana w kazdym module.
 */
void Module::Process() {
	// nic sie nie dzieje
}

///**
// * Zwraca obiekt wejscia numer inputNum.
// */
//Input& Module::input(int inputNum) {
//	return inputs[inputNum];
//}

///**
// * Zwraca obiekt wejscia numer outputNum.
// */
//Output& Module::output(int outputNum) {
//	return outputs[outputNum];
//}

///**
// * Zwraca wartosc parametru numer paramNum
// */
//float Module::param(int paramNum) {
//	return params[paramNum].value;
//}

/**
 * Zmiana wartosci parametru numer paramNum na wartosc value.
 */
void Module::SetParam(int paramNum, float value) {
	params[paramNum]->value = value;
}
