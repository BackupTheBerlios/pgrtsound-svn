#include "mainForm.h"
#include "newModuleForm.h"
#include "newConnectionForm.h"
#include <iostream>
#include "core_mp.h"

#define FRAMES_PER_BLOCK (64)
#define SAMPLE_RATE (44100)


//------------------------------------------------------------------------------
MainForm::MainForm(Desk *d) {
    TRACE("MainForm::MainForm()", "Tworze okno...");
    desk = d;
    set_title("RTSDesk");
    set_default_size(200, 200);

    add(firstBox); 

    m_refActionGroup = Gtk::ActionGroup::create();

    //File menu:
    m_refActionGroup->add( Gtk::Action::create("FileMenu", "Plik") );
    m_refActionGroup->add( Gtk::Action::create("FileNew", Gtk::Stock::NEW),
                           sigc::mem_fun(*this, &MainForm::OnNew) );
    m_refActionGroup->add( Gtk::Action::create("FileQuit", Gtk::Stock::QUIT),
                           sigc::mem_fun(*this, &MainForm::OnMenuFileQuit) );
    m_refActionGroup->add( Gtk::Action::create("FileSave", Gtk::Stock::SAVE),
                           sigc::mem_fun(*this, &MainForm::OnSaveFile) );
    m_refActionGroup->add( Gtk::Action::create("FileLoad", Gtk::Stock::OPEN),
                           sigc::mem_fun(*this, &MainForm::OnLoadFile) );
    //Module menu:
    m_refActionGroup->add( Gtk::Action::create("ModuleMenu", "Moduly & polaczenia") );
    m_refActionGroup->add( Gtk::Action::create("ModuleNew", Gtk::Stock::ADD , "Nowy modul...", "Dodaj nowy modul"),
                           sigc::mem_fun(*this, &MainForm::OnNewModule) );
    m_refActionGroup->add( Gtk::Action::create("ModuleConnectionNew", Gtk::Stock::CONNECT , "Nowe polaczenie...", "Dodaj nowe polaczenie"),
                           sigc::mem_fun(*this, &MainForm::OnNewConnection) );

    //Help menu:
    m_refActionGroup->add( Gtk::Action::create("HelpMenu", "Help") );
    m_refUIManager = Gtk::UIManager::create();
    m_refUIManager->insert_action_group(m_refActionGroup);

    add_accel_group(m_refUIManager->get_accel_group());

    
    try {
        Glib::ustring ui_info =
            "<ui>"
            "  <menubar name='MenuBar'>"
            "    <menu action='FileMenu'>"
            "      <menuitem action='FileNew'/>"
            "      <menuitem action='FileLoad'/>"
            "      <menuitem action='FileSave'/>"
            "      <separator/>"
            "      <menuitem action='FileQuit'/>"
            "    </menu>"
            "    <menu action='ModuleMenu'>"
            "      <menuitem action='ModuleNew'/>"
            "      <menuitem action='ModuleConnectionNew'/>"
            "    </menu>"
            "    <menu action='HelpMenu'>"
            "      <menuitem action='HelpAbout'/>"
            "    </menu>"
            "  </menubar>"
            "  <toolbar  name='ToolBar'>"
            "    <toolitem action='ModuleNew'/>"
            "    <toolitem action='ModuleConnectionNew'/>"
            "    <toolitem action='FileQuit'/>"
            "  </toolbar>"
            "</ui>";
        m_refUIManager->add_ui_from_string(ui_info);
    } catch(const Glib::Error& ex) {
        std::cerr << "building menus failed: " <<  ex.what();
    }


    Gtk::Widget* pMenubar = m_refUIManager->get_widget("/MenuBar");
    if(pMenubar)
        firstBox.pack_start(*pMenubar, Gtk::PACK_SHRINK);

    Gtk::Widget* pToolbar = m_refUIManager->get_widget("/ToolBar") ;
    if(pToolbar)
        firstBox.pack_start(*pToolbar, Gtk::PACK_SHRINK);
    
    firstBox.pack_end(secondBox);
  
    secondBox.pack_start(dnd, Gtk::PACK_SHRINK);
  
    secondBox.pack_end(paramPanel);
    lbName.set_text("Nazwa:");
    lbType.set_text("Typ:");
    lbParams.set_text("Parametry:");
    paramPanel.set_size_request(200,600);
    paramPanel.put(lbName,10,10);
    paramPanel.put(edName,100,10);
    paramPanel.put(lbType,10,40);  
    paramPanel.put(edType,100,40);  
    paramPanel.put(lbParams,10,70);
  
    //DnD
    area = new GtkDesk(desk);
    dnd.put(*area,0,0);
    listTargets.push_back( Gtk::TargetEntry("STRING") );
    listTargets.push_back( Gtk::TargetEntry("text/plain") );
    dnd.drag_dest_set(listTargets);
    dnd.signal_drag_data_received().connect( sigc::mem_fun(*this, &MainForm::OnModuleDropDragDataReceived) );
    area->set_size_request(600,600);
    
    show_all_children();  
    TRACE("MainForm::MainForm()", "Okno utworzone");
}

