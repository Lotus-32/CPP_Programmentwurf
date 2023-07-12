#include <CTextToCPP.h>
#include <CTextToEscSeq.h>
#include <CTextToHexSeq.h>
#include <CTextToOctSeq.h>
#include <CTextToRawHexSeq.h>
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

const string VAR_NAME = "varname";
const string VAR_SEQENZ = "seq";
const string VAR_NEWLINE = "nl";
const string VAR_TEXTPOS = "addtextpos";
const string VAR_TEXTSEGMENT = "addtextsegment";
const string VAR_DOXYGEN = "doxygen";
const string SEQENZ_ESC = "ESC";
const string SEQENZ_HEX = "HEX";
const string SEQENZ_OCT = "OCT";
const string SEQENZ_RAWHEX = "RAWHEX";
const string NL_DOS = "DOS";
const string NL_UNIX = "UNIX";
const string NL_MAC = "MAC";
const string TAG_START = "@start";
const string TAG_END = "@end";
const string TAG_GLOBAL = "@global";
const string TAG_VAR = "@variable";
const string TAG_ENDVAR = "@endvariable";

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

CTextToCPP* processVariableParams(const string& parameters, const string& text,
                                  const string& inputFileName,
                                  int* unnamedVarCount) {
  Json::Value json;
  Json::CharReaderBuilder readerBuilder;
  unique_ptr<Json::CharReader> reader(readerBuilder.newCharReader());
  string errors;

  if (reader->parse(parameters.c_str(),
                    parameters.c_str() + parameters.length(), &json, &errors)) {
    string varname = json.get(VAR_NAME, inputFileName).asString();
    if (!json.isMember(VAR_NAME)) {
      transform(varname.begin(), varname.end(), varname.begin(), ::toupper);
      varname += to_string((*unnamedVarCount)++);
    }
    string sequenz = json.get(VAR_SEQENZ, "").asString();
    if (sequenz == SEQENZ_ESC) {
      return new CTextToEscSeq(varname, text,
                               json.get(VAR_NEWLINE, NL_UNIX).asString(),
                               json.get(VAR_TEXTPOS, false).asBool(),
                               json.get(VAR_TEXTSEGMENT, false).asBool(),
                               json.get(VAR_DOXYGEN, "").asString());
    }
    if (sequenz == SEQENZ_HEX) {
      return new CTextToHexSeq(varname, text,
                               json.get(VAR_NEWLINE, NL_UNIX).asString(),
                               json.get(VAR_TEXTPOS, false).asBool(),
                               json.get(VAR_TEXTSEGMENT, false).asBool(),
                               json.get(VAR_DOXYGEN, "").asString());
    }
    if (sequenz == SEQENZ_OCT) {
      return new CTextToOctSeq(varname, text,
                               json.get(VAR_NEWLINE, NL_UNIX).asString(),
                               json.get(VAR_TEXTPOS, false).asBool(),
                               json.get(VAR_TEXTSEGMENT, false).asBool(),
                               json.get(VAR_DOXYGEN, "").asString());
    }
    if (sequenz == SEQENZ_RAWHEX) {
      return new CTextToRawHexSeq(varname, text,
                                  json.get(VAR_NEWLINE, NL_UNIX).asString(),
                                  json.get(VAR_TEXTPOS, false).asBool(),
                                  json.get(VAR_TEXTSEGMENT, false).asBool(),
                                  json.get(VAR_DOXYGEN, "").asString());
    }
    LOG(ERROR) << "Keine implementierte Sequenz" << errors << endl;
    return nullptr;
  } else {
    LOG(ERROR) << "Fehler beim Parsen der Parameter: " << errors << endl;
    return nullptr;
  }
}

int main(int argc, char** argv) {
  initLogging();

  Options options;
  options.parseOptions(argc, argv);

  vector<string> files = options.getFileNames();

  CTextToCPP* textToCPP = nullptr;

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

    string variableName = getFileNameWithoutExtension(file);

    // Keine Tags vorhanden
    if (!(fileContent.find("@start") != string::npos &&
          fileContent.find("@end") != string::npos)) {
      textToCPP = new CTextToEscSeq(variableName, fileContent);
      continue;
    }

    string extractedContent =
        extractContentBetweenTags(fileContent, "@start\n", "@end\n");
    if (extractedContent.empty()) {
      LOG(ERROR) << "Keine Parameter gefunden!" << endl;
      continue;
    }

    vector<string> globales;

    string variable_options;
    string variable_text;
    int unnamedVariableCounter = 0;

    istringstream iss(extractedContent);
    string line;
    bool isComment = false;
    string currentVariableContent = "";
    while (getline(iss, line)) {
      if (line.find(TAG_GLOBAL) != string::npos) {
        size_t start = line.find('{');
        size_t end = line.find('}');

        if (start != std::string::npos && end != std::string::npos &&
            start < end) {
          // Extrahieren des Inhalts mit den geschweiften Klammern
          std::string content = line.substr(start, end - start + 1);
          globales.push_back(content);
        }
      }
      if (line.find(TAG_VAR) != string::npos) {
        size_t start = line.find('{');
        size_t end = line.find('}');

        if (start != std::string::npos && end != std::string::npos &&
            start < end) {
          // Extrahieren des Inhalts mit den geschweiften Klammern
          std::string content = line.substr(start, end - start + 1);
          variable_options = content;
          isComment = true;
        }
      } else if (line.find(TAG_ENDVAR) != string::npos) {
        isComment = false;
        variable_text = currentVariableContent;
        currentVariableContent.clear();
        if (textToCPP == nullptr) {
          textToCPP =
              processVariableParams(variable_options, variable_text,
                                    variableName, &unnamedVariableCounter);
        } else {
          textToCPP->addElement(
              processVariableParams(variable_options, variable_text,
                                    variableName, &unnamedVariableCounter));
        }

      } else if (isComment) {
        currentVariableContent += line + " ";
      }
    }
    for (auto& global : globales) {
      LOG(DEBUG) << "Global: " << global << endl;
    }

    if (textToCPP != nullptr) {
      LOG(INFO) << "Inhalt: \n" << textToCPP->writeDeclaration() << endl;
      textToCPP->clear();
      LOG(INFO) << "Inhalt nach clear: \n"
                << textToCPP->writeDeclaration() << endl;
    }
    delete textToCPP;
  }

  LOG(INFO) << "Inhalt: \n" << textToCPP->writeDeclaration() << endl;
  textToCPP->clear();
  LOG(INFO) << "Inhalt nach clear: \n" << textToCPP->writeDeclaration() << endl;

  delete textToCPP;

  return 0;
}