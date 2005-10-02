#ifndef MODULE_H
#define MODULE_H

#include "../debug.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>

#ifndef M_PI
#define M_PI  (3.14159265f)
#endif

#ifndef M_2PI
#define M_2PI  (6.2831853f)
#endif

using namespace std;

/*
 Ponizsze makro wykorzystane w publicznej czesci definicji klasy modulu, zalatwi
 koniecznosc definiowania kilku niezbedych funckji potrzbenych wewnatrz systemu.
*/
#define REGISTER_MODULE( mod_type, class_name ) \
	virtual string GetType() { return mod_type; } \
	static string GetTypeStatic() { return mod_type; } \
	static Module* Create() { return new class_name; }

class Output;

/**
 Wejscie modulu.
 Klasa reprezentujaca wyjscia modulow. Najisotniejsza jej metoda jest
 ConnectTo() pozwalajaca podlaczyc aktualne wejscie do zadanego wyjscia.
*/
class Input	{
	public:
		Input( string name_ );
		~Input();
		int GetID() const;
		string GetName() const;
		inline float* GetSignal();
		void SetID( int newID );
		void ConnectTo( Output* output );
		void Disconnect();
		bool IsConnected();

	protected:
		bool isConnected;
		Output* outputConnected; /**< Wkaznik do wyjscia, do ktorego wejscie jest podlaczone */
		int id; /**< Numer idetyfikujacy wejscie */
		string name; /**< Nazwa wejscia */
};

/**
 * Wyjscie modulu.
 * Klasa obietkow zwiazanych z kazdym wyjsciem modulu. Obiekty
 * te przechowuja miedzy innymi nazwe i identyfikator wjescia a takze
 * buforem wyjsciowym.
 */
class Output {
	public:
		Output( string name_ );
		~Output();
		int GetID() const;
		string GetName() const;
		float* GetSignal() const;
		void SetID( int newID );
		void SetSignal( float* sig );
		void SetBufferSize( unsigned long newBufferSize );

	protected:
		int id;  // numer idetyfikujacy wejscie
		string name; // nazwa wejscia
		float* signal; // wskaznik na bufor wyjsciowy
};

//------------------------------------------------------------------------------
float* Input::GetSignal() {
	return outputConnected->GetSignal();
}

//------------------------------------------------------------------------------
class Parameter {
   	protected:
   		int		id;
		string	description;	/**< Legedna dla parametru np. "Hz" dla czestotliwosci */
        string	label;			/**< Opis parametru - najczesciej wartosc jaka parametr przyjmuje */

	private:
   		string	type;
   		string	name;

	public:
		Parameter(string type_, string name_);
		virtual ~Parameter();
		int GetID() const;
		string GetType() const;
		string GetName() const;
		string GetLabel() const;
		string GetDescription() const;
		void SetID(int newID);
		void SetLabel(string newLabel);
		void SetDescription(string newDesc);
};

//------------------------------------------------------------------------------
class ParameterFloat : public Parameter {
	public:
		ParameterFloat(string name_);
		virtual ~ParameterFloat() {};
		void Bound(float min, float max, float step);
		virtual void SetValue(float newValue);
		virtual float GetValue() const;
		virtual float GetMin() const;
		virtual float GetMax() const;
		virtual float GetStep() const;

	protected:
   		float   value;
		float	minValue;
		float	maxValue;
		float   step;       /**< Minimlany przyrost wartosci parzmetru (tylko gdy bounded == true) */
		bool    bounded;    /**< Prawda jesli parametr przyjmuje wartosci z przedzialu */
};

inline void ParameterFloat::SetValue(float newValue) {
	if( bounded ) {
		value = floorf(newValue / step) * step;
		value = (newValue > maxValue)? maxValue : value;
		value = (newValue < minValue)? minValue : value;
		return;
	}

	value = newValue;
}

inline float ParameterFloat::GetValue() const {
	return value;
}

//------------------------------------------------------------------------------
class ParameterString : public Parameter {
	public:
   		ParameterString(string name_);
   		virtual ~ParameterString();
		virtual void SetText(string newText);
		virtual string GetText();

	protected:
        string   text;
};

/**
 Interfejs modulu. Potomkami tej klasy sa wszystkie dostepne w systemie moduly.
 Takze autorzy zewnetrznych modulow musza dostosowac sie do tego interfejsu.
 W klasie tej zaimplementowana jest podstawowa funckjoncalnosc dotyczaca dodawania
 wejsc, wyjsc oraz parmaetrow modulu.
*/
class Module {
	public:
	    static unsigned long framesPerBlock;
	    static float	sampleRate;

	    Module();
	    virtual ~Module();
	    int AddInput(Input& input);
	    int AddOutput(Output& output);
	    int AddParameter(Parameter& param);
	    virtual void Process();
	    virtual void Init();
	    virtual void OnProces() {}; // TODO: zaimplemtnowac w systemie wywolywanie OnProcess
   	    virtual void SampleRateChanged();
	    virtual void BlockSizeChanged();
	    virtual string GetType();
   	    static string GetTypeStatic();
	    static Module* Create();
	    void SetName(string newName);
	    string GetName() const;
	    int GetOutputCount();
	    int GetInputCount();
	    int GetParameterCount() const;
	    Input* GetInput(int inputID);
	    Output* GetOutput(int outputID);
	    Parameter* GetParameter(int pID);
	    void UpdateBlockSize();

	protected:
	    vector<Input*> inputs; // wektor wejsc
	    vector<Output*> outputs; // wektor wyjsc
	    vector<Parameter*> parameters; // wektor parametrow
	    string name; // nazwa modulu
};

inline Input* Module::GetInput(int inputID) {
    return inputs[inputID];
}

inline Output* Module::GetOutput(int outputID) {
    return outputs[outputID];
}

inline Parameter* Module::GetParameter(int pID) {
    return parameters[pID];
}

/**
 Globalny modul 'zerowy'.
 Pomocniczy obiekt-singleton pozwalajac realizowac wewnetrznie 'wiszace'
 (nie polaczone) wejscia modulow.
*/
class NullModuleSingleton : public Module {
	private:
	    Output oNull;
	    static NullModuleSingleton NullModule;

		// prywatny konstruktor - singleton
		NullModuleSingleton() : oNull("null") {
	        AddOutput(oNull);
	        BlockSizeChanged();
	    }

	public:
		void BlockSizeChanged();

	    static NullModuleSingleton& Instance() {
	        return NullModule;
	    }
};


#endif // MODULE_H
