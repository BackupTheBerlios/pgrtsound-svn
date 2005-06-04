#include "rtmainwindow.h"
#include "../callback.h"

RTMainWindow::RTMainWindow() : mainBox(false, 0), algo(256) {
	set_title("Real Time GUI");
	set_size_request(300, 550);

	TRACE("RTMainWindow::RTMainWindow()", "Tworze okno...");

	// menu i akcje
	m_refActionGroup = Gtk::ActionGroup::create();

	//File menu:
	m_refActionGroup->add( Gtk::Action::create("FileMenu", "File") );
	m_refActionGroup->add( Gtk::Action::create("OpenFile", Gtk::Stock::OPEN),
		sigc::mem_fun(*this, &RTMainWindow::OnOpenFile) );
	m_refActionGroup->add( Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
		sigc::mem_fun(*this, &RTMainWindow::OnMenuFileQuit) );

	//Transport menu:
	m_refActionGroup->add( Gtk::Action::create("TransportMenu", "Transport") );
	m_refActionGroup->add( Gtk::Action::create("Play", Gtk::Stock::MEDIA_PLAY, "Play", "Starts audio"),
		sigc::mem_fun(*this, &RTMainWindow::OnPlay) );
	m_refActionGroup->add( Gtk::Action::create("Stop", Gtk::Stock::MEDIA_STOP, "Stop", "Stops audio"),
		sigc::mem_fun(*this, &RTMainWindow::OnStop) );

	//Help menu:
	//  m_refActionGroup->add( Gtk::Action::create("HelpMenu", "Help") );
	m_refUIManager = Gtk::UIManager::create();
	m_refUIManager->insert_action_group(m_refActionGroup);
	add_accel_group(m_refUIManager->get_accel_group());

	//Layout the actions in a menubar and toolbar:
	try	{
		Glib::ustring ui_info =
		    "<ui>"
		    "  <menubar name='MenuBar'>"
		    "    <menu action='FileMenu'>"
		    "      <menuitem action='OpenFile'/>"
		    "      <separator/>"
		    "      <menuitem action='FileQuit'/>"
		    "    </menu>"
		    "    <menu action='TransportMenu'>"
		    "      <menuitem action='Play'/>"
		    "      <menuitem action='Stop'/>"
		    "    </menu>"
		    "  </menubar>"
		    "  <toolbar  name='ToolBar'>"
		    "    <toolitem action='Play'/>"
		    "    <toolitem action='Stop'/>"
		    "    <toolitem action='FileQuit'/>"
		    "  </toolbar>"
		    "</ui>";

			m_refUIManager->add_ui_from_string(ui_info);
	}
	catch(const Glib::Error& ex) {
		std::cerr << "building menus failed: " <<  ex.what();
	}
	
    add(mainBox);

   	//Get the menubar and toolbar widgets, and add them to a container widget:
	Gtk::Widget* pMenubar = m_refUIManager->get_widget("/MenuBar");
	if(pMenubar)
    	mainBox.pack_start(*pMenubar, Gtk::PACK_SHRINK);
    	
	Gtk::Widget* pToolbar = m_refUIManager->get_widget("/ToolBar") ;
	if(pToolbar)
		mainBox.pack_start(*pToolbar, Gtk::PACK_SHRINK);

	mainBox.set_homogeneous(false);
    mainBox.pack_start(scrollWindow);
    mainBox.pack_end(cpuUsageLabel, Gtk::PACK_SHRINK);

	cpuUsageLabel.set_justify(Gtk::JUSTIFY_LEFT);
    cpuUsageLabel.set_label("CPU usage");

   	scrollWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	modulesBox.set_border_width(5);
	modulesBox.set_homogeneous(false);
	modulesBox.set_spacing(5);

	scrollWindow.add(modulesBox);

	// timer
	my_slot = sigc::mem_fun(*this, &RTMainWindow::OnTimeOut);
	conn = Glib::signal_timeout().connect(my_slot, 1000);

	TRACE("RTMainWindow::RTMainWindow()", "Okno aplikacji utworzone");
}

RTMainWindow::~RTMainWindow()
{
	TRACE("RTMainWindow::~RTMainWindow()", "Usuwanie parametrow i modulow...");
//	for(int i = 0; i < guiParameters.size(); i++) {
//		delete guiParameters[i];
//	}

	for(int i = 0; i < guiModules.size(); i++) {
		delete guiModules[i];
	}
	TRACE("RTMainWindow::~RTMainWindow()", "Usuniete");
}

