/**
 * @file CTextToEscSeq.cpp
 * @author Marc Sachße
 * @brief Converts the variable text to a C-String with escape sequences
 * @version 1.0
 * @date 15-07-2023
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <CTextToEscSeq.h>

namespace Codegenerator {

/**
 * The CTextToEscSeq constructor initializes the member variables of the class.
 *
 * @param name The name of the CTextToEscSeq variable.
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
CTextToEscSeq::CTextToEscSeq(string name, string text, int signperline,
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
  LOG(DEBUG) << "CTextToEscSeq: " << this->name << " " << this->nl << " "
             << this->addtextpos << " " << this->addtextsegment << " "
             << this->doxygen << "\n"
             << this->text;

  if (text == "") {
    cerr << "Error: The input file is not formatted correctly" << endl;
    exit(1);
  }
}

CTextToEscSeq::~CTextToEscSeq() {}

/**
 * The function `writeImplementation` converts special characters in a given
 * text into their corresponding escape sequences and returns the implementation
 * code for the converted text.
 *
 * @return a string that represents the implementation of the variable.
 */
string CTextToEscSeq::writeImplementation() {
  std::string imp;

  for (char c : text) {
    switch (c) {
      case '\a':
        imp += "\\a";
        break;  // Bell
      case '\b':
        imp += "\\b";
        break;  // Backspace
      case '\e':
        imp += "\\e";
        break;  // Escape character
      case '\f':
        imp += "\\f";
        break;  // Formfeed Page Break
      case '\n':
        if (nl == "DOS") {
          imp += "\\r\\n";  // DOS (CR LF)
        } else if (nl == "MAC") {
          imp += "\\r";  // MAC (CR)
        } else {
          imp += "\\n";  // UNIX (LF)
        }
        break;
      case '\r':
        imp += "\\r";
        break;  // Carriage Return
      case '\t':
        imp += "\\t";
        break;  // Horizontal Tab
      case '\v':
        imp += "\\v";
        break;  // Vertical Tab
      case '\\':
        imp += "\\\\";
        break;  // Backslash
      case '\'':
        imp += "\\\'";
        break;  // Apostrophe
      case '\"':
        imp += "\\\"";
        break;  // Double quotation mark
      case '\?':
        imp += "\\?";
        break;  // Question mark
      default:
        // Überprüfen, ob das Zeichen nicht druckbar ist (ASCII-Werte < 32 oder
        // >= 127)
        if (c < 32 || c >= 127) {
          // Escape-Sequenz für octal number erzeugen
          imp += "\\";
          imp += std::to_string(static_cast<unsigned int>(c));
        } else {
          imp += c;
        }
        break;
    }
  }

  imp = VAR_IMPLEMENTATION + name + " = {\n" + wordWrap(imp, ' ', signperline) +
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
 * takes a string `text`, ignores the character `cut`, and an integer
 * `maxLineLength` as input, and returns a string `wrappedText` that wraps the
 * input text into multiple lines with a maximum line length. It can start a new
 * line at any character, except ther is a escape sequence.
 *
 * @param text The input text that needs to be wrapped into multiple lines.
 * @param cut The "cut" parameter is ignored in this function.
 * @param maxLineLength The `maxLineLength` parameter specifies the maximum
 * length of each line in the wrapped text.
 *
 * @return a string that represents the wrapped text.
 */
string CTextToEscSeq::wordWrap(string text, const char cut, int maxLineLength) {
  string wrappedText;
  int lineLength = 0;
  bool inEscapeSequence = false;

  for (int i = 0; i < text.length(); i++) {
    if (text[i] == '\\' && !inEscapeSequence) {
      inEscapeSequence = true;
    } else {
      inEscapeSequence = false;
    }

    if (lineLength >= maxLineLength && !inEscapeSequence ||
        lineLength + 1 >= maxLineLength && inEscapeSequence) {
      wrappedText += "\"" + text.substr(0, i) + "\" \\\n";
      text = text.substr(i);
      lineLength = 0;
      i = -1;  // Resetting the loop index to start from the beginning
    }

    lineLength++;
  }

  wrappedText += "\"" + text + "\"";

  return wrappedText;
}

}  // namespace Codegenerator