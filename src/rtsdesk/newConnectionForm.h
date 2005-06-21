#ifndef GTKMM_NEWCONNECTIONFORM_H
#define GTKMM_NEWCONNECTIONFORM_H

#include <gtkmm.h>
#include "desk.h"


class NewConnectionForm : public Gtk::Window
{
    public:
        NewConnectionForm(Desk *desk_);
        virtual ~NewConnectionForm();
    protected:
        Gtk::Table          table;
        Gtk::Label          lbModule1;
        Gtk::Label          lbModule2;
        Gtk::Label          lbOutput;
        Gtk::Label          lbInput;
        Gtk::ComboBoxText   comboModule1;
        Gtk::ComboBoxText   comboModule2;
        Gtk::ComboBoxText   comboOutput;
        Gtk::ComboBoxText   comboInput;
        Gtk::Button         btAdd;
        Gtk::Button         btCancel;
        Gtk::HBox           hbox;

        Desk *desk;

    //Signal handlers:
        virtual void OnCancel();
        virtual void OnAdd();
        virtual void OnComboModule1Changed();
        virtual void OnComboModule2Changed();
};

#endif
