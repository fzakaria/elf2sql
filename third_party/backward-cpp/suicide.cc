#include <iostream>

/**
 * Obvious suicide function to test stacktrace printing of backward-cpp
 */
int main() {
  std::cout << "Running suicide application" << std::endl;
  int* ptr = nullptr;
  *ptr += 1;
  return 0;
}