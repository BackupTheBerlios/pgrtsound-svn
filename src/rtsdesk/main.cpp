#include <gtkmm.h>

#include "MainForm.h"
#include "desk.h"

#define FRAMES_PER_BLOCK (64)
#define SAMPLE_RATE (44100)

int main (int argc, char *argv[])
{
    Algorithm *algo= new Algorithm(FRAMES_PER_BLOCK);
    //algo->SetFramesPerBlock(FRAMES_PER_BLOCK);
    algo->SetSampleRate(SAMPLE_RATE);

    Desk *desk = new Desk(algo);

    Gtk::Main kit(argc, argv, false);

    MainForm mainForm(desk);
    Gtk::Main::run(mainForm);

    return EXIT_SUCCESS;
} 