//------------------------------------------------------------------------------

MainForm::~MainForm() {
    TRACE("MainForm::~MainForm()", "bye");
}

//------------------------------------------------------------------------------

void MainForm::OnNew( ){
    TRACE("MainForm::OnNew()", "start");
    //delete desk;
    for (int i = 0;i<desk->deskModules.size();i++)
    {
         delete desk->deskModules[i]->widget;
         delete desk->deskModules[i]->text;
    } 
    Algorithm *algo= new Algorithm(FRAMES_PER_BLOCK);
    desk = new Desk(algo);
    area -> desk = desk;
    area -> on_expose_event(NULL);
    TRACE("MainForm::OnNew()", "end");
}


//------------------------------------------------------------------------------
void MainForm::OnMenuFileQuit()
{
    TRACE("MainForm::OnMenuFileQuit()", "bye");
    hide(); //Zamkniecie okna
}


//------------------------------------------------------------------------------
void MainForm::ShowModuleParameters(DeskModule*  deskModule) {
    TRACE("MainForm::ShowModuleParameters()", "start");
    //dla poprzednio aktywnego
    if (desk->GetDeskModuleActive() != NULL)
    {
        Module* rtsModuleActive = desk->GetDeskModuleActive()->rtsModule;
        for (int i=0; i<desk->GetDeskModuleActive()->rtsModule->GetParameterCount(); i++)
        {
            if (rtsModuleActive->GetParameter(i)->GetGUIType()!= gtInvisible)
            {
                //zczytanie wartosci parametru do modu³u
                if ((rtsModuleActive->GetParameter(i)->GetGUIType() == gtSlider) or 
                    (rtsModuleActive->GetParameter(i)->GetGUIType() == gtProperty))
                {
                    ParameterFloat* param =
						(ParameterFloat*)rtsModuleActive->GetParameter(i);					
					param->SetValue(atof(((Gtk::Entry *)widgetList[2*i+1])->get_text().c_str()));
                }
                
                if (rtsModuleActive->GetParameter(i)->GetGUIType() == gtEditBox)
                {
                    ParameterString* param =
						(ParameterString*)rtsModuleActive->GetParameter(i);					
					param->SetText(((Gtk::Entry *)widgetList[2*i+1])->get_text().c_str());
                }
                
                //usuwanie z formy
                delete  widgetList[2*i];
                delete  widgetList[2*i+1];
            }
        }
    }   
        
    //zmiana aktywnego
    desk->SetDeskModuleActive(deskModule);
    
    //dla aktualnie aktywnego
    if (desk->GetDeskModuleActive() != NULL)
    {
        Module* rtsModuleActive = desk->GetDeskModuleActive()->rtsModule;
        rtsModuleActive->GetName() = edName.get_text();
        for (int i=0; i<rtsModuleActive->GetParameterCount(); i++)
        {
            if (rtsModuleActive->GetParameter(i)->GetGUIType()!= gtInvisible)
            {
                Gtk::Label* label = new Gtk::Label(rtsModuleActive->GetParameter(i)->GetName());
                paramPanel.put(*label,10,100 + i *20);
                widgetList[2*i] = label;    
                
                //Float parameter
                if ((rtsModuleActive->GetParameter(i)->GetGUIType() == gtSlider) or 
                    (rtsModuleActive->GetParameter(i)->GetGUIType() == gtProperty))
                {
                    Gtk::Entry* entry = new Gtk::Entry();                   
                    entry->set_text(IntToString(((ParameterFloat*)rtsModuleActive->GetParameter(i))->GetValue()));   
                    paramPanel.put(*entry,100,100 + i *20);
                    widgetList[2*i + 1] = entry;    
                }
                
                //String parameter    
                if (rtsModuleActive->GetParameter(i)->GetGUIType() == gtEditBox)
                {
                    Gtk::Entry* entry = new Gtk::Entry();  
                    ParameterString* param = (ParameterString*)rtsModuleActive->GetParameter(i);					              
                    entry->set_text(param->GetText());   
                    paramPanel.put(*entry,100,100 + i *20);
                    widgetList[2*i + 1] = entry;    
                }
            }
        }
        edType.set_text(rtsModuleActive->GetType());
        edName.set_text(rtsModuleActive->GetName());
    }     
    
    show_all_children();  
    TRACE("MainForm::ShowModuleParameters()", "end");
}

