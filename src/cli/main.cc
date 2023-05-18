#include "gflags/gflags.h"
#include "glog/logging.h"

DEFINE_bool(verbose, false, "Display program name before message");

int main(int argc, char** argv) {
  gflags::SetUsageMessage("Convert ELF to SQLite databases");
  gflags::SetVersionString("1.0.0");
  gflags::ParseCommandLineFlags(&argc, &argv, true);

  // to simplify our application lets log to STDERR rather than log files
  // as this is a CLI application
  FLAGS_logtostderr = true;

  // Initialize Google’s logging library.
  google::InitGoogleLogging(argv[0]);
  google::InstallFailureSignalHandler();
  if (argc < 1) {}

  gflags::ShutDownCommandLineFlags();
  return 0;
}