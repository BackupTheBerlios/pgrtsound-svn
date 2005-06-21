#ifndef GTKMM_NEWMODULEFORM_H
#define GTKMM_NEWMODULEFORM_H

#include <gtkmm.h>
#include "desk.h"

/* Klasa okna dodawania nowego modu�u do algorytmu
 */
class NewModuleForm : public Gtk::Window {
    public:
        NewModuleForm(Desk *desk_);
        virtual ~NewModuleForm();
    protected:
        Gtk::Table          table;
        Gtk::Label          lbModuleType;
        Gtk::Label          lbModuleName;
        Gtk::Entry          edName;
        Gtk::ComboBoxText   comboType;
        Gtk::Button         btAdd;
        Gtk::Button         btCancel;
        Gtk::HBox           hbox;
    
        Desk *desk;

        void SetComboType();
    //Sygna�y do obs�ugi przycisk�w       
        virtual void OnAdd();
        virtual void OnCancel();
        virtual void OnComboTypeChanged();
        
};

#endif
