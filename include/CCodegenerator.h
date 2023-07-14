#ifndef CCODEGENERATOR_H
#define CCODEGENERATOR_H

#include <CTextToCPP.h>
#include <CTextToEscSeq.h>
#include <CTextToHexSeq.h>
#include <CTextToOctSeq.h>
#include <CTextToRawHexSeq.h>
#include <Options.h>
#include <easylogging++.h>
#include <jsoncpp/json/json.h>

using namespace std;

namespace Codegenerator {

class CCodegenerator {
 private:
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

  CTextToCPP* processVariableParams(const string& parameters,
                                    const string& text,
                                    const string& inputFileName,
                                    int* unnamedVarCount, int lineNumber,
                                    int signPerLine);

  string extractContentBetweenTags(const string& content,
                                   const string& startTag,
                                   const string& endTag);

  string getFileNameWithoutExtension(const string& filename);

 public:
  CCodegenerator();
  ~CCodegenerator();

  void processString(const string& fileContent, const string& filename,
                     CTextToCPP* extractedTextToCPP, Options* localeOptions);
};

}  // namespace Codegenerator

#endif