#ifndef CTEXTTOHEXSEQ_H
#define CTEXTTOHEXSEQ_H

#include <CTextToCPP.h>

class CTextToHexSeq : public CTextToCPP {
 public:
  CTextToHexSeq();
  ~CTextToHexSeq();

  string writeDeclaration();
  string writeImplementation();
};

#endif