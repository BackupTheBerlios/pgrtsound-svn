#include <iostream>
//Block
#include "Blocks\Generator.h"
#include "Blocks\Amplifier.h"
#include "Blocks\CConnector.h"

#if defined(__WINDOWS_DS__)
#include "RTAudio.h"
#endif

#define FRAMES_PER_BUFFER  (256)

using namespace std;

// struktura przekazywana do rtsoundCallback()
typedef struct
{
	Generator *gen;
	Amplifier *amp;
	CConnector *conn;
}
rtsoundData;

//-----------------------------------------------------------------------------
int rtsoundCallback(char *outputBuffer, int bufferSize, void *userData)
{
	 rtsoundData *data = (rtsoundData*)userData;
	
	float *out = (float*)outputBuffer;
    float *connOut;
    int i;
    
   	// przetwarzanie 
    data->gen->Process();
	data->amp->Process();
	
	// zapis do bufora karty
    connOut = data->conn->Out();
    for( i = 0; i < bufferSize; i++ )
    {
		*out++ = *connOut;		/* lewy  */
        *out++ = *connOut++;	/* prawy */
    }
    return 0;
}

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

// jesli RTAudio wlaczone
#if defined(__WINDOWS_DS__)
	int channels = 2;
	int sampleRate = 44100;
	int bufferSize = FRAMES_PER_BUFFER;
	int nBuffers = 24;		// number of internal buffers used by device
	int device = 0;			// 0 indicates the default or first available device
	char input;
	RtAudio *audio;		
    rtsoundData data;
    
    // struktura przekazywana do rtsoundCallback()
    data.gen = gen;
    data.amp = amp;
    data.conn = conn2;	// gdzie wyjscie calego systemu
    
	// Open a stream during RtAudio instantiation
	try
	{
		audio = new RtAudio(device, channels, 0, 0, RTAUDIO_FLOAT32,
                        sampleRate, &bufferSize, nBuffers);
	}
	catch (RtError &error)
	{
		error.printMessage();
		exit(EXIT_FAILURE);
	}

	try
	{
		// Set the stream callback function
		audio->setStreamCallback(&rtsoundCallback, (void *)&data);

		// Start the stream
		audio->startStream();
	}
	catch (RtError &error)
	{
		error.printMessage();
		goto cleanup;
	}

	cout << "\nPlaying ... press <enter> to quit.\n";
	cin.get(input);

	try
	{
		// Stop and close the stream
		audio->stopStream();
		audio->closeStream();
	}
	catch (RtError &error)
	{
		error.printMessage();
	}

	cleanup:
		delete audio;
#endif

	return 0;
}