//------------------------------------------------------------------------------
void MainForm::AddNewModuleToDesk() {
    TRACE("MainForm::AddNewModuleToDesk()", "start");
    for (int i = 0;i<desk->deskModules.size();i++)
        if (desk->deskModules[i]->widget == NULL)
        {
            //Rysunek
            GtkModule* module = new GtkModule(desk->deskModules[i]->rtsModule->GetInputCount(),desk->deskModules[i]->rtsModule->GetOutputCount());
            module->drag_source_set(listTargets);
            module->signal_drag_data_get().connect( sigc::mem_fun(*this, &MainForm::OnModuleDragDataGet));
            
            
            dnd.put(*module,desk->deskModules[i]->x,desk->deskModules[i]->y);
            desk->deskModules[i]->widget = module;
            //module->signal_drag_begin().connect( sigc::mem_fun(*this, &MainForm::onModuleClick) );
            
            //Nazwa
            Gtk::Entry* text = new Gtk::Entry();
            text->set_editable(false);
           
            text->set_text(desk->deskModules[i]->rtsModule->GetName());
            text->set_max_length (20);
            text->set_width_chars (10);
            desk->deskModules[i]->text = text;
            dnd.put(*text,desk->deskModules[i]->x,desk->deskModules[i]->y+module->size+3);
            
            area->on_expose_event(NULL);
            show_all_children();        
        }
    TRACE("MainForm::AddNewModuleToDesk()", "end");
}

//------------------------------------------------------------------------------
void MainForm::OnNewModule() {
    TRACE("MainForm::OnNewModule()", "start");
    NewModuleForm form(desk);
    Gtk::Main::run(form);
    AddNewModuleToDesk();
    TRACE("MainForm::OnNewModule()", "end");
}

//------------------------------------------------------------------------------
void MainForm::OnNewConnection() {
    TRACE("MainForm::OnNewConnection()", "start");
    NewConnectionForm form(desk);
    Gtk::Main::run(form);
    area->on_expose_event(NULL);
    show_all_children();
    TRACE("MainForm::OnNewConnection()", "end");
}

//------------------------------------------------------------------------------
void MainForm::OnModuleDragDataGet(const Glib::RefPtr<Gdk::DragContext>&, Gtk::SelectionData& selection_data, guint, guint) {
    TRACE("MainForm::OnModuleDragDataGet()", "drag");
    selection_data.set(selection_data.get_target(), 8 , (const guchar*)"", 0 );
}

//------------------------------------------------------------------------------
void MainForm::OnModuleDropDragDataReceived(const Glib::RefPtr<Gdk::DragContext>& context, int x, int y, const Gtk::SelectionData& selection_data, guint info, guint time) {
    TRACE("MainForm::OnModuleDropDragDataReceived()", "drop");
    dnd.move ( *(Gtk::Widget::drag_get_source_widget(context)), x,y);
    for (int i = 0;i<desk->deskModules.size();i++)
        if (desk->deskModules[i]->widget == (Gtk::Widget::drag_get_source_widget(context))) {            
            ShowModuleParameters(desk->deskModules[i]);
            dnd.move ( *desk->deskModules[i]->text, x,y+30);
            desk->deskModules[i]->x=x;
            desk->deskModules[i]->y=y;        
        }  
    area->on_expose_event(NULL);
}

//------------------------------------------------------------------------------
void MainForm::OnSaveFile() {
    TRACE("MainForm::OnSaveFile()", "start save");
    Gtk::FileChooserDialog dialog("Podaj nazwê pliku do zapisania", Gtk::FILE_CHOOSER_ACTION_SAVE );
    dialog.set_transient_for(*this);

    //Add response buttons the the dialog:
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button("Zapisz", Gtk::RESPONSE_OK);

    int result = dialog.run();

    //Handle the response:
    switch(result)
    {
        case(Gtk::RESPONSE_OK):
        {
            desk->SaveToFile(dialog.get_filename());
            TRACE2("MainForm::OnSaveFile()", "Zapis w pliku: ", dialog.get_filename());
            break;
        }
        case(Gtk::RESPONSE_CANCEL):
        {
            TRACE("MainForm::OnSaveFile()", "Anulowano zapis ");
            break;
        }
        default:
        {
            TRACE("MainForm::OnSaveFile()", "Nieznana akcja ");
            break;
        }
    }
}

//------------------------------------------------------------------------------
void MainForm::OnLoadFile()
{
    TRACE("MainForm::OnLoadFile()", "start load");
    Gtk::FileChooserDialog dialog("Podaj nazwê pliku do zapisania", Gtk::FILE_CHOOSER_ACTION_OPEN );
    dialog.set_transient_for(*this);

    //Add response buttons the the dialog:
    dialog.add_button(Gtk::Stock::CANCEL, Gtk::RESPONSE_CANCEL);
    dialog.add_button("Otworz", Gtk::RESPONSE_OK);


    int result = dialog.run();

    //Handle the response:
    switch(result)
    {
        case(Gtk::RESPONSE_OK):
        {
            OnNew();
            desk->LoadFromFile(dialog.get_filename());
            TRACE2("MainForm::OnLoadFile()", "Nieznana akcja ", dialog.get_filename());            
            break;
        }
        case(Gtk::RESPONSE_CANCEL):
        {
            TRACE("MainForm::OnLoadFile()", "Anulacja odczytu ");
            break;
        }
        default:
        {
            TRACE("MainForm::OnLoadFile()", "Nieznana akcja ");
            break;
        }
    }
    TRACE("MainForm::OnLoadFile()", "AddNewModuleToDesk");
    AddNewModuleToDesk();
}
