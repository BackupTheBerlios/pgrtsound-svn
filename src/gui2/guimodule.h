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
		GuiModule(Module* moduleToAttach);
		~GuiModule();
		void SetParentView(AlgorithmView* algoView);
		void SetXY(int x_, int y_);
		void SetText(std::string newText);
		virtual Gtk::Widget* GetGui();
		Module* GetModule();
		void PaintInput(int num, bool isSelected);
		void PaintOutput(int num, bool isSelected);
		void FindXput(int x, int y);
		void GetPosition(int& x, int& y);
		void GetInputPosition(int inNum, int& x, int& y);
		void GetOutputPosition(int outNum, int& x, int& y);
		int GetCurrentInputNumber();
		int GetCurrentOutputNumber();
		void SetInputGuiConnection( int inputId, GuiConnection* conn );
		GuiConnection* GetInputGuiConnection( int inputId );
		void SetModuleId( ModuleId modId );
		ModuleId GetModuleId();
		void SetGui( Gui* gui );
		void OpenGuiWindow(Gtk::Window& parent);
		bool on_leave_notify_event(GdkEventCrossing* event);
		bool on_enter_notify_event(GdkEventCrossing* event);
		bool on_expose_event(GdkEventExpose* e);
		void on_realize();

		static const int socketSize;
		static const int socketSizeDoubled;

	protected:
		Module* module;
		Gui* gui;
		int	x, y;
		int width, height;
		int inputCount, outputCount;
		int currentInput, currentOutput;
		std::string text;
		ModuleId moduleId;
		GuiConnection** inputGuiConnections;
		Glib::RefPtr<Gdk::GC> gc;
		Gdk::Color fgColor, bgColor;
		Glib::RefPtr<Gdk::Window> window;
		Glib::RefPtr<Gdk::Pixmap> pixmapBuffer;
		AlgorithmView* algorithmView;
		ModuleGuiWindow *guiWindow;
		bool isGuiWindowCreated;
		Glib::RefPtr<Pango::Layout> pangolayout;
};

#endif // GUIMODULE_H
