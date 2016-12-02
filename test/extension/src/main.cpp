#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <boost/asio/impl/src.hpp>

int main(int argc, char *argv[])
{
    ::testing::InitGoogleMock(&argc, argv);
    return RUN_ALL_TESTS();
}
