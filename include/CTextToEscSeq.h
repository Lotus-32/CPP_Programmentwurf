/**
 * @file CTextToEscSeq.h
 * @author Marc Sachße
 * @brief Converts the variable text to a C-String with escape sequences
 * @version 1.0
 * @date 15-07-2023
 *
 * @copyright Copyright (c) 2023
 *
 */

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

 private:
  string wordWrap(string text, const char cut, int maxLineLength) override;
};

}  // namespace Codegenerator

#endif