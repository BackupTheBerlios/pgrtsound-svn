#ifndef GWAVPLAYER_H
#define GWAVPLAYER_H

#include "mwavplayer.h"
#include "../../gui2/guis/gui.h"

#include <gtkmm/button.h>
#include <gtkmm/label.h>
#include <gtkmm/box.h>

/**
 No description
*/
class GWavPlayer : public Gui {
	public:
		GWavPlayer( Module* mod  );
		~GWavPlayer();
		virtual Gtk::Widget* GetGui();
		REGISTER_GUI( MWavPlayer::GetTypeStatic(), GWavPlayer )
		void OnFileOpen();
		
	protected:
		Gtk::VBox box;
		Gtk::Button openFileButton;
		Gtk::Label  fileLabel;
};

#endif // GWAVPLAYER_H
