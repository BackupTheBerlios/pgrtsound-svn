#ifndef GMFFT_H
#define GMFFT_H

#include "gui.h"
#include "mygtkfloatentry.h"
#include "../../modules/fft.h"

#include <gtkmm/button.h>
#include <gtkmm/box.h>

/*
 * No description
 */
class GFFT : public Gui
{
	public:
		GFFT( Module* mod );
		~GFFT();
		Gtk::Widget* GetGui();
		REGISTER_GUI( FFT::GetTypeStatic(), GFFT )
		void OnApply();

	protected:
        MyGtkFloatEntry entry;
        Gtk::HBox box;
		Gtk::Button buttonApply;
};

#endif // GMFFT_H
