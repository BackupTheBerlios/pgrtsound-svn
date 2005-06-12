#ifndef DESK_H
#define DESK_H

#include <gtkmm.h>
#include "algorithm.h"
#include "tinyxml.h"
#include "modules/module.h"
#include "gtkModule.h"

//boost
#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/topological_sort.hpp>
#include <boost/property_map.hpp>




typedef Graph::vertex_descriptor ModuleId;


class DeskModule
{
    public:
        DeskModule(int x_, int y_)
        {
            x       = x_;
            y       = y_;
            widget  = NULL;
        }
        ~DeskModule() {
            delete widget;
            delete text;   
        };
        int             x,y;    //pozycja
        GtkModule*      widget;
        Gtk::Entry*     text;
        
        Module*         GetRTSModule();
        void            SetModuleId(ModuleId moduleId_);
        void            SetRTSModule(Module* rtsModule_);
        
    private:
        ModuleId        moduleId;
        Module*         rtsModule;
};





class Desk
{
    public:


      //Metody
        //Konstruktor i destruktor
        Desk(Algorithm *algo);
        ~Desk();
        
        //Obs³uga modu³ów
        void        Clear();
        void        AddModule(string type, string name);
        void        AddModule(ModuleId moduleId);
        Module*     FindModule(string name);
        int         FindOutput(Module* module,string outName);
        int         FindOutput(string nameModule,string outName);
        int         FindInput(Module* module,string inName);
        int         FindInput(string nameModule,string inName);
        void        SetPosition(string nameModule,int x, int y);
        DeskModule* GetDeskModuleActive();
        void        SetDeskModuleActive(DeskModule* deskModule);
        void        DeleteActiveModule();
        void        AddAllModuleToDesk();
               
        //Zapisywanie do plik
        void SaveToFile(string filename);
        //Odczyt z pliku
        void LoadFromFile(string filename);
        
      //Parametry
        Algorithm                   *algorithm;
        vector<DeskModule*>         deskModules;
        
    private:
        DeskModule*                 deskModuleActive;
        

};


#endif