void RTMainWindow::AddModule(Module* module) {
	GuiParameterSlider*	slider;
	GuiModule*			guiModule;
	Parameter*			param;
	int					paramCount;

	paramCount = module->GetParameterCount();

	for(int m = 0; m < paramCount; m++) {
		param = module->GetParameter(m);

		if((param->GetGUIType() != gtInvisible) && (param->GetGUIType() != gtProperty) ) {
           	TRACE2("RTMainWindow::AddModule()", "Dodaje modul ", module->GetName());
			// beda jakies widoczne parametry tego modulu wiec go dodajmy
			guiModule = new GuiModule;
			guiModules.push_back(guiModule);
			guiModule->set_label( module->GetName() );
			
			for(int i = 0; i < paramCount; i++) {
				param = module->GetParameter(i);
				if(param->GetGUIType() != gtInvisible) {
					guiModule->AddParameter(param);
				}
			}
			
			modulesBox.pack_start(*guiModule, Gtk::PACK_SHRINK);
			TRACE("RTMainWindow::AddModule()", "Modul dodany");
			break;
		}
	}
}

void RTMainWindow::OnOpenFile() {
	Gtk::FileChooserDialog dialog("Please choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);
	//Add response buttons the the dialog:
	dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
	dialog.add_button(Gtk::Stock::OPEN, Gtk::RESPONSE_OK);

	//Add filters, so that only certain file types can be selected:
	Gtk::FileFilter filter_text;
	filter_text.set_name("Algortihm XML files");
	//filter_text.add_mime_type("text/xml");
	filter_text.add_pattern("*.xml");
	dialog.add_filter(filter_text);

	//Show the dialog and wait for a user response:
	int result = dialog.run();

	switch(result) {
		case(Gtk::RESPONSE_OK): {
			std::cout << "Open clicked." << std::endl;
			std::string filename = dialog.get_filename(); //Notice that it is a std::string, not a Glib::ustring.
			std::cout << "File selected: " <<  filename << std::endl;

			try {
		        audio.Stop();
				audio.Close();
			}	catch (AudioDriverError& error) {
		        cout << "!!" << endl << "!! Error: " << error.what() << endl << "!!" << endl;
		        exit(1);
		    }

			try {
			    xmlConfig.OpenFile(filename.c_str());
    			//algo.SetFramesPerBlock(256);
				algo.SetSampleRate(44100);
				algo.Clear();
				xmlConfig.LoadAlgorithm(&algo);
				algo.Init();
				algo.CreateQueue();
		    } catch (RTSError& error) {
		        cout << "Error: " << error.what() << endl;
		        exit(1);
		    }

			try {
		        //audio.PrintDevices();
				audio.SetCallback(paCallback, (void*)&algo);
				audio.EnableInput();
				audio.Open(44100.0, 256, 0); // gotowi do grania
			}	catch (AudioDriverError& error) {
		        cout << "!!" << endl << "!! Error: " << error.what() << endl << "!!" << endl;
		        exit(1);
		    }
		    
			ClearModules();

			Module* mod;
			for(mod = algo.GetFirstModule(); mod; mod = algo.GetNextModule()) {
				if(mod->GetParameterCount() > 0) AddModule(mod);
			}

			modulesBox.show_all_children();
			break;
		}
	}
}

void RTMainWindow::OnPlay() {
	audio.Start();
}

void RTMainWindow::OnStop() {
    audio.Stop();
}

void RTMainWindow::OnMenuFileQuit() {
	hide();
}

void RTMainWindow::ClearModules() {
	TRACE("RTMainWindow::ClearModules()", "Czyszcze moduly...");
	for(int i = 0; i < guiModules.size(); i++) {
		guiModules[i]->Clear();
		delete guiModules[i];
	}

	guiModules.clear();
	TRACE("RTMainWindow::ClearModules()", "Moduly wyczyszczone");
	show_all_children();
}

bool RTMainWindow::OnTimeOut() {
	char txt[20];
    g_snprintf(txt, 20, "CPU Usage: %#.2f %%", audio.GetCPUUsage()*100);
	cpuUsageLabel.set_label(txt);
	
	return true;
}
