#**************************************************************************************************
#  Main authors:
#     Inga Abel <inga.abel@tum.de>, 
#	  Maximilian Neuner <maximilian.neuner@tum.de>, 
#	  Michael Eick <michael.eick@tum.de>
#
# 
#  Copyright (C) 2021
#  Chair of Electronic Design Automation
#  Univ.-Prof. Dr.-Ing. Ulf Schlichtmann
#  TU Muenchen
#  Arcisstrasse 21
#  D-80333 Muenchen
#  Germany
#
#  This file is part of acst, a analog circuit analysis, sizing and synthesis enviroment:
#
#  Permission is hereby granted, free of charge, to any person obtaining
#  a copy of this software and associated documentation files (the
#  "Software"), to deal in the Software without restriction, including
#  without limitation the rights to use, copy, modify, merge, publish,
#  distribute, sublicense, and/or sell copies of the Software, and to
#  permit persons to whom the Software is furnished to do so, subject to
#  the following conditions:
#
#  The above copyright notice and this permission notice shall be
#  included in all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
#  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
#  NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
#  LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
#  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
#  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
#
#**************************************************************************************************

# ACST main Cmake File
cmake_minimum_required (VERSION 2.8)
project (acst)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread") 

add_definitions(-D_GLIBCXX_USE_CXX17_ABI=1)
set(COMPONENTS Log Control Core HSpice StructRec  AutomaticSizing GecodeExtensions
	Partitioning Synthesis src/pymaga)
set(BINARY_NAME acst)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
include_directories(${CMAKE_SOURCE_DIR})

set(EXTERN_LIBS )

set(CMAKE_BUILD_TYPE Debug)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)

###
# Find RapidXml
###
list(APPEND CMAKE_INCLUDE_PATH
    /home/pham/thirdparty_libs/rapidxml-1.13/ 
)
find_package(RapidXml REQUIRED)
include_directories(SYSTEM ${RAPIDXML_INCLUDE_DIR})

###
# Find Boost library
###
set(BOOST_ROOT /opt/thirdparty_libs/boost_1_82_0)
set(Boost_USE_STATIC_LIBS        ON)
set(Boost_USE_MULTITHREADED     OFF)
set(Boost_USE_STATIC_RUNTIME    OFF)
 set(Boost_DEBUG TRUE)
find_package(Boost COMPONENTS program_options)
if(Boost_FOUND)
    message("##########Boost found:")
    message("  includes: ${Boost_INCLUDE_DIRS}")
    message("  libs: ${Boost_LIBRARIES}")
else()
    message("Boost not found!")
endif()
include_directories(SYSTEM ${Boost_INCLUDE_DIRS})



###
# Find CppUnit
###
find_package(CppUnit)
if(CPPUNIT_FOUND)
    message("CppUnit found:")
    message("  includes: ${CPPUNIT_INCLUDE_DIR}")
    message("  libs: ${CPPUNIT_LIBRARIES}")
    include_directories(SYSTEM ${CPPUNIT_INCLUDE_DIR})
else()
    message("CppUnit not found!")
endif()

###
# acst stuff
###
include(ComponentsManagement)
include(AddSource)
include(EnteringMessage)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/Control/src) # for .gen files
comp_def_function(ANALYSIS)



SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS} -rdynamic -Wall -Wextra -Wformat=2  -Werror=return-type -Wold-style-cast -Wno-reorder -Xlinker -export-dynamic -g -fPIC") # -pg")



# add_dependencies(${TEST_BUILD_TARGET} common_test)

# add_subdirectory(tests)
# target_include_directories(${CMAKE_SOURCE_DIR}/tests)
# include_directories(${CMAKE_SOURCE_DIR}/tests)
# include_directories(${CMAKE_SOURCE_DIR}/tests)
# target_sources(synthesis_small_library  INTERFACE  ${CMAKE_SOURCE_DIR}/tests/synthesis_small_library.cpp)
# add_subdirectory(${CMAKE_SOURCE_DIR}/tests)
# add_executable(synthesis_small_library tests/synthesis_small_library.cpp)
# target_link_libraries(synthesis_small_library Log Control ManagedComponents boost_program_options.a rt ) # rt = runtime

# add_executable(common_object tests/common/object.cpp)
# target_link_libraries(common_object Control) # rt = runtime

# add_executable(analysisfactory tests/control_cmd/AnalysesFactory.cpp)
# target_link_libraries(analysisfactory Control Log Control ManagedComponents ${Boost_LIBRARIES} ) # rt = runtime
# target_link_libraries(analysisfactory Log Control ManagedComponents boost_program_options.a rt) # rt = runtime

# target_include_directories(control_cmd PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/Control/incl/Analysis)

include_directories(/opt/thirdparty_libs/gecode-release-6.2.0)
link_directories(/opt/thirdparty_libs/gecode-release-6.2.0)

# add_dependencies(tests synthesis_small_library)

foreach(dir IN LISTS COMPONENTS)
    message("Adding component ${dir}")
    add_subdirectory(${dir})
endforeach(dir)

add_executable (${BINARY_NAME}
    main.cpp
)

enable_testing()
set(TEST_BUILD_TARGET "tests")
add_custom_target(${TEST_BUILD_TARGET})
add_subdirectory(${CMAKE_SOURCE_DIR}/tests)

target_link_libraries(${BINARY_NAME} Log Control ManagedComponents boost_program_options.a rt) # rt = runtime






if(NOT EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BINARY_NAME}.cds)
    add_custom_command(TARGET ${BINARY_NAME} POST_BUILD 
                       COMMAND echo -e "#!/bin/bash\\nunset LD_LIBRARY_PATH\\n${BINARY_NAME} $@\\n" > ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BINARY_NAME}.cds 
                       COMMAND chmod a+x ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${BINARY_NAME}.cds VERBATIM
                      )
endif()

if(NOT EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/acst.sh)
    configure_file(Control/script/acst.sh ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/acst.sh)
    
    add_custom_command(TARGET ${BINARY_NAME} POST_BUILD 
                    COMMAND chmod a+x ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/acst.sh VERBATIM
                    )
endif()    
    

include(Control/Components.cmake)


