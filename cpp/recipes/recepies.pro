# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

#TARGET = recepies

HEADERS = \
   $$PWD/src/buffer/buffer.h \
   $$PWD/src/buffer/test.h \
   $$PWD/src/bytebuf/bytebuf.h \
   $$PWD/src/bytebuf/test.h \
   $$PWD/src/guid_map/guid_hash_map.h \
   $$PWD/src/tests/tests.h \
    src/guid_map/guid_hash_map.h \
    src/guid_map/test.h

SOURCES = \
   $$PWD/src/buffer/buffer.cpp \
   $$PWD/src/bytebuf/bytebuf.cpp \
   $$PWD/src/guid_map/guid_hash_map.cpp \
   $$PWD/src/main.cpp

INCLUDEPATH = \
    $$PWD/src/buffer \
    $$PWD/src/bytebuf \
    $$PWD/src/guid_map \
    $$PWD/src/tests

#DEFINES = 

QMAKE_CXXFLAGS += -std=c++17
