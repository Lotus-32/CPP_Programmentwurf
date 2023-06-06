#ifndef OPTIONS_H
#define OPTIONS_H

#include <getopt.h>

#include <string>
#include <vector>

using namespace std;

namespace Codegenerator {
class Options {
 public:
  Options();
  ~Options();

  void parseOptions(int argc, char** argv);

  string getOutputFilename() const;
  string getOutputType() const;
  string getHeaderDir() const;
  string getSourceDir() const;
  string getNamespace() const;
  int getSignPerLine() const;
  bool getSortByVarName() const;

  void setOutputFilename(string outputFilename);
  void setOutputType(string outputType);
  void setHeaderDir(string headerDir);
  void setSourceDir(string sourceDir);
  void setNamespace(string namespaceStr);
  void setSignPerLine(int signPerLine);
  void setSortByVarName(bool sortByVarName);

  vector<string> getFileNames() const;

 private:
  bool isSetOutputFilename = false;
  string outputFilename;
  bool isSetOutputType = false;
  string outputType;
  bool isSetHeaderDir = false;
  string headerDir;
  bool isSetSourceDir = false;
  string sourceDir;
  bool isSetNamespace = false;
  string namespaceStr;
  bool isSetSignPerLine = false;
  int signPerLine;
  bool isSetSortByVarName = false;
  bool sortByVarName;

  vector<string> fileNames;

  void printHelp();

  static const char* const short_options;

  const char* const HELP_MESSAGE = R"(
    Input: codegenerator [Options] [Files]

    Options:
      -o, --outputfilename FILENAME   Sets the name of the output fileNames.
      -t, --outputtype TYPE           Sets the file type of the generated code (valid options: C, CPP).
      -d, --headerdir DIR             Sets the directory for the header file.
      -s, --sourcedir DIR             Sets the directory for the source file.
      -n, --namespace NAMESPACE       Sets the namespace.
      -l, --signperline X             Sets the number of characters per line before a line break happens. 
                                      For generated variables.
      -v, --sortbyvarname             Sorts the variables by their name.
      -h, --help                      Displays this help text and exits.

    Examples:
      codegenerator -o test -t C -d ./include -s ./src -n test -l 80 -v file1.txt file2.txt
      codegenerator --outputfilename test --outputtype C --namespace test --signperline 80 --sortbyvarname file1.txt

    Author Team:
      Timo Bauermeister
      Jannik Kiebler-Schauer
      Marc Sachße
      Tobias Skoberla

    Contact:
      tobias.skoberla@gmail.com
    )";
};
}  // namespace Codegenerator
#endif  // OPTIONS_H