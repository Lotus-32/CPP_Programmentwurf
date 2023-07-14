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
      varname = toUpperCases(varname);
      varname += to_string((*unnamedVarCount)++);
    }
    bool addtextpos = json.get(VAR_TEXTPOS, false).asBool();
    if (!addtextpos) {
      lineNumber = 0;
    }

    string sequenz = json.get(VAR_SEQENZ, "").asString();
    if (sequenz == SEQENZ_ESC) {
      return new CTextToEscSeq(
          varname, text, signPerLine, json.get(VAR_NEWLINE, NL_UNIX).asString(),
          lineNumber, json.get(VAR_TEXTSEGMENT, false).asBool(),
          json.get(VAR_DOXYGEN, "").asString());
    }
    if (sequenz == SEQENZ_HEX) {
      return new CTextToHexSeq(
          varname, text, signPerLine, json.get(VAR_NEWLINE, NL_UNIX).asString(),
          lineNumber, json.get(VAR_TEXTSEGMENT, false).asBool(),
          json.get(VAR_DOXYGEN, "").asString());
    }
    if (sequenz == SEQENZ_OCT) {
      return new CTextToOctSeq(
          varname, text, signPerLine, json.get(VAR_NEWLINE, NL_UNIX).asString(),
          lineNumber, json.get(VAR_TEXTSEGMENT, false).asBool(),
          json.get(VAR_DOXYGEN, "").asString());
    }
    if (sequenz == SEQENZ_RAWHEX) {
      return new CTextToRawHexSeq(
          varname, text, signPerLine, json.get(VAR_NEWLINE, NL_UNIX).asString(),
          lineNumber, json.get(VAR_TEXTSEGMENT, false).asBool(),
          json.get(VAR_DOXYGEN, "").asString());
    }
    cerr << "Thats not an implemented sequence!" << errors << endl;
    exit(1);
  } else {
    cerr << "Error parsing attribute parameters: " << errors << endl;
    exit(1);
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
  } else {
    return "";
  }
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
  } else {
    return filename;
  }
}

string CCodegenerator::toUpperCases(const string &input) {
  string output = input;
  transform(output.begin(), output.end(), output.begin(), ::toupper);
  return output;
}

void CCodegenerator::processString(const string &fileContent,
                                   const string &filename,
                                   CTextToCPP *extractedTextToCPP,
                                   Options *localeOptions) {
  string fileNameWithoutExt = getFileNameWithoutExtension(filename);

  // No tags
  if (!(fileContent.find("@start") != string::npos &&
        fileContent.find("@end") != string::npos)) {
    extractedTextToCPP->addElement(new CTextToEscSeq(
        fileNameWithoutExt, fileContent, localeOptions->getSignPerLine()));
    localeOptions->setOutputFilename(fileNameWithoutExt);
    return;
  }

  // Has tags
  string extractedContent =
      extractContentBetweenTags(fileContent, "@start\n", "@end\n");
  if (extractedContent.empty()) {
    cerr << "Din't find any parameters!" << endl;
    exit(1);
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
        // Extract the content between the curly braces
        std::string content = line.substr(start, end - start + 1);
        if (!content.empty()) {
          localeOptions->parseLocalOptions(content, fileNameWithoutExt);
        } else {
          cerr << "Error: options not correclty formatted!" << endl;
          exit(1);
        }
      }
    }
    if (line.find(TAG_VAR) != string::npos) {
      size_t start = line.find('{');
      size_t end = line.find('}');

      if (start != std::string::npos && end != std::string::npos &&
          start < end) {
        // Extract the content with the curly braces
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
      LOG(DEBUG) << "Variables Content: " << variable_text << endl;
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

string CCodegenerator::generateNamespace(const string &namespaceName,
                                         const string &namespaceContent) {
  return "namespace " + namespaceName + " {\n" + namespaceContent + "}";
}

string CCodegenerator::generateSourceHead(const string &filename,
                                          const string &fileContent) {
  return "#include <" + filename + ".h>\n\n" + fileContent;
}

string CCodegenerator::generateHeaderSurroundings(const string &filename,
                                                  const string &fileContent) {
  string nameCaps = toUpperCases(filename);
  return "#ifndef _" + nameCaps + "_\n#define _" + nameCaps + "_\n\n" +
         fileContent + "\n#endif";
}

}  // namespace Codegenerator