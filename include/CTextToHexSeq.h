#ifndef CTEXTTOHEXSEQ_H
#define CTEXTTOHEXSEQ_H

#include <CTextToCPP.h>

namespace Codegenerator {

class CTextToHexSeq : public CTextToCPP {
 public:
  CTextToHexSeq(string name, string nl, bool addtextpos, bool addtextsegment,
                string doxygen, string text);
  ~CTextToHexSeq();

  string writeDeclaration() override;
  string writeImplementation() override;
};

}  // namespace Codegenerator

#endif