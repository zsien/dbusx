gtest_dep = dependency('gtest', main: true, required: true)
gmock_dep = dependency('gmock', required: true)

type_tests = executable(
    'type_tests',
    sources: [
        'type_tests.cc',
    ],
    include_directories: includes,
    dependencies: [gtest_dep, gmock_dep],
)

test('type_tests', type_tests)

object_path_tests = executable(
    'object_path_tests',
    sources: [
        'object_path_tests.cc',
    ],
    include_directories: includes,
    dependencies: [gtest_dep, gmock_dep],
    link_with: [libdbusx],
)

test('object_path_tests', object_path_tests)
