#include "module.h"

int Module::framesPerBlock = 64;
int Module::sampleRate = 44100;

Module::Module() {
	type = "no_type_given";
}

Module::~Module() {
	for(int i =0; i < outputs.size(); i++) {
		delete output(i).signal;
	}
}

/**
 * Dodanie parametru o nazwie name
 */
int Module::addParam(string name) {
	Param param;
	
	param.name = name;
	param.id = params.size();
	param.value = 0;

	params.push_back(param);
	
	return param.id;
}

/**
 * Dodanie wejscia o nazwie name.
 * Funkcja po dodaniu wejscia zwraca numer idnetyfikujacy to wejscie w module.
 * Nazwa moze byc dowolna, byc moze bedzie miala wieksze zastosowanie w GUI.
 */
int Module::addInput(string name) {
	Input input;
	
	input.name = name;
	input.id = inputs.size();
	input.signal = NULL;

	inputs.push_back(input);
	
	return input.id;
}

/**
 * Dodanie wyjscia o nazwie name
 */
int Module::addOutput(string name) {
	Output output;
	
	output.name = name;
	output.id = outputs.size();
	// stowrzenie bufora o dlugosci framesPerBlock
	
	output.signal = new float[Module::framesPerBlock];
 
	outputs.push_back(output);
	
	return output.id;
}

/**
 * Funckja przetwarzania. Musi zostac zredefiniowana w kazdym module.
 */
void Module::process() {
	// nic sie nie dzieje
}

/**
 * Zwraca obiekt wejscia numer inputNum.
 */
Input& Module::input(int inputNum) {
	return inputs[inputNum];
}

/**
 * Zwraca obiekt wejscia numer outputNum.
 */
Output& Module::output(int outputNum) {
	return outputs[outputNum];
}

/**
 * Zwraca wartosc parametru numer paramNum
 */
float Module::param(int paramNum) {
	return params[paramNum].value;
}

/**
 * Zmiana wartosci parametru numer paramNum na wartosc value.
 */
void Module::setParam(int paramNum, float value) {
	params[paramNum].value = value;
}
