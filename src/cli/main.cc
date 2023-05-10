#include "gflags/gflags.h"

DEFINE_bool(verbose, false, "Display program name before message");

int main(int argc, char** argv) {
  gflags::SetUsageMessage("Convert ELF to SQLite databases");
  gflags::SetVersionString("1.0.0");
  gflags::ParseCommandLineFlags(&argc, &argv, true);
  gflags::ShutDownCommandLineFlags();
  return 0;
}