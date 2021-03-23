
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

    std::throw_with_nested(std::runtime_error("Jons file not found " + pFilename));

    return "";
}

/**
 * @brief This will load a json file and then trap any error.
 * This is so that we can test for files that should fail without stopping the test suite.
 */
static bool UnitTestFile(const std::string& pFilename, bool pShowError = true)
{
    try
    {
        tinyjson::JsonProcessor json(LoadFileIntoString(pFilename));
        return true;
    }
    catch( const std::exception& why)
    {
        if( pShowError )
        {
            std::cout << "Failed to read json file, " << why.what() << "\n";
        }
    }

    return false;
}

/**
 * @brief test root types
 */
static bool TestRootTypes()
{
    std::cout << "Testing root types\n";

    {
        const char* jsonString = "{\"Hello World\":\"TheWorld\"}";
        tinyjson::JsonProcessor json(jsonString);
        const tinyjson::JsonValue& root = json.GetRoot();
        std::cout << root["Hello World"].mValue << '\n';
        if( root["Hello World"].mValue != "TheWorld" )
        {
            return false;// Whoops, expected value wrong. :/
        }
    }

    {
        const char* jsonString = "\"String Root\"";

        tinyjson::JsonProcessor json(jsonString);
        const tinyjson::JsonValue& root = json.GetRoot();
        std::cout << root.mValue << '\n';
        if( root.mValue != "String Root" )
        {
            return false;// Whoops, expected value wrong. :/
        }
    }

    {
        const char* jsonString = "122334";

        tinyjson::JsonProcessor json(jsonString);
        const tinyjson::JsonValue& root = json.GetRoot();
        std::cout << root.mValue << '\n';
        if( root.mValue != "122334" )
        {
            return false;// Whoops, expected value wrong. :/
        }
    }

    {
        const char* jsonString = "null";

        tinyjson::JsonProcessor json(jsonString);
        const tinyjson::JsonValue& root = json.GetRoot();
        std::cout << root.GetIsNull() << '\n';
        if( root.GetIsNull() == false )
        {
            return false;// Whoops, expected value wrong. :/
        }
    }

    {
        const char* jsonString = "true";

        tinyjson::JsonProcessor json(jsonString);
        const tinyjson::JsonValue& root = json.GetRoot();
        std::cout << root.GetBoolean() << '\n';
        if( root.GetBoolean() != true )
        {
            return false;// Whoops, expected value wrong. :/
        }
    }

    {
        const char* jsonString = "false";

        tinyjson::JsonProcessor json(jsonString);
        const tinyjson::JsonValue& root = json.GetRoot();
        std::cout << root.GetBoolean() << '\n';
        if( root.GetBoolean() != false )
        {
            return false;// Whoops, expected value wrong. :/
        }
    }

    return true;    
}

/**
 * @brief simple test or parsing strings.
 * 
 * @return true 
 * @return false 
 */
static bool TestBasicTypesWork()
{
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
    if( root["Hello World"].GetString() != "TheWorld" )
    {
        return false;// Whoops, expected value wrong. :/
    }

    std::cout << "Empty String -> \"" << root["empty string test"].mValue << "\"\n";
    if( root["empty string test"].GetString() != "" )
    {
        return false;// Whoops, expected value wrong. :/
    }

    std::cout << JsonValueTypeToString(root["A Null"].GetType()) << '\n';
    if( root["A Null"].GetIsNull() == false )
    {
        return false;// Whoops, expected value wrong. :/
    }

    std::cout << JsonValueTypeToString(root["A True"].GetType()) << '\n';
    if( root["A True"].GetBoolean() == false )
    {
        return false;// Whoops, expected value wrong. :/
    }

    std::cout << JsonValueTypeToString(root["A False"].GetType()) << '\n';
    if( root["A False"].GetBoolean() == true )
    {
        return false;// Whoops, expected value wrong. :/
    }

    std::cout << root["Int"].GetInt() << '\n';
    if( root["Int"].GetInt() != 123456 )
    {
        return false;// Whoops, expected value wrong. :/
    }

    std::cout << root["Float"].GetFloat() << '\n';
    if( root["Float"].GetFloat() != 3.14f )
    {
        return false;// Whoops, expected value wrong. :/
    }

    std::cout << root["Exponent"].GetFloat() << '\n';
    if( root["Exponent"].GetFloat() != -1.50139930144708198E18f )
    {
        return false;// Whoops, expected value wrong. :/
    }
    return true;    
}

/**
 * @brief simple test or parsing strings.
 */
static bool TestThatIncorrectTypeRequestsWork()
{
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
            },
            "array":
            [
                "test\\",
                -3.3325685522591933E18,
                {
                    "Null In An Array in an array":
                    [
                        null,
                        123456,
                        null,
                        null
                    ]
                },
                {
                    "!t,灝Y 1䗉罵?c饃호䉂Cᐭ쒘z(즽sZG㬣sഖE4뢜㓕䏞丮Qp簍6EZឪ겛fx'ꩱQ0罣i{k锩*㤴㯞r迎jTⲤ渔m炅肳":
                    [
                        null
                    ]
                }
            ]
        }
    )";
    tinyjson::JsonProcessor json(jsonString);
    const tinyjson::JsonValue& root = json.GetRoot();

    std::cout << root["Level1"]["Level2"]["Level3"].GetString() << '\n';
    std::cout << root["Level1"]["Level2"]["Level3Number"].GetInt() << '\n';

    return true;
}

/**
 * @brief Tests against files that must pass and ones that must fail.
 */
static bool RunningUnitTestFiles()
{
    std::cout << "Running test files, some should fail, some should pass.\n";

    // Test the fail cases first.
    for(int n = 1 ; n < 34 ; n++ )
    {
        const std::string fname = "tests/fail" + std::to_string(n) + ".json";
        if( UnitTestFile(fname,false) )
        {
            std::runtime_error("A file that should have failed passed!");
        }
    }

    // Test the pass cases first.
    for(int n = 1 ; n < 3 ; n++ )
    {
        const std::string fname = "tests/pass" + std::to_string(n) + ".json";
        if( UnitTestFile(fname,false) == false )
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
 */
static bool LargeComplexJsonFileTest()
{
    std::cout << "Running test, big complex uber size json file\n";
    return UnitTestFile("sample.json");
}

/**
 * @brief Times the load of a file that has been used by others for bench marking. The file is over 2MB in size.
 */
static bool TimedTestedAgainstVeryLargeCanadaFile()
{
    std::cout << "Running test and bench mark on very large file\n";
    std::cout << "Valgrind slows this down from 60ms to 2100ms so check which you'll looking at before passing judgment :)\n";
#ifdef RELEASE_BUILD
    std::cout << "Release build, optimisation on\n";
#else
    std::cout << "Debug build, optimisation off\n";
#endif

	auto frameStart = std::chrono::system_clock::now();

    tinyjson::JsonProcessor json(LoadFileIntoString("canada.json"));
    std::cout << json.GetRoot()["type"].GetString() << "\n";

    auto frameEnd = std::chrono::system_clock::now();
    std::chrono::duration<float,std::milli> time = (frameEnd - frameStart);

    std::cout << "Loading canada.json took: " << time.count() << " ms\n";

    return true;
}

/**
 * @brief Checks that if chosen, duplicate keys will be found.
 */
static bool TestForDuplicateKeyDiscovery()
{
    const char* Duplicate1 =
    R"(
        {
            "Key1":"Fred",
            "Key2":"Tom",
            "Key1":"Tim",
            "Key4":"Sam"
        }
    )";

    const char* Good1 =
    R"(
        {
            "Key1":"Fred",
            "Key2":"Tom",
            "Key3":"Tim",
            "Key4":"Sam"
        }
    )";

    const char* Duplicate2 =
    R"(
        {
            "Key1":
            {
                "Name":"Fred",
                "Age":50
            },
            "Key1":
            {
                "Name":"Fred",
                "Age":50
            }
        }
    )";

    const char* Duplicate3 = // Only Key7 in object Key1 is in error.
    R"(
        {
            "array2":[
                "Key1",
                {
                    "Key7":12,
                    "Key7":144
                },
                {
                    "Key2":12
                },
                {
                    "Key2":12
                }
            ]
        }
    )";

    const char* Good2 =
    R"(
        {
            "Key1":
            {
                "Name":"Fred",
                "Age":50
            },
            "Key2":
            {
                "Name":"Fred",
                "Age":50
            }
        }
    )";

    const char* Good3 = // Duplicate strings in an array are ok.
    R"(
        {
            "Array1":
            [
                "Key1",
                "Key1"
            ]
        }
    )";

    const char* Good4 = // no errors despite some duplicates in the array.
    R"(
        {
            "array2":[
                "Key1",
                {
                    "Key7":12,
                    "Key9":144
                },
                {
                    "Key2":12
                },
                {
                    "Key2":12
                }
            ]
        }
    )";
// First check the failures. pFailOnDuplicateKeys set to true.
    const std::vector<const char*>duplicates =
    {
        Duplicate1,
        Duplicate2,
        Duplicate3
    };

    for( auto jsonString : duplicates )
    {
        bool ok = false;
        try
        {
            tinyjson::JsonProcessor json(jsonString,true);
            const tinyjson::JsonValue& root = json.GetRoot();
            std::cout << root.mObject.begin()->first << '\n';
            ok = true;
        }
        catch(const std::exception& e)
        {
            std::cerr << "Duplicate error trapped correctly, " << e.what() << '\n';
        }

        if( ok )
        {
            throw std::runtime_error("A duplicate that should have failed passed!");
        }
    }

// First check the passes with pFailOnDuplicateKeys set to true. Should all pass.
    const std::vector<const char*>nonDuplicates =
    {
        Good1,
        Good2,
        Good3,
        Good4
    };

    for( auto jsonString : nonDuplicates )
    {
        tinyjson::JsonProcessor json(jsonString,true);
        const tinyjson::JsonValue& root = json.GetRoot();
        std::cout << root.mObject.begin()->first << '\n';
    }

// Now check the good and duplicates pass with the option pFailOnDuplicateKeys set to false.
    const std::vector<const char*>good =
    {
        Good1,
        Good2,
        Good3,
        Good4,
        Duplicate1,
        Duplicate2,
        Duplicate3
    };

    for( auto jsonString : duplicates )
    {
        tinyjson::JsonProcessor json(jsonString,false);
        const tinyjson::JsonValue& root = json.GetRoot();
        std::cout << root.mObject.begin()->first << '\n';
    }

    return true;
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
        TestRootTypes,
        TestBasicTypesWork,
        TestThatIncorrectTypeRequestsWork,
        SimpleObjectTreeTest,
        SimpleControlCharacterInStringTest,
        BigValidWeatherFileTest,
        RunningUnitTestFiles,
        LargeComplexJsonFileTest,
        TimedTestedAgainstVeryLargeCanadaFile,
        TestForDuplicateKeyDiscovery
    };

    for( auto& test : tests )
    {
        std::cout << "*****************************************************\n";

        if( !test() )
        {
            std::cout << "Halting, a test failed!\n";
            return EXIT_FAILURE;
        }
    }
    std::cout << "*****************************************************\n";

// And quit";
    std::cout << "All tests passed!\n";

    return EXIT_SUCCESS;
}
