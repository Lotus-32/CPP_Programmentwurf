/**
 * @file CTextToCPP.h
 * @author Marc Sachße
 * @brief Base class for converting text to C or C++ code
 * @version 1.0
 * @date 15-07-2023
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef CTEXTTOCPP_H
#define CTEXTTOCPP_H

#include <easylogging++.h>

#include <iomanip>

using namespace std;

namespace Codegenerator {

/**
 * @brief Base class for converting text to C or C++ code
 *
 */
class CTextToCPP {
 protected:
  const string VAR_DECLARATION = "extern const char * const ";
  const string VAR_IMPLEMENTATION = "const char * const ";

  CTextToCPP* next;
  string name;
  string nl;
  int addtextpos;
  bool addtextsegment;
  string doxygen;
  string text;
  int signperline;
  string getOriginalTextComment();
  virtual string wordWrap(string text, const char cut, int maxLineLength = 60);

 public:
  CTextToCPP();
  virtual ~CTextToCPP();

  virtual string writeDeclaration();
  virtual string writeImplementation();
  void addElement(CTextToCPP* element);
  void sort();
  void clear();

  string getName() const;

 private:
  void insert(CTextToCPP*& sortedList, CTextToCPP* newElement);
};

}  // namespace Codegenerator

#endif