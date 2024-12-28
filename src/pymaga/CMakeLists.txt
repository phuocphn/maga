add_subdirectory("${PROJECT_SOURCE_DIR}/external/pybind11" "external/pybind11" EXCLUDE_FROM_ALL)

set(PYTHON_MODULE_NAME "pymaga")
pybind11_add_module(${PYTHON_MODULE_NAME} bindings.cpp AcstUtility.cpp)
target_link_libraries(${PYTHON_MODULE_NAME} PRIVATE Partitioning AutomaticSizing GTest::gtest_main Log  StructRec Synthesis Control ManagedComponents boost_program_options.a rt)