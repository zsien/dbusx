gtest_dep = dependency('gtest', main: true, required: true)
gmock_dep = dependency('gmock', required: true)

type_tests = executable(
    'type_tests',
    sources: [
        'type_tests.cpp',
    ],
    include_directories: includes,
    dependencies: [gtest_dep, gmock_dep],
)

test('type_tests', type_tests)
