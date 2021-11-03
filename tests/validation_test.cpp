#include <gtest/gtest.h>

#include <iostream>

#include "PNMImage.hpp"
#include "PRQuadTreeImage.hpp"

using namespace utec::spatial;

TEST(QuadTest, BasicTest) {
  PRQuadTreeImage qt;
  qt.load("images/utec.pgm");
  qt.compress("prueba.qt");
  qt.decompress("prueba.qt");
  qt.convertToPGM("images/prueba.pgm");

  PNMImage comparator;
  EXPECT_TRUE(comparator.equals("images/utec.pgm", "images/prueba.pgm"));
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}