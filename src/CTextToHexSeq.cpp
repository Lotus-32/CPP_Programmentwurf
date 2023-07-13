#include <CTextToHexSeq.h>

namespace Codegenerator {

CTextToHexSeq::CTextToHexSeq(string name, string text, string nl,
                             int addtextpos, bool addtextsegment,
                             string doxygen) {
  this->name = name;
  this->text = text;
  this->nl = nl;
  this->addtextpos = addtextpos;
  this->addtextsegment = addtextsegment;
  this->doxygen = doxygen;
  next = nullptr;
  LOG(DEBUG) << "CTextToHexSeq: " << name << " " << nl << " " << addtextpos
             << " " << addtextsegment << " " << doxygen << "\n"
             << text;
}

CTextToHexSeq::~CTextToHexSeq() {}

string CTextToHexSeq::writeImplementation() { return ""; }

}  // namespace Codegenerator