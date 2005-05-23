#ifndef RTMAINWINDOW_H
#define RTMAINWINDOW_H

//#include <gtkmm/main.h>
//#include <gtkmm/button.h>
//#include <gtkmm/window.h>
//#include <gtkmm/frame.h>
//#include <gtkmm/notebook.h>

#include <vector>
#include <gtkmm.h>

#include "xmlconfigfile.h"
#include "guimodule.h"

/*
 * No description
 */
class RTMainWindow : public Gtk::Window {
	public:
		RTMainWindow();
		~RTMainWindow();
   		void AddModule(Module* module);
   		void ClearModules();
   		void OnPlay();
   		void OnStop();
		void OnOpenFile();
   		void OnMenuFileQuit();
		bool OnTimeOut();

	private:
		std::vector<GuiModule*>	guiModules;
        Gtk::ScrolledWindow		scrollWindow;
        Gtk::VBox				mainBox, modulesBox;
        Gtk::Label              cpuUsageLabel;

		Algorithm				algo;
		XMLConfigFile			xmlConfig;
		AudioDriver				audio;
		
		Glib::RefPtr<Gtk::UIManager>      m_refUIManager;
		Glib::RefPtr<Gtk::ActionGroup>    m_refActionGroup;
		
		sigc::slot<bool>	my_slot;
		sigc::connection	conn;
};

#endif // RTMAINWINDOW_H
