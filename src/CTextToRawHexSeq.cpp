#include <CTextToRawHexSeq.h>

namespace Codegenerator {

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
}

CTextToRawHexSeq::~CTextToRawHexSeq() {}

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

string CTextToRawHexSeq::writeImplementation() {
  string imp;
  stringstream ss;

  for (char c : text) {
    if (c == '\n') {
      if (nl == "DOS" || nl == "MAC") {
        ss << "0x" << hex << (int)'\r' << ", ";
        imp += ss.str();
        ss.str("");
        if (nl == "MAC") {
          continue;
        }
      }
    }
    ss << "0x" << hex << (int)c << ", ";
    imp += ss.str();
    ss.str("");
  }
  // Komma und Leerzeichen am Ende entfernen
  if (!imp.empty()) {
    imp.pop_back();
    imp.pop_back();
  }

  imp = "const char " + name + "[] = {\n" +
        wordWrap(imp, ' ', true, signperline) + "\n};\n";

  if (addtextsegment) {
    imp += getOriginalTextComment();
  }

  if (next != nullptr) {
    return imp + "\n" + next->writeImplementation();
  }
  return imp;
}

}  // namespace Codegenerator