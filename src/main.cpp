#include <CTextToCPP.h>
#include <Options.h>
#include <easylogging++.h>
#include <getopt.h>
#include <stdio.h>

using namespace std;

INITIALIZE_EASYLOGGINGPP

/**
 * @brief Initializes the logging system
 *
 */
void initLogging() {
  // Laedt das Konfigurationsfile und konfiguriert die Logger
  el::Configurations conf("assets/logging.ini");
  // Schaltet Mechanismus aus der das Programm bei fatalem Error beendet
  el::Loggers::addFlag(el::LoggingFlag::DisableApplicationAbortOnFatalLog);
  // Bei Systemcrash wird der Crashgrund in der Log Datei noch hinterlegt
  el::Loggers::addFlag(el::LoggingFlag::LogDetailedCrashReason);
  // Falls Bildschirmausgabe, dann farbig
  el::Loggers::addFlag(el::LoggingFlag::ColoredTerminalOutput);
  // Uebertraegt die Einstellungen an die Logger die laufen
  el::Loggers::reconfigureLogger("default", conf);
  el::Loggers::reconfigureAllLoggers(conf);

  // LOG(INFO) << "Infos zum Ablauf";
  // LOG(DEBUG) << "Debugging von Programmteile";
  // LOG(WARNING) << "Das Programm kann noch weiterlaufen";
  // LOG(ERROR) << "Das ist schon ein richtig schwerwiegender Fehler";
  // LOG(FATAL) << "Hier fehlt z.B. schon der Speicher um weiterzu machen";
}

int main(int argc, char** argv) {
  initLogging();

  Options options;
  options.parseOptions(argc, argv);

  if (options.isSetHeaderDir()) {
    cout << "Headerdir: " << options.getHeaderDir() << endl;
  }
  if (options.isSetSourceDir()) {
    cout << "Sourcedir: " << options.getSourceDir() << endl;
  }
  if (options.isSetOutputFilename()) {
    cout << "Outputfilename: " << options.getOutputFilename() << endl;
  }
  if (options.isSetOutputType()) {
    cout << "Outputtype: " << options.getOutputType() << endl;
  }
  if (options.isSetNamespace()) {
    cout << "Namespace: " << options.getNamespace() << endl;
  }
  if (options.isSetSignPerLine()) {
    cout << "Signperline: " << options.getSignPerLine() << endl;
  }
  if (options.isSetSortByVarName()) {
    cout << "Sortbyvarname: " << options.getSortByVarName() << endl;
  }

  return 0;
}