#ifndef _CTEXTTOOCTSEQ_H_
#define _CTEXTTOOCTSEQ_H_

#include <CTextToCPP.h>

namespace Codegenerator {

class CTextToOctSeq : public CTextToCPP {
 public:
  CTextToOctSeq(string name, string nl, bool addtextpos, bool addtextsegment,
                string doxygen, string text);
  ~CTextToOctSeq();

  string writeDeclaration() override;
  string writeImplementation() override;
};

}  // namespace Codegenerator

#endif