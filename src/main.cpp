#include <cstdlib>
#include <iostream>
#include <fstream>

#include "audiodriver.h"
#include "algorithm.h"
#include "xmlconfigfile.h"
#include "callback.h"

#define FRAMES_PER_BLOCK (256)
#define SAMPLE_RATE (44100.0)

using namespace std;

// logowanie do pliku
//ofstream logfile("RTSoundlog.txt");
//streambuf* out = cout.rdbuf(logfile.rdbuf());

//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    Algorithm algo( FRAMES_PER_BLOCK );
	AudioDriver audio;
	XMLConfigFile xmlConfig;

	char c = ' ';
    bool running = false;

   	algo.SetSampleRate( SAMPLE_RATE );

    audio.PrintDevices();

 	try {
		//xmlConfig.OpenFile("../../examples/ADSR test.xml");
		xmlConfig.OpenFile("../../examples/wav.xml");
		//xmlConfig.OpenFile("../../examples/gate.xml");

        algo.Clear(); // test
		xmlConfig.LoadAlgorithm(&algo);
		algo.CreateQueue();

    } catch (RTSError& error) {
        cout << "Error: " << error.what() << endl;
        exit(1);
    }
	try {
		audio.SetCallback(paCallback, (void*)&algo);
		audio.SetSampleRate(SAMPLE_RATE);
		audio.Open(); // gotowi do grania
	}	catch (AudioDriverError& error) {
        cout << "!!" << endl << "!! Error: " << error.what() << endl << "!!" << endl;
        exit(1);
    }
    
    algo.PrintEdges();

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

	//TEST zmiany rozmiaru bloku
//	audio.Stop();
//	audio.Close();
//
//	audio.SetBufferSize(512);
//	algo.SetBlockSize(512);
//
//	audio.Open();
//
//	c = ' ';
//
//     while(c != 'q') {
//		c = cin.get();
//
//		if(!running) {
//			audio.Start();
//			running = true;
//		}
//		else {
//			audio.Stop();
//			running = false;
//		}
//	}

	//system( "PAUSE" );
	return EXIT_SUCCESS;
}
