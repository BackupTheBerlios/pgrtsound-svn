#include <cstdlib>
#include <iostream>

#include "audiodriver.h"
#include "algorithm.h"
#include "xmlconfigfile.h"

#define FRAMES_PER_BLOCK (256)
#define SAMPLE_RATE (44100)

using namespace std;

// logowanie do pliku
ofstream logfile("RTSoundlog.txt");
streambuf* out = cout.rdbuf(logfile.rdbuf());

//------------------------------------------------------------------------------
// callback dla portuadio
static int paCallback( void *inputBuffer, void *outputBuffer,
	unsigned long framesPerBuffer, PaTimestamp outTime, void *userData )
{
	unsigned long i;

	Algorithm* alg = (Algorithm*)userData;

	float* put = alg->GetModule(0)->GetOutput(0)->GetSignal();	// AudioPortIn
	float* gen = alg->GetModule(1)->GetInput(0)->GetSignal();   // AudioPortOut
	float* in = (float*)inputBuffer;
	float* out = (float*)outputBuffer;
	
	// algorytm oblicza blok probek
	alg->Process();

	if(in == NULL) {
		for(i = 0; i < framesPerBuffer; i++) {
			/* TODO (#1#): Zaimplementowac kanal lewy i prawy */
			*out++ = *gen;
			*out++ = *gen++;
		}
	}
	else {
        for(i = 0; i < framesPerBuffer; i++) {
			/* TODO (#1#): Zaimplementowac kanal lewy i prawy */
			*put++ = ( (*in++) + (*in++) )*0.5; // na razie miksowanie do mono
			*out++ = *gen;
			*out++ = *gen++;
		}
	}
	
	return 0;
}

//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
	AudioDriver audio;
    Algorithm algo;
	XMLConfigFile xmlConfig;

    algo.SetFramesPerBlock(FRAMES_PER_BLOCK);
    algo.SetSampleRate(SAMPLE_RATE);
    
	xmlConfig.OpenFile("examples/fm2.xml");

	try {
		//xmlConfig.LoadModules(&algo);
		//xmlConfig.LoadConnections(&algo);
		//xmlConfig.LoadParameters(&algo);

		xmlConfig.LoadAlgorithm(&algo);
		algo.Init();
		algo.CreateQueue();
    } catch (RTSError& error) {
        cout << "Error: " << error.what() << endl;
        exit(1);
    }

	algo.PrintInfo();

	try {
		audio.PrintDevices();
		audio.SetCallback(paCallback, (void*)&algo);
		audio.EnableInput();
		audio.Open(SAMPLE_RATE, FRAMES_PER_BLOCK, 0); // gotowi do grania
	}	catch (AudioDriverError& error) {
        cout << "!!" << endl << "!! Error: " << error.what() << endl << "!!" << endl;
        exit(1);
    }

    char c;
    bool running = false;

    while(c != 'q') {
		c = cin.get();

		if(!running) {
			audio.Start();
			running = true;
		}
		else {
			audio.Stop();
			running = false;
		}
	}


	return EXIT_SUCCESS;
}
