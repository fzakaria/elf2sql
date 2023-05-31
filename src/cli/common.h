#pragma once

#include "absl/debugging/failure_signal_handler.h"
#include "absl/debugging/symbolize.h"
#include "absl/flags/parse.h"
#include "absl/flags/usage.h"
#include "absl/log/initialize.h"

void InitGoogle(const int* argc, char*** argv, absl::string_view usageMessage);
