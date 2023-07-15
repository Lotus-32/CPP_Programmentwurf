/**
 * @file CTextToRawHexSeq.cpp
 * @author Marc Sachße
 * @brief Converts the variable text to a raw hex sequence
 * @version 1.0
 * @date 15-07-2023
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <CTextToRawHexSeq.h>

namespace Codegenerator {

/**
 * The CTextToRawHexSeq constructor initializes the member variables of the
 * class.
 *
 * @param name The name of the CTextToRawHexSeq variable.
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
CTextToRawHexSeq::CTextToRawHexSeq(string name, string text, int signperline,
                                   string nl, int addtextpos,
                                   bool addtextsegment, string doxygen) {
  this->name = name;
  this->text = text;
  this->nl = nl;
  this->addtextpos = addtextpos;
  this->addtextsegment = addtextsegment;
  this->doxygen = doxygen;
  this->signperline = signperline;
  next = nullptr;
  LOG(DEBUG) << "CTextToRawHexSeq: " << name << " " << nl << " " << addtextpos
             << " " << addtextsegment << " " << doxygen << "\n"
             << text;

  if (text == "") {
    cerr << "Error: The input file is not formatted correctly" << endl;
    exit(1);
  }
}

CTextToRawHexSeq::~CTextToRawHexSeq() {}

/**
 * The function `writeDeclaration` generates a C++ declaration for a variable,
 * including any associated doxygen comments.
 *
 * @return a string that represents the declaration of a variable.
 */
string CTextToRawHexSeq::writeDeclaration() {
  string declaration;

  if (doxygen != "") {
    declaration += "/** " + doxygen;
    if (addtextpos) {
      declaration += " (aus Zeile " + to_string(addtextpos) + ")";
    }
    declaration += " */\n";
  }

  declaration += "extern const char " + name + "[];\n";

  if (next != nullptr) {
    declaration += next->writeDeclaration();
  }
  return declaration;
}

/**
 * The function `writeImplementation` converts a given text into a sequence of
 * raw hexadecimal values and returns it as a string.
 *
 * @return a string that represents the implementation of the variable.
 */
string CTextToRawHexSeq::writeImplementation() {
  string imp;
  stringstream ss;

  for (char c : text) {
    if (c == '\n') {
      if (nl == "DOS" || nl == "MAC") {
        ss << "0x" << hex << setw(2) << setfill('0') << (int)'\r' << ", ";
        imp += ss.str();
        ss.str("");
        if (nl == "MAC") {
          continue;
        }
      }
    }
    ss << "0x" << hex << setw(2) << setfill('0') << (int)c << ", ";
    imp += ss.str();
    ss.str("");
  }
  // Komma und Leerzeichen am Ende entfernen
  if (!imp.empty()) {
    imp.pop_back();
    imp.pop_back();
  }

  imp = "const char " + name + "[] = {\n" + wordWrap(imp, ' ', signperline) +
        "\n};\n";

  if (addtextsegment) {
    imp += getOriginalTextComment();
  }

  if (next != nullptr) {
    return imp + "\n" + next->writeImplementation();
  }
  return imp;
}

/**
 * The function `wordWrap` overrides the `wordWrap` function from CTextToCPP
 * takes a string `text`, a character `cut`, and an integer `maxLineLength` as
 * input, and returns a new string with line breaks inserted at appropriate
 * positions to ensure that no line exceeds the specified maximum line length.
 *
 * @param text The input text that needs to be wrapped into multiple lines.
 * @param cut The "cut" parameter is a character that is used to determine where
 * to wrap the text. When the character specified by "cut" is encountered, the
 * text will be wrapped to the next line.
 * @param maxLineLength The `maxLineLength` parameter specifies the maximum
 * length of each line in the wrapped text.
 *
 * @return the wrapped text, which is a string.
 */
string CTextToRawHexSeq::wordWrap(string text, const char cut,
                                  int maxLineLength) {
  string wrappedText;
  int lineLength = 0;
  int lastSpace = 0;

  for (int i = 0; i < text.length(); i++) {
    if (text[i] == cut) {
      lastSpace = i;
    }

    if (lineLength >= maxLineLength) {
      wrappedText += text.substr(0, lastSpace) + " \\\n";

      text = text.substr(lastSpace);
      lineLength = 0;
      i = 0;
    }

    lineLength++;
  }

  return wrappedText + text;
}

}  // namespace Codegenerator