#include <benchmark/benchmark.h>

#include "../tests/utils/DataGenerator.hpp"
#include "BasicSpatial.hpp"
#include "Point.h"
#include "Validator.h"

using namespace utec::spatial;

using data_t = double;
using point_t = Point<data_t, 1>;

class SetUp {
 private:
  SetUp() {
    const std::size_t num_points = 10000;
    const data_t min = 0, max = 1000;
    SetUp::generator =
        std::make_unique<DataGenerator1D<data_t>>(num_points, min, max);
  }

 public:
  static void PerformSetUp() { static SetUp setUp; }
  inline static std::unique_ptr<DataGenerator1D<data_t>> generator;
};

template <class SpatialDS>
static void BM_range(benchmark::State& state) {
  SetUp::PerformSetUp();
  const data_t max = 1000;

  SpatialDS instancia;
  auto points = SetUp::generator->get_data();

  for (auto& p : points) {
    instancia.insert(p);
  }

  point_t query_min = point_t({max * 0.2});
  point_t query_max = point_t({max * 0.8});

  for (auto _ : state) {
    auto result = instancia.range(query_min, query_max);
  }
}

BENCHMARK_TEMPLATE(BM_range, Validator<point_t>);
BENCHMARK_TEMPLATE(BM_range, BasicSpatial<point_t>);

BENCHMARK_MAIN();