#include "app.hpp"

namespace cfd {

App::App(int width, int height, const std::string& title) : window_(width, height, title) {}

void App::run() const {
  while (not window_.should_close()) {
    window_.poll_events();
  }
}

}  // namespace cfd
