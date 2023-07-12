#include <CTextToEscSeq.h>

namespace Codegenerator {

CTextToEscSeq::CTextToEscSeq(string name, string text, string nl,
                             bool addtextpos, bool addtextsegment,
                             string doxygen)
    : CTextToCPP(name, text, nl, addtextpos, addtextsegment, doxygen) {
  LOG(DEBUG) << "CTextToEscSeq: " << name << " " << nl << " " << addtextpos
             << " " << addtextsegment << " " << doxygen << "\n"
             << text;
}

CTextToEscSeq::~CTextToEscSeq() {}

string CTextToEscSeq::writeDeclaration() {
  if (next != nullptr) {
    return text + "\n" + next->writeDeclaration();
  }
  return text;
}

string CTextToEscSeq::writeImplementation() { return ""; }

}  // namespace Codegenerator