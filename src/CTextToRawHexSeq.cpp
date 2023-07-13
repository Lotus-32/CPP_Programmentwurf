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

string CTextToRawHexSeq::writeImplementation() {
  string imp;
  stringstream ss;

  for (char c : text) {
    ss << "0x" << hex << (int)c << ", ";
    imp += ss.str();
    ss.str("");
  }
  // Komma und Leerzeichen am Ende entfernen
  if (!imp.empty()) {
    imp.pop_back();
    imp.pop_back();
  }

  if (next != nullptr) {
    return imp + "\n" + next->writeImplementation();
  }
  return imp;
}

}  // namespace Codegenerator