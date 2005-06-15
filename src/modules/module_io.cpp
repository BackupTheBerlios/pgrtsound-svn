#include "module_io.h"

extern float* nullBuffer;

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
	delete signal;
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
