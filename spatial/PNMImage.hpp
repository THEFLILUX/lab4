#pragma once

#include "../utils/pnm.hpp"

namespace utec {
namespace spatial {

/**
 * PNMImage implementation
 */
class PNMImage {
 private:
 public:
  static bool equals(const std::string& left, const std::string& right) {
    return false;
  }
};

}  // namespace spatial
}  // namespace utec
