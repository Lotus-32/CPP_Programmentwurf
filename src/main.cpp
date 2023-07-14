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

#include <filesystem>
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

void createFile(const string& fileName, const string& content,
                const string& path) {
  string correctedPath = path;
  replace(correctedPath.begin(), correctedPath.end(), '\\', '/');
  LOG(DEBUG) << "Pfad: " << correctedPath << endl;

  if (!filesystem::exists(correctedPath)) {
    if (!filesystem::create_directories(correctedPath)) {
      LOG(ERROR) << "Fehler beim Erstellen des Verzeichnisses: "
                 << correctedPath << endl;
      exit(1);
    }
  }

  ofstream outputFile(correctedPath + "/" + fileName);
  if (!outputFile) {
    LOG(ERROR) << "Fehler beim Erstellen der Datei: " << fileName << endl;
    exit(1);
  }

  outputFile << content;

  LOG(INFO) << "Datei erstellt: " << correctedPath << fileName << endl;
}

string toLowerCases(const string& input) {
  string output = input;
  transform(output.begin(), output.end(), output.begin(), ::tolower);
  return output;
}

int main(int argc, char** argv) {
  initLogging();

  CTextToCPP* textToCPP = new CTextToCPP();
  Options* commandOptions = new Options();
  commandOptions->parseGlobaleOptions(argc, argv);
  vector<string> files = commandOptions->getFileNames();

  string header = "";
  string source = "";

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

    Options* localeOptions = new Options(*commandOptions);
    CCodegenerator* codegenerator = new CCodegenerator();
    codegenerator->processString(fileContent, file, textToCPP, localeOptions);

    if (localeOptions->getSortByVarName()) {
      textToCPP->sort();
    }
    string declaration = textToCPP->writeDeclaration();
    string implementation = textToCPP->writeImplementation();
    if (localeOptions->getNamespace() != "" &&
        localeOptions->getOutputType() != "C") {
      declaration = codegenerator->generateNamespace(
          localeOptions->getNamespace(), declaration);
      implementation = codegenerator->generateNamespace(
          localeOptions->getNamespace(), implementation);
    }
    if (commandOptions->getIsSetOutputFilename()) {
      header += declaration;
      source += implementation;
    } else {
      declaration = codegenerator->generateHeaderSurroundings(
          localeOptions->getOutputFilename(), declaration);
      implementation = codegenerator->generateSourceHead(
          localeOptions->getOutputFilename(), implementation);

      string sourceType = "." + toLowerCases(localeOptions->getOutputType());
      createFile(localeOptions->getOutputFilename() + ".h", declaration,
                 localeOptions->getHeaderDir());
      createFile(localeOptions->getOutputFilename() + sourceType,
                 implementation, localeOptions->getSourceDir());
    }

    // ----Testausgaben---------------------------------------------------

    // LOG(INFO) << "Inhalt: \n" << textToCPP->writeDeclaration() << endl;
    // textToCPP->sort();
    // LOG(INFO) << "Inhalt nach sort: \n"
    //           << textToCPP->writeDeclaration() << endl;
    // textToCPP->addElement(new CTextToEscSeq("Test", "Hallo Welt",
    //                                         localeOptions->getSignPerLine()));
    // textToCPP->addElement(new CTextToHexSeq("Ann_den_Anfang", "Hallo Welt",
    //                                         localeOptions->getSignPerLine()));
    // textToCPP->sort();

    // LOG(INFO) << "Declaration mit Namespace: \n" << declaration << endl;
    // LOG(INFO) << "Implementierung mit Namespache: \n" << implementation <<
    // endl;

    // ----Ende:-Testausgaben---------------------------------------------------

    delete codegenerator;
    delete localeOptions;
    textToCPP->clear();
  }
  delete textToCPP;

  return 0;
}