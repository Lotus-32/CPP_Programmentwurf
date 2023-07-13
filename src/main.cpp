#include <CCodegenerator.h>
#include <CTextToCPP.h>
#include <CTextToEscSeq.h>
#include <CTextToHexSeq.h>
#include <CTextToOctSeq.h>
#include <CTextToRawHexSeq.h>
#include <Options.h>
#include <easylogging++.h>
#include <getopt.h>
#include <stdio.h>

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
using namespace Codegenerator;

INITIALIZE_EASYLOGGINGPP

/**
 * This function initializes logging.
 */
void initLogging() {
  // Laedt das Konfigurationsfile und konfiguriert die Logger
  el::Configurations conf("assets/logging.ini");
  // Schaltet Mechanismus aus der das Programm bei fatalem Error beendet
  el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
  // Bei Systemcrash wird der Crashgrund in der Log Datei noch hinterlegt
  el::Loggers::addFlag(el::LoggingFlag::LogDetailedCrashReason);
  // Falls Bildschirmausgabe, dann farbig
  el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
  // Uebertraegt die Einstellungen an die Logger die laufen
  el::Loggers::reconfigureLogger("default", conf);
  el::Loggers::reconfigureAllLoggers(conf);

  // LOG(INFO) << "Infos zum Ablauf";
  // LOG(DEBUG) << "Debugging von Programmteile";
  // LOG(WARNING) << "Das Programm kann noch weiterlaufen";
  // LOG(ERROR) << "Das ist schon ein richtig schwerwiegender Fehler";
  // LOG(FATAL) << "Hier fehlt z.B. schon der Speicher um weiterzu machen";
}

int main(int argc, char** argv) {
  initLogging();

  Options options;
  options.parseGlobaleOptions(argc, argv);

  vector<string> files = options.getFileNames();

  CTextToCPP* textToCPP = new CTextToCPP();

  // Start of Codegenerator ---------------------------------------------

  for (string file : files) {
    LOG(INFO) << "Datei: " << file << endl;

    // Inputdatei oeffnen und Inhalt lesen
    ifstream inputFile(file);
    if (!inputFile) {
      LOG(ERROR) << "Fehler beim Oeffnen der Datei: " << file << endl;
      continue;
    }

    string fileContent((istreambuf_iterator<char>(inputFile)),
                       istreambuf_iterator<char>());
    inputFile.close();

    string* locales = new string();
    CCodegenerator* codegenerator = new CCodegenerator();
    codegenerator->processString(fileContent, file, textToCPP, locales);

    if (!locales->empty()) {
      LOG(INFO) << "Globale Variablen: \n" << *locales << endl;
      options.parseLocalOptions(*locales, file);
    }

    // ----Testausgaben---------------------------------------------------

    // LOG(INFO) << "Globale Variablen: \n" << *locales << endl;

    // LOG(INFO) << "Inhalt: \n" << textToCPP->writeDeclaration() << endl;
    // textToCPP->sort();
    // LOG(INFO) << "Inhalt nach sort: \n"
    //           << textToCPP->writeDeclaration() << endl;
    // textToCPP->addElement(new CTextToEscSeq("Test", "Hallo Welt"));
    // textToCPP->addElement(new CTextToHexSeq("Ann_den_Anfang", "Hallo Welt"));
    // textToCPP->sort();
    LOG(INFO) << "Inhalt nach sort: \n"
              << textToCPP->writeDeclaration() << endl;

    LOG(INFO) << "Implementierung: \n"
              << textToCPP->writeImplementation() << endl;

    // ----Ende:-Testausgaben---------------------------------------------------

    delete codegenerator;
    delete locales;
    textToCPP->clear();
  }
  delete textToCPP;

  return 0;
}