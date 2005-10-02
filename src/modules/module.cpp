#include "module.h"

// domyslne
unsigned long Module::framesPerBlock = 256;
float Module::sampleRate = 44100;

// gloablny modul wysylajacy zera
NullModuleSingleton NullModuleSingleton::NullModule;
NullModuleSingleton& nullModule = NullModuleSingleton::Instance();

/*
 Konstruktor.
 Poprawne utworzenie obiektu modulu wymaga podania jego nazwy.
 @param moduleName Unikalna nazwa modulu
*/
Module::Module() : name("") {
}

Module::~Module() {
}

/*
 Dodanie parametru do modulu.
 Po dodaniu parametru, zwraca numer identyfikujacy je w module.
 @param param Wskaznik do obiektu parametru, ktory chcemy dodac
*/
int Module::AddParameter(Parameter& param) {
	param.SetID( parameters.size() );
	parameters.push_back(&param);
	return param.GetID();
}

/*
 Dodanie wejscia do modulu.
 Funkcja po dodaniu wejscia, zwraca numer idnetyfikujacy to wejscie w module.
 @param input Wskaznik do obiektu wejscia, ktory chcemy dodac
*/
int Module::AddInput(Input& input) {
	input.SetID( inputs.size() );
	inputs.push_back(&input);
	return input.GetID();
}

/**
 Dodanie wyjscia do modulu.
 Po dodania wyjscia zwraca numer identyfikujacy je w module.
 @param output Wskaznik do obiektu wyjscia, ktory chcemy dodac
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
string Module::GetTypeStatic() {
	return "no type defined";
}

string Module::GetType() {
	return "no type defined";
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
	for(unsigned long i = 0; i < outputs.size(); i++) {
        TRACE( "Output::SetBufferSize - Wyjscie [%s][%i] zmienia rozmiar bufora na",
			name.c_str(), outputs[i]->GetName().c_str(), Module::framesPerBlock );
		outputs[i]->SetBufferSize(Module::framesPerBlock);
	}
	
	// niech modul obsluzy fakt zmiany rozmiaru bloku
	BlockSizeChanged();
}


void NullModuleSingleton::BlockSizeChanged() {
    float* buff;
    buff = oNull.GetSignal();

    buff = new float[Module::framesPerBlock];
    for(unsigned long i = 0; i < Module::framesPerBlock; i++) {
        *buff++ = 0.0f;
    }
}

extern NullModuleSingleton& nullModule;

Input::Input(string name_) {
	name = name_;
	isConnected = false;
	outputConnected =  nullModule.GetOutput(0);
		// niepodlaczone wejscie do gloablnego zerowego modulu
	//input.ConnectTo( nullModule.GetOutput(0) );
}

Input::~Input() {
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
	isConnected = true;
}

void Input::Disconnect() {
    isConnected = false;
    outputConnected =  nullModule.GetOutput(0);
}

bool Input::IsConnected() {
	return isConnected;
}
//------------------------------------------------------------------------------
Output::Output(string name_) {
	name = name_;
	signal = NULL;

	/* TODO (#1#): zrobic try block */
	SetBufferSize( Module::framesPerBlock );
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

void Output::SetBufferSize( unsigned long newBufferSize ) {
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
		for( unsigned long i = 0; i <newBufferSize; i++ )
			outBuff[i] = 0.0f;

		signal = outBuff;
	}
}

Parameter::Parameter(string type_, string name_) {
	type = type_;
	name = name_;
	description =  "";
	label = "";
}

Parameter::~Parameter() {
}

int Parameter::GetID() const {
	return id;
}

string Parameter::GetName() const {
	return name;
}

string Parameter::GetType() const {
	return type;
}
string Parameter::GetLabel() const {
	return label;
}

string Parameter::GetDescription() const {
	return description;
}

void Parameter::SetID(int newID) {
	id = newID;
}

void Parameter::SetLabel(string newLabel) {
	label = newLabel;
}

void Parameter::SetDescription(string newDesc) {
	description = newDesc;
}

//------------------------------------------------------------------------------
ParameterFloat::ParameterFloat(string name_) :
	Parameter("float", name_)
{
    value = 0;
    bounded = false;
}

void ParameterFloat::Bound(float min, float max, float stp) {
	minValue = min;
	maxValue = max;
	step = stp;
	bounded = true;
}

float ParameterFloat::GetMin() const {
	return minValue;
}

float ParameterFloat::GetMax() const {
	return maxValue;
}

float ParameterFloat::GetStep() const {
	return step;
}

//------------------------------------------------------------------------------
ParameterString::ParameterString(string name_) :
	Parameter("string", name_)
{
	description = "no description";
	label       = "no label";
	text        = "";
}

ParameterString::~ParameterString() {
}

string ParameterString::GetText() {
	return text;
}

void ParameterString::SetText(string newText) {
     text = newText;
}



