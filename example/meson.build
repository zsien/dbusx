example_sources = files('''
    main.cpp
    example.cpp
'''.split())

executable(
    'example',
    sources: example_sources,
    include_directories: includes,
    link_with: libdbusx,
    dependencies: [tl_expected],
    install: true,
)
