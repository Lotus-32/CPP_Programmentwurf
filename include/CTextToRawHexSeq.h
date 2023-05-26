#ifndef CTEXTTORAWHEXSEQ_H
#define CTEXTTORAWHEXSEQ_H

#include <CTextToCPP.h>

class CTextToRawHexSeq : public CTextToCPP {
 public:
  CTextToRawHexSeq();
  ~CTextToRawHexSeq();

  string writeDeclaration();
  string writeImplementation();
};

#endif