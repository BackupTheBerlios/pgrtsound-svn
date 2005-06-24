#ifndef GUIMODULEWIDGET_H
#define GUIMODULEWIDGET_H

#include <string>

#include <gtkmm/eventbox.h>

#include "debug.h"
#include "guimodules/guimodule.h"

// deklaracja potrzebna (naglowek "algorithmview.h" dalby petle plikow naglowkowych)
class AlgorithmView;

/**
 * Graficzny element GTK reprezentujacy modul.
 */
class GuiModuleWidget : public Gtk::EventBox {
	public:
		GuiModuleWidget(GuiModule* guiMod, AlgorithmView* algoView,
			int inCount, int outCount);
		~GuiModuleWidget();
		void SetText(std::string newText);
		void PaintInput(int num, bool isSelected);
		void PaintOutput(int num, bool isSelected);
		void FindXput(int x, int y);
		GuiModule* GetGuiModule();
		int GetCurrentInputNumber();
		int GetCurrentOutputNumber();
		bool on_leave_notify_event(GdkEventCrossing* event);
		bool on_enter_notify_event(GdkEventCrossing* event);
		bool on_expose_event(GdkEventExpose* e);
		void on_realize();
		
		static const int socketSize;
		static const int socketSizeDoubled;

	protected:
		int width, height;
		int inputCount, outputCount;
		int currentInput, currentOutput;
		std::string text;

		// graphic context i kolory
		Glib::RefPtr<Gdk::GC> gc;
		Gdk::Color fgColor, bgColor;

		// wskaznik do GuiModuleView - rodzica
		AlgorithmView* algorithmView;
		
		GuiModule* guiModule;
};

#endif // GUIMODULEWIDGET_H
