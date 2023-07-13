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

string CTextToOctSeq::writeImplementation() { return ""; }

}  // namespace Codegenerator