#include <Variable.h>
#include <easylogging++.h>

namespace Codegenerator {

Variable::Variable(string name, string seq, string nl, bool addtextpos,
                   bool addtextsegment, string doxygen, string text)
    : name(name),
      seq(seq),
      nl(nl),
      addtextpos(addtextpos),
      addtextsegment(addtextsegment),
      doxygen(doxygen),
      text(text) {
  LOG(DEBUG) << "Variable: " << name << " " << seq << " " << nl << " "
             << addtextpos << " " << addtextsegment << " " << doxygen << " "
             << text;
}

Variable::~Variable() {}

string Variable::getName() const { return name; }

string Variable::getSeq() const { return seq; }

string Variable::getNl() const { return nl; }

bool Variable::getAddtextpos() const { return addtextpos; }

bool Variable::getAddtextsegment() const { return addtextsegment; }

string Variable::getDoxygen() const { return doxygen; }

string Variable::getText() const { return text; }

}  // namespace Codegenerator