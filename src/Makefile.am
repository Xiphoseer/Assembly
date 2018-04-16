lib_LTLIBRARIES = libassembly.la

libassembly_la_SOURCES = \
	cli.cpp package.cpp catalog.cpp manifest.cpp database.cpp \
	stringutil.cpp filesystem.cpp fdb_io.cpp fdb_query.cpp \
	filesystem_linux.cpp reader.cpp zone_io.cpp scene_io.cpp \
	scene.cpp utf.cpp namevalue.cpp namevalue_io.cpp zone_path.cpp \
	zone_path_io.cpp types.cpp

libassembly_la_LDFLAGS = -version-info 1:2:0
libassembly_la_LIBADD  = -lstdc++
libassembly_la_CXXFLAGS = -std=c++14

pkginclude_HEADERS = \
	cli.hpp package.hpp catalog.hpp manifest.hpp database.hpp \
	filesystem.hpp stringutil.hpp fdb_io.hpp fdb_query.hpp \
	functional.hpp types.hpp reader.hpp zone.hpp zone_io.hpp \
	scene.hpp scene_io.hpp utf.hpp namevalue.hpp namevalue_io.hpp \
	zone_path.hpp zone_path_io.hpp
