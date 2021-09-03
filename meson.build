project(
    'dbusx',
    'cpp',
    default_options : [
        'cpp_std=c++20',
        'warning_level=3',
    ]
)

libsystemd = dependency('libsystemd', required: true)

includes = include_directories('include')

subdir('include')
subdir('src')
subdir('example')
