#ifndef CTEXTTOCPP_H
#define CTEXTTOCPP_H

#include <string>

using namespace std;

/**
 * @brief Base class for converting text to C or C++ code
 *
 */
class CTextToCPP {
 protected:
  CTextToCPP* next;
  string text;

 public:
  CTextToCPP(string test);
  ~CTextToCPP();

  string writeDeclaration();
  string writeImplementation();
  void addElement(CTextToCPP& element);
  void sort();
  void clear();
};

#endif