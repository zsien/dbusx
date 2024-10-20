#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "dbusx/type.h"

using namespace dbusx;

TEST(type_tests, test_type_char) {
    ASSERT_THAT(type<char>::signature, testing::ElementsAreArray({'y'}));
    EXPECT_TRUE(type<char>::signature_str == "y");
}

TEST(type_tests, test_type_bool) {
    ASSERT_THAT(type<bool>::signature, testing::ElementsAreArray({'b'}));
    EXPECT_TRUE(type<bool>::signature_str == "b");
}

TEST(type_tests, test_type_int16) {
    ASSERT_THAT(type<int16_t>::signature, testing::ElementsAreArray({'n'}));
    EXPECT_TRUE(type<int16_t>::signature_str == "n");
}

TEST(type_tests, test_type_uint16) {
    ASSERT_THAT(type<uint16_t>::signature, testing::ElementsAreArray({'q'}));
    EXPECT_TRUE(type<uint16_t>::signature_str == "q");
}

TEST(type_tests, test_type_int32) {
    ASSERT_THAT(type<int32_t>::signature, testing::ElementsAreArray({'i'}));
    EXPECT_TRUE(type<int32_t>::signature_str == "i");
}

TEST(type_tests, test_type_uint32) {
    ASSERT_THAT(type<uint32_t>::signature, testing::ElementsAreArray({'u'}));
    EXPECT_TRUE(type<uint32_t>::signature_str == "u");
}

TEST(type_tests, test_type_int64) {
    ASSERT_THAT(type<int64_t>::signature, testing::ElementsAreArray({'x'}));
    EXPECT_TRUE(type<int64_t>::signature_str == "x");
}

TEST(type_tests, test_type_uint64) {
    ASSERT_THAT(type<uint64_t>::signature, testing::ElementsAreArray({'t'}));
    EXPECT_TRUE(type<uint64_t>::signature_str == "t");
}

TEST(type_tests, test_type_double) {
    ASSERT_THAT(type<double>::signature, testing::ElementsAreArray({'d'}));
    EXPECT_TRUE(type<double>::signature_str == "d");
}

TEST(type_tests, test_type_string) {
    ASSERT_THAT(type<std::string>::signature, testing::ElementsAreArray({'s'}));
    EXPECT_TRUE(type<std::string>::signature_str == "s");
}

TEST(type_tests, test_type_object_path) {
    ASSERT_THAT(type<object_path>::signature, testing::ElementsAreArray({'o'}));
    EXPECT_TRUE(type<object_path>::signature_str == "o");
}

TEST(type_tests, test_type_signature) {
    ASSERT_THAT(type<signature>::signature, testing::ElementsAreArray({'g'}));
    EXPECT_TRUE(type<signature>::signature_str == "g");
}

TEST(type_tests, test_type_array) {
    ASSERT_THAT(type<std::vector<std::string>>::signature, testing::ElementsAreArray({'a', 's'}));
    EXPECT_TRUE(type<std::vector<std::string>>::signature_str == "as");
}

TEST(type_tests, test_type_map) {
    ASSERT_THAT((type<std::unordered_map<std::string, std::string>>::signature),
                testing::ElementsAreArray({'a', '{', 's', 's', '}'}));
    EXPECT_TRUE((type<std::unordered_map<std::string, std::string>>::signature_str) == "a{ss}");
}

TEST(type_tests, test_type_struct) {
    ASSERT_THAT(type<std::tuple<std::string>>::signature,
                testing::ElementsAreArray({'(', 's', ')'}));
    EXPECT_TRUE(type<std::tuple<std::string>>::signature_str == "(s)");

    ASSERT_THAT((type<std::tuple<std::string, uint32_t>>::signature),
                testing::ElementsAreArray({'(', 's', 'u', ')'}));
    EXPECT_TRUE((type<std::tuple<std::string, uint32_t>>::signature_str) == "(su)");
}

TEST(type_tests, test_type_void) {
    ASSERT_THAT(type<void>::signature, testing::ElementsAreArray(std::array<char, 0>{}));
    EXPECT_TRUE(type<void>::signature_str == "");
}

TEST(type_tests, test_type_mixed_container) {
    ASSERT_THAT(
        (type<std::vector<std::unordered_map<std::string, std::tuple<uint32_t, std::string>>>>::
             signature),
        testing::ElementsAreArray({'a', 'a', '{', 's', '(', 'u', 's', ')', '}'}));
    EXPECT_TRUE(
        (type<std::vector<std::unordered_map<std::string, std::tuple<uint32_t, std::string>>>>::
             signature_str) == "aa{s(us)}");

    ASSERT_THAT((type<std::unordered_map<std::string, std::vector<uint32_t>>>::signature),
                testing::ElementsAreArray({'a', '{', 's', 'a', 'u', '}'}));
    EXPECT_TRUE((type<std::unordered_map<std::string, std::vector<uint32_t>>>::signature_str) ==
                "a{sau}");
}
