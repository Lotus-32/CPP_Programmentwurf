#include <Options.h>
#include <easylogging++.h>
#include <jsoncpp/json/json.h>

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
          LOG(ERROR) << "Ungültiger Outputtype: " << outputType;
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
        LOG(ERROR) << "Unbekannte Option: " << optopt;
    }
  }

  if (optind == argc) {
    LOG(ERROR) << "Keine Dateien angegeben.";
    printHelp();
  }

  // Get the fileNames
  for (int i = optind; i < argc; i++) {
    fileNames.push_back(argv[i]);
  }
}

void Options::parseLocalOptions(string& locales, string& inputFileName) {
  Json::Value json;
  Json::CharReaderBuilder readerBuilder;
  unique_ptr<Json::CharReader> reader(readerBuilder.newCharReader());
  string errors;

  LOG(DEBUG) << "Parse: " << locales;

  if (reader->parse(locales.c_str(), locales.c_str() + locales.length(), &json,
                    &errors)) {
    if (!isSetOutputFilename) {
      outputFilename = json.get("outputfilename", inputFileName).asString();
      isSetOutputFilename = true;
      LOG(INFO) << "Outputfilename: " << inputFileName;
    }
    if (!isSetOutputType) {
      outputType = json.get("outputtype", outputType).asString();
      isSetOutputType = true;
      LOG(INFO) << "Outputtype: " << outputType;
    }
    if (!isSetHeaderDir) {
      headerDir = json.get("headerdir", headerDir).asString();
      isSetHeaderDir = true;
      LOG(INFO) << "Headerdir: " << headerDir;
    }
    if (!isSetSourceDir) {
      sourceDir = json.get("sourcedir", sourceDir).asString();
      isSetSourceDir = true;
      LOG(INFO) << "Sourcedir: " << sourceDir;
    }
    if (!isSetNamespace) {
      namespaceStr = json.get("namespace", namespaceStr).asString();
      isSetNamespace = true;
      LOG(INFO) << "Namespace: " << namespaceStr;
    }
    if (!isSetSignPerLine) {
      signPerLine = json.get("signperline", signPerLine).asInt();
      isSetSignPerLine = true;
      LOG(INFO) << "Signperline: " << signPerLine;
    }
    if (!isSetSortByVarName) {
      sortByVarName = json.get("sortbyvarname", sortByVarName).asBool();
      isSetSortByVarName = true;
      LOG(INFO) << "Sortbyvarname: " << sortByVarName;
    }
  } else {
    LOG(ERROR) << "Fehler beim Parsen der lokalen Parameter: " << errors
               << endl;
  }
}

string Options::getOutputFilename() const { return outputFilename; }

string Options::getOutputType() const { return outputType; }

string Options::getHeaderDir() const { return headerDir; }

string Options::getSourceDir() const { return sourceDir; }

string Options::getNamespace() const { return namespaceStr; }

int Options::getSignPerLine() const { return signPerLine; }

bool Options::getSortByVarName() const { return sortByVarName; }

void Options::setOutputFilename(string outputFilename) {
  if (!isSetOutputFilename) {
    this->outputFilename = outputFilename;
  }
}

void Options::setOutputType(string outputType) {
  if (!isSetOutputType) {
    this->outputType = outputType;
  }
}

void Options::setHeaderDir(string headerDir) {
  if (!isSetHeaderDir) {
    this->headerDir = headerDir;
  }
}

void Options::setSourceDir(string sourceDir) {
  if (!isSetSourceDir) {
    this->sourceDir = sourceDir;
  }
}

void Options::setNamespace(string namespaceStr) {
  if (!isSetNamespace) {
    this->namespaceStr = namespaceStr;
  }
}

void Options::setSignPerLine(int signPerLine) {
  if (!isSetSignPerLine) {
    this->signPerLine = signPerLine;
  }
}

void Options::setSortByVarName(bool sortByVarName) {
  if (!isSetSortByVarName) {
    this->sortByVarName = sortByVarName;
  }
}

vector<string> Options::getFileNames() const { return fileNames; }

void Options::printHelp() {
  cout << HELP_MESSAGE << endl;
  exit(EXIT_SUCCESS);
}

const char* const Options::short_options = "ho:t:d:s:n:l:v";

}  // namespace Codegenerator