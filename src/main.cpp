#include <CTextToCPP.h>
#include <Options.h>
#include <easylogging++.h>
#include <getopt.h>
#include <jsoncpp/json/json.h>
#include <stdio.h>

#include <fstream>
#include <iostream>

using namespace std;
using namespace Codegenerator;

INITIALIZE_EASYLOGGINGPP

/**
 * @brief Initializes the logging system
 *
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

std::string getFileNameWithoutExtension(const std::string& filename) {
  size_t lastDot = filename.find_last_of(".");
  size_t slashIndex = filename.find_last_of("/");
  if (lastDot != std::string::npos && lastDot > slashIndex) {
    return filename.substr(slashIndex + 1, lastDot - slashIndex - 1);
  }
  return filename;
}

std::string extractContentBetweenTags(const std::string& content,
                                      const std::string& startTag,
                                      const std::string& endTag) {
  size_t startPos = content.find(startTag);
  size_t endPos = content.find(endTag);
  if (startPos != std::string::npos && endPos != std::string::npos &&
      startPos < endPos) {
    startPos += startTag.length();
    return content.substr(startPos, endPos - startPos);
  }
  return "";
}

void processParameters(const std::string& parameters) {
  // Hier können Sie die Parameter auswerten und die entsprechenden Optionen
  // definieren In diesem Beispiel geben wir einfach die Parameter im
  // JSON-Format aus
  Json::Value json;
  Json::CharReaderBuilder readerBuilder;
  std::unique_ptr<Json::CharReader> reader(readerBuilder.newCharReader());
  std::string errors;

  if (reader->parse(parameters.c_str(),
                    parameters.c_str() + parameters.length(), &json, &errors)) {
    std::cout << "Parameter: " << json.toStyledString() << std::endl;
  } else {
    std::cout << "Fehler beim Parsen der Parameter: " << errors << std::endl;
  }
}

int main(int argc, char** argv) {
  initLogging();

  Options options;
  options.parseOptions(argc, argv);

  vector<string> files = options.getFileNames();

  // Start of Codegenerator ---------------------------------------------

  // 1. Die Inputdatei öffnen und deren Inhalt lesen
  std::ifstream inputFile(files[0]);
  if (!inputFile) {
    std::cout << "Fehler beim Öffnen der Datei!" << std::endl;
    return 1;
  }

  std::string fileContent((std::istreambuf_iterator<char>(inputFile)),
                          std::istreambuf_iterator<char>());

  inputFile.close();

  // 2. Den Dateinamen ohne Erweiterung extrahieren und als Variablenname
  // verwenden
  std::string variableName = getFileNameWithoutExtension(files[0]);

  // 3. Überprüfen, ob der Inhalt der Datei nur reinen Text enthält
  if (fileContent.find("@start") != std::string::npos &&
      fileContent.find("@end") != std::string::npos) {
    std::string extractedContent =
        extractContentBetweenTags(fileContent, "@start", "@end");
    if (!extractedContent.empty()) {
      processParameters(extractedContent);
    } else {
      std::cout << "Keine Parameter gefunden!" << std::endl;
    }
  } else {
    cout << "Variable: " << variableName << endl;
    cout << "Inhalt: \n" << fileContent << endl;
  }

  return 0;
}