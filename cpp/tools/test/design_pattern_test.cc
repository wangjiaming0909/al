#include <gtest/gtest.h>
#include "design_patterns/basic/solid.h"
#include "design_patterns/creational/factory/factory.h"
#include "design_patterns/creational/prototype/prototype.h"
#include "design_patterns/structural/bridge.h"

using namespace design_patterns;

TEST(SOLID, open_close) {
  design_patterns::OC_test();
}

TEST(design_pattern, creational) {
  test_abstract_factory();
  test_prototype();
}

TEST(design_pattern, structural) {
  test_bridge();
}
