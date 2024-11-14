cmake_minimum_required(VERSION 2.8)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(GTest::GTest INTERFACE IMPORTED)
target_link_libraries(GTest::GTest INTERFACE gtest_main)

add_executable(test_fn synthesis_small_library.cpp)
target_link_libraries(test_fn PRIVATE GTest::GTest) #add the lib that we want to test here!
# target_link_libraries(test_fn PRIVATE GTest::GTest control_cmd) #add the lib that we want to test here!



