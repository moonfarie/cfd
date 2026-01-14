#include "app.hpp"

namespace cfd {

App::App(uint32_t width, uint32_t height, const std::string& title)
    : width_(width), height_(height), window_(width_, height_, title) {}

void App::run() const {
  while (not window_.should_close()) {
    window_.poll_events();
  }
}

}  // namespace cfd
