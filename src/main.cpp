#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>
#include <math.h>

#include "SDL.h"
#include "audiodriver.h"
#include "algorithm.h"

#define FRAMES_PER_BLOCK (128)
#define SAMPLE_RATE (44100)

using namespace std;

//------------------------------------------------------------------------------
int SDL_main(int argc, char *argv[]) {
	SDL_Event event;
    int quit = 0;
	AudioDriver audio;
    Algorithm algo;
        
	//--------------------------------------------------------------------------
	// SDL
	/* Initialise SDL */
	if( SDL_Init( SDL_INIT_VIDEO ) < 0){
	    fprintf( stderr, "Could not initialise SDL: %s\n", SDL_GetError() );
	    exit( -1 );
	}
	
	/* Set a video mode */
	if( !SDL_SetVideoMode( 320, 200, 32, SDL_HWSURFACE|SDL_DOUBLEBUF ) ){
	    fprintf( stderr, "Could not set video mode: %s\n", SDL_GetError() );
	    SDL_Quit();
	    exit( -1 );
	}
   
    //--------------------------------------------------------------------------
    // wazna kolejnosc
    algo.setFramesPerBlock(FRAMES_PER_BLOCK);
    algo.setSampleRate(SAMPLE_RATE);
    
    // DEMO
    //      
    // Const.(freq_mod) --> SinOsc(mod) --> (Gain)gain_mod --> Sum(sum) --> SinOsc(osc) --> AudioPortOut
	//                                                              ^
    //                                                              |
    //                                           (SinOsc)freq_osc ---
	int osc = algo.addModule("SinOsc");
    int mod = algo.addModule("SinOsc");
    int freq_osc = algo.addModule("Constant");
    int freq_mod = algo.addModule("Constant");
    int gain_mod = algo.addModule("Gain");
    int sum = algo.addModule("Sumator");
        
	algo.connectModules(freq_mod, 0, mod, 0);
	algo.connectModules(mod, 0, gain_mod, 0);
	algo.connectModules(gain_mod, 0, sum, 0);
	algo.connectModules(freq_osc, 0, sum, 1);
	algo.connectModules(sum, 0, osc, 0);
	algo.connectModules(osc, 0, 0, 0);	// 0 = AudioPortOut
	
	algo.module(freq_mod)->setParam(0, 5);
	algo.module(gain_mod)->setParam(0, 150);	// glebokosc modulacji
	algo.module(freq_osc)->setParam(0, 440);
	
	// reczne ustalenie kolejnosci	
	int order[] = {freq_mod, mod, gain_mod, freq_osc, sum, osc, 0};
	algo.setQueueManually(order, 7);
    algo.printInfo();
	
	audio.setCallback((void*)&algo);
	audio.init(SAMPLE_RATE, 16, FRAMES_PER_BLOCK, 0);
	
	while(SDL_WaitEvent( &event ) and !quit) {
		switch( event.type ) {
            case SDL_KEYDOWN:
				switch(event.key.keysym.sym) {
					case SDLK_F5:
						audio.start();
						break;
					case SDLK_F8:
						audio.stop();
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
    
	SDL_Quit();
}
