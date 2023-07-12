#include <CTextToHexSeq.h>

namespace Codegenerator {

CTextToHexSeq::CTextToHexSeq(string name, string nl, bool addtextpos,
                             bool addtextsegment, string doxygen, string text)
    : CTextToCPP(name, nl, addtextpos, addtextsegment, doxygen, text) {
  LOG(DEBUG) << "CTextToHexSeq: " << name << " " << nl << " " << addtextpos
             << " " << addtextsegment << " " << doxygen << " " << text;
}

CTextToHexSeq::~CTextToHexSeq() {}

string CTextToHexSeq::writeDeclaration() { return ""; }

string CTextToHexSeq::writeImplementation() { return ""; }

}  // namespace Codegenerator