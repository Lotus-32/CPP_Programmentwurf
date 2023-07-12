#ifndef CTEXTTOCPP_H
#define CTEXTTOCPP_H

#include <easylogging++.h>

#include <string>

using namespace std;

namespace Codegenerator {

/**
 * @brief Base class for converting text to C or C++ code
 *
 */
class CTextToCPP {
 protected:
  CTextToCPP* next;
  string name;
  string nl;
  bool addtextpos;
  bool addtextsegment;
  string doxygen;
  string text;

 public:
  CTextToCPP();
  virtual ~CTextToCPP();
  bool hasNext();

  virtual string writeDeclaration();
  virtual string writeImplementation();
  void addElement(CTextToCPP* element);
  void sort();
  void clear();
};

}  // namespace Codegenerator

#endif