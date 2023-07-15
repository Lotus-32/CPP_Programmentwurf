/**
 * @file Options.h
 * @author Timo Bauermeister
 * @brief Responsible for parsing and storing the command line options and file
 * options.
 * @version 1.0
 * @date 15-07-2023
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef OPTIONS_H
#define OPTIONS_H

#include <easylogging++.h>
#include <getopt.h>

#ifndef _WIN32
#include <jsoncpp/json/json.h>
#else
#include <json/json.h>
#endif

#include <vector>

using namespace std;

namespace Codegenerator {
/** class The `Options` class is responsible for parsing and storing the command
 * line options and file options. It provides methods to access the values
 * of these options. */
class Options {
 public:
  Options();
  ~Options();

  void parseGlobaleOptions(int argc, char** argv);
  void parseLocalOptions(const string& locales, const string& inputFileName);

  string getOutputFilename() const;
  string getOutputType() const;
  string getHeaderDir() const;
  string getSourceDir() const;
  string getNamespace() const;
  int getSignPerLine() const;
  bool getSortByVarName() const;

  bool getIsSetOutputFilename() const;

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
    Generates a header and source file from a given input file.

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
      Marc Sachße
      Tobias Skoberla
      (Jannik Kiebler-Schauer - exmatriculated)

    Contact:
      tobias.skoberla@gmail.com
    )";
};
}  // namespace Codegenerator
#endif  // OPTIONS_H