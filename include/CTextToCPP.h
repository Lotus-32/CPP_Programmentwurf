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
  const string VAR_DECLARATION = "extern const char * const ";
  const string VAR_IMPLEMENTATION = "const char * const ";

  CTextToCPP* next;
  string name;
  string nl;
  int addtextpos;
  bool addtextsegment;
  string doxygen;
  string text;

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