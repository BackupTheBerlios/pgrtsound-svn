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
	AudioDriver audio;
    Algorithm algo(FRAMES_PER_BLOCK);
	XMLConfigFile xmlConfig;
	char c = ' ';
    bool running = false;

    algo.SetSampleRate(SAMPLE_RATE);
    
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

	algo.PrintInfo();
	ModuleId mid  = algo.AddModule("gain", "moje gajn");
	algo.PrintInfo();
	algo.DeleteModule(mid);
	
	try {
		audio.PrintDevices();
		audio.SetCallback(paCallback, (void*)&algo);
		audio.EnableInput();
		audio.Open(SAMPLE_RATE, FRAMES_PER_BLOCK, 0); // gotowi do grania
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


	return EXIT_SUCCESS;
}
