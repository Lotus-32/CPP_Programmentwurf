#include <CTextToOctSeq.h>

namespace Codegenerator {

CTextToOctSeq::CTextToOctSeq(string name, string text, string nl,
                             bool addtextpos, bool addtextsegment,
                             string doxygen)
    : CTextToCPP(name, text, nl, addtextpos, addtextsegment, doxygen) {
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