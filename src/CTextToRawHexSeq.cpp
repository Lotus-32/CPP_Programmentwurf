#include <CTextToRawHexSeq.h>

namespace Codegenerator {

CTextToRawHexSeq::CTextToRawHexSeq(string name, string nl, bool addtextpos,
                                   bool addtextsegment, string doxygen,
                                   string text)
    : CTextToCPP(name, nl, addtextpos, addtextsegment, doxygen, text) {
  LOG(DEBUG) << "CTextToRawHexSeq: " << name << " " << nl << " " << addtextpos
             << " " << addtextsegment << " " << doxygen << " " << text;
}

CTextToRawHexSeq::~CTextToRawHexSeq() {}

string CTextToRawHexSeq::writeDeclaration() { return ""; }

string CTextToRawHexSeq::writeImplementation() { return ""; }

}  // namespace Codegenerator