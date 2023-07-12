#ifndef CTEXTTOESCSEQ_H
#define CTEXTTOESCSEQ_H

#include <CTextToCPP.h>

namespace Codegenerator {

class CTextToEscSeq : public CTextToCPP {
 public:
  CTextToEscSeq(string name, string text, string nl = "UNIX",
                bool addtextpos = false, bool addtextsegment = false,
                string doxygen = "");
  ~CTextToEscSeq();

  string writeDeclaration() override;
  string writeImplementation() override;
};

}  // namespace Codegenerator

#endif