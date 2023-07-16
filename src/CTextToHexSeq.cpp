/**
 * @file CTextToHexSeq.cpp
 * @author Marc Sachße
 * @brief Converts the variable text to a C-String with hex sequences
 * @version 1.0
 * @date 15-07-2023
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <CTextToHexSeq.h>

namespace Codegenerator {

/**
 * The CTextToHexSeq constructor initializes the member variables of the class.
 *
 * @param name The name of the CTextToHexSeq variable.
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
CTextToHexSeq::CTextToHexSeq(string name, string text, int signperline,
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
  LOG(DEBUG) << "CTextToHexSeq: " << name << " " << nl << " " << addtextpos
             << " " << addtextsegment << " " << doxygen << "\n"
             << text;
  if (text == "") {
    cerr << "Error: The input file is not formatted correctly" << endl;
    exit(1);
  }
}

CTextToHexSeq::~CTextToHexSeq() {}

/**
 * The function `writeImplementation` converts a given text into a hexadecimal
 * sequence and returns it as a string.
 *
 * @return a string that represents the implementation of the variable.
 */
string CTextToHexSeq::writeImplementation() {
  string imp;
  stringstream ss;
  bool addnewline = false;

  for (char c : text) {
    if (c == '\n') {
      if (nl == "DOS" || nl == "MAC") {
        ss << hex << setw(2) << setfill('0') << (int)'\r';
        imp += "\\x";
        imp += ss.str();
        ss.str("");
        if (nl == "MAC") {
          imp += "\n";
          continue;
        }
      }
      addnewline = true;
    }
    ss << hex << setw(2) << setfill('0') << (int)c;
    imp += "\\x";
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