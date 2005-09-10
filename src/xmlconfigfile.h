#ifndef XMLCONFIGFILE_H
#define XMLCONFIGFILE_H

#include "tinyxml.h"
#include "algorithm.h"
#include "audiodriver.h"

#include <map>

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

	private:
   		void LoadAlgorithmSettings(Algorithm* algo);
		void LoadParameters(Algorithm* algo);
		void LoadConnections(Algorithm* algo);

		string fileName;
		TiXmlDocument document;
};

#endif // XMLCONFIGFILE_H
