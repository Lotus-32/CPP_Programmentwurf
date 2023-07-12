#ifndef CTEXTTOHEXSEQ_H
#define CTEXTTOHEXSEQ_H

#include <CTextToCPP.h>

namespace Codegenerator {

class CTextToHexSeq : public CTextToCPP {
 public:
  CTextToHexSeq(string name, string text, string nl = "UNIX",
                bool addtextpos = false, bool addtextsegment = false,
                string doxygen = "");
  ~CTextToHexSeq();

  string writeDeclaration() override;
  string writeImplementation() override;
};

}  // namespace Codegenerator

#endif