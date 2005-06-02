#ifndef GTKMM_NEWMODULEFORM_H
#define GTKMM_NEWMODULEFORM_H

#include <gtkmm.h>
#include "desk.h"

class NewModuleForm : public Gtk::Window
{
    public:
        NewModuleForm(Desk *d);
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
    //Algorithm
        Desk *desk;

    //Signal handlers:
        virtual void onCancel();
        virtual void onAdd();
        virtual void onComboTypeChanged();
};

#endif
