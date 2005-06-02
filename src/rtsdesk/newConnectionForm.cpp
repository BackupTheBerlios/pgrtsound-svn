#include "newConnectionForm.h"
#include <gtkmm/stock.h>
#include <iostream>
#include "core_mp.h"

NewConnectionForm::NewConnectionForm(Desk *d)
{
    desk    = d;
    set_title("Add a new connection");
    set_default_size(200, 200);
    set_modal(true);
    set_border_width(10);
    
    
    
    add(table);

// module 1
    lbModule1.set_label("Modul 1:");
    table.attach(lbModule1,0,1,0,1);
    
    comboModule1.clear();
    comboModule1.signal_changed().connect( sigc::mem_fun(*this, &NewConnectionForm::on_comboModule1_changed) );
    table.attach(comboModule1,1,2,0,1);
    comboModule1.append_text("< ? >");
    for (int i = 0;i<desk->deskModules.size();i++)
        comboModule1.append_text(desk->deskModules[i]->rtsModule->GetName());
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
    comboModule2.signal_changed().connect( sigc::mem_fun(*this, &NewConnectionForm::on_comboModule2_changed) );
    table.attach(comboModule2,1,2,2,3);
    comboModule2.append_text("< ? >");
    for (int i = 0;i<desk->deskModules.size();i++)
        comboModule2.append_text(desk->deskModules[i]->rtsModule->GetName());
    comboModule2.set_active_text("< ? >");
    

    lbInput.set_label("Wejscie modulu 2:");
    table.attach(lbInput,0,1,3,4);
    
    comboInput.clear();
    comboInput.append_text("< ? >");
    comboInput.set_active_text("< ? >");
    table.attach(comboInput,1,2,3,4);


//Dodaj lub anuluj =)

    table.attach(hbox,0,2,4,5);

    btCancel.set_label("Anuluj");
    hbox.pack_start(btCancel);
    btCancel.signal_clicked().connect( sigc::mem_fun(*this, &NewConnectionForm::onCancel) );
    
    btAdd.set_label("Dodaj");
    hbox.pack_start(btAdd);
    btAdd.signal_clicked().connect( sigc::mem_fun(*this, &NewConnectionForm::onAdd) );
  
    show_all_children();

}

NewConnectionForm::~NewConnectionForm()
{
}

//SIGNALS ----------------------------------------------------------------------
void NewConnectionForm::onCancel()
{
    hide();
}


void NewConnectionForm::onAdd()
{
    desk->algorithm->ConnectModules( desk->FindModule(comboModule1.get_active_text())
                                    ,desk->FindOutput(comboModule1.get_active_text(),comboOutput.get_active_text())
                                    ,desk->FindModule(comboModule2.get_active_text())
                                    ,desk->FindInput(comboModule2.get_active_text(),comboInput.get_active_text()) );
    cout << desk->FindModule(comboModule1.get_active_text()) <<" , " <<desk->FindOutput(comboModule1.get_active_text(),comboOutput.get_active_text())<< "," << desk->FindModule(comboModule2.get_active_text()) << "," <<desk->FindInput(comboModule2.get_active_text(),comboInput.get_active_text())<<endl;
    hide();
}

void NewConnectionForm::on_comboModule1_changed()
{
    int id = desk->FindModule(comboModule1.get_active_text());
    if (id != -1)
    {
        comboOutput.clear();
        comboOutput.append_text("< ? >");
        for (int i = 0; i < desk->algorithm->GetModule(id)->GetOutputCount(); i++)
            comboOutput.append_text(desk->algorithm->GetModule(id)->GetOutput(i)->GetName());
    } else {
        comboOutput.clear();    
    }
    comboOutput.set_active_text("< ? >");
}

void NewConnectionForm::on_comboModule2_changed()
{
    int id = desk->FindModule(comboModule2.get_active_text());
    if (id != -1)
    {
        comboInput.clear();
        comboInput.append_text("< ? >");
        for (int i = 0; i < desk->algorithm->GetModule(id)->GetInputCount(); i++)
            comboInput.append_text(desk->algorithm->GetModule(id)->GetInput(i)->GetName());
    } else {
        comboInput.clear();   
    }
    comboInput.set_active_text("< ? >");
}
