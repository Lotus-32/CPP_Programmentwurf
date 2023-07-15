/**
 * @file Options.cpp
 * @author Timo Bauermeister
 * @brief Responsible for parsing and storing the command line options and file
 * options.
 * @version 1.0
 * @date 15-07-2023
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <Options.h>

namespace Codegenerator {

Options::Options()
    : outputFilename(""),
      outputType("CPP"),
      headerDir("./"),
      sourceDir("./"),
      namespaceStr(""),
      signPerLine(60),
      sortByVarName(false) {}

Options::~Options() {}

/**
 * The function `parseGlobalOptions` parses command line options and arguments,
 * storing the values in member variables.
 *
 * @param argc The parameter `argc` is the number of command-line arguments
 * passed to the program. It includes the name of the program itself as the
 * first argument.
 * @param argv The `argv` parameter is an array of C-style strings (char**) that
 * represents the command-line arguments passed to the program. Each element of
 * the array is a null-terminated string, where the first element (`argv[0]`) is
 * the name of the program itself, and the subsequent elements
 */
void Options::parseGlobaleOptions(int argc, char** argv) {
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
        isSetOutputFilename = true;
        outputFilename = optarg;
        LOG(INFO) << "Outputfilename: " << outputFilename;
        break;
      case 't':
        outputType = optarg;
        if (outputType != "C" && outputType != "CPP") {
          cerr << "Invalid output type: " << outputType << endl;
          exit(1);
          break;
        }
        isSetOutputType = true;
        LOG(INFO) << "Outputtype: " << outputType;
        break;
      case 'd':
        isSetHeaderDir = true;
        headerDir = optarg;
        LOG(INFO) << "Headerdir: " << headerDir;
        break;
      case 's':
        isSetSourceDir = true;
        sourceDir = optarg;
        LOG(INFO) << "Sourcedir: " << sourceDir;
        break;
      case 'n':
        isSetNamespace = true;
        namespaceStr = optarg;
        LOG(INFO) << "Namespace: " << namespaceStr;
        break;
      case 'l':
        isSetSignPerLine = true;
        signPerLine = stoi(optarg);  // string to int
        LOG(INFO) << "Signperline: " << signPerLine;
        break;
      case 'v':
        isSetSortByVarName = true;
        sortByVarName = true;
        LOG(INFO) << "Sortbyvarname: true";
        break;
      case '?':
        cerr << "Unknown option! "
             << "\nHere are correct ones: " << endl;
        printHelp();
    }
  }

  if (optind == argc) {
    cerr << "No files specified. Options: " << endl;
    printHelp();
  }

  // Get the fileNames
  for (int i = optind; i < argc; i++) {
    fileNames.push_back(argv[i]);
  }
}

/**
 * The function `parseLocalOptions` parses a JSON string containing local
 * options and sets the corresponding variables if they are not already set.
 *
 * @param locales The `locales` parameter is a string that contains
 * JSON-formatted data representing various options for the program. These
 * options include the output filename, output type, header directory, source
 * directory, namespace, sign per line, and sort by variable name.
 * @param inputFileName The `inputFileName` parameter is a string that
 * represents the name of the input file.
 */
void Options::parseLocalOptions(const string& locales,
                                const string& inputFileName) {
  Json::Value json;
  Json::CharReaderBuilder readerBuilder;
  unique_ptr<Json::CharReader> reader(readerBuilder.newCharReader());
  string errors;

  LOG(DEBUG) << "Parse: " << locales;

  if (reader->parse(locales.c_str(), locales.c_str() + locales.length(), &json,
                    &errors)) {
    if (!isSetOutputFilename) {
      outputFilename = json.get("outputfilename", inputFileName).asString();
      LOG(INFO) << "Outputfilename: " << inputFileName;
    }
    if (!isSetOutputType) {
      outputType = json.get("outputtype", outputType).asString();
      LOG(INFO) << "Outputtype: " << outputType;
    }
    if (!isSetHeaderDir) {
      headerDir = json.get("headerdir", headerDir).asString();
      LOG(INFO) << "Headerdir: " << headerDir;
    }
    if (!isSetSourceDir) {
      sourceDir = json.get("sourcedir", sourceDir).asString();
      LOG(INFO) << "Sourcedir: " << sourceDir;
    }
    if (!isSetNamespace) {
      namespaceStr = json.get("namespace", namespaceStr).asString();
      LOG(INFO) << "Namespace: " << namespaceStr;
    }
    if (!isSetSignPerLine) {
      signPerLine = json.get("signperline", signPerLine).asInt();
      LOG(INFO) << "Signperline: " << signPerLine;
    }
    if (!isSetSortByVarName) {
      sortByVarName = json.get("sortbyvarname", sortByVarName).asBool();
      LOG(INFO) << "Sortbyvarname: " << sortByVarName;
    }
  } else {
    cerr << "No correct formated input file!" << endl;
    return;
  }
}

