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
int SDL_main(int argc, char *argv[]) {
	SDL_Event event;
    int quit = 0;
	AudioDriver audio;
    Algorithm algo;
    
    
    //--------------------------------------------------------------------------
	// SDL
	// Initialise SDL 
	if( SDL_Init( SDL_INIT_VIDEO ) < 0){
	    fprintf( stderr, "Could not initialise SDL: %s\n", SDL_GetError() );
	    exit( -1 );
	}
	
	// Set a video mode 
	if( !SDL_SetVideoMode( 320, 200, 32, SDL_HWSURFACE|SDL_DOUBLEBUF ) ){
	    fprintf( stderr, "Could not set video mode: %s\n", SDL_GetError() );
	    SDL_Quit();
	    exit( -1 );
	}
   
    //--------------------------------------------------------------------------	
    
	
    // wazne tu i teraz
    algo.setFramesPerBlock(FRAMES_PER_BLOCK);
    algo.setSampleRate(SAMPLE_RATE);
    
    // DEMO
    //      
    // Const.(freq_mod) --> SinOsc(mod) --> (Gain)gain_mod --> Sum(sum) --> SinOsc(osc) --> AudioPortOut
	//                                                              ^
    //                                                              |
    //                                           (SinOsc)freq_osc ---
/*	int osc = algo.addModule("SinOsc");           //1
    int mod = algo.addModule("SinOsc");           //2
    int freq_osc = algo.addModule("Constant");    //3
    int freq_mod = algo.addModule("Constant");    //4
    int gain_mod = algo.addModule("Gain");        //5
    int sum = algo.addModule("Sumator");          //6

 	algo.connectModules(1, 0, 0, 0);	// 0 = AudioPortOut
	algo.connectModules(2, 0, 5, 0);
	algo.connectModules(3, 0, 6, 1);
    algo.connectModules(4, 0, 2, 0);
    algo.connectModules(5, 0, 6, 0);
	algo.connectModules(6, 0, 1, 0);  
	
	algo.module(freq_mod)->setParam(0, 5);		// czest. modulacji
	algo.module(gain_mod)->setParam(0, 150);	// glebokosc modulacji
	algo.module(freq_osc)->setParam(0, 440);	// czest. nosnej
*/	   


    cout <<"Wczytywanie modulow..." << endl;    
    algo.LoadModulesFromFile("d:\\DemoModules.xml");  
      
    cout <<"Tworzenie macierzy sasiedztwa..." << endl; 
    algo.CreateAdjacencyMatrix();
    
    cout <<"Wczytywanie powiazan..." << endl;    
    algo.LoadConnectionsFromFile("d:\\DemoConnections.xml");    
    
    cout <<"Wczytywanie parametrów..." << endl;    
    algo.LoadParametersFromFile("d:\\DemoParameters.xml");
   
	// ustalenie kolejnosci	 
    algo.CreateQueue();

	
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
					case SDLK_q:
						algo.module(4)->setParam(0, 5);
						break;
					case SDLK_w:
						algo.module(4)->setParam(0, 100);
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
