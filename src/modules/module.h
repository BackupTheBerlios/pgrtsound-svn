#ifndef MODULE_H
#define MODULE_H

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#ifndef M_PI
#define M_PI  (3.14159265)
#endif

using namespace std;

//------------------------------------------------------------------------------
/**
 * Wyjscie modulu
 * Jest to prosty kontener danych zwiazanych z kazdym wyjsciem modulu. Obiekty
 * tej klasy przechowuja miedzy innymi nazwe i identyfikator wjescia a takze
 * odpowiadaja za zarzadzanie buforem wyjsciowym.
 */
class Output {
	public:
		int		id;		/**< Numer idetyfikujacy wejscie */
		string	name;	/**< Nazwa wejscia */
		float*	signal;	/**< Wskaznik na bufor wyjsciowy */
};

//------------------------------------------------------------------------------
/**
 * Wejscie modulu.
 * Pelni zadanie podobne jak klasa Output, ale dla wejsc modulu.
 */
class Input {
	public:
		int		id;					/**< Numer idetyfikujacy wejscie */
		string	name;				/**< Nazwa wejscia */
		float*	signal;				/**< Wskaznik na podlaczony do wejscia bufor */
		
		/**
		 * Przeladowyany operator = sluzy do ustanowiania polaczen miedzy
		 * wejsciem i wyjsciem dwoch modulow.
		 */
		Input&	operator=(Output& out) {
			// poczatek bufora skad czerpac sygnal
			this->signal = out.signal;
			return *this;									
		}
};

//------------------------------------------------------------------------------
/** Struktura parametru modulu */
struct Param {
	int		id;
	string	name;
	float	value;
};

//------------------------------------------------------------------------------
/**
 * Interfejs modulu. Potomkami tej klasy sa wszystkie dostepne w systemie moduly.
 * Takze autorzy zewnetrznych modulow musza dostosowac sie do tego interfejsu.
 * W klasie tej zaimplementowana jest podstawowa funckjoncalnosc dotyczaca dodawania
 * wejsc, wyjsc oraz parmaetrow modulu.
 */
class Module {
	public:
		string type;
		int	id;	/**< Identyfikator modulu. */
		static int framesPerBlock;
		static int sampleRate;
		
		Module();
		~Module();
		int addInput(string name);
		int addOutput(string name);
		int addParam(string name);
		void setParam(int paramNum, float value);
		float param(int paramNum);
		Input& input(int inputNum);
		Output& output(int outputNum);
		virtual void process();
		
	private:
		vector<Param> params;	/**< Wektor parametrow. */
		vector<Input> inputs;	/**< Wektor wejsc. */
		vector<Output> outputs;	/**< Wektor wyjsc. */
};

#endif // MODULE_H
