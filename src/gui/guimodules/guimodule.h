#ifndef GUIMODULE_H
#define GUIMODULE_H

#include <string>
#include <gtkmm/eventbox.h>

#include "debug.h"
#include "../../modules/module.h"

// deklaracja potrzebna (naglowek "algorithmview.h" dalby petle plikow naglowkowych)
// niestety jest zaleznosc GuiModule od AlgorithmView, ale moze uda sie ja usunac
// pozniej
class AlgorithmView;

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
		int GetX();
		int GetY();
		Module* GetModule();
		void PaintInput(int num, bool isSelected);
		void PaintOutput(int num, bool isSelected);
		void FindXput(int x, int y);
		int GetCurrentInputNumber();
		int GetCurrentOutputNumber();
		bool on_leave_notify_event(GdkEventCrossing* event);
		bool on_enter_notify_event(GdkEventCrossing* event);
		bool on_expose_event(GdkEventExpose* e);
		void on_realize();

		static const int socketSize;
		static const int socketSizeDoubled;

	protected:
		Module* module;
		int	x, y;
		int width, height;
		int inputCount, outputCount;
		int currentInput, currentOutput;
		std::string text;

		// graphic context i kolory
		Glib::RefPtr<Gdk::GC> gc;
		Gdk::Color fgColor, bgColor;

		// wskaznik do GuiModuleView - rodzica
		AlgorithmView* algorithmView;
};

#endif // GUIMODULE_H
