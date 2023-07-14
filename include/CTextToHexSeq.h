#ifndef CTEXTTOHEXSEQ_H
#define CTEXTTOHEXSEQ_H

#include <CTextToCPP.h>

namespace Codegenerator {

/**
 * @brief Converts the variable text to a C-String with hex sequences
 *
 */
class CTextToHexSeq : public CTextToCPP {
 public:
  CTextToHexSeq(string name, string text, int signperline, string nl = "UNIX",
                int addtextpos = 0, bool addtextsegment = false,
                string doxygen = "");
  ~CTextToHexSeq();

  string writeImplementation() override;
};

}  // namespace Codegenerator

#endif