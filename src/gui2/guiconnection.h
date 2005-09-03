#ifndef GUICONNECTION_H
#define GUICONNECTION_H

#include "guimodules/guimodule.h"
#include "../algorithm.h"

struct GuiConnection {
	ConnectionId connectionId;
	int sourceX, sourceY;
	int destinationX, destinationY;
	GuiModule* sourceGuiModule, * destinationGuiModule;
	int sourceOutputId, destinationInputId;
	// TODO: Gtk::EventBox strzalka;

	void Set( ConnectionId connId, GuiModule* srcGuiMod, int srcId,
		GuiModule* destGuiMod, int destId );
	void Update();
	ConnectionId GetConnectionId();
};


#endif // GUICONNECTION_H
