#include <CTextToOctSeq.h>

#include <iomanip>

namespace Codegenerator {

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
}

CTextToOctSeq::~CTextToOctSeq() {}

string CTextToOctSeq::writeImplementation() {
  string imp;
  stringstream ss;

  for (char c : text) {
    if (c == '\n') {
      if (nl == "DOS" || nl == "MAC") {
        ss << oct << setw(3) << setfill('0') << (int)'\r';
        imp += "\\";
        imp += ss.str();
        ss.str("");
        if (nl == "MAC") {
          continue;
        }
      }
    }
    ss << oct << setw(3) << setfill('0') << (int)c;
    imp += "\\";
    imp += ss.str();
    ss.str("");
  }

  imp = VAR_IMPLEMENTATION + name + " = {\n" +
        wordWrap(imp, '\\', false, signperline) + "\n};\n";

  if (addtextsegment) {
    imp += getOriginalTextComment();
  }

  if (next != nullptr) {
    return imp + "\n" + next->writeImplementation();
  }
  return imp;
}

}  // namespace Codegenerator