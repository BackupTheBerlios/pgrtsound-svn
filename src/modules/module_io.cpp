#include "module_io.h"
#include "module.h"

//extern float* nullBuffer;

Input::Input(string name_) {
   	name = name_;
   	// niepodlaczone wejscie bedzie pobierac dane z bufora nullBuffer
   //signal = NULL;
}

Input::~Input() {
}

/* TODO (#1#): zrobic inline */
float* Input::GetSignal() {
	return outputConnected->GetSignal();
}

void Input::SetID(int newID) {
	id = newID;
}

int Input::GetID() const {
	return id;
}

string Input::GetName() const {
	return name;
}

/**
 Laczy wejscie biezacego modulu z wyjsciem innego.
 @param output Wskaznik do wyjscia, do ktorego podlaczone ma byc biezace wejscie.
*/
void Input::ConnectTo(Output* output) {
	outputConnected = output;
}

//------------------------------------------------------------------------------
Output::Output(string name_) {
	name = name_;
	signal = NULL;

	/* TODO (#1#): zrobic try block */
	SetBufferSize(Module::framesPerBlock);
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

void Output::SetBufferSize(unsigned long newBufferSize) {
	float* outBuff;

	// kasowanie bufora poprzedniego o ile isnieje
	if(signal != NULL) {
		delete []signal;
		signal = (float*)NULL;
	}
	
	// stworzenie bufora wyjsciowego o dlugosci newBufferSize
	outBuff = new float[newBufferSize];
	if(outBuff == NULL) {
        throw RTSError("Output::SetBufferSize(): Nie mozna zaalokowac pamieci na bufor wyjsciowy");
	}
	else {
		signal = outBuff;
	}
}
