#ifndef CTEXTTORAWHEXSEQ_H
#define CTEXTTORAWHEXSEQ_H

#include <CTextToCPP.h>

namespace Codegenerator {

class CTextToRawHexSeq : public CTextToCPP {
 public:
  CTextToRawHexSeq(string name, string text, string nl = "UNIX",
                   int addtextpos = 0, bool addtextsegment = false,
                   string doxygen = "");
  ~CTextToRawHexSeq();

  string writeImplementation() override;
};

}  // namespace Codegenerator

#endif