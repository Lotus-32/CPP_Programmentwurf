#include <CTextToRawHexSeq.h>

namespace Codegenerator {

CTextToRawHexSeq::CTextToRawHexSeq(string name, string text, string nl,
                                   bool addtextpos, bool addtextsegment,
                                   string doxygen)
    : CTextToCPP(name, text, nl, addtextpos, addtextsegment, doxygen) {
  LOG(DEBUG) << "CTextToRawHexSeq: " << name << " " << nl << " " << addtextpos
             << " " << addtextsegment << " " << doxygen << "\n"
             << text;
}

CTextToRawHexSeq::~CTextToRawHexSeq() {}

string CTextToRawHexSeq::writeDeclaration() {
  if (next != nullptr) {
    return text + "\n" + next->writeDeclaration();
  }
  return text;
}

string CTextToRawHexSeq::writeImplementation() { return ""; }

}  // namespace Codegenerator