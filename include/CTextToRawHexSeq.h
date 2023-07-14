#ifndef CTEXTTORAWHEXSEQ_H
#define CTEXTTORAWHEXSEQ_H

#include <CTextToCPP.h>

namespace Codegenerator {

/**
 * @brief Converts the variable text to a raw hex sequence
 *
 */
class CTextToRawHexSeq : public CTextToCPP {
 public:
  CTextToRawHexSeq(string name, string text, int signperline,
                   string nl = "UNIX", int addtextpos = 0,
                   bool addtextsegment = false, string doxygen = "");
  ~CTextToRawHexSeq();

  string writeDeclaration() override;
  string writeImplementation() override;
};

}  // namespace Codegenerator

#endif