
#include <iostream>
#include <chrono>
#include <sstream>
#include <fstream>

#include <assert.h>

#include "../TinyJson.h"

std::string LoadFileIntoString(const std::string& pFilename)
{
    std::ifstream jsonFile(pFilename);
    if( jsonFile.is_open() )
    {
        std::stringstream jsonStream;
        jsonStream << jsonFile.rdbuf();// Read the whole file in...

        return jsonStream.str();
    }

    throw std::runtime_error("Jons file not found " + pFilename);

    return "";
}

static bool UnitTestFile(const std::string& pFilename, bool pShowError = true)
{
    try
    {
        tinyjson::JsonProcessor json(LoadFileIntoString(pFilename));
        return true;
    }
    catch( const std::runtime_error& why)
    {
        if( pShowError )
        {
            std::cout << "Failed to read json file, " << why.what() << "\n";
        }
    }

    return false;
}

/**
 * @brief simple test or parsing strings.
 * 
 * @return true 
 * @return false 
 */
static bool TestBasicTypesWork()
{
    std::cout << "*****************************************************\n";
    std::cout << "Testing basic types work\n";

    const char* jsonString =
    R"(
        {
            "Hello World":"TheWorld",
            "empty string test":"",
            "A Null": null,
            "A True": true,
            "A False": false,
            "Int": 123456,
            "Float" : 3.14,
            "Exponent":-1.50139930144708198E18
        }
    )";

    tinyjson::JsonProcessor json(jsonString);
    const tinyjson::JsonValue& root = json.GetRoot();

    std::cout << root["Hello World"].mValue << '\n';
    std::cout << "Empty String -> \"" << root["empty string test"].mValue << "\"\n";
    std::cout << JsonValueTypeToString(root["A Null"].GetType()) << '\n';
    std::cout << JsonValueTypeToString(root["A True"].GetType()) << '\n';
    std::cout << JsonValueTypeToString(root["A False"].GetType()) << '\n';
    std::cout << root["Int"].GetInt() << '\n';
    std::cout << root["Float"].GetFloat() << '\n';
    std::cout << root["Exponent"].GetFloat() << '\n';


    return true;    
}

/**
 * @brief simple test or parsing strings.
 * 
 * @return true 
 * @return false 
 */
static bool TestThatIncorrectTypeRequestsWork()
{
    std::cout << "*****************************************************\n";
    std::cout << "Testing incorrect type requests work\n";

    const char* jsonString =
    R"(
        {
            "Hello World":"TheWorld",
            "empty string test":""
        }
    )";

    tinyjson::JsonProcessor json(jsonString);
    const tinyjson::JsonValue& root = json.GetRoot();

    std::cout << root["Hello World"].mValue << '\n';
    assert( root.HasValue("empty string test") );

    // Test throwing of an exception when key not found or type is wrong.
    try
    {
        std::cout << root["Hello World"].GetInt() << '\n';
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error trapped correctly, " << e.what() << '\n';
    }

    try
    {
        std::cout << root["MyCash"].GetInt() << '\n';
    }
    catch(const std::exception& e)
    {
        std::cerr << "Error trapped correctly, " << e.what() << '\n';
    }

    // Now do it again but test that the safe version of these functions do not throw an exception.
    std::cout << root.GetDouble("Hello World",55.101f) << '\n';
    std::cout << root.GetDouble("BigNumber") << '\n';

    return true;    
}


/**
 * @brief Simple object tree
 * 
 * @return true 
 * @return false 
 */
static bool SimpleObjectTreeTest()
{
    std::cout << "*****************************************************\n";
    std::cout << "Running test, simple object tree\n";
    
    const char* jsonString =
    R"(
        {
            "Level1":
            {
                "Level2":
                {
                    "Level3":"It worked!",
                    "Level3Number":12
                }
            }
        }
    )";
    tinyjson::JsonProcessor json(jsonString);
    const tinyjson::JsonValue& root = json.GetRoot();

    std::cout << root["Level1"]["Level2"]["Level3"].GetString() << '\n';
    std::cout << root["Level1"]["Level2"]["Level3Number"].GetInt() << '\n';

    return true;
}

static bool RunningUnitTestFiles()
{
    std::cout << "*****************************************************\n";
    std::cout << "Running test files, some should fail, some should pass.\n";

    // Test the fail cases first.
    for(int n = 1 ; n < 34 ; n++ )
    {
        if( UnitTestFile("tests/fail" + std::to_string(n) + ".json",false) )
        {
            std::runtime_error("A file that should have failed passed!");
        }
    }

    // Test the pass cases first.
    for(int n = 1 ; n < 3 ; n++ )
    {
        if( UnitTestFile("tests/fail" + std::to_string(n) + ".json",false) )
        {
            std::runtime_error("A file that should have passed failed!");
        }
    }

    std::cout << " All good!\n";
    return true;
}

/**
 * @brief Checks that having a : char in a string is ok.
 * 
 * @return true 
 * @return false 
 */
static bool SimpleControlCharacterInStringTest()
{
    std::cout << "*****************************************************\n";
    std::cout << "Control character test, ";
    
    const char* jsonString =
    R"(
        {
            "1":"Left:Right",
            "2":"Left\\Right"
        }
    )";

    tinyjson::JsonProcessor json(jsonString);
    const tinyjson::JsonValue& root = json.GetRoot();

    root.GetIsNull();

    std::cout << "Pass\n";

    return true;
}

/**
 * @brief Now test a big weather json file
 * 
 * @return true 
 * @return false 
 */
static bool BigValidWeatherFileTest()
{
    std::cout << "*****************************************************\n";
    std::cout << "Running test, big weather json\n";
    
    tinyjson::JsonProcessor json(LoadFileIntoString("weather.json"));
    const tinyjson::JsonValue& weatherData = json.GetRoot();

    assert( weatherData.HasValue("current") );
    assert( weatherData["current"].HasValue("weather") );
    assert( weatherData["current"]["weather"].GetArraySize() > 0 );
    assert( weatherData["current"]["weather"][0].HasValue("main") );
    std::cout << weatherData["current"]["weather"][0]["main"].GetString() << '\n';

    return true;
}

/**
 * @brief This loads a valid but horrendous json file that should work.
 * Does not check the values read, need to look at a good way to test against rapid json.
 * 
 * @param pFilename 
 */
static bool LargeComplexJsonFileTest()
{
    std::cout << "*****************************************************\n";
    std::cout << "Running test, big complex uber size json file\n";
    return UnitTestFile("sample.json");
}

int main(int argc, char *argv[])
{
    std::cout << "Unit test app for TinyJson build by appbuild.\n";

// Display the constants defined by app build. \n";
    std::cout << "Application Version " << APP_VERSION << '\n';
    std::cout << "Build date and time " << APP_BUILD_DATE_TIME << '\n';
    std::cout << "Build date " << APP_BUILD_DATE << '\n';
    std::cout << "Build time " << APP_BUILD_TIME << '\n';

    const std::vector<bool(*)()> tests = {
        TestBasicTypesWork,
        TestThatIncorrectTypeRequestsWork,
        SimpleObjectTreeTest,
        SimpleControlCharacterInStringTest,
        BigValidWeatherFileTest,
        RunningUnitTestFiles,
//        LargeComplexJsonFileTest, Work to do, currently fails :(
    };

    std::cout << "*****************************************************\n";

    for( auto& test : tests )
    {
        if( !test() )
        {
            std::cout << "Halting, a test failed!\n";
            return EXIT_FAILURE;
        }
    }

// And quit";
    std::cout << "All tests passed!\n";

    return EXIT_SUCCESS;
}
