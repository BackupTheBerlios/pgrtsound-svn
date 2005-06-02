#include "newModuleForm.h"
#include <gtkmm/stock.h>
#include <iostream>

#include "core_mp.h"

NewModuleForm::NewModuleForm(Desk *d)
{
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
    comboType.append_text("constant");
    comboType.append_text("gain");
    comboType.append_text("audioportin");
    comboType.append_text("audioportout");    
    comboType.append_text("sinosc2");
    comboType.append_text("sumator");
    comboType.append_text("textfileout");
    comboType.set_active_text("constant");
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
    string name =edName.get_text();
    cout << "Name :"<<name<<endl;
    //ToDo Trimowanie nazwy
    //name.erase(" ")
    if ( (name != "") & (desk->FindModule(name) < 0) )
    {
        desk->AddModule(comboType.get_active_text(),name);
        hide();
    } else
    {
        Gtk::MessageDialog dialog(*this, "Niewlasciwa nazwa modulu!");
        dialog.set_secondary_text("");
        dialog.run();
    }
}

void NewModuleForm::onComboTypeChanged()
{
    edName.set_text(comboType.get_active_text()+" "+IntToString(desk->deskModules.size() + 1) );
}
