#ifndef OPTIONS_H
#define OPTIONS_H

#include <getopt.h>

#include <string>

using namespace std;

class Options {
 public:
  Options();
  ~Options();

  void parseOptions(int argc, char** argv);

  bool isSetOutputFilename() const;
  string getOutputFilename() const;
  bool isSetOutputType() const;
  string getOutputType() const;
  bool isSetHeaderDir() const;
  string getHeaderDir() const;
  bool isSetSourceDir() const;
  string getSourceDir() const;
  bool isSetNamespace() const;
  string getNamespace() const;
  bool isSetSignPerLine() const;
  int getSignPerLine() const;
  bool isSetSortByVarName() const;
  bool getSortByVarName() const;

 private:
  bool isOutputFilename = false;
  string outputFilename;
  bool isOutputType = false;
  string outputType;
  bool isHeaderDir = false;
  string headerDir;
  bool isSourceDir = false;
  string sourceDir;
  bool isNamespace = false;
  string namespaceStr;
  bool isSignPerLine = false;
  int signPerLine;
  bool isSortByVarName = false;
  bool sortByVarName;

  void printHelp();

  static const char* const short_options;

  const char* const HELP_MESSAGE = R"(
Eingabe: codegenerator [Optionen] [Dateien]

Optionen:
-o, --outputfilename FILENAME Setzt den Namen der Outputfiles
-t, --outputtype TYPE Setzt den Dateityp des generierten Codes (gültige Optionen: C, CPP)
-d, --headerdir DIR Setzt das Verzeichnis für die Header-Datei
-s, --sourcedir DIR Setzt das Verzeichnis für die Source-Datei
-n, --namespace NAMESPACE Setzt den Namespace
-l, --signperline X Setzt die Anzahl der Zeichen pro Zeile für Zeilenumbrüche in generierten Variablen
-v, --sortbyvarname Sortiert die Variablen nach ihrem Namen
-h, --help Zeigt diesen Hilfetext an

Autorenteam:
Timo Bauermeister
Jannik Kiebler-Schauer
Marc Sachße
Tobias Skoberla

Email:
tobias.skoberla@gmail.com
)";
};

#endif  // OPTIONS_H