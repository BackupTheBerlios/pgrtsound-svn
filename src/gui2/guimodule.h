#ifndef GUIMODULE_H
#define GUIMODULE_H

#include "moduleguiwindow.h"
#include "guis/gui.h"

#include "../algorithm.h"
#include "../debug.h"
#include "../modules/module.h"

#include <gtkmm/eventbox.h>
#include <pangomm/layout.h>

#include <string>

// deklaracja potrzebna (naglowek "algorithmview.h" dalby petle plikow naglowkowych)
// niestety jest zaleznosc GuiModule od AlgorithmView, ale moze uda sie ja usunac
// pozniej
class AlgorithmView;
class GuiConnection;

/**
 * Interfejs modulu graficznego.
 */
class GuiModule : public Gtk::EventBox {
	public:
   		static const int socketSize;
		static const int socketSizeDoubled;

		GuiModule(Module* moduleToAttach);
		~GuiModule();
	    virtual void Init() {};
   	    virtual void SampleRateChanged() {};
	    virtual void BlockSizeChanged() {};
		void ChangeName(); // TODO: zamienic na SetName()
		void SetParentView( AlgorithmView* algoView );
		void SetXY( int x_, int y_ ); // TODO: zamienic na SetPosition()
		//void SetText(std::string newText);
		void SetInputGuiConnection( int inputId, GuiConnection* conn );
		void SetModuleId( ModuleId modId );
		virtual Gtk::Widget* GetGui();
		Module* GetModule();
		void GetPosition(int& x, int& y);
		int GetWidth() const;
		int GetHeight() const;
		int GetCurrentInputNumber();
		int GetCurrentOutputNumber();
		void GetInputPosition( int inNum, int& x, int& y );
		void GetOutputPosition( int outNum, int& x, int& y );
		GuiConnection* GetInputGuiConnection( int inputId );
		
		void FindXput(int x, int y);
		ModuleId GetModuleId();
		void SetGui( Gui* gui );
		void OpenGuiWindow();
		bool on_leave_notify_event(GdkEventCrossing* event);
		bool on_enter_notify_event(GdkEventCrossing* event);
		bool on_expose_event(GdkEventExpose* e);
		void on_realize();

	protected:
		Module* module;
		Gui* gui;
		int	x, y;
		int width, height;
		int inputCount, outputCount;
		int currentInput, currentOutput;
		bool isGuiWindowCreated;
		ModuleId moduleId;
		GuiConnection** inputGuiConnections;
		Glib::RefPtr<Gdk::GC> gc;
		Gdk::Color fgColor, bgColor;
		Glib::RefPtr<Gdk::Window> window;
		Glib::RefPtr<Gdk::Pixmap> pixmapBuffer;
		AlgorithmView* algorithmView;
		ModuleGuiWindow *guiWindow;
		Glib::RefPtr<Pango::Layout> pangolayout;

		void Repaint();
		void PaintInput(int num, bool isSelected);
		void PaintOutput(int num, bool isSelected);
};


#endif // GUIMODULE_H
