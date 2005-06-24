#ifndef MODULE_IO_H
#define MODULE_IO_H

#include <string>

#include "../debug.h"

using namespace std;

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
		
	protected:
		Output* outputConnected; /**< Wkaznik do wyjscia, do ktorego wejscie jest podlaczone */
		int id;     /**< Numer idetyfikujacy wejscie */
		string name;    /**< Nazwa wejscia */
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

#endif // MODULE_IO_H
