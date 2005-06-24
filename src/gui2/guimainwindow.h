#ifndef GUIMAINWINDOW_H
#define GUIMAINWINDOW_H

#include <vector>

#include <gtkmm/window.h>
#include <gtkmm/box.h>
#include <gtkmm/label.h>
#include <gtkmm/enums.h>
#include <gtkmm/button.h>
#include <gtkmm/stock.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/uimanager.h>
#include <gtkmm/main.h>
#include <gtkmm/filechooserbutton.h>

#include "../xmlconfigfile.h"
#include "../modules/module.h"
#include "../gui/guimodulefactory.h"
#include "audiosetupform.h"
#include "algorithmview.h"

/*
 * No description
 */
class GuiMainWindow : public Gtk::Window {
	public:
		GuiMainWindow();
		~GuiMainWindow();
   		void AddModule(Module* module);
   		void ClearModules();
   		void AllowPlay(bool allow);
   		void AllowStop(bool allow);
   		void OnPlay();
   		void OnStop();
		void OnOpenFile();
   		void OnMenuFileQuit();
		bool OnTimeOut();
		void OnAudioSetup();

	private:
        Algorithm					algo;
		AlgorithmView               algoView;
		XMLConfigFile				xmlConfig;
		AudioDriver					audio;
		
		bool                    	fileLoaded;
		std::vector<GuiModule*>		guiModules;
		std::vector<Gtk::Frame*>	guis;
        Gtk::ScrolledWindow			scrollWindow;
        Gtk::VBox					mainBox;
        Gtk::Label              	cpuUsageLabel;
		sigc::slot<bool>			my_slot; // sygnal aktualziacji obciazenia CPU
		sigc::connection			conn;
		Glib::RefPtr<Gtk::UIManager>      UIManager;
		Glib::RefPtr<Gtk::ActionGroup>    ActionGroup;
};

#endif // GUIMAINWINDOW_H
