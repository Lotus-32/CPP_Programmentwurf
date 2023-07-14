#ifndef _CTEXTTOOCTSEQ_H_
#define _CTEXTTOOCTSEQ_H_

#include <CTextToCPP.h>

namespace Codegenerator {

/**
 * @brief Converts the variable text to a C-String with octal sequences
 *
 */
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