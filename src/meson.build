libdbusx_private_headers = files('''
    bus_private.h
    message_private.h
'''.split())

libdbusx_sources = files('''
    bus.cpp
    bus_private.cpp
    message.cpp
    message_private.cpp
'''.split())

libdbusx = library(
    'dbusx',
    sources:libdbusx_public_headers + libdbusx_private_headers + libdbusx_sources,
    include_directories: includes,
    dependencies: [libsystemd, tl_expected],
    version: '0.0.1',
    soversion: '0',
    install: true,
)
