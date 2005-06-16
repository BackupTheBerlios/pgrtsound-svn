#include <cstdlib>
#include <iostream>

#include "audiodriver.h"
#include "algorithm.h"
#include "xmlconfigfile.h"
#include "callback.h"

#define FRAMES_PER_BLOCK (256)
#define SAMPLE_RATE (44100)

using namespace std;

// logowanie do pliku
ofstream logfile("RTSoundlog.txt");
streambuf* out = cout.rdbuf(logfile.rdbuf());

//------------------------------------------------------------------------------
int main(int argc, char *argv[]) {
    Algorithm algo(FRAMES_PER_BLOCK);
	AudioDriver audio;
	XMLConfigFile xmlConfig;

	char c = ' ';
    bool running = false;

    algo.SetSampleRate(SAMPLE_RATE);

    audio.PrintDevices();
    
 	try {
        xmlConfig.OpenFile("../../examples/fm2.xml");
        algo.Clear(); // test
		xmlConfig.LoadAlgorithm(&algo);
		algo.Init();
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
	
	algo.Clear();

	return EXIT_SUCCESS;
}
