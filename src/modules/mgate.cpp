#include "mgate.h"

/*
 Modul emitujacy sygnal bramkujacy. Sensownie modul ten wykorzystac mozna jedynie
 w GUI.
*/
MGate::MGate() :
	oOut("gate signal")
{
	AddOutput( oOut );
	enabled = false;
}

MGate::~MGate() {
}

void MGate::Init() {
	// wypelnienie buforu wyjsciowego zerami
	float* out = oOut.GetSignal();
	for( unsigned int i = 0; i < Module::framesPerBlock; ++i )
		*out++ = 0.0f;
	TRACE( "MGate::Init - Zakonczono inicjalizacje\n" );
}

void MGate::Switch() {
	// funckja taj est wywolywana przez GUI
	enabled = !enabled;
    float* out = oOut.GetSignal();
	if( enabled ) {
        //TRACE( "MGate::Switch - Bramka wlaczona\n" );
 		for( unsigned int i = 0; i < Module::framesPerBlock; ++i )
			*out++ = 1.0f;
	}
	else {
        for( unsigned int i = 0; i < Module::framesPerBlock; ++i )
			*out++ = 0.0f;
		//TRACE( "MGate::Switch - Bramka wylaczona\n" );
	}
}

//void MGate::Process() {
//	 nic nie trzeba robic :)
//}

