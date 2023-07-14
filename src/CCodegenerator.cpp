#include <CCodegenerator.h>

namespace Codegenerator {

CCodegenerator::CCodegenerator() {}

CCodegenerator::~CCodegenerator() {}

CTextToCPP *CCodegenerator::processVariableParams(
    const string &parameters, const string &text, const string &inputFileName,
    int *unnamedVarCount, int lineNumber, int signPerLine) {
  LOG(DEBUG) << "Linenumber: " << lineNumber << "\n";
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
    bool addtextpos = json.get(VAR_TEXTPOS, false).asBool();
    if (!addtextpos) {
      lineNumber = 0;
    }

    string sequenz = json.get(VAR_SEQENZ, "").asString();
    if (sequenz == SEQENZ_ESC) {
      return new CTextToEscSeq(
          varname, text, signPerLine, json.get(VAR_NEWLINE, NL_UNIX).asString(), lineNumber,
          json.get(VAR_TEXTSEGMENT, false).asBool(),
          json.get(VAR_DOXYGEN, "").asString());
    }
    if (sequenz == SEQENZ_HEX) {
      return new CTextToHexSeq(
          varname, text, signPerLine, json.get(VAR_NEWLINE, NL_UNIX).asString(), lineNumber,
          json.get(VAR_TEXTSEGMENT, false).asBool(),
          json.get(VAR_DOXYGEN, "").asString());
    }
    if (sequenz == SEQENZ_OCT) {
      return new CTextToOctSeq(
          varname, text, signPerLine, json.get(VAR_NEWLINE, NL_UNIX).asString(), lineNumber,
          json.get(VAR_TEXTSEGMENT, false).asBool(),
          json.get(VAR_DOXYGEN, "").asString());
    }
    if (sequenz == SEQENZ_RAWHEX) {
      return new CTextToRawHexSeq(
          varname, text, signPerLine, json.get(VAR_NEWLINE, NL_UNIX).asString(), lineNumber,
          json.get(VAR_TEXTSEGMENT, false).asBool(),
          json.get(VAR_DOXYGEN, "").asString());
    }
    LOG(ERROR) << "Keine implementierte Sequenz" << errors << endl;
    return nullptr;
  } else {
    LOG(ERROR) << "Fehler beim Parsen der Attribut-Parameter: " << errors
               << endl;
    return nullptr;
  }
}

string CCodegenerator::extractContentBetweenTags(const string &content,
                                                 const string &startTag,
                                                 const string &endTag) {
  int startPos = content.find(startTag);
  int endPos = content.find(endTag);
  if (startPos != string::npos && endPos != string::npos && startPos < endPos) {
    startPos += startTag.length();
    return content.substr(startPos, endPos - startPos);
  }
  return "";
}

/**
 * The function "getFileNameWithoutExtension" takes a filename as input and
 * returns the filename without the file extension.
 *
 * @param filename The "filename" parameter is a string that represents the name
 * of a file, including its extension.
 *
 * @return The filename without the file extension.
 */
string CCodegenerator::getFileNameWithoutExtension(const string &filename) {
  char dot = filename.find_last_of(".");
  char slash = filename.find_last_of("/");
  if (dot != string::npos && dot > slash) {
    return filename.substr(slash + 1, dot - slash - 1);
  }
  return filename;
}

void CCodegenerator::processString(const string &fileContent,
                                   const string &filename,
                                   CTextToCPP *extractedTextToCPP,
                                   Options *localeOptions) {
  string fileNameWithoutExt = getFileNameWithoutExtension(filename);

  // Keine Tags vorhanden
  if (!(fileContent.find("@start") != string::npos &&
        fileContent.find("@end") != string::npos)) {
    extractedTextToCPP->addElement(
        new CTextToEscSeq(fileNameWithoutExt, fileContent, localeOptions->getSignPerLine()));
    return;
  }

  // Tags vorhanden
  string extractedContent =
      extractContentBetweenTags(fileContent, "@start\n", "@end\n");
  if (extractedContent.empty()) {
    LOG(ERROR) << "Keine Parameter gefunden!" << endl;
    return;
  }

  string variable_options;
  string variable_text;
  int unnamedVariableCounter = 0;

  istringstream iss(extractedContent);
  string line;
  bool isComment = false;
  string currentVariableContent = "";
  int lineNumber = 1;
  while (getline(iss, line)) {
    if (line.find(TAG_GLOBAL) != string::npos) {
      size_t start = line.find('{');
      size_t end = line.find('}');

      if (start != std::string::npos && end != std::string::npos &&
          start < end) {
        // Extrahieren des Inhalts mit den geschweiften Klammern
        std::string content = line.substr(start, end - start + 1);
        if (!content.empty()) {
          localeOptions->parseLocalOptions(content, fileNameWithoutExt);
        }
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
      if (!currentVariableContent.empty() &&
          currentVariableContent.back() == '\n') {
        currentVariableContent.pop_back();
      }
      isComment = false;
      variable_text = currentVariableContent;
      LOG(DEBUG) << "Variablen Inhalt: " << variable_text << endl;
      currentVariableContent.clear();
      extractedTextToCPP->addElement(
          processVariableParams(variable_options, variable_text,
                                fileNameWithoutExt, &unnamedVariableCounter,
                                lineNumber, localeOptions->getSignPerLine()));

    } else if (isComment) {
      currentVariableContent += line + "\n";
      lineNumber++;
    }
    lineNumber++;
  }
}

}  // namespace Codegenerator