/**
 * @file CTextToOctSeq.cpp
 * @author Marc Sachße
 * @brief Converts the variable text to a C-String with octal sequences
 * @version 1.0
 * @date 15-07-2023
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <CTextToOctSeq.h>

namespace Codegenerator {

/**
 * The CTextToOctSeq constructor initializes the member variables of the class.
 *
 * @param name The name of the CTextToOctSeq variable.
 * @param text The "text" parameter is a string that represents the input text
 * that needs to be converted.
 * @param signperline The "signperline" parameter specifies the number of
 * characters to be displayed per line after converting.
 * @param nl The "nl" parameter stands for "new line" and it represents the
 * the operating system of the new line.
 * @param addtextpos The parameter "addtextpos" is an integer that represents
 * the position at which the variable was in the source file.
 * @param addtextsegment A boolean flag indicating whether to add the original
 * text as a comment.
 * @param doxygen The "doxygen" parameter is a string that represents the
 * doxygen documentation for the variable.
 */
CTextToOctSeq::CTextToOctSeq(string name, string text, int signperline,
                             string nl, int addtextpos, bool addtextsegment,
                             string doxygen) {
  this->name = name;
  this->text = text;
  this->nl = nl;
  this->addtextpos = addtextpos;
  this->addtextsegment = addtextsegment;
  this->doxygen = doxygen;
  this->signperline = signperline;
  next = nullptr;
  LOG(DEBUG) << "CTextToOctSeq: " << name << " " << nl << " " << addtextpos
             << " " << addtextsegment << " " << doxygen << "\n"
             << text;

  if (text.empty()) {
    cerr << "Error: The input file is not formatted correctly" << endl;
    exit(1);
  }
}

CTextToOctSeq::~CTextToOctSeq() {}

/**
 * The function `writeImplementation` converts a given text into an octal
 * sequence and returns it as a string.
 *
 * @return a string that represents the implementation of variable.
 */
string CTextToOctSeq::writeImplementation() {
  string imp;
  stringstream ss;
  bool addnewline = false;

  for (char c : text) {
    if (c == '\n') {
      if (nl == "DOS" || nl == "MAC") {
        ss << oct << setw(3) << setfill('0') << (int)'\r';
        imp += "\\";
        imp += ss.str();
        ss.str("");
        if (nl == "MAC") {
          imp += "\n";
          continue;
        }
      }
      addnewline = true;
    }
    ss << oct << setw(3) << setfill('0') << (int)c;
    imp += "\\";
    imp += ss.str();
    ss.str("");
    if (addnewline) {
      imp += "\n";
      addnewline = false;
    }
  }

  imp = VAR_IMPLEMENTATION + name + " = {\n" +
        wordWrap(imp, '\\', signperline) + "\n};\n";

  if (addtextsegment) {
    imp += getOriginalTextComment();
  }

  if (next != nullptr) {
    return imp + "\n" + next->writeImplementation();
  }
  return imp;
}

}  // namespace Codegenerator