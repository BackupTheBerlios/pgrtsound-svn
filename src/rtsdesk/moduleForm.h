#ifndef GTKMM_MODULEFORM_H
#define GTKMM_MODULEFORM_H

#include <gtkmm.h>
#include "desk.h"

/* Klasa okna dodawania nowego modu³u do algorytmu
 */
class ModuleForm : public Gtk::Window {
    public:
        ModuleForm(Desk *desk_);
        virtual ~ModuleForm();
    protected:
        Gtk::Table          table;
        Gtk::Button         btClose;

        Desk *desk;

        virtual void OnClose();

        
};

#endif
