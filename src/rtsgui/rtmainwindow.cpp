#include "rtmainwindow.h"
#include "../callback.h"

#define FRAMES_PER_BUFFER 256

RTMainWindow::RTMainWindow() : mainBox(false, 0), algo(FRAMES_PER_BUFFER) {
	TRACE("RTMainWindow::RTMainWindow()", "Tworze okno...");

	set_title("Real Time GUI");
	set_size_request(300, 550);

	// menu i akcje
	ActionGroup = Gtk::ActionGroup::create();

	//File menu:
	ActionGroup->add( Gtk::Action::create("FileMenu", "File") );
	ActionGroup->add( Gtk::Action::create("OpenFile", Gtk::Stock::OPEN),
		sigc::mem_fun(*this, &RTMainWindow::OnOpenFile) );
	ActionGroup->add( Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
		sigc::mem_fun(*this, &RTMainWindow::OnMenuFileQuit) );

	//Transport menu:
	ActionGroup->add( Gtk::Action::create("TransportMenu", "Transport") );
	ActionGroup->add( Gtk::Action::create("Play", Gtk::Stock::MEDIA_PLAY, "Play", "Starts audio"),
		sigc::mem_fun(*this, &RTMainWindow::OnPlay) );
	ActionGroup->add( Gtk::Action::create("Stop", Gtk::Stock::MEDIA_STOP, "Stop", "Stops audio"),
		sigc::mem_fun(*this, &RTMainWindow::OnStop) );

	// Setup menu
	ActionGroup->add( Gtk::Action::create("SetupMenu", "Setup") );
	ActionGroup->add( Gtk::Action::create("AudioDevices", Gtk::Stock::PREFERENCES, "Audio devices...", "Setup audio devices"),
		sigc::mem_fun(*this, &RTMainWindow::OnAudioSetup) );

	//Help menu:
	//  m_refActionGroup->add( Gtk::Action::create("HelpMenu", "Help") );
	UIManager = Gtk::UIManager::create();
	UIManager->insert_action_group(ActionGroup);
	add_accel_group(UIManager->get_accel_group());

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
   		    "    <menu action='SetupMenu'>"
		    "      <menuitem action='AudioDevices'/>"
		    "    </menu>"
		    "  </menubar>"
		    "  <toolbar  name='ToolBar'>"
		    "    <toolitem action='Play'/>"
		    "    <toolitem action='Stop'/>"
		    "    <toolitem action='FileQuit'/>"
		    "  </toolbar>"
		    "</ui>";
			UIManager->add_ui_from_string(ui_info);
	}
	catch(const Glib::Error& ex) {
		std::cerr << "building menus failed: " <<  ex.what();
	}

    add(mainBox);

   	//Get the menubar and toolbar widgets, and add them to a container widget:
	Gtk::Widget* pMenubar = UIManager->get_widget("/MenuBar");
	if(pMenubar)
    	mainBox.pack_start(*pMenubar, Gtk::PACK_SHRINK);
    	
	Gtk::Widget* pToolbar = UIManager->get_widget("/ToolBar") ;
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

	// timer CPU usage
	my_slot = sigc::mem_fun(*this, &RTMainWindow::OnTimeOut);
	conn = Glib::signal_timeout().connect(my_slot, 1000);

	fileLoaded = false;
	AllowPlay(false);
	AllowStop(false);

	TRACE("RTMainWindow::RTMainWindow()", "Okno aplikacji utworzone");
}

RTMainWindow::~RTMainWindow() {
	TRACE("RTMainWindow::~RTMainWindow()", "Usuwanie parametrow i modulow...");
	ClearModules();
	TRACE("RTMainWindow::~RTMainWindow()", "Usuniete");
}

void RTMainWindow::AddModule(Module* module) {
   	TRACE3("RTMainWindow::AddModule()", "Dodaje GUI dla modul '", module->GetName(), "'");
    GuiModule*		guiModule;
    Gtk::Widget*	gui;

	// tworzymy GuiModule
	guiModule = guiModuleFactory.CreateGuiModule(module);
	guiModules.push_back(guiModule);
	
	gui = guiModule->GetGui();
	if(gui != NULL) {
		// jest GUI, wiec tworzymy ramke i dodajemy je
		Gtk::Frame* frame = new Gtk::Frame;
		guis.push_back(frame);
		frame->add(*gui);
		frame->set_label( module->GetName() );
		modulesBox.pack_start(*frame, Gtk::PACK_SHRINK);
		TRACE("RTMainWindow::AddModule()", "GUI dodane");
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
			    xmlConfig.OpenFile( filename.c_str() );
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
				audio.SetSampleRate(44100.0);
				audio.Open();
			}	catch (AudioDriverError& error) {
		        cout << "!!" << endl << "!! Error: " << error.what() << endl << "!!" << endl;
		        exit(1);
		    }
		    
			ClearModules();

			Module* mod;
			for(mod = algo.GetFirstModule(); mod; mod = algo.GetNextModule()) {
				//if(mod->GetParameterCount() > 0) AddModule(mod);
				AddModule(mod);
			}

			fileLoaded = true;
			AllowPlay(true);

			set_title( "Real Time GUI - " + algo.GetName() );
			modulesBox.show_all_children();
			break;
		}
	}
}

void RTMainWindow::OnPlay() {
	audio.Start();
	AllowPlay(false);
	AllowStop(true);
}

void RTMainWindow::OnStop() {
    audio.Stop();
   	AllowStop(false);
  	if(fileLoaded) AllowPlay(true);
}

void RTMainWindow::OnMenuFileQuit() {
	hide();
}

void RTMainWindow::ClearModules() {
	TRACE("RTMainWindow::ClearModules()", "Czyszcze...");

    typedef vector<GuiModule*>::iterator GuiModIt;
	for(GuiModIt it = guiModules.begin(); it != guiModules.end(); it++) {
		delete *it;
	}
	guiModules.clear();
	
	TRACE("RTMainWindow::ClearModules()", "GuiModules wyczyszczone");
	
    typedef vector<Gtk::Frame*>::iterator GuiIt;
	for(GuiIt it = guis.begin(); it != guis.end(); it++) {
		delete *it;
	}
	guis.clear();
	TRACE("RTMainWindow::ClearModules()", "GUI's wyczyszczone");

	show_all_children();
	TRACE("RTMainWindow::ClearModules()", "Moduly wyczyszczone");
	
}

bool RTMainWindow::OnTimeOut() {
	char txt[20];
    g_snprintf(txt, 20, "CPU Usage: %#.2f %%", audio.GetCPUUsage()*100);
	cpuUsageLabel.set_label(txt);
	
	return true;
}

void RTMainWindow::OnAudioSetup() {
    TRACE("RTMainWindow::OnAudioSetup()", "Uruchamiam okno konfiguracji urzadzen audio...");
    
	OnStop();

	AudioSetupForm asForm(&audio);
    Gtk::Main::run(asForm);
    //area->on_expose_event(NULL);
    show_all_children();
	TRACE("RTMainWindow::OnAudioSetup()", "Konfiguracja zakonczona");
}

void RTMainWindow::AllowPlay(bool allow) {
	//Glib::RefPtr<Gtk::Action>::cast_static( ActionGroup->get_action("Play") )->set_sensitive(allow);
	ActionGroup->get_action("Play")->set_sensitive(allow);
}

void RTMainWindow::AllowStop(bool allow) {
	ActionGroup->get_action("Stop")->set_sensitive(allow);
}
