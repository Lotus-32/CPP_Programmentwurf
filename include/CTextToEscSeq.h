#ifndef CTEXTTOESCSEQ_H
#define CTEXTTOESCSEQ_H

#include <CTextToCPP.h>

namespace Codegenerator {

class CTextToEscSeq : public CTextToCPP {
 public:
  CTextToEscSeq(string name, string nl, bool addtextpos, bool addtextsegment,
                string doxygen, string text);
  ~CTextToEscSeq();

  string writeDeclaration() override;
  string writeImplementation() override;
};

}  // namespace Codegenerator

#endif