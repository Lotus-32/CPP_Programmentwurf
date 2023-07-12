#ifndef VARIABLE_H
#define VARIABLE_H

#include <string>

using namespace std;

namespace Codegenerator {

class Variable {
 public:
  Variable(string name, string seq, string nl, bool addtextpos,
           bool addtextsegment, string doxygen, string text);
  ~Variable();

  string getName() const;
  string getSeq() const;
  string getNl() const;
  bool getAddtextpos() const;
  bool getAddtextsegment() const;
  string getDoxygen() const;
  string getText() const;

 private:
  string name;
  string seq;
  string nl;
  bool addtextpos;
  bool addtextsegment;
  string doxygen;
  string text;
};

}  // namespace Codegenerator

#endif  // VARIABLE_H