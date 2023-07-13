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

string CTextToHexSeq::writeImplementation() {
  string imp;
  stringstream ss;

  for (char c : text) {
    ss << hex << (int)c;
    imp += "\\x";
    imp += ss.str();
    ss.str("");
  }
  if (next != nullptr) {
    return imp + "\n" + next->writeImplementation();
  }
  return imp;
}

}  // namespace Codegenerator