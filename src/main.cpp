#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>

#include "SDL/SDL.h"
#include "audiodriver.h"
#include "algorithm.h"

#define FRAMES_PER_BLOCK (128)
#define SAMPLE_RATE (44100)

using namespace std;

//----SDL_---------------------------------------------------------------------------
int main(int argc, char *argv[]) {
	SDL_Event event;
    int quit = 0;
	AudioDriver audio;
    Algorithm algo;
    
    //--------------------------------------------------------------------------
	// SDL
	// Initialise SDL 
	if( SDL_Init( SDL_INIT_VIDEO ) < 0) {
	    fprintf( stderr, "Could not initialise SDL: %s\n", SDL_GetError() );
	    exit( -1 );
	}
	
	// Set a video mode 
	if( !SDL_SetVideoMode( 320, 200, 32, SDL_HWSURFACE|SDL_DOUBLEBUF ) ) {
	    fprintf( stderr, "Could not set video mode: %s\n", SDL_GetError() );
	    SDL_Quit();
	    exit( -1 );
	}

    //--------------------------------------------------------------------------	
    // wazne tu i teraz
    algo.SetFramesPerBlock(FRAMES_PER_BLOCK);
    algo.SetSampleRate(SAMPLE_RATE);
    
	char fileName[] = "examples/fm2.xml";
	try {
		algo.LoadModulesFromFile(fileName);
		algo.CreateAdjacencyMatrix();
		algo.LoadConnectionsFromFile(fileName);
	    algo.LoadParametersFromFile(fileName);
		algo.CreateQueue();
    } catch (RTSError& error) {
        cout << "Error: " << error.what() << endl;
    }
    
	//algo.printInfo();

	float freq = 220;
	float freq_mod = 5;

	audio.SetCallback((void*)&algo);
	audio.Init(SAMPLE_RATE, 16, FRAMES_PER_BLOCK, 0);
	  	
	while(SDL_WaitEvent( &event ) and !quit) {
		switch( event.type ) {
            case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_F5:
						//cout << "F5" << endl;
						audio.Start();
						break;
					case SDLK_F8:
                        //cout << "F8" << endl;
						audio.Stop();
						break;

//					// demo
//					case SDLK_UP:
//						freq = freq + 10;
//						((ParameterFloat*)algo.modules[5]->GetParameter(0))->SetValue(freq);
//						break;
//
//					case SDLK_DOWN:
//						freq = freq - 10;
//						//algo.modules[5]->SetParam(0, freq);
//						break;
//
//   					case SDLK_RIGHT:
//						freq_mod = freq_mod + 1;
//						//algo.modules[3]->SetParam(0, freq_mod);
//						break;
//
//					case SDLK_LEFT:
//						freq_mod = freq_mod - 1;
//						//algo.modules[3]->SetParam(0, freq_mod);
//						break;
//					// eof demo

					default:
						break;
				}
				break;
				
            case SDL_QUIT:
				quit = 1;
				break;

			default:
				break;
		}
	}

	audio.Close();
	SDL_Quit();
}
