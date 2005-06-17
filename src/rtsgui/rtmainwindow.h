#ifndef RTMAINWINDOW_H
#define RTMAINWINDOW_H

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

/*
 * No description
 */
class RTMainWindow : public Gtk::Window {
	public:
		RTMainWindow();
		~RTMainWindow();
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
        Algorithm				algo;
        GuiModuleFactory        guiModuleFactory;
		XMLConfigFile			xmlConfig;
		AudioDriver				audio;
		bool                    fileLoaded;
		std::vector<GuiModule*>	guiModules;
		std::vector<Gtk::Frame*>	guis;
        Gtk::ScrolledWindow		scrollWindow;
        Gtk::VBox				mainBox, modulesBox;
        Gtk::Label              cpuUsageLabel;
		sigc::slot<bool>		my_slot;
		sigc::connection		conn;
		Glib::RefPtr<Gtk::UIManager>      UIManager;
		Glib::RefPtr<Gtk::ActionGroup>    ActionGroup;
};

#endif // RTMAINWINDOW_H
