#include <CTextToRawHexSeq.h>

namespace Codegenerator {

CTextToRawHexSeq::CTextToRawHexSeq(string name, string text, string nl,
                                   int addtextpos, bool addtextsegment,
                                   string doxygen) {
  this->name = name;
  this->text = text;
  this->nl = nl;
  this->addtextpos = addtextpos;
  this->addtextsegment = addtextsegment;
  this->doxygen = doxygen;
  next = nullptr;
  LOG(DEBUG) << "CTextToRawHexSeq: " << name << " " << nl << " " << addtextpos
             << " " << addtextsegment << " " << doxygen << "\n"
             << text;
}

CTextToRawHexSeq::~CTextToRawHexSeq() {}

string CTextToRawHexSeq::writeImplementation() { return ""; }

}  // namespace Codegenerator