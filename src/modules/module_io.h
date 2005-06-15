#ifndef MODULE_IO_H
#define MODULE_IO_H

#include <string>

using namespace std;

/**
 * Wyjscie modulu.
 * Klasa obietkow zwiazanych z kazdym wyjsciem modulu. Obiekty
 * te przechowuja miedzy innymi nazwe i identyfikator wjescia a takze
 * buforem wyjsciowym.
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
		int		id;		// numer idetyfikujacy wejscie
		string	name;	// nazwa wejscia
		float*	signal;	// wskaznik na bufor wyjsciowy
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

#endif // MODULE_IO_H
