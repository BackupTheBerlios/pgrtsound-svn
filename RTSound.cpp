#include <iostream>
//Block
#include "Blocks\Generator.h"
#include "Blocks\Amplifier.h"
#include "Blocks\CConnector.h"

#include "Kernel\Loader\Loader.h"
#include "Kernel\Scheduler\Scheduler.h"


#define FRAMES_PER_BUFFER  (256)

using namespace std;



//-----------------------------------------------------------------------------
int main (int argc, char *argv[])
{
    bool isDebugMode = true;
    
    cout << "Start system ..." << endl;

    Scheduler *project = new Scheduler();
    project -> isDebugMode = isDebugMode;
    cout << "Scheduler initialized." << endl;

    Loader *load = new Loader();    
    load->project = project;
    load->Demo();
    cout << "Project loaded." << endl;

    project->CreateTaskList();
    cout << "Created task list." << endl;
   
    return 0;

	cout << "Blocks initialised" << endl;

/*	// kilka razy (4) wywolujemy Process()
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

*/
// jesli RTAudio wlaczone

}

