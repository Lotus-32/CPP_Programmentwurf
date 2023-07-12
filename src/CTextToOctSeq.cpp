#include <CTextToOctSeq.h>

namespace Codegenerator {

CTextToOctSeq::CTextToOctSeq(string name, string nl, bool addtextpos,
                             bool addtextsegment, string doxygen, string text)
    : CTextToCPP(name, nl, addtextpos, addtextsegment, doxygen, text) {
  LOG(DEBUG) << "CTextToOctSeq: " << name << " " << nl << " " << addtextpos
             << " " << addtextsegment << " " << doxygen << " " << text;
}

CTextToOctSeq::~CTextToOctSeq() {}

string CTextToOctSeq::writeDeclaration() { return ""; }

string CTextToOctSeq::writeImplementation() { return ""; }

}  // namespace Codegenerator