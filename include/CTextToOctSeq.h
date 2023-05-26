#ifndef _CTEXTTOOCTSEQ_H_
#define _CTEXTTOOCTSEQ_H_

#include <CTextToCPP.h>

class CTextToOctSeq : public CTextToCPP {
 public:
  CTextToOctSeq();
  ~CTextToOctSeq();

  string writeDeclaration();
  string writeImplementation();
};

#endif