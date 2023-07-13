#include <CTextToEscSeq.h>

namespace Codegenerator {

CTextToEscSeq::CTextToEscSeq(string name, string text, string nl,
                             int addtextpos, bool addtextsegment,
                             string doxygen) {
  this->name = name;
  this->text = text;
  this->nl = nl;
  this->addtextpos = addtextpos;
  this->addtextsegment = addtextsegment;
  this->doxygen = doxygen;
  next = nullptr;
  LOG(DEBUG) << "CTextToEscSeq: " << this->name << " " << this->nl << " "
             << this->addtextpos << " " << this->addtextsegment << " "
             << this->doxygen << "\n"
             << this->text;
}

CTextToEscSeq::~CTextToEscSeq() {}

string CTextToEscSeq::writeImplementation() { return ""; }

}  // namespace Codegenerator