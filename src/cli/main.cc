#include "absl/flags/flag.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/log/initialize.h"
#include "absl/debugging/failure_signal_handler.h"
#include "absl/debugging/symbolize.h"
#include "absl/log/log.h"

void InitGoogle(int* argc, char*** argv) {
  absl::InitializeLog();
  absl::SetProgramUsageMessage("Convert ELF to SQLite databases");
  // Initialize the symbolizer to get a human-readable stack trace
  absl::InitializeSymbolizer(*argv[0]);
  absl::ParseCommandLine(*argc, *argv);
  absl::InstallFailureSignalHandler(absl::FailureSignalHandlerOptions {});
}


int main(int argc, char** argv) {
  InitGoogle(&argc, &argv);

  if (argc <= 1) {
    LOG(ERROR) << "You must include at least one filename.";
    return EXIT_FAILURE;
  }

  return 0;
}