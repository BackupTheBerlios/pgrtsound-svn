#ifndef GUIMAINWINDOW_H
#define GUIMAINWINDOW_H

#include <gtkmm/stock.h>
#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/button.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/main.h>
#include <gtkmm/filechooserbutton.h>

#include "audiosetupform.h"
#include "algorithmview.h"

/*
 * No description
 */
class GuiMainWindow : public Gtk::Window {
	public:
		GuiMainWindow();
		~GuiMainWindow();
   		void ClearModules();
   		void AllowPlay(bool allow);
   		void AllowStop(bool allow);
   		void OnPlay();
   		void OnStop();
		void OnOpenFile();
   		void OnMenuFileQuit();
		bool OnTimeOut();
		void OnAudioSetup();
		// No description
		void SetStatus( Glib::ustring str );
		void OnMySignal( Glib::ustring str );
		void OnNewFile();
		void OnSaveFile();
		void OnSaveFileAs();


	private:
		AlgorithmView algoView;
		Algorithm *algo;
		AudioDriver audio;

		//bool fileLoaded;
		std::vector<GuiModule*> guiModules;
		std::vector<Gtk::Frame*> guis;
		std::string fileName;
		Gtk::HBox statusBar;
        Gtk::ScrolledWindow scrollWindow;
        Gtk::VBox mainBox;
        Gtk::Label cpuUsageLabel, statusLabel;
		sigc::slot<bool> my_slot; // sygnal aktualziacji obciazenia CPU
		sigc::connection conn;
		Glib::RefPtr<Gtk::UIManager> UIManager;
		Glib::RefPtr<Gtk::ActionGroup> ActionGroup;
};

#endif // GUIMAINWINDOW_H
