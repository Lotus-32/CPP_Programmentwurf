#ifndef CTEXTTOCPP_H
#define CTEXTTOCPP_H

#include <string>
#include <vector>

using namespace std;

/**
 * @brief Base class for converting text to C or C++ code
 *
 */
class CTextToCPP {
 protected:
  vector<string> elements;

 public:
  CTextToCPP();
  ~CTextToCPP();

  string writeDeclaration();
  string writeImplementation();
  void addElement(const string& element);
  void sort();
  void clear();
};

#endif