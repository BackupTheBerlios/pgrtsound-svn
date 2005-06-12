#ifndef MODULE_H
#define MODULE_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>

#include "module_parameters.h"
#include "../debug.h"

#ifndef M_PI
#define M_PI  (3.14159265f)
#endif

#ifndef M_2PI
#define M_2PI  (6.2831853f)
#endif


using namespace std;

void SetNullBuffer(unsigned long size);

//------------------------------------------------------------------------------
/**
 * Wyjscie modulu
 * Klasa obietkow zwiazanych z kazdym wyjsciem modulu. Obiekty
 * te przechowuja miedzy innymi nazwe i identyfikator wjescia a takze
 * odpowiadaja za zarzadzanie buforem wyjsciowym.
 */
class Output {
	public:
				Output(string name_);
				~Output();
		int		GetID() const;
		string	GetName() const;
		float*	GetSignal() const;
		void	SetID(int newID);
		void	SetSignal(float* sig);
		
	protected:
		int		id;		/**< Numer idetyfikujacy wejscie */
		string	name;	/**< Nazwa wejscia */
		float*	signal;	/**< Wskaznik na bufor wyjscia */
};

//------------------------------------------------------------------------------
/**
 * Wejscie modulu.
 * Pelni zadanie podobne jak klasa Output, ale dla wejsc modulu.
 */
class Input {
	public:
				Input(string name_);
				~Input();
		int		GetID() const;
		int		GetIDModule() const;
		int 	GetIDModuleOutput() const;
		string	GetName() const;
		float*	GetSignal();
		void	SetID(int newID);
		void	SetSignal(float* sig);

	protected:
		int		id;		           /**< Numer idetyfikujacy wejscie */
		string	name;	           /**< Nazwa wejscia */
		float*	signal;	           /**< Wskaznik na podlaczony do wejscia bufor */
};

//------------------------------------------------------------------------------
/**
 * Interfejs modulu. Potomkami tej klasy sa wszystkie dostepne w systemie moduly.
 * Takze autorzy zewnetrznych modulow musza dostosowac sie do tego interfejsu.
 * W klasie tej zaimplementowana jest podstawowa funckjoncalnosc dotyczaca dodawania
 * wejsc, wyjsc oraz parmaetrow modulu.
 */
class Module {
  	protected:
        vector<Input*>		inputs;			/**< Wektor wejsc. */
        vector<Output*>		outputs;	    /**< Wektor wyjsc. */
        vector<Parameter*>	parameters;		/**< Wektor parametrow. */
        string	name;	/**< Dowolna nazwa modulu, mozliwa zmianaprzez uzytkownika */
		string	type;	/**< Typ modulu. Musi byc wyjatkowy w systemie, ustalany przez programiste. */

	public:
  		static int		framesPerBlock;
		static float	sampleRate;

				Module(string type_, string name_);
				~Module();
		int		AddInput(Input* input);
		int		AddOutput(Output* output);
		int		AddParameter(Parameter* param);
		void	ConnectInputTo(int numInput, float *sourceSignal);
		virtual void	Process();
		virtual void	Init();
		void	SetName(string newName);
		string	GetType() const;
		string	GetName() const;
		int		GetOutputCount();
		int		GetInputCount();
		int		GetParameterCount() const;
		Input*		GetInput(int inputID);
		Output*		GetOutput(int outputID);
		Parameter*	GetParameter(int pID);
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

#endif // MODULE_H
