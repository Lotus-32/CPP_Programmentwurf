/**
 * @file main.cpp
 * @authors Timo Bauermeister, Tobias Skoberla
 * @brief The main file of the code generator
 * @version 1.0
 * @date 15-07-2023
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <CCodegenerator.h>
#include <CTextToCPP.h>
#include <Options.h>
#include <easylogging++.h>

#include <fstream>
#include <iostream>

// Cross-platform directory creation
#ifdef _WIN32
#include <sys/stat.h>
#define CREATE_DIRECTORY(path) mkdir(path)
#else
#include <sys/types.h>
#define CREATE_DIRECTORY(path) mkdir(path, 0700)
#endif

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
 * The function "pathExists" checks if a file or directory exists at the given
 * path.
 *
 * @param path The path parameter is a string that represents the file or
 * directory path that we want to check for existence.
 *
 * @return a boolean value. It returns true if the path exists and false if it
 * does not exist.
 */
bool pathExists(const string& path) {
  if (access(path.c_str(), F_OK) != -1) {
    return true;
  } else {
    return false;
  }
}

/**
 * The function creates directories recursively based on a given path.
 *
 * @param path The `path` parameter is a string that represents the directory
 * path that needs to be created.
 */
void createDirectories(const string& path) {
  string subPath;
  for (char i : path) {
    if (i == '/') {
      if (!pathExists(subPath)) {
        CREATE_DIRECTORY(subPath.c_str());
      }
    }
    subPath += i;
  }
  if (!pathExists(subPath)) {
    CREATE_DIRECTORY(subPath.c_str());
  }
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

  if (!pathExists(correctedPath)) {
    createDirectories(correctedPath);
  }

  ofstream outputFile(correctedPath + "/" + fileName);
  if (!outputFile) {
    cerr << "Error creating the file: " << fileName << endl;
    exit(1);
  }

  outputFile << content;
  outputFile.close();

  cout << "File \"" << fileName << "\" created successfully in: \"" << path
       << "\"" << endl;
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

  LOG(INFO) << "Codegenerator started" << endl;
  cout << "Codegenerator started" << endl;

  CTextToCPP* textToCPP = new CTextToCPP();
  Options* commandOptions = new Options();
  commandOptions->parseGlobaleOptions(argc, argv);
  vector<string> files = commandOptions->getFileNames();

  // Start of Codegenerator ---------------------------------------------

  for (const string& file : files) {
    LOG(INFO) << "File: " << file << endl;

    // Open input file and read its contents
    ifstream inputFile(file);
    if (!inputFile) {
      LOG(ERROR) << "Error opening the file: " << file << endl;
      cerr << "Error opening the file: " << file << endl;
      exit(1);
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

    LOG(INFO) << "Header path exists: "
              << pathExists(localeOptions->getHeaderDir()) << endl;
    LOG(INFO) << "Source path exists: "
              << pathExists(localeOptions->getSourceDir()) << endl;
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