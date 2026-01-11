#pragma once

#include "window.hpp"

namespace cfd {

class App {
 public:
  App(int width, int height, const std::string& title);
  ~App() = default;

  void run() const;

 private:
  Window window_;
};

}  // namespace cfd
