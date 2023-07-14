#include <CTextToEscSeq.h>

namespace Codegenerator {

CTextToEscSeq::CTextToEscSeq(string name, string text, string nl,
                             int addtextpos, bool addtextsegment,
                             string doxygen) {
  this->name = name;
  this->text = text;
  this->nl = nl;
  this->addtextpos = addtextpos;
  this->addtextsegment = addtextsegment;
  this->doxygen = doxygen;
  next = nullptr;
  LOG(DEBUG) << "CTextToEscSeq: " << this->name << " " << this->nl << " "
             << this->addtextpos << " " << this->addtextsegment << " "
             << this->doxygen << "\n"
             << this->text;
}

CTextToEscSeq::~CTextToEscSeq() {}

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

  imp = VAR_IMPLEMENTATION + name + " = {\n" + wordWrap(imp, ' ', false) +
        "\n};\n";

  if (addtextsegment) {
    imp += getOriginalTextComment();
  }

  if (next != nullptr) {
    return imp + "\n" + next->writeImplementation();
  }
  return imp;
}

}  // namespace Codegenerator