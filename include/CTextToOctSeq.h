#ifndef _CTEXTTOOCTSEQ_H_
#define _CTEXTTOOCTSEQ_H_

#include <CTextToCPP.h>

namespace Codegenerator {

class CTextToOctSeq : public CTextToCPP {
 public:
  CTextToOctSeq(string name, string text, string nl = "UNIX",
                bool addtextpos = false, bool addtextsegment = false,
                string doxygen = "");
  ~CTextToOctSeq();

  string writeDeclaration() override;
  string writeImplementation() override;
};

}  // namespace Codegenerator

#endif