#include <Options.h>
#include <easylogging++.h>

namespace Codegenerator {

Options::Options() {}

Options::~Options() {}

void Options::parseOptions(int argc, char** argv) {
  const struct option long_options[] = {
      {"outputfilename", required_argument, 0, 'o'},
      {"outputtype", required_argument, 0, 't'},
      {"headerdir", required_argument, 0, 'd'},
      {"sourcedir", required_argument, 0, 's'},
      {"namespace", required_argument, 0, 'n'},
      {"signperline", required_argument, 0, 'l'},
      {"sortbyvarname", no_argument, 0, 'v'},
      {"help", no_argument, 0, 'h'},
      {0, 0, 0, 0}};

  int option;
  while ((option = getopt_long(argc, argv, short_options, long_options,
                               nullptr)) != -1) {
    switch (option) {
      case 'h':
        printHelp();
        break;
      case 'o':
        isOutputFilename = true;
        outputFilename = optarg;
        LOG(INFO) << "Outputfilename: " << outputFilename;
        break;
      case 't':
        outputType = optarg;
        if (outputType != "C" && outputType != "CPP") {
          LOG(ERROR) << "Ungültiger Outputtype: " << outputType;
          break;
        }
        isOutputType = true;
        LOG(INFO) << "Outputtype: " << outputType;
        break;
      case 'd':
        isHeaderDir = true;
        headerDir = optarg;
        LOG(INFO) << "Headerdir: " << headerDir;
        break;
      case 's':
        isSourceDir = true;
        sourceDir = optarg;
        LOG(INFO) << "Sourcedir: " << sourceDir;
        break;
      case 'n':
        isNamespace = true;
        namespaceStr = optarg;
        LOG(INFO) << "Namespace: " << namespaceStr;
        break;
      case 'l':
        isSignPerLine = true;
        signPerLine = stoi(optarg);  // string to int
        LOG(INFO) << "Signperline: " << signPerLine;
        break;
      case 'v':
        isSortByVarName = true;
        sortByVarName = true;
        LOG(INFO) << "Sortbyvarname: true";
        break;
      case '?':
        LOG(ERROR) << "Unbekannte Option: " << optopt;
    }
  }
  // TODO: File handling
  for (int i = optind; i < argc; i++) {
    cout << "Übergebene Datei: " << argv[i] << endl;
  }
}

bool Options::isSetOutputFilename() const { return isOutputFilename; }

string Options::getOutputFilename() const { return outputFilename; }

bool Options::isSetOutputType() const { return isOutputType; }

string Options::getOutputType() const { return outputType; }

bool Options::isSetHeaderDir() const { return isHeaderDir; }

string Options::getHeaderDir() const { return headerDir; }

bool Options::isSetSourceDir() const { return isSourceDir; }

string Options::getSourceDir() const { return sourceDir; }

bool Options::isSetNamespace() const { return isNamespace; }

string Options::getNamespace() const { return namespaceStr; }

bool Options::isSetSignPerLine() const { return isSignPerLine; }

int Options::getSignPerLine() const { return signPerLine; }

bool Options::isSetSortByVarName() const { return isSortByVarName; }

bool Options::getSortByVarName() const { return sortByVarName; }

void Options::printHelp() {
  cout << HELP_MESSAGE << endl;
  exit(EXIT_SUCCESS);
}

const char* const Options::short_options = "ho:t:d:s:n:l:v";

}  // namespace Codegenerator