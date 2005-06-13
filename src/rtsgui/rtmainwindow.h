#ifndef RTMAINWINDOW_H
#define RTMAINWINDOW_H

#include <vector>
#include <gtkmm.h>

#include "../xmlconfigfile.h"
#include "../modules/module.h"

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
		std::vector<ModuleGui*>	guiModules;
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
