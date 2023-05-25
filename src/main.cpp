#include <easylogging++.h>
#include <getopt.h>
#include <stdio.h>

using namespace std;

INITIALIZE_EASYLOGGINGPP

const char* help_message = R"(
Eingabe: codegenerator [Optionen] [Dateien]

Optionen:
  -o, --outputfilename FILENAME Setzt den Namen der Outputfiles
  -t, --outputtype TYPE         Setzt den Dateityp des generierten Codes (gültige Optionen: C, CPP)
  -d, --headerdir DIR           Setzt das Verzeichnis für die Header-Datei
  -s, --sourcedir DIR           Setzt das Verzeichnis für die Source-Datei
  -n, --namespace NAMESPACE     Setzt den Namespace
  -l, --signperline X           Setzt die Anzahl der Zeichen pro Zeile für Zeilenumbrüche in generierten Variablen
  -v, --sortbyvarname           Sortiert die Variablen nach ihrem Namen
  -h, --help                    Zeigt diesen Hilfetext an


Autorenteam:
  Timo Bauermeister
  Jannik Kiebler-Schauer
  Mark Sachße
  Tobias Skoberla

Email:
  tobias.skoberla@gmail.com
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

  string outputFilename;
  string outputType;
  string headerDir;
  string sourceDir;
  string namespaceStr;
  int signPerLine = -1;
  bool sortByVarName = false;

  static const struct option long_options[] = {
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
  while ((option = getopt_long(argc, argv, "ho:t:d:s:n:l:v", long_options,
                               nullptr)) != -1) {
    switch (option) {
      case 'h':
        std::cout << help_message << std::endl;
        return 0;
      case 'o':
        outputFilename = optarg;
        LOG(INFO) << "Outputfilename: " << outputFilename;
        break;
      case 't':
        outputType = optarg;
        if (outputType != "C" && outputType != "CPP") {
          LOG(ERROR) << "Ungültiger Outputtype: " << outputType;
          return 1;
        }
        LOG(INFO) << "Outputtype: " << outputType;
        break;
      case 'd':
        headerDir = optarg;
        LOG(INFO) << "Headerdir: " << headerDir;
        break;
      case 's':
        sourceDir = optarg;
        LOG(INFO) << "Sourcedir: " << sourceDir;
        break;
      case 'n':
        namespaceStr = optarg;
        LOG(INFO) << "Namespace: " << namespaceStr;
        break;
      case 'l':
        signPerLine = stoi(optarg);  // string to int
        LOG(INFO) << "Signperline: " << signPerLine;
        break;
      case 'v':
        LOG(INFO) << "Sortbyvarname: true";
        break;
      case '?':
        LOG(ERROR) << "Unbekannte Option: " << optopt;
        return 1;
    }
  }

  // TODO: File handling
  for (int i = optind; i < argc; i++) {
    cout << "Übergebene Datei: " << argv[i] << endl;
  }

  return 0;
}