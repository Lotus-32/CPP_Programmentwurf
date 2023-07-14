#ifndef _CTEXTTOOCTSEQ_H_
#define _CTEXTTOOCTSEQ_H_

#include <CTextToCPP.h>

namespace Codegenerator {

class CTextToOctSeq : public CTextToCPP {
 public:
  CTextToOctSeq(string name, string text, int signperline, string nl = "UNIX",
                int addtextpos = 0, bool addtextsegment = false,
                string doxygen = "");
  ~CTextToOctSeq();

  string writeImplementation() override;
};

}  // namespace Codegenerator

#endif