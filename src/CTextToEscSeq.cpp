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

  imp = VAR_IMPLEMENTATION + name + " = {\n" +
        wordWrap(imp, ' ', false, signperline) + "\n};\n";

  if (addtextsegment) {
    imp += getOriginalTextComment();
  }

  if (next != nullptr) {
    return imp + "\n" + next->writeImplementation();
  }
  return imp;
}

}  // namespace Codegenerator