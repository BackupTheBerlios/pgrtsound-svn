#ifndef XMLCONFIGFILE_H
#define XMLCONFIGFILE_H

#include <map>

#include "tinyxml.h"
#include "algorithm.h"
#include "audiodriver.h"

/**
 * Obiekt pliku konfiguracyjnego XML.
 * Umozliwa wczytywanie konfiguracji algorytmu zapisanej w pliku XML ustalonego
 * formatu.
 */
class XMLConfigFile
{
	public:
		XMLConfigFile();
		~XMLConfigFile();
		
		void OpenFile(const char * fileName);
		void LoadAlgorithm(Algorithm* algo);
		void LoadAudioDriveSettings(AudioDriver* audio);
		void LoadModules(Algorithm* algo);
		void LoadParameters(Algorithm* algo);
		void LoadConnections(Algorithm* algo);
		
	private:
		string fileName;
		TiXmlDocument document;
		map <string, int>	moduleName2IdMap;	// mapa asocjacyjna nazwy z id
};

#endif // XMLCONFIGFILE_H
