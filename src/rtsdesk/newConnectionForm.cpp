#include "newConnectionForm.h"
#include <gtkmm/stock.h>
#include <iostream>
#include "core_mp.h"

NewConnectionForm::NewConnectionForm(Desk *desk_) {
    desk    = desk_;
    set_title("Add a new connection");
    set_default_size(200, 200);
    set_modal(true);
    set_border_width(10);
    
   add(table);

// module 1
    lbModule1.set_label("Modul 1:");
    table.attach(lbModule1,0,1,0,1);
    
    comboModule1.clear();
    comboModule1.signal_changed().connect( sigc::mem_fun(*this, &NewConnectionForm::OnComboModule1Changed) );
    table.attach(comboModule1,1,2,0,1);
    comboModule1.append_text("< ? >");
    for (int i = 0;i<desk->guiModules.size();i++) {
        if (desk->guiModules[i]->GetModule()->GetOutputCount() > 0)    
            comboModule1.append_text(desk->guiModules[i]->GetModule()->GetName());
    }
    comboModule1.set_active_text("< ? >");
    
    lbOutput.set_label("Wyjscie modulu 1:");
    table.attach(lbOutput,0,1,1,2);

    comboOutput.clear();
    comboOutput.append_text("< ? >");
    table.attach(comboOutput,1,2,1,2);
    comboOutput.set_active_text("< ? >");

//module 2
    lbModule2.set_label("Modul 2:");
    table.attach(lbModule2,0,1,2,3);
    comboModule2.clear();
    comboModule2.signal_changed().connect( sigc::mem_fun(*this, &NewConnectionForm::OnComboModule2Changed) );
    table.attach(comboModule2,1,2,2,3);
    comboModule2.append_text("< ? >");
    for (int i = 0;i<desk->guiModules.size();i++) {
        if (desk->guiModules[i]->GetModule()->GetInputCount() > 0)
            comboModule2.append_text(desk->guiModules[i]->GetModule()->GetName());
    }
    comboModule2.set_active_text("< ? >");
    

    lbInput.set_label("Wejscie modulu 2:");
    table.attach(lbInput,0,1,3,4);
    
    comboInput.clear();
    comboInput.append_text("< ? >");
    comboInput.set_active_text("< ? >");
    table.attach(comboInput,1,2,3,4);

    table.attach(hbox,0,2,4,5);

    btCancel.set_label("Anuluj");
    hbox.pack_start(btCancel);
    btCancel.signal_clicked().connect( sigc::mem_fun(*this, &NewConnectionForm::OnCancel) );
    
    btAdd.set_label("Dodaj");
    hbox.pack_start(btAdd);
    btAdd.signal_clicked().connect( sigc::mem_fun(*this, &NewConnectionForm::OnAdd) );
  
    show_all_children(); 

}

NewConnectionForm::~NewConnectionForm() {
}

//SYGNALY ----------------------------------------------------------------------
void NewConnectionForm::OnCancel() {
    hide();
}


void NewConnectionForm::OnAdd() {
    desk->algorithm->ConnectModules( comboModule1.get_active_text()
                                    ,desk->FindOutput(comboModule1.get_active_text(),comboOutput.get_active_text())
                                    ,comboModule2.get_active_text()
                                    ,desk->FindInput(comboModule2.get_active_text(),comboInput.get_active_text()) 
                                    );
    hide();
}

void NewConnectionForm::OnComboModule1Changed() {
    string name = comboModule1.get_active_text();
    if (name != "< ? >")
    if (name != "") {
        comboOutput.clear();
        comboOutput.append_text("< ? >");
        for (int i = 0; i < desk->algorithm->GetModule(name)->GetOutputCount(); i++)
            comboOutput.append_text(desk->algorithm->GetModule(name)->GetOutput(i)->GetName());
    } else {
        comboOutput.clear();    
    }
    comboOutput.set_active_text("< ? >");
}

void NewConnectionForm::OnComboModule2Changed() {
    string name = comboModule2.get_active_text();
    if (name != "< ? >")
    if (name != "")
    {
        comboInput.clear();
        comboInput.append_text("< ? >");
        for (int i = 0; i < desk->algorithm->GetModule(name)->GetInputCount(); i++)
            comboInput.append_text(desk->algorithm->GetModule(name)->GetInput(i)->GetName());
    } else {
        comboInput.clear();   
    }
    comboInput.set_active_text("< ? >");
}
