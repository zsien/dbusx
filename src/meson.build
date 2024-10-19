libdbusx_private_headers = files('''
    sdbus/bus_private.h
    sdbus/message_private.h
'''.split())

libdbusx_sources = files('''
    sdbus/bus.cpp
    sdbus/bus_private.cpp
    sdbus/message.cpp
    sdbus/message_private.cpp
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
