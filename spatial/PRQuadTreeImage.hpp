#pragma once

#include "SpatialImageBase.hpp"

namespace utec {
namespace spatial {

/**
 * PRQuadTreeImage implementation
 */
class PRQuadTreeImage : public SpatialImageBase {
 private:
 public:
  void load(const std::string& filename) override {}
  void compress(const std::string& filename) override {}
  void decompress(const std::string& filename) override {}
  void convertToPGM(const std::string& filename) override {}
};

}  // namespace spatial
}  // namespace utec
