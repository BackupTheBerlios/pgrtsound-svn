#include "moduleForm.h"
#include <gtkmm/stock.h>
#include <iostream>

#include "core_mp.h"

/* Konstruktor
 * @param desk_ wskaŸnik do obiektu desk
 */
ModuleForm::ModuleForm(Desk *desk_) {
    TRACE("NewModuleForm::NewModuleForm()"," tworzenie okna");
    desk    =desk_;
    set_title("Dodaj nowy modul...");
    set_default_size(200, 200);
    set_modal(true);
    set_border_width(10);
    
    add(table);
    Gtk::Frame* frame = new Gtk::Frame;

    frame->add(*desk->GetDeskModuleActive()->GetGui());
    frame->set_label( desk->GetDeskModuleActive()->GetModule()->GetName() );


  
    table.attach(*frame,0,1,0,1);

   

    btClose.set_label("Zamknij");
    table.attach(btClose,0,2,2,3);
    btClose.signal_clicked().connect( sigc::mem_fun(*this, &ModuleForm::OnClose) );
    
  
    show_all_children();
    TRACE("NewModuleForm::NewModuleForm()"," okno utworzone");
}

ModuleForm::~ModuleForm() {
}



//SYGNA£Y ----------------------------------------------------------------------

/* Akcja zamknij okno
 */
void ModuleForm::OnClose() {
    hide();
}

