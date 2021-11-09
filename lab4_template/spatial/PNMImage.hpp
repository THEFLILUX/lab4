#pragma once

#include "../utils/pnm.hpp"
using namespace pnm::literals;

namespace utec {
namespace spatial {

/**
 * PNMImage implementation
 */

class PNMImage {
 private:
 public:
  static bool equals(const std::string& left, const std::string& right) {
    bool result = true;
    pnm::image<pnm::rgb_pixel> ppmOriginal = pnm::read("images/utec.pgm");
    pnm::image<pnm::rgb_pixel> ppmCopy = pnm::read("images/prueba.pgm");

    for(std::size_t y=0; y<ppmOriginal.y_size(); ++y)
    {
        for(std::size_t x=0; x<ppmOriginal.x_size(); ++x)
        {
            if(ppmOriginal[y][x] != ppmCopy[y][x]){
                result = false;
            }
        }
    }
    return result;
  }
};

}  // namespace spatial
}  // namespace utec
