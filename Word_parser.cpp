
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "HashTable.h"
#include "File_io.h"

using namespace std;

int main(int argc, char *argv[])
{
	fstream dictionaryFile;
	fstream zoneFile;
	HashTable dictionaryTable;
	dictionaryFile.open(argv[1]);
	if (dictionaryFile.is_open()) {
		File_IO fileHandler(dictionaryFile);
		fileHandler.readTxtInput(dictionaryTable);
		dictionaryFile.close();

		fstream zoneFileHandler;
		zoneFileHandler.open(argv[2]);
		if (zoneFileHandler.is_open()) {
			File_IO zoneFileObject(zoneFileHandler);
			zoneFileObject.readZoneInput(dictionaryTable);
		}

	}

	return 0;
}