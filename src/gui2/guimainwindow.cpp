#include "guimainwindow.h"
#include "../callback.h"

GuiMainWindow::GuiMainWindow() : mainBox(false, 0) {
	TRACE("GuiMainWindow::GuiMainWindow()", "Tworze okno...");

	algo = algoView.GetAlgorithm();
	algoView.SetParentWindow(this);

	set_title("RTSGUI");
	set_size_request(600, 400);
	set_position(Gtk::WIN_POS_CENTER);
	
	// menu i akcje
	ActionGroup = Gtk::ActionGroup::create();

	//File menu:
	ActionGroup->add( Gtk::Action::create("FileMenu", "File") );
	ActionGroup->add( Gtk::Action::create("OpenFile", Gtk::Stock::OPEN),
		sigc::mem_fun(*this, &GuiMainWindow::OnOpenFile) );
	ActionGroup->add( Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
		sigc::mem_fun(*this, &GuiMainWindow::OnMenuFileQuit) );

	//Transport menu:
	ActionGroup->add( Gtk::Action::create("TransportMenu", "Transport") );
	ActionGroup->add( Gtk::Action::create("Play", Gtk::Stock::MEDIA_PLAY, "Play", "Starts audio"),
		sigc::mem_fun(*this, &GuiMainWindow::OnPlay) );
	ActionGroup->add( Gtk::Action::create("Stop", Gtk::Stock::MEDIA_STOP, "Stop", "Stops audio"),
		sigc::mem_fun(*this, &GuiMainWindow::OnStop) );

	// Setup menu
	ActionGroup->add( Gtk::Action::create("SetupMenu", "Setup") );
	ActionGroup->add( Gtk::Action::create("AudioDevices", Gtk::Stock::PREFERENCES, "Audio devices...", "Setup audio devices"),
		sigc::mem_fun(*this, &GuiMainWindow::OnAudioSetup) );

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

	//cpuUsageLabel.set_justify(Gtk::JUSTIFY_LEFT);
    cpuUsageLabel.set_label("CPU usage");
	cpuUsageLabel.set_alignment(0, 0.5);
	cpuUsageLabel.set_width_chars(100);
	
	mainBox.set_homogeneous(false);
    mainBox.pack_start(scrollWindow);
    mainBox.pack_start(cpuUsageLabel,  Gtk::PACK_SHRINK, Gtk::FILL, 0);

	scrollWindow.set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);

	//modulesBox.set_border_width(5);
	//modulesBox.set_homogeneous(false);
	//modulesBox.set_spacing(5);

	scrollWindow.add(algoView);

	// timer CPU usage
	my_slot = sigc::mem_fun(*this, &GuiMainWindow::OnTimeOut);
	conn = Glib::signal_timeout().connect(my_slot, 1000);

	fileLoaded = false;
	AllowPlay(false);
	AllowStop(false);

	TRACE("GuiMainWindow::GuiMainWindow()", "Okno aplikacji utworzone");
}

GuiMainWindow::~GuiMainWindow() {
	TRACE("GuiMainWindow::~GuiMainWindow()", "Usuwanie parametrow i modulow...");
	ClearModules();
	TRACE("GuiMainWindow::~GuiMainWindow()", "Usuniete");
}

void GuiMainWindow::OnOpenFile() {
	Gtk::FileChooserDialog dialog("Please choose a file", Gtk::FILE_CHOOSER_ACTION_OPEN);
	dialog.set_transient_for(*this);
	dialog.set_position(Gtk::WIN_POS_CENTER_ON_PARENT);
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
				algoView.LoadFromFile( filename );
				algoView.GetAlgorithm()->CreateQueue();
		    } catch (RTSError& error) {
		        cout << "Error: " << error.what() << endl;
		        exit(1);
		    }

			try {
		        //audio.PrintDevices();
				audio.SetCallback( paCallback, (void*)algo );
				audio.SetSampleRate(44100.0);
				audio.Open();
			}	catch (AudioDriverError& error) {
		        cout << "!!" << endl << "!! Error: " << error.what() << endl << "!!" << endl;
		        exit(1);
		    }
		    
			fileLoaded = true;
			AllowPlay(true);

			set_title( "Real Time GUI - " + algo->GetName() );
			
			break;
		}
	}
}

void GuiMainWindow::OnPlay() {
	audio.Start();
	AllowPlay(false);
	AllowStop(true);
}

void GuiMainWindow::OnStop() {
    audio.Stop();
   	AllowStop(false);
  	if(fileLoaded) AllowPlay(true);
}

void GuiMainWindow::OnMenuFileQuit() {
	hide();
}

void GuiMainWindow::ClearModules() {
	TRACE("GuiMainWindow::ClearModules()", "Czyszcze...");

    typedef vector<GuiModule*>::iterator GuiModIt;
	for(GuiModIt it = guiModules.begin(); it != guiModules.end(); it++) {
		delete *it;
	}
	guiModules.clear();
	
	TRACE("GuiMainWindow::ClearModules()", "GuiModules wyczyszczone");
	
    typedef vector<Gtk::Frame*>::iterator GuiIt;
	for(GuiIt it = guis.begin(); it != guis.end(); it++) {
		delete *it;
	}
	guis.clear();
	TRACE("GuiMainWindow::ClearModules()", "GUI's wyczyszczone");

	show_all_children();
	TRACE("GuiMainWindow::ClearModules()", "Moduly wyczyszczone");
	
}

bool GuiMainWindow::OnTimeOut() {
	char txt[60];
    g_snprintf(txt, 60, "CPU usage: %#.2f %%  in: %#.1f ms  out: %#.1f ms", audio.GetCPUUsage()*100,
		audio.GetInputLatency()*1000, audio.GetOutputLatency()*1000 );
	cpuUsageLabel.set_label(txt);
	
	return true;
}

void GuiMainWindow::OnAudioSetup() {
    TRACE("GuiMainWindow::OnAudioSetup()", "Uruchamiam okno konfiguracji urzadzen audio...");
    
	OnStop();
    
	AudioSetupForm asForm(&audio);
    
    
    //Stay On Top :)
    asForm.set_transient_for(*this);
    	
    Gtk::Main::run(asForm);
    show_all_children();
	TRACE("GuiMainWindow::OnAudioSetup()", "Konfiguracja zakonczona");
}

void GuiMainWindow::AllowPlay(bool allow) {
	ActionGroup->get_action("Play")->set_sensitive(allow);
}

void GuiMainWindow::AllowStop(bool allow) {
	ActionGroup->get_action("Stop")->set_sensitive(allow);
}
