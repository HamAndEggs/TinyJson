
#include <iostream>

#include "../TinyJson.h"

int main(int argc, char *argv[])
{
    std::cout << "Unit test app for TinyJson build by appbuild.\n";

// Display the constants defined by app build. \n";
    std::cout << "Application Version " << APP_VERSION << '\n';
    std::cout << "Build date and time " << APP_BUILD_DATE_TIME << '\n';
    std::cout << "Build date " << APP_BUILD_DATE << '\n';
    std::cout << "Build time " << APP_BUILD_TIME << '\n';

    const char* test1Json = R"({"Hello World":"TheWorld"})";

    tinyjson::JsonProcessor test1(test1Json);

// And quit\n";
    return EXIT_SUCCESS;
}
