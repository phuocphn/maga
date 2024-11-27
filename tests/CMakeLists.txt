cmake_minimum_required(VERSION 2.8)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# the following setting is equivalent to 
# export LD_LIBRARY_PATH="/opt/thirdparty_libs/gecode-release-6.2.0:${LD_LIBRARY_PATH}"
# export LD_LIBRARY_PATH="/opt/thirdparty_libs/boost_1_82_0/stage/lib:${LD_LIBRARY_PATH}"
# export LD_LIBRARY_PATH="/opt/thirdparty_libs/boost_1_82_0:${LD_LIBRARY_PATH}"

set(CMAKE_EXE_LINKER_FLAGS "-static-libstdc++")

# Add test cases 
# ************************************
# add_executable(gtest_basic gtest_basic.cpp)
# target_link_libraries(gtest_basic PRIVATE GTest::gtest_main)

# add_executable(common_simple common_test1.cpp)
# target_link_libraries(common_simple GTest::gtest_main Log Control ManagedComponents boost_program_options.a rt) #add the lib that we want to test here!

add_executable(common_test common_test.cpp)
target_link_libraries(common_test GTest::gtest_main Log Control ManagedComponents boost_program_options.a rt ) 


add_executable(cmd_synthesis_hspice cmd_synthesis_hspice.cpp)
target_link_libraries(cmd_synthesis_hspice GTest::gtest_main Log Control ManagedComponents boost_program_options.a rt ) 

add_executable(cmd_synthesis_fbb cmd_synthesis_fbb.cpp)
target_link_libraries(cmd_synthesis_fbb GTest::gtest_main Log Control ManagedComponents boost_program_options.a rt ) 

add_executable(cmd_structure_recognition cmd_structure_recognition.cpp)
target_link_libraries(cmd_structure_recognition GTest::gtest_main Log Control ManagedComponents boost_program_options.a rt ) 

add_executable(cmd_toplibgen cmd_toplibgen.cpp)
target_link_libraries(cmd_toplibgen GTest::gtest_main Log Control ManagedComponents boost_program_options.a rt ) 