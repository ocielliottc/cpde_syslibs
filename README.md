# op_lite
A portable, lightweight object persistence library for C++

## Introduction
The name op_lite stands for _Object Persistence – Lightweight_. It is a C++ library that offers automatic in-process object persistence, the application code never explicitly reads or writes to the database – it all happens behind the scenes, given that certain programming patterns are followed. This is similar to most other "real" OO databases. The effect is that the objects are perceived to “live in the database”.

For more details see [the white paper](doc/op_lite_white_paper.odt).

# Build instructions
op_lite comes with makefiles for Windows and Linux. For Windows the MSVC compiler is assumed. Tested for MSVC2013 Professional and Express versions.

## Dependencies
op_lite requires boost (http://www.boost.org/ ) and msgpack-c ( https://msgpack.org/ )

## Linux builds
boost and msgpack-c are assumed to exit in `/usr/local`. If not, edit the Makefile entries for boost and msgpack-c to fit.

<pre>
git clone https://github.com/arnholm/op_lite
cd op_lite
make
</pre>

Shared object library generated in .cmp\gcc\bin\Release

## Windows builds
Open a MSVC command line build terminal, e.g. "VS2013 x64 Cross Tools Command Prompt". Edit Makefile.msvc and set entries for boost and msgpack-c to fit.

<pre>
git clone https://github.com/arnholm/op_lite
cd op_lite
nmake -f Makefile.msvc
</pre>

DLL and import library generated in .cmp\msvc\bin\Release
