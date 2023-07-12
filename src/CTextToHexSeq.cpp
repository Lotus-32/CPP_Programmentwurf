#include <CTextToHexSeq.h>

namespace Codegenerator {

CTextToHexSeq::CTextToHexSeq(string name, string text, string nl,
                             bool addtextpos, bool addtextsegment,
                             string doxygen)
    : CTextToCPP(name, text, nl, addtextpos, addtextsegment, doxygen) {
  LOG(DEBUG) << "CTextToHexSeq: " << name << " " << nl << " " << addtextpos
             << " " << addtextsegment << " " << doxygen << "\n"
             << text;
}

CTextToHexSeq::~CTextToHexSeq() {}

string CTextToHexSeq::writeDeclaration() {
  if (next != nullptr) {
    return text + "\n" + next->writeDeclaration();
  }
  return text;
}

string CTextToHexSeq::writeImplementation() { return ""; }

}  // namespace Codegenerator