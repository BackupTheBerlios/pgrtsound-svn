#include <iostream>
//Block
#include "Blocks\Generator.h"
#include "Blocks\Amplifier.h"
#include "Blocks\CConnector.h"
//#include "include\portaudio+v18_1\pa_common\portaudio.h"
// portaudio stuff

#define NUM_SECONDS   (3)
#define SAMPLE_RATE   (44100)
#define FRAMES_PER_BUFFER  (64)
#define OUTPUT_DEVICE Pa_GetDefaultOutputDeviceID()
#ifndef M_PI
#define M_PI  (3.14159265)
#endif
#define TABLE_SIZE   (200)
// EOF portaudio stuff

using namespace std;

//// struktura przekazywana do rtsoundCallback()
//typedef struct
//{
//	Generator *gen;
//	Amplifier *amp;
//	CConnector *conn;
//}
//rtsoundData;

//-----------------------------------------------------------------------------
//static int rtsoundCallback(   void *inputBuffer, void *outputBuffer,
//                             unsigned long framesPerBuffer,
//                             PaTimestamp outTime, void *userData )
//{
//    rtsoundData *data = (rtsoundData*)userData;
//    
//    float *out = (float*)outputBuffer;
//    float *connOut;
//    unsigned long i;
//    int finished = 0;
//    
//    (void) outTime; /* Prevent unused variable warnings. */
//    (void) inputBuffer;
//    
//    //printf("PortAudio Test: framesPerBuffer = %d\n", framesPerBuffer);
//
//	// przetwarzanie 
//    data->gen->Process();
//	data->amp->Process();
//	
//	// zapis do bufora karty
//    connOut = data->conn->Out();
//    for( i = 0; i < framesPerBuffer; i++ )
//    {
//		*out++ = *connOut;		/* left */
//        *out++ = *connOut++;	/* right */
//    }
//    return finished;
//}

//-----------------------------------------------------------------------------
int main (int argc, char *argv[])
{
    cout << "Start system" << endl;
      
    //utworzenie przewodow
    CConnector *conn1 = new CConnector(FRAMES_PER_BUFFER);
    CConnector *conn2 = new CConnector(FRAMES_PER_BUFFER);
        
    //inicjowanie generatora
    Generator *gen       = new Generator();
    gen->BUFFOR_SIZE     = FRAMES_PER_BUFFER;
    gen->param           = new float[*gen->paramCount];
    gen->param[0]        = 0.25;
    gen->param[1]        = 440./44100.;
    gen->param[2]        = 0;
    gen->outConnection	= conn1;
      
    //inicjowanie wzmacniacza
    Amplifier *amp       = new Amplifier();
    amp->BUFFOR_SIZE     = FRAMES_PER_BUFFER;
    amp->param           = new float[*amp->paramCount];
    amp->param[0]        = 2.;  
    amp->inConnection	= conn1;
    amp->outConnection	= conn2;

	cout << "Blocks initialised" << endl;

	// kilka razy (4) wywolujemy Process()
	for(int n = 0; n < 4; n++)
	{
		gen->Process();
		amp->Process();
			
		// konce kabli :)
		float *endBuff1 = conn1->Out();
		float *endBuff2 = conn2->Out();
		
		// drukujemy to co w kalblach
		for(int i = 0; i < FRAMES_PER_BUFFER; i++)
		{
    		cout << "wy gen=" << *endBuff1++ << "     wy amp=" << *endBuff2++ << endl;
		}  
	}
   
	cin.get();

//    PortAudioStream *stream;
//    PaError err;
//    rtsoundData data;
//	int i;
//
//	// struktura przekazywana do rtsoundCallback()
//    data.gen = gen;
//    data.amp = amp;
//    data.conn = conn2;	// skad bierzemy wyjscie
//    
//	// copy & paste z przykladu portadio
//    printf("PortAudio Test: output sine wave. SR = %d, BufSize = %d, devID = %d\n",
//    	SAMPLE_RATE, FRAMES_PER_BUFFER, OUTPUT_DEVICE);
//
//    err = Pa_Initialize();
//    if( err != paNoError ) goto error;
//    
//    err = Pa_OpenStream(
//              &stream,
//              paNoDevice,		/* default input device */
//              0,				/* no input */
//              paFloat32,		/* 32 bit floating point input */
//              NULL,
//              OUTPUT_DEVICE,
//              2,				/* stereo output */
//              paFloat32,		/* 32 bit floating point output */
//              NULL,
//              SAMPLE_RATE,
//              FRAMES_PER_BUFFER,
//              0,				/* number of buffers, if zero then use default minimum */
//              paClipOff,		/* we won't output out of range samples so don't bother clipping them */
//              rtsoundCallback,
//              &data );
//              
//    if( err != paNoError ) goto error;
//    err = Pa_StartStream( stream );
//    if( err != paNoError ) goto error;
//
//    printf("Play for %d seconds.\n", NUM_SECONDS ); fflush(stdout);
//    Pa_Sleep( NUM_SECONDS * 1000 );
//
//    err = Pa_StopStream( stream );
//    if( err != paNoError ) goto error;
//    err = Pa_CloseStream( stream );
//    if( err != paNoError ) goto error;
//    Pa_Terminate();
//    printf("Test finished.\n");
//
//	delete conn1;
//	delete conn2;   
//
//	return err;
//	
//error:
//    Pa_Terminate();
//    fprintf( stderr, "An error occured while using the portaudio stream\n" );
//    fprintf( stderr, "Error number: %d\n", err );
//    fprintf( stderr, "Error message: %s\n", Pa_GetErrorText( err ) );
//    return err;

return 0;
}

