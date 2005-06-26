#include "guiconnection.h"

void GuiConnection::Set(ConnectionId* connId, GuiModule* srcGuiMod, int srcId,
	GuiModule* destGuiMod, int destId)
{
    connectionId = connId;
	sourceGuiModule = srcGuiMod;
	destinationGuiModule = destGuiMod;
	sourceOutputId = srcId;
	destinationInputId = destId;
	
	Update();
}

void GuiConnection::Update() {
   //	int x, y;
	//sourceGuiModule->GetPosition(x, y);
	sourceGuiModule->GetOutputPosition(sourceOutputId, sourceX, sourceY);
	//sourceX += x; sourceY += y;

	if(destinationGuiModule != NULL) {
		//destinationGuiModule->GetPosition(x, y);
		destinationGuiModule->GetInputPosition(destinationInputId, destinationX, destinationY);
		//destinationX += x; destinationY += y;
	}
}
