#include <CTextToEscSeq.h>

namespace Codegenerator {

CTextToEscSeq::CTextToEscSeq(string name, string nl, bool addtextpos,
                             bool addtextsegment, string doxygen, string text)
    : CTextToCPP(name, nl, addtextpos, addtextsegment, doxygen, text) {
  LOG(DEBUG) << "CTextToEscSeq: " << name << " " << nl << " " << addtextpos
             << " " << addtextsegment << " " << doxygen << " " << text;
}

CTextToEscSeq::~CTextToEscSeq() {}

string CTextToEscSeq::writeDeclaration() { return ""; }

string CTextToEscSeq::writeImplementation() { return ""; }

}  // namespace Codegenerator