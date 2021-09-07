#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "dbusx/type.h"

using namespace dbusx;

TEST(type_tests, test_type_char) {
    ASSERT_THAT(type<char>::signature, testing::ElementsAreArray({'y'}));
    ASSERT_THAT(type<char>::signature_nt, testing::ElementsAreArray("y"));
}

TEST(type_tests, test_type_bool) {
    ASSERT_THAT(type<bool>::signature, testing::ElementsAreArray({'b'}));
    ASSERT_THAT(type<bool>::signature_nt, testing::ElementsAreArray("b"));
}

TEST(type_tests, test_type_int16) {
    ASSERT_THAT(type<int16_t>::signature, testing::ElementsAreArray({'n'}));
    ASSERT_THAT(type<int16_t>::signature_nt, testing::ElementsAreArray("n"));
}

TEST(type_tests, test_type_uint16) {
    ASSERT_THAT(type<uint16_t>::signature, testing::ElementsAreArray({'q'}));
    ASSERT_THAT(type<uint16_t>::signature_nt, testing::ElementsAreArray("q"));
}

TEST(type_tests, test_type_int32) {
    ASSERT_THAT(type<int32_t>::signature, testing::ElementsAreArray({'i'}));
    ASSERT_THAT(type<int32_t>::signature_nt, testing::ElementsAreArray("i"));
}

TEST(type_tests, test_type_uint32) {
    ASSERT_THAT(type<uint32_t>::signature, testing::ElementsAreArray({'u'}));
    ASSERT_THAT(type<uint32_t>::signature_nt, testing::ElementsAreArray("u"));
}

TEST(type_tests, test_type_int64) {
    ASSERT_THAT(type<int64_t>::signature, testing::ElementsAreArray({'x'}));
    ASSERT_THAT(type<int64_t>::signature_nt, testing::ElementsAreArray("x"));
}

TEST(type_tests, test_type_uint64) {
    ASSERT_THAT(type<uint64_t>::signature, testing::ElementsAreArray({'t'}));
    ASSERT_THAT(type<uint64_t>::signature_nt, testing::ElementsAreArray("t"));
}

TEST(type_tests, test_type_double) {
    ASSERT_THAT(type<double>::signature, testing::ElementsAreArray({'d'}));
    ASSERT_THAT(type<double>::signature_nt, testing::ElementsAreArray("d"));
}

TEST(type_tests, test_type_string) {
    ASSERT_THAT(type<std::string>::signature, testing::ElementsAreArray({'s'}));
    ASSERT_THAT(type<std::string>::signature_nt, testing::ElementsAreArray("s"));
}

TEST(type_tests, test_type_object_path) {
    ASSERT_THAT(type<object_path>::signature, testing::ElementsAreArray({'o'}));
    ASSERT_THAT(type<object_path>::signature_nt, testing::ElementsAreArray("o"));
}

TEST(type_tests, test_type_signature) {
    ASSERT_THAT(type<signature>::signature, testing::ElementsAreArray({'g'}));
    ASSERT_THAT(type<signature>::signature_nt, testing::ElementsAreArray("g"));
}
