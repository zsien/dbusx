#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "dbusx/object_path.h"

using namespace dbusx;

TEST(object_path_tests, test_object_path_valid) {
    ASSERT_TRUE(object_path("/").is_valid());
    ASSERT_TRUE(object_path("/cn").is_valid());
    ASSERT_TRUE(object_path("/cn/zsien").is_valid());
    ASSERT_TRUE(object_path("/cn/zsien/1000").is_valid());
    ASSERT_TRUE(object_path("/cn/zsien/_1000").is_valid());
}

TEST(object_path_tests, test_object_path_invalid) {
    ASSERT_FALSE(object_path("").is_valid());
    ASSERT_FALSE(object_path("cn").is_valid());
    ASSERT_FALSE(object_path("/cn/").is_valid());
    ASSERT_FALSE(object_path("/cn/-").is_valid());
    ASSERT_FALSE(object_path("/cn/-z").is_valid());
}
