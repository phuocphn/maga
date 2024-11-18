#include <iostream>
#include <gtest/gtest.h>

void hello_world()
{
    std::cout << "Testing ...." << std::endl;
}


TEST (BasicTest, HelloWorld){
    hello_world();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}