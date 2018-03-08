lib_LTLIBRARIES = libassembly.la

libassembly_la_SOURCES = \
	cli.cpp \
	package.cpp \
	catalog.cpp \
	manifest.cpp \
	database.cpp \
	stringutil.cpp \
	filesystem.cpp \
	fdb_reader.cpp \
	filedatabase.cpp \
	filesystem_linux.cpp

libassembly_la_LDFLAGS = -version-info 1:2:0
libassembly_la_LIBADD  = -lstdc++
libassembly_la_CXXFLAGS = -std=c++14

pkginclude_HEADERS = \
	cli.hpp \
	package.hpp \
	catalog.hpp \
	manifest.hpp \
	database.hpp \
	filesystem.hpp \
	stringutil.hpp \
	fdb_reader.hpp \
	filedatabase.hpp \
	functional.hpp
