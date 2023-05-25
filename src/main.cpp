#include <easylogging++.h>
#include <getopt.h>
#include <stdio.h>

using namespace std;

INITIALIZE_EASYLOGGINGPP

// getopt_long options
static const struct option long_options[] = {{"help", no_argument, 0, 'h'}, 0};

const char* help_message = R"(
Usage: programm [Optionen] [Dateien]

Optionen:
  -h, --help            Zeigt diesen Hilfetext an
)";

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

  int option;
  while ((option = getopt_long(argc, argv, "h", long_options, nullptr)) != -1) {
    switch (option) {
      // TODO: More options
      case 'h':
        cout << help_message << endl;
        return 0;
      case '?':
        // Undefined option
        LOG(ERROR) << "Unbekannte Option: " << argv[optind - 1];
        return 1;
    }
  }

  // TODO: File handling
  for (int i = optind; i < argc; i++) {
    std::cout << "Übergebene Datei: " << argv[i] << std::endl;
  }

  return 0;
}