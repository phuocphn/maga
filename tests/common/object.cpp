#include "Core/incl/Common/Object.h"
#include <iostream>

class TestObject : public Core::Object {
    // TestObject inherits from Object to use its toStr method and overloaded operators.
    // Additional functionality can be added if required.
};

int main() {

    // Test the Object itself
    TestObject obj;

    // Test the toStr method
    std::cout << "Testing toStr method:" << std::endl;
    std::cout << obj.toStr() << std::endl;

    // Test the << operator with object reference
    std::cout << "Testing << operator with object reference:" << std::endl;
    std::cout << obj << std::endl;

    // Test the << operator with object pointer
    TestObject *objPtr = &obj;
    std::cout << "Testing << operator with object pointer:" << std::endl;
    std::cout << objPtr << std::endl;

    // Test the << operator with null pointer
    TestObject *nullPtr = nullptr;
    std::cout << "Testing << operator with null pointer:" << std::endl;
    std::cout << nullPtr << std::endl;

    // Test the + operator with string and object reference
    std::cout << "Testing + operator with string and object reference:" << std::endl;
    std::string str1 = "Object info: ";
    std::cout << str1 + obj << std::endl;

    // Test the + operator with string and object pointer
    std::cout << "Testing + operator with string and object pointer:" << std::endl;
    std::string str2 = "Object pointer info: ";
    std::cout << str2 + objPtr << std::endl;

    // Test the + operator with string and null pointer
    std::cout << "Testing + operator with string and null pointer:" << std::endl;
    std::cout << str2 + nullPtr << std::endl;


    return 0;
}