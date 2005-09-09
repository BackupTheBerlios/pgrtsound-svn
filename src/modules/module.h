#ifndef MODULE_H
#define MODULE_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "module_parameters.h"
#include "module_io.h"
#include "../debug.h"

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

	    Module( string name_ );
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
		NullModuleSingleton() : Module("null"), oNull("null") {
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
