#include "newModuleForm.h"
#include <gtkmm/stock.h>
#include <iostream>

#include "core_mp.h"

NewModuleForm::NewModuleForm(Desk *d)
{
    TRACE("NewModuleForm::NewModuleForm()"," tworzenie okna");
    desk    = d;
    set_title("Add a new module");
    set_default_size(200, 200);
    set_modal(true);
    set_border_width(10);
    
    
    
    add(table);
  
    lbModuleType.set_label("Typ modulu:");
    table.attach(lbModuleType,0,1,0,1);

    lbModuleName.set_label("Nazwa modulu:");
    table.attach(lbModuleName,0,1,1,2);

//ToDo
//póki co rêcznie ale to raczej z jakiego mo¿e pliku configuruj¹cego czy cóœ =]
    comboType.clear();
    comboType.append_text("< ? >");
//    comboType.append_text("audioportin");
//    comboType.append_text("audioportout");     
    comboType.append_text("constant");
    comboType.append_text("gain");   
    comboType.append_text("sinosc2");
    comboType.append_text("slider");
    comboType.append_text("sumator");
    comboType.append_text("textfileout");
    
    //ustawienie aktywnego
    comboType.set_active_text("< ? >");
    table.attach(comboType,1,2,0,1);
    comboType.signal_changed().connect( sigc::mem_fun(*this, &NewModuleForm::onComboTypeChanged) );

    edName.set_text(comboType.get_active_text()+" "+IntToString(desk->deskModules.size() + 1));
    table.attach(edName,1,2,1,2);

    table.attach(hbox,0,2,2,3);

    btCancel.set_label("Anuluj");
    hbox.pack_start(btCancel);
    btCancel.signal_clicked().connect( sigc::mem_fun(*this, &NewModuleForm::onCancel) );
    
    btAdd.set_label("Dodaj");
    hbox.pack_start(btAdd);
    btAdd.signal_clicked().connect( sigc::mem_fun(*this, &NewModuleForm::onAdd) );
  
    show_all_children();
    TRACE("NewModuleForm::NewModuleForm()"," okno utworzone");
}

NewModuleForm::~NewModuleForm()
{
}

//SIGNALS ----------------------------------------------------------------------
void NewModuleForm::onCancel()
{
    hide();
}


void NewModuleForm::onAdd()
{
    bool ok = true;
    
    string type = comboType.get_active_text();
    string name = edName.get_text();
    
    //czy wybrano typ
    if ((ok)&(type == "< ? >")) {
        Gtk::MessageDialog dialog(*this, "Wybierz rodzaj modulu!");
        dialog.set_secondary_text("");
        dialog.run();
        ok = false;
    }

    //jesli portaudioin/out czy juz istnieja
    if ((ok)&((type == "audioportin") | (type == "audioportout")))
    {
        for (int m = 0; m < desk->algorithm->GetModulesCount(); m++)
            if (desk->algorithm->GetModule(m)->GetType()==type)
                ok = false; 
        if (!ok) {
            Gtk::MessageDialog dialog(*this, "Modul tego typu juz istnieje!");
            dialog.set_secondary_text("Modul specjalny moze istniec tylko jeden.");
            dialog.run();    
        }
    }
    
    //czy poprawna nazwa
    if ((ok)&((name == "") | (desk->FindModule(name) > 0)))
    {
        Gtk::MessageDialog dialog(*this, "Niewlasciwa nazwa modulu!");
        dialog.set_secondary_text("");
        dialog.run();
        ok = false;
    }
    
    //jezeli wszystko dobrze to dodaj
    if (ok)
    {
        TRACE3("NewModuleForm::onAdd"," Dodano modul:",name,type);
        desk->AddModule(type,name);
        hide();
    }
    
}

void NewModuleForm::onComboTypeChanged()
{
    if (comboType.get_active_text()!= "< ? >")
        edName.set_text(comboType.get_active_text()+" "+IntToString(desk->deskModules.size() + 1) );
    else 
        edName.set_text("");
}
