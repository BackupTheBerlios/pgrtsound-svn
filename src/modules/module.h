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
        string		name;	/**< Dowolna nazwa modulu, mozliwa zmianaprzez uzytkownika */
		string		type;	/**< Typ modulu. Musi byc wyjatkowy w systemie, ustalany przez programiste. */

	public:
  		static int		framesPerBlock;
		static float	sampleRate;

				Module(string type_, string name_);
				~Module();
		int		AddInput(Input& input);
		int		AddOutput(Output& output);
		int		AddParameter(Parameter& param);
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
		void		UpdateBlockSize();
		virtual void SampleRateChanged();
		virtual void BlockSizeChanged();
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
	
		NullModuleSingleton() : Module("null", "null"), oNull("null") {
			AddOutput(oNull);
			BlockSizeChanged();
	}

	public:
		static NullModuleSingleton& Instance() {
			return NullModule;
		}

		void BlockSizeChanged() {
       		//TRACE2("NullModule::NullModule()", "Alokuje bufor zerowy rozmiaru ", Module::framesPerBlock);
			float* buff;
			buff = oNull.GetSignal();

			buff = new float[Module::framesPerBlock];
			for(unsigned long i = 0; i < Module::framesPerBlock; i++) {
				*buff++ = 0.0f;
			}
			//TRACE2("module.cpp", "Bufor zerowy zaalokowany ", buff);
		}
};

#endif // MODULE_H
