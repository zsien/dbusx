libdbusx_private_headers = files('''
    sdbus/bus_private.h
    sdbus/message_private.h
'''.split())

libdbusx_sources = files('''
    sdbus/bus.cc
    sdbus/bus_private.cc
    sdbus/message.cc
    sdbus/message_private.cc
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
