#[[**************************************************************************
* gta5hla GTA V Hardlinking Assistant
* Copyright (C) 2022 Syping
*
* Redistribution and use in source and binary forms, with or without modification,
* are permitted provided that the following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice,
* this list of conditions and the following disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice,
* this list of conditions and the following disclaimer in the documentation
* and/or other materials provided with the distribution.
*
* This software is provided as-is, no warranties are given to you, we are not
* responsible for anything with use of the software, you are self responsible.
****************************************************************************]]

cmake_minimum_required(VERSION 3.7)

project(gta5hla LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(FORCE_QT_VERSION "" CACHE STRING "Force Qt Version")
if(FORCE_QT_VERSION)
    set(QT_VERSION_MAJOR ${FORCE_QT_VERSION})
else()
    find_package(QT NAMES Qt6 Qt5 COMPONENTS Core REQUIRED)
endif()
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

if(WIN32)
    list(APPEND GTA5HLA_LIBS
        version
    )
    list(APPEND GTA5HLA_DEFINES
        -DUNICODE
        -D_UNICODE
        -DWIN32
    )
    list(APPEND GTA5HLA_RESOURCES
        src/win32/app.rc
    )
endif()

list(APPEND GTA5HLA_DEFINES
    -DGTA5HLA_CMAKE
    -DGTA5HLA_GUI
    -DGTA5HLA_PROJECT
)

set(GTA5HLA_SOURCES
    src/main.cpp
    src/HardlinkAssistant.cpp
    src/HardlinkThread.cpp
    src/ProgressDialog.cpp
    src/UserInterface.cpp
)

set(GTA5HLA_HEADERS
    src/HardlinkAssistant.h
    src/HardlinkThread.h
    src/ProgressDialog.h
    src/UserInterface.h
)

set(GTA5HLA_FORMS
    src/ProgressDialog.ui
    src/UserInterface.ui
)

add_executable(gta5hla
    WIN32
    ${GTA5HLA_HEADERS}
    ${GTA5HLA_SOURCES}
    ${GTA5HLA_FORMS}
    ${GTA5HLA_RESOURCES}
)

target_compile_definitions(gta5hla PRIVATE ${GTA5HLA_DEFINES})
target_link_libraries(gta5hla PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${GTA5HLA_LIBS})

install(TARGETS gta5hla DESTINATION bin)
