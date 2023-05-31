#include "src/cli/common.h"

void InitGoogle(const int* argc, char*** argv, absl::string_view usageMessage) {
  absl::InitializeLog();
  absl::SetProgramUsageMessage(usageMessage);
  // Initialize the symbolizer to get a human-readable stack trace
  absl::InitializeSymbolizer(*argv[0]);
  absl::ParseCommandLine(*argc, *argv);
  absl::InstallFailureSignalHandler(absl::FailureSignalHandlerOptions{});
}