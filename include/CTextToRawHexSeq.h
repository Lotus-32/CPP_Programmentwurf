#ifndef CTEXTTORAWHEXSEQ_H
#define CTEXTTORAWHEXSEQ_H

#include <CTextToCPP.h>

namespace Codegenerator {

class CTextToRawHexSeq : public CTextToCPP {
 public:
  CTextToRawHexSeq(string name, string nl, bool addtextpos, bool addtextsegment,
                   string doxygen, string text);
  ~CTextToRawHexSeq();

  string writeDeclaration() override;
  string writeImplementation() override;
};

}  // namespace Codegenerator

#endif