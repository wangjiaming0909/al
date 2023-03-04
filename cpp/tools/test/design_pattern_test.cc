#include <gtest/gtest.h>
#include "design_patterns/basic/solid.h"
#include "design_patterns/creational/factory/factory.h"

TEST(SOLID, open_close) {
  design_patterns::OC_test();
}

TEST(design_pattern, creational) {
  test_abstract_factory();
}