/**
 * The function "getOutputFilename" returns the output filename as a string.
 *
 * @return The outputFilename is being returned.
 */
string Options::getOutputFilename() const { return outputFilename; }

/**
 * The function "getOutputType" returns the output type as a string.
 *
 * @return the value of the variable "outputType" as a string.
 */
string Options::getOutputType() const { return outputType; }

/**
 * The function "getHeaderDir" returns the value of the "headerDir" variable.
 *
 * @return the value of the member variable "headerDir", which is a string.
 */
string Options::getHeaderDir() const { return headerDir; }

/**
 * The function getSourceDir() returns the source directory.
 *
 * @return the value of the variable "sourceDir".
 */
string Options::getSourceDir() const { return sourceDir; }

/**
 * The function "getNamespace" returns the value of the namespaceStr variable.
 *
 * @return the value of the variable `namespaceStr`, which is a string.
 */
string Options::getNamespace() const { return namespaceStr; }

/**
 * The function returns the value of the signPerLine variable.
 *
 * @return The value of the member variable "signPerLine" is being returned.
 */
int Options::getSignPerLine() const { return signPerLine; }

/**
 * The function returns the value of the sortByVarName variable.
 *
 * @return the value of the member variable `sortByVarName`.
 */
bool Options::getSortByVarName() const { return sortByVarName; }

/**
 * The function returns a boolean value indicating whether the output filename
 * has been set.
 *
 * @return the value of the variable "isSetOutputFilename".
 */
bool Options::getIsSetOutputFilename() const { return isSetOutputFilename; }

/**
 * The function sets the output filename if it has not been previously set.
 *
 * @param outputFilename The output filename is a string that represents the
 * name of the file where the output will be written to.
 */
void Options::setOutputFilename(string outputFilename) {
  if (!isSetOutputFilename) {
    this->outputFilename = outputFilename;
  }
}

/**
 * The function sets the output type if it has not been previously set.
 *
 * @param outputType The outputType parameter is a string that represents the
 * type of output that needs to be set.
 */
void Options::setOutputType(string outputType) {
  if (!isSetOutputType) {
    this->outputType = outputType;
  }
}

/**
 * The function "setHeaderDir" sets the header directory if it has not been
 * previously set.
 *
 * @param headerDir The parameter "headerDir" is a string that represents the
 * directory where header files are located.
 */
void Options::setHeaderDir(string headerDir) {
  if (!isSetHeaderDir) {
    this->headerDir = headerDir;
  }
}

/**
 * The function "setSourceDir" sets the source directory if it has not been
 * previously set.
 *
 * @param sourceDir The source directory that you want to set.
 */
void Options::setSourceDir(string sourceDir) {
  if (!isSetSourceDir) {
    this->sourceDir = sourceDir;
  }
}

/**
 * The function "setNamespace" sets the namespace string if it has not been
 * previously set.
 *
 * @param namespaceStr The parameter `namespaceStr` is a string that represents
 * the namespace value that we want to set.
 */
void Options::setNamespace(string namespaceStr) {
  if (!isSetNamespace) {
    this->namespaceStr = namespaceStr;
  }
}

/**
 * The function sets the number of signs per line if it has not already been
 * set.
 *
 * @param signPerLine The parameter "signPerLine" is an integer that represents
 * the number of signs per line.
 */
void Options::setSignPerLine(int signPerLine) {
  if (!isSetSignPerLine) {
    this->signPerLine = signPerLine;
  }
}

/**
 * The function sets the value of the sortByVarName variable if it has not
 * already been set.
 *
 * @param sortByVarName A boolean value indicating whether or not to sort by
 * variable name.
 */
void Options::setSortByVarName(bool sortByVarName) {
  if (!isSetSortByVarName) {
    this->sortByVarName = sortByVarName;
  }
}

/**
 * The function getFileNames() returns a vector of strings containing file
 * names.
 *
 * @return A vector of strings containing file names.
 */
vector<string> Options::getFileNames() const { return fileNames; }

void Options::printHelp() {
  cout << HELP_MESSAGE << endl;
  exit(EXIT_SUCCESS);
}

const char* const Options::short_options = "ho:t:d:s:n:l:v";

}  // namespace Codegenerator