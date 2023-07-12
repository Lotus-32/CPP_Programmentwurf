#include <CTextToOctSeq.h>

namespace Codegenerator {

CTextToOctSeq::CTextToOctSeq(string name, string text, string nl,
                             bool addtextpos, bool addtextsegment,
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

string CTextToOctSeq::writeDeclaration() {
  if (next != nullptr) {
    return text + "\n" + next->writeDeclaration();
  }
  return text;
}

string CTextToOctSeq::writeImplementation() { return ""; }

}  // namespace Codegenerator