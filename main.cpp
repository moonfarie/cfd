#include <exception>
#include <iostream>

#include "app.hpp"

int main() {
  cfd::App app(800, 600, "CFD");

  try {
    app.run();
  } catch (const std::exception& err) {
    std::cerr << err.what() << '\n';
  }

  return 0;
}
