#include <CCodegenerator.h>
#include <CTextToCPP.h>
#include <Options.h>
#include <easylogging++.h>
#include <stdio.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
using namespace Codegenerator;

INITIALIZE_EASYLOGGINGPP

/**
 * This function initializes logging.
 */
void initLogging() {
  // Load the configuration file and configure the loggers
  el::Configurations conf("assets/logging.ini");
  // Disable program termination on fatal error
  el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
  // Include detailed crash reason in the log file upon system crash
  el::Loggers::addFlag(el::LoggingFlag::LogDetailedCrashReason);
  // Enable colored terminal output if available
  el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
  // Apply the configurations to all running loggers
  el::Loggers::reconfigureLogger("default", conf);
  el::Loggers::reconfigureAllLoggers(conf);

  // LOG(INFO) << "Information about the process";
  // LOG(DEBUG) << "Debugging from program parts";
  // LOG(WARNING) << "The program can still continue";
  // LOG(ERROR) << "This is already a serious error";
  // LOG(FATAL) << "For example, there is not enough memory to continue";
}

/**
 * Creates a file with the given name and content in the specified path,
 * handling any necessary directory creation.
 *
 * @param fileName The name of the file to be created.
 * @param content The `content` that will be written to the file.
 * @param path The `path` where the file should be created.
 */
void createFile(const string& fileName, const string& content,
                const string& path) {
  string correctedPath = path;
  replace(correctedPath.begin(), correctedPath.end(), '\\', '/');
  LOG(DEBUG) << "Path: " << correctedPath << endl;

  if (!filesystem::exists(correctedPath)) {
    if (!filesystem::create_directories(correctedPath)) {
      LOG(ERROR) << "Error creating the directory: " << correctedPath << endl;
      cerr << "Error creating the directory: " << correctedPath << endl;
      exit(1);
    }
  }

  ofstream outputFile(correctedPath + "/" + fileName);
  if (!outputFile) {
    LOG(ERROR) << "Error creating the file: " << fileName << endl;
    cerr << "Error creating the file: " << fileName << endl;
    exit(1);
  }

  outputFile << content;

  LOG(INFO) << "File created: " << correctedPath << fileName << endl;
}

/**
 * Takes a string as input and returns a new string with all characters
 * converted to lowercase.
 *
 * @param input The input parameter is a constant reference to a string.
 *
 * @return a string that is the lowercase version of the input string.
 */
string toLowerCases(const string& input) {
  string output = input;
  transform(output.begin(), output.end(), output.begin(), ::tolower);
  return output;
}

/**
 * The main function reads input files, processes their contents, generates C++
 * code, and writes the generated code to output files.
 *
 * @param argc The parameter `argc` is an integer that represents the number of
 * command-line arguments passed to the program.
 * @param argv argv is an array of strings that represents the command-line
 * arguments passed to the program. The first element (argv[0]) is the name of
 * the program itself, and the following elements (argv[1], argv[2], etc.) are
 * the arguments provided by the user when running the program.
 *
 * @return The main function is returning an integer value of 0 if everything
 * was successful.
 */
int main(int argc, char** argv) {
  initLogging();

  CTextToCPP* textToCPP = new CTextToCPP();
  Options* commandOptions = new Options();
  commandOptions->parseGlobaleOptions(argc, argv);
  vector<string> files = commandOptions->getFileNames();

  // Start of Codegenerator ---------------------------------------------

  for (string file : files) {
    LOG(INFO) << "File: " << file << endl;

    // Open input file and read its contents
    ifstream inputFile(file);
    if (!inputFile) {
      LOG(ERROR) << "Error opening the file: " << file << endl;
      cerr << "Error opening the file: " << file << endl;
      continue;
    }

    string fileContent((istreambuf_iterator<char>(inputFile)),
                       istreambuf_iterator<char>());
    inputFile.close();

    Options* localeOptions = new Options(*commandOptions);
    CCodegenerator* codegenerator = new CCodegenerator();
    codegenerator->processString(fileContent, file, textToCPP, localeOptions);

    if (localeOptions->getSortByVarName()) {
      textToCPP->sort();
    }
    string declaration = textToCPP->writeDeclaration();
    string implementation = textToCPP->writeImplementation();
    if (localeOptions->getNamespace() != "" &&
        localeOptions->getOutputType() != "C") {
      declaration = codegenerator->generateNamespace(
          localeOptions->getNamespace(), declaration);
      implementation = codegenerator->generateNamespace(
          localeOptions->getNamespace(), implementation);
    }

    declaration = codegenerator->generateHeaderSurroundings(
        localeOptions->getOutputFilename(), declaration);
    implementation = codegenerator->generateSourceHead(
        localeOptions->getOutputFilename(), implementation);

    string sourceType = "." + toLowerCases(localeOptions->getOutputType());
    createFile(localeOptions->getOutputFilename() + ".h", declaration,
               localeOptions->getHeaderDir());
    createFile(localeOptions->getOutputFilename() + sourceType, implementation,
               localeOptions->getSourceDir());

    // Clean up
    delete codegenerator;
    delete localeOptions;
    textToCPP->clear();
  }
  delete textToCPP;

  return 0;
}