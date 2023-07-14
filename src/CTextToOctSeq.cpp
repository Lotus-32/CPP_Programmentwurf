#include <CTextToOctSeq.h>

namespace Codegenerator {

CTextToOctSeq::CTextToOctSeq(string name, string text, string nl,
                             int addtextpos, bool addtextsegment,
                             string doxygen) {
  this->name = name;
  this->text = text;
  this->nl = nl;
  this->addtextpos = addtextpos;
  this->addtextsegment = addtextsegment;
  this->doxygen = doxygen;
  next = nullptr;
  LOG(DEBUG) << "CTextToOctSeq: " << name << " " << nl << " " << addtextpos
             << " " << addtextsegment << " " << doxygen << "\n"
             << text;
}

CTextToOctSeq::~CTextToOctSeq() {}

string CTextToOctSeq::writeImplementation() {
  string imp;
  stringstream ss;

  for (char c : text) {
    if (c == '\n') {
      if (nl == "DOS" || nl == "MAC") {
        ss << oct << (int)'\r';
        imp += "\\";
        imp += ss.str();
        ss.str("");
        if (nl == "MAC") {
          continue;
        }
      }
    }
    ss << oct << (int)c;
    imp += "\\";
    imp += ss.str();
    ss.str("");
  }

  imp = VAR_IMPLEMENTATION + name + " = {\n" + wordWrap(imp, '\\', false) +
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