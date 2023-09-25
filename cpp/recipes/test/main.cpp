#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

int main(int argc, char** argv){
    ::testing::InitGoogleMock(&argc, argv);
    std::shared_ptr<int> p {};
    return RUN_ALL_TESTS();
}
