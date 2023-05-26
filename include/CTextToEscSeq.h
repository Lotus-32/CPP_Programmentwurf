#ifndef CTEXTTOESCSEQ_H
#define CTEXTTOESCSEQ_H

#include <CTextToCPP.h>

class CTextToEscSeq : public CTextToCPP {
 public:
  CTextToEscSeq();
  ~CTextToEscSeq();

  string writeDeclaration();
  string writeImplementation();
};

#endif