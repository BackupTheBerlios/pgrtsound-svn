#include "newModuleForm.h"
#include <gtkmm/stock.h>
#include <iostream>

#include "core_mp.h"

/* Konstruktor
 * @param desk_ wskaŸnik do obiektu desk
 */
NewModuleForm::NewModuleForm(Desk *desk_) {
    TRACE("NewModuleForm::NewModuleForm()"," tworzenie okna");
    desk    =desk_;
    set_title("Dodaj nowy modul...");
    set_default_size(200, 200);
    set_modal(true);
    set_border_width(10);
    
    add(table);
  
    lbModuleType.set_label("Typ modulu:");
    table.attach(lbModuleType,0,1,0,1);

    lbModuleName.set_label("Nazwa modulu:");
    table.attach(lbModuleName,0,1,1,2);

    //ustawienie listy dostêpnych modu³ów
    SetComboType();
    comboType.append_text("noise");
    comboType.append_text("butt12db");
    
    //ustawienie aktywnego
    comboType.set_active_text("< ? >");
    table.attach(comboType,1,2,0,1);
    comboType.signal_changed().connect( sigc::mem_fun(*this, &NewModuleForm::OnComboTypeChanged) );

    edName.set_text(comboType.get_active_text());
    table.attach(edName,1,2,1,2);

    table.attach(hbox,0,2,2,3);

    btCancel.set_label("Anuluj");
    hbox.pack_start(btCancel);
    btCancel.signal_clicked().connect( sigc::mem_fun(*this, &NewModuleForm::OnCancel) );
    
    btAdd.set_label("Dodaj");
    hbox.pack_start(btAdd);
    btAdd.signal_clicked().connect( sigc::mem_fun(*this, &NewModuleForm::OnAdd) );
  
    show_all_children();
    TRACE("NewModuleForm::NewModuleForm()"," okno utworzone");
}

NewModuleForm::~NewModuleForm() {
}

/* Ustawiene mo¿liwych typów modu³ów dostêpnych w systemie 
 *
 */
void NewModuleForm::SetComboType() {
    //ToDo
    //póki co rêcznie ale to raczej z jakiego mo¿e pliku configuruj¹cego czy cóœ =]
    comboType.clear();
    comboType.append_text("< ? >");   
    comboType.append_text("constant");
    comboType.append_text("gain");   
    comboType.append_text("sinosc2");
    comboType.append_text("slider");
    comboType.append_text("sumator");
    comboType.append_text("textfileout");

}

//SYGNA£Y ----------------------------------------------------------------------

/* Akcja zamknij okno
 */
void NewModuleForm::OnCancel() {
    hide();
}


/* Akcja dodaj nowy modu³
 */
void NewModuleForm::OnAdd() {
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


    //czy poprawna nazwa
    if ((ok)&((name == "") | (desk->FindModule(name) != NULL)))
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


/* Akcja Gdy zmieniono typ modu³u
 * Podpowiada/proponuje nazwê modu³u
 */
void NewModuleForm::OnComboTypeChanged() {
    if (comboType.get_active_text()!= "< ? >")
        edName.set_text(comboType.get_active_text()+" "+IntToString(desk->guiModules.size() + 1) );
    else 
        edName.set_text("< ? >");
}
