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
      return new CTextToEscSeq(varname,
                               json.get(VAR_NEWLINE, NL_UNIX).asString(),
                               json.get(VAR_TEXTPOS, false).asBool(),
                               json.get(VAR_TEXTSEGMENT, false).asBool(),
                               json.get(VAR_DOXYGEN, "").asString(), text);
    }
    if (sequenz == SEQENZ_HEX) {
      return new CTextToHexSeq(varname,
                               json.get(VAR_NEWLINE, NL_UNIX).asString(),
                               json.get(VAR_TEXTPOS, false).asBool(),
                               json.get(VAR_TEXTSEGMENT, false).asBool(),
                               json.get(VAR_DOXYGEN, "").asString(), text);
    }
    if (sequenz == SEQENZ_OCT) {
      return new CTextToOctSeq(varname,
                               json.get(VAR_NEWLINE, NL_UNIX).asString(),
                               json.get(VAR_TEXTPOS, false).asBool(),
                               json.get(VAR_TEXTSEGMENT, false).asBool(),
                               json.get(VAR_DOXYGEN, "").asString(), text);
    }
    if (sequenz == SEQENZ_RAWHEX) {
      return new CTextToRawHexSeq(varname,
                                  json.get(VAR_NEWLINE, NL_UNIX).asString(),
                                  json.get(VAR_TEXTPOS, false).asBool(),
                                  json.get(VAR_TEXTSEGMENT, false).asBool(),
                                  json.get(VAR_DOXYGEN, "").asString(), text);
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
      // TODO: CTo...
      LOG(DEBUG) << "Variable: " << variableName << endl;
      LOG(DEBUG) << "Inhalt: \n" << fileContent << endl;
      continue;
    }

    string extractedContent =
        extractContentBetweenTags(fileContent, "@start\n", "@end\n");
    if (extractedContent.empty()) {
      LOG(ERROR) << "Keine Parameter gefunden!" << endl;
      continue;
    }

    // LOG(DEBUG) << "Content: \n" << extractedContent << endl;
    vector<string> globales;
    vector<string> variables;
    vector<string> variablesText;

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
          variables.push_back(content);
          isComment = true;
        }
      } else if (line.find(TAG_ENDVAR) != string::npos) {
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

    int unnamedVariableCounter = 0;
    for (int i = 0; i < variables.size(); i++) {
      CTextToCPP* var =
          processVariableParams(variables[i], variablesText[i], variableName,
                                &unnamedVariableCounter);
      if (var == nullptr) {
        LOG(ERROR) << "Fehler beim Verarbeiten der Variable: " << variables[i]
                   << endl;
        cout << "Inkorrekte Parameter in der Datei: " << file << endl;
        return 1;
      }

      // TODO: Variablen in CTo... stecken
    }

    /* CTextToCPP textToCPP = CTextToCPP("Das ist ein Test");
    CTextToCPP textToCPP2 = CTextToCPP("Das ist ein Test2");
    CTextToCPP textToCPP3 = CTextToCPP("Das ist ein Test3");

    textToCPP.addElement(textToCPP2);
    textToCPP.addElement(textToCPP3);
    textToCPP.writeDeclaration();
    textToCPP.clear();
    textToCPP.writeDeclaration(); */
  }

  return 0;
}