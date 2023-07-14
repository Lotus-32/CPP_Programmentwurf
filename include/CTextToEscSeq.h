#ifndef CTEXTTOESCSEQ_H
#define CTEXTTOESCSEQ_H

#include <CTextToCPP.h>

namespace Codegenerator {

/**
 * @brief Converts the variable text to a C-String with escape sequences
 *
 */
class CTextToEscSeq : public CTextToCPP {
 public:
  CTextToEscSeq(string name, string text, int signperline, string nl = "UNIX",
                int addtextpos = 0, bool addtextsegment = false,
                string doxygen = "");
  ~CTextToEscSeq();

  string writeImplementation() override;
};

}  // namespace Codegenerator

#endif