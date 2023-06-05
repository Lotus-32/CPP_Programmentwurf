#include <CTextToCPP.h>
#include <Options.h>
#include <easylogging++.h>
#include <getopt.h>
#include <jsoncpp/json/json.h>
#include <stdio.h>

#include <fstream>
#include <iostream>
#include <sstream>

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

string getFileNameWithoutExtension(const string& filename) {
  char dot = filename.find_last_of(".");
  char slash = filename.find_last_of("/");
  if (dot != string::npos && dot > slash) {
    return filename.substr(slash + 1, dot - slash - 1);
  }
  return filename;
}

string extractContentBetweenTags(const string& content, const string& startTag,
                                 const string& endTag) {
  int startPos = content.find(startTag);
  int endPos = content.find(endTag);
  if (startPos != string::npos && endPos != string::npos && startPos < endPos) {
    startPos += startTag.length();
    return content.substr(startPos, endPos - startPos);
  }
  return "";
}

void processParameters(const string& parameters) {
  // Hier können Sie die Parameter auswerten und die entsprechenden Optionen
  // definieren In diesem Beispiel geben wir einfach die Parameter im
  // JSON-Format aus
  Json::Value json;
  Json::CharReaderBuilder readerBuilder;
  unique_ptr<Json::CharReader> reader(readerBuilder.newCharReader());
  string errors;

  if (reader->parse(parameters.c_str(),
                    parameters.c_str() + parameters.length(), &json, &errors)) {
    // LOG(DEBUG) << "Parameter: " << json.toStyledString() << endl;
  } else {
    LOG(ERROR) << "Fehler beim Parsen der Parameter: " << errors << endl;
  }
}

int main(int argc, char** argv) {
  initLogging();

  Options options;
  options.parseOptions(argc, argv);

  vector<string> files = options.getFileNames();

  // Start of Codegenerator ---------------------------------------------

  // 1. Die Inputdatei öffnen und deren Inhalt lesen
  ifstream inputFile(files[0]);
  if (!inputFile) {
    LOG(ERROR) << "Fehler beim Öffnen der Datei!" << endl;
    return 1;
  }

  string fileContent((istreambuf_iterator<char>(inputFile)),
                     istreambuf_iterator<char>());

  inputFile.close();

  // 2. Den Dateinamen ohne Erweiterung extrahieren und als Variablenname
  // verwenden
  string variableName = getFileNameWithoutExtension(files[0]);

  // 3. Überprüfen, ob der Inhalt der Datei nur reinen Text enthält
  if (fileContent.find("@start") != string::npos &&
      fileContent.find("@end") != string::npos) {
    string extractedContent =
        extractContentBetweenTags(fileContent, "@start\n", "@end\n");
    if (!extractedContent.empty()) {
      // LOG(DEBUG) << "Content: \n" << extractedContent << endl;
      vector<string> globales;
      vector<string> variables;
      vector<string> variablesText;
      istringstream iss(extractedContent);
      string line;
      bool isComment = false;
      string currentVariableContent = "";
      while (getline(iss, line)) {
        if (line.find("@global") != string::npos) {
          size_t start = line.find('{');
          size_t end = line.find('}');

          if (start != std::string::npos && end != std::string::npos &&
              start < end) {
            // Extrahieren des Inhalts mit den geschweiften Klammern
            std::string content = line.substr(start, end - start + 1);
            processParameters(content);  // TODO: Parameter auswerten
            globales.push_back(content);
          }
        }
        if (line.find("@variable") != string::npos) {
          size_t start = line.find('{');
          size_t end = line.find('}');

          if (start != std::string::npos && end != std::string::npos &&
              start < end) {
            // Extrahieren des Inhalts mit den geschweiften Klammern
            std::string content = line.substr(start, end - start + 1);
            processParameters(content);  // TODO: Parameter auswerten
            variables.push_back(content);
            isComment = true;
          }
        } else if (line.find("@endvariable") != string::npos) {
          isComment = false;
          variablesText.push_back(currentVariableContent);
          currentVariableContent.clear();
        } else if (isComment) {
          currentVariableContent += line + " ";
        }
      }
      for (auto& global : globales) {
        LOG(DEBUG) << "Global: " << global << endl;
      }
      for (auto& variable : variables) {
        LOG(DEBUG) << "Variable: " << variable << endl;
      }
      for (auto& text : variablesText) {
        LOG(DEBUG) << "VariablesText: " << text << endl;
      }
      // processParameters(extractedContent);
    } else {
      LOG(ERROR) << "Keine Parameter gefunden!" << endl;
    }
  } else {
    LOG(INFO) << "Variable: " << variableName << endl;
    LOG(INFO) << "Inhalt: \n" << fileContent << endl;
  }

  return 0;
}