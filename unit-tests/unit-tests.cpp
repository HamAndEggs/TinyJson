
#include <iostream>
#include <chrono>
#include <assert.h>

#include "../TinyJson.h"

extern const char* weatherBigTestJson;

/**
 * @brief simple test or parsing strings.
 * 
 * @return true 
 * @return false 
 */
static bool UnitTest1()
{
    std::cout << "Running test, simple json string\n";

    const char* jsonString = R"({"Hello World":"TheWorld","empty string test":""})";
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
        std::cerr << e.what() << '\n';
    }

    try
    {
        std::cout << root["MyCash"].GetInt() << '\n';
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
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
static bool UnitTest2()
{
    std::cout << "Running test, simple object tree\n";
    
    const char* jsonString = R"({"Level1":{"Level2":{"Level3":"It worked!","Level3Number":12}}})";
    tinyjson::JsonProcessor json(jsonString);
    const tinyjson::JsonValue& root = json.GetRoot();

    std::cout << root["Level1"]["Level2"]["Level3"].GetString() << '\n';
    std::cout << root["Level1"]["Level2"]["Level3Number"].GetInt() << '\n';

    return true;
}

/**
 * @brief Now test a big weather json file
 * 
 * @return true 
 * @return false 
 */
static bool UnitTest3()
{
    std::cout << "Running test, big complex json\n";
    tinyjson::JsonProcessor json(weatherBigTestJson);
    const tinyjson::JsonValue& weatherData = json.GetRoot();

    assert( weatherData.HasValue("current") );
    assert( weatherData["current"].HasValue("weather") );
    assert( weatherData["current"]["weather"].GetArraySize() > 0 );
    assert( weatherData["current"]["weather"][0].HasValue("main") );
    std::cout << weatherData["current"]["weather"][0]["main"].GetString() << '\n';

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

    UnitTest1();
    UnitTest2();
    UnitTest3();

// And quit";
    std::cout << "All tests passed!\n";
    return EXIT_SUCCESS;
}

const char* weatherBigTestJson = 
R"(
{
    "lat": 50.7282,
    "lon": -1.1524,
    "timezone": "Europe/London",
    "timezone_offset": 0,
    "current": {
        "dt": 1611854723,
        "sunrise": 1611819957,
        "sunset": 1611852539,
        "temp": 283.15,
        "feels_like": 277.59,
        "pressure": 1002,
        "humidity": 95,
        "dew_point": 282.39,
        "uvi": 0,
        "clouds": 100,
        "visibility": 10000,
        "wind_speed": 7.72,
        "wind_deg": 240,
        "weather": [
            {
                "id": 804,
                "main": "Clouds",
                "description": "overcast clouds",
                "icon": "04n"
            }
        ]
    },
    "minutely": [
        {
            "dt": 1611854760,
            "precipitation": 0
        },
        {
            "dt": 1611854820,
            "precipitation": 0
        },
        {
            "dt": 1611854880,
            "precipitation": 0
        },
        {
            "dt": 1611854940,
            "precipitation": 0
        },
        {
            "dt": 1611855000,
            "precipitation": 0
        },
        {
            "dt": 1611855060,
            "precipitation": 0
        },
        {
            "dt": 1611855120,
            "precipitation": 0
        },
        {
            "dt": 1611855180,
            "precipitation": 0
        },
        {
            "dt": 1611855240,
            "precipitation": 0
        },
        {
            "dt": 1611855300,
            "precipitation": 0
        },
        {
            "dt": 1611855360,
            "precipitation": 0
        },
        {
            "dt": 1611855420,
            "precipitation": 0
        },
        {
            "dt": 1611855480,
            "precipitation": 0
        },
        {
            "dt": 1611855540,
            "precipitation": 0
        },
        {
            "dt": 1611855600,
            "precipitation": 0
        },
        {
            "dt": 1611855660,
            "precipitation": 0
        },
        {
            "dt": 1611855720,
            "precipitation": 0
        },
        {
            "dt": 1611855780,
            "precipitation": 0
        },
        {
            "dt": 1611855840,
            "precipitation": 0
        },
        {
            "dt": 1611855900,
            "precipitation": 0
        },
        {
            "dt": 1611855960,
            "precipitation": 0
        },
        {
            "dt": 1611856020,
            "precipitation": 0
        },
        {
            "dt": 1611856080,
            "precipitation": 0
        },
        {
            "dt": 1611856140,
            "precipitation": 0
        },
        {
            "dt": 1611856200,
            "precipitation": 0
        },
        {
            "dt": 1611856260,
            "precipitation": 0
        },
        {
            "dt": 1611856320,
            "precipitation": 0
        },
        {
            "dt": 1611856380,
            "precipitation": 0
        },
        {
            "dt": 1611856440,
            "precipitation": 0
        },
        {
            "dt": 1611856500,
            "precipitation": 0
        },
        {
            "dt": 1611856560,
            "precipitation": 0
        },
        {
            "dt": 1611856620,
            "precipitation": 0
        },
        {
            "dt": 1611856680,
            "precipitation": 0
        },
        {
            "dt": 1611856740,
            "precipitation": 0
        },
        {
            "dt": 1611856800,
            "precipitation": 0
        },
        {
            "dt": 1611856860,
            "precipitation": 0
        },
        {
            "dt": 1611856920,
            "precipitation": 0
        },
        {
            "dt": 1611856980,
            "precipitation": 0
        },
        {
            "dt": 1611857040,
            "precipitation": 0
        },
        {
            "dt": 1611857100,
            "precipitation": 0
        },
        {
            "dt": 1611857160,
            "precipitation": 0
        },
        {
            "dt": 1611857220,
            "precipitation": 0
        },
        {
            "dt": 1611857280,
            "precipitation": 0
        },
        {
            "dt": 1611857340,
            "precipitation": 0
        },
        {
            "dt": 1611857400,
            "precipitation": 0
        },
        {
            "dt": 1611857460,
            "precipitation": 0
        },
        {
            "dt": 1611857520,
            "precipitation": 0
        },
        {
            "dt": 1611857580,
            "precipitation": 0
        },
        {
            "dt": 1611857640,
            "precipitation": 0
        },
        {
            "dt": 1611857700,
            "precipitation": 0
        },
        {
            "dt": 1611857760,
            "precipitation": 0
        },
        {
            "dt": 1611857820,
            "precipitation": 0
        },
        {
            "dt": 1611857880,
            "precipitation": 0
        },
        {
            "dt": 1611857940,
            "precipitation": 0
        },
        {
            "dt": 1611858000,
            "precipitation": 0
        },
        {
            "dt": 1611858060,
            "precipitation": 0
        },
        {
            "dt": 1611858120,
            "precipitation": 0
        },
        {
            "dt": 1611858180,
            "precipitation": 0
        },
        {
            "dt": 1611858240,
            "precipitation": 0
        },
        {
            "dt": 1611858300,
            "precipitation": 0
        },
        {
            "dt": 1611858360,
            "precipitation": 0
        }
    ],
    "hourly": [
        {
            "dt": 1611853200,
            "temp": 283.15,
            "feels_like": 277.59,
            "pressure": 1002,
            "humidity": 95,
            "dew_point": 282.39,
            "uvi": 0,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 7.72,
            "wind_deg": 240,
            "weather": [
                {
                    "id": 804,
                    "main": "Clouds",
                    "description": "overcast clouds",
                    "icon": "04n"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1611856800,
            "temp": 283.16,
            "feels_like": 277.58,
            "pressure": 1002,
            "humidity": 96,
            "dew_point": 282.55,
            "uvi": 0,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 7.81,
            "wind_deg": 234,
            "weather": [
                {
                    "id": 804,
                    "main": "Clouds",
                    "description": "overcast clouds",
                    "icon": "04n"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1611860400,
            "temp": 283.31,
            "feels_like": 277.54,
            "pressure": 1002,
            "humidity": 94,
            "dew_point": 282.39,
            "uvi": 0,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 8.02,
            "wind_deg": 235,
            "weather": [
                {
                    "id": 804,
                    "main": "Clouds",
                    "description": "overcast clouds",
                    "icon": "04n"
                }
            ],
            "pop": 0.7
        },
        {
            "dt": 1611864000,
            "temp": 283.37,
            "feels_like": 276.76,
            "pressure": 1001,
            "humidity": 94,
            "dew_point": 282.45,
            "uvi": 0,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 9.24,
            "wind_deg": 231,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10n"
                }
            ],
            "pop": 0.93,
            "rain": {
                "1h": 0.11
            }
        },
        {
            "dt": 1611867600,
            "temp": 283.34,
            "feels_like": 277.05,
            "pressure": 1000,
            "humidity": 96,
            "dew_point": 282.73,
            "uvi": 0,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 8.89,
            "wind_deg": 232,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10n"
                }
            ],
            "pop": 0.98,
            "rain": {
                "1h": 0.42
            }
        },
        {
            "dt": 1611871200,
            "temp": 283.46,
            "feels_like": 276.62,
            "pressure": 999,
            "humidity": 94,
            "dew_point": 282.63,
            "uvi": 0,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 9.6,
            "wind_deg": 248,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10n"
                }
            ],
            "pop": 1,
            "rain": {
                "1h": 0.86
            }
        },
        {
            "dt": 1611874800,
            "temp": 283.38,
            "feels_like": 278.02,
            "pressure": 999,
            "humidity": 91,
            "dew_point": 282.09,
            "uvi": 0,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 7.28,
            "wind_deg": 252,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10n"
                }
            ],
            "pop": 1,
            "rain": {
                "1h": 0.44
            }
        },
        {
            "dt": 1611878400,
            "temp": 283.21,
            "feels_like": 276.91,
            "pressure": 999,
            "humidity": 90,
            "dew_point": 281.66,
            "uvi": 0,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 8.51,
            "wind_deg": 256,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10n"
                }
            ],
            "pop": 1,
            "rain": {
                "1h": 0.45
            }
        },
        {
            "dt": 1611882000,
            "temp": 282.9,
            "feels_like": 276.32,
            "pressure": 999,
            "humidity": 88,
            "dew_point": 281.03,
            "uvi": 0,
            "clouds": 0,
            "visibility": 10000,
            "wind_speed": 8.69,
            "wind_deg": 266,
            "weather": [
                {
                    "id": 800,
                    "main": "Clear",
                    "description": "clear sky",
                    "icon": "01n"
                }
            ],
            "pop": 0.12
        },
        {
            "dt": 1611885600,
            "temp": 282.36,
            "feels_like": 275.71,
            "pressure": 999,
            "humidity": 87,
            "dew_point": 280.46,
            "uvi": 0,
            "clouds": 0,
            "visibility": 10000,
            "wind_speed": 8.55,
            "wind_deg": 263,
            "weather": [
                {
                    "id": 800,
                    "main": "Clear",
                    "description": "clear sky",
                    "icon": "01n"
                }
            ],
            "pop": 0.03
        },
        {
            "dt": 1611889200,
            "temp": 282.18,
            "feels_like": 275.54,
            "pressure": 999,
            "humidity": 89,
            "dew_point": 280.47,
            "uvi": 0,
            "clouds": 5,
            "visibility": 10000,
            "wind_speed": 8.59,
            "wind_deg": 255,
            "weather": [
                {
                    "id": 800,
                    "main": "Clear",
                    "description": "clear sky",
                    "icon": "01n"
                }
            ],
            "pop": 0.02
        },
        {
            "dt": 1611892800,
            "temp": 282.32,
            "feels_like": 275.21,
            "pressure": 998,
            "humidity": 87,
            "dew_point": 280.42,
            "uvi": 0,
            "clouds": 27,
            "visibility": 10000,
            "wind_speed": 9.2,
            "wind_deg": 250,
            "weather": [
                {
                    "id": 802,
                    "main": "Clouds",
                    "description": "scattered clouds",
                    "icon": "03n"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1611896400,
            "temp": 282.51,
            "feels_like": 274.5,
            "pressure": 997,
            "humidity": 85,
            "dew_point": 280.13,
            "uvi": 0,
            "clouds": 42,
            "visibility": 10000,
            "wind_speed": 10.43,
            "wind_deg": 252,
            "weather": [
                {
                    "id": 802,
                    "main": "Clouds",
                    "description": "scattered clouds",
                    "icon": "03n"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1611900000,
            "temp": 282.22,
            "feels_like": 273.26,
            "pressure": 997,
            "humidity": 82,
            "dew_point": 279.35,
            "uvi": 0,
            "clouds": 52,
            "visibility": 10000,
            "wind_speed": 11.54,
            "wind_deg": 249,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10n"
                }
            ],
            "pop": 0.36,
            "rain": {
                "1h": 0.17
            }
        },
        {
            "dt": 1611903600,
            "temp": 282.38,
            "feels_like": 272.19,
            "pressure": 997,
            "humidity": 81,
            "dew_point": 279.35,
            "uvi": 0,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 13.29,
            "wind_deg": 256,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10n"
                }
            ],
            "pop": 0.48,
            "rain": {
                "1h": 0.1
            }
        },
        {
            "dt": 1611907200,
            "temp": 282.18,
            "feels_like": 272.04,
            "pressure": 997,
            "humidity": 79,
            "dew_point": 278.87,
            "uvi": 0,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 13.05,
            "wind_deg": 257,
            "weather": [
                {
                    "id": 804,
                    "main": "Clouds",
                    "description": "overcast clouds",
                    "icon": "04d"
                }
            ],
            "pop": 0.48
        },
        {
            "dt": 1611910800,
            "temp": 281.85,
            "feels_like": 271.48,
            "pressure": 998,
            "humidity": 83,
            "dew_point": 279.22,
            "uvi": 0.21,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 13.5,
            "wind_deg": 265,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10d"
                }
            ],
            "pop": 0.52,
            "rain": {
                "1h": 0.2
            }
        },
        {
            "dt": 1611914400,
            "temp": 282.16,
            "feels_like": 272.75,
            "pressure": 999,
            "humidity": 82,
            "dew_point": 279.29,
            "uvi": 0.2,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 12.17,
            "wind_deg": 275,
            "weather": [
                {
                    "id": 804,
                    "main": "Clouds",
                    "description": "overcast clouds",
                    "icon": "04d"
                }
            ],
            "pop": 0.48
        },
        {
            "dt": 1611918000,
            "temp": 282.57,
            "feels_like": 273.3,
            "pressure": 1000,
            "humidity": 80,
            "dew_point": 279.47,
            "uvi": 0.3,
            "clouds": 89,
            "visibility": 10000,
            "wind_speed": 11.98,
            "wind_deg": 283,
            "weather": [
                {
                    "id": 804,
                    "main": "Clouds",
                    "description": "overcast clouds",
                    "icon": "04d"
                }
            ],
            "pop": 0.58
        },
        {
            "dt": 1611921600,
            "temp": 282.4,
            "feels_like": 273.8,
            "pressure": 1001,
            "humidity": 82,
            "dew_point": 279.53,
            "uvi": 0.34,
            "clouds": 85,
            "visibility": 10000,
            "wind_speed": 11.08,
            "wind_deg": 279,
            "weather": [
                {
                    "id": 804,
                    "main": "Clouds",
                    "description": "overcast clouds",
                    "icon": "04d"
                }
            ],
            "pop": 0.48
        },
        {
            "dt": 1611925200,
            "temp": 282.58,
            "feels_like": 274.56,
            "pressure": 1002,
            "humidity": 80,
            "dew_point": 279.49,
            "uvi": 0.79,
            "clouds": 99,
            "visibility": 10000,
            "wind_speed": 10.2,
            "wind_deg": 280,
            "weather": [
                {
                    "id": 804,
                    "main": "Clouds",
                    "description": "overcast clouds",
                    "icon": "04d"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1611928800,
            "temp": 282.8,
            "feels_like": 274.95,
            "pressure": 1002,
            "humidity": 80,
            "dew_point": 279.62,
            "uvi": 0.56,
            "clouds": 76,
            "visibility": 10000,
            "wind_speed": 10.01,
            "wind_deg": 281,
            "weather": [
                {
                    "id": 803,
                    "main": "Clouds",
                    "description": "broken clouds",
                    "icon": "04d"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1611932400,
            "temp": 282.65,
            "feels_like": 275.39,
            "pressure": 1003,
            "humidity": 81,
            "dew_point": 279.57,
            "uvi": 0.28,
            "clouds": 81,
            "visibility": 10000,
            "wind_speed": 9.19,
            "wind_deg": 279,
            "weather": [
                {
                    "id": 803,
                    "main": "Clouds",
                    "description": "broken clouds",
                    "icon": "04d"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1611936000,
            "temp": 282.35,
            "feels_like": 275.64,
            "pressure": 1003,
            "humidity": 82,
            "dew_point": 279.47,
            "uvi": 0,
            "clouds": 85,
            "visibility": 10000,
            "wind_speed": 8.36,
            "wind_deg": 276,
            "weather": [
                {
                    "id": 804,
                    "main": "Clouds",
                    "description": "overcast clouds",
                    "icon": "04d"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1611939600,
            "temp": 282.19,
            "feels_like": 276.15,
            "pressure": 1003,
            "humidity": 82,
            "dew_point": 279.45,
            "uvi": 0,
            "clouds": 88,
            "visibility": 10000,
            "wind_speed": 7.36,
            "wind_deg": 279,
            "weather": [
                {
                    "id": 804,
                    "main": "Clouds",
                    "description": "overcast clouds",
                    "icon": "04n"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1611943200,
            "temp": 282.02,
            "feels_like": 277.04,
            "pressure": 1003,
            "humidity": 83,
            "dew_point": 279.38,
            "uvi": 0,
            "clouds": 91,
            "visibility": 10000,
            "wind_speed": 5.85,
            "wind_deg": 273,
            "weather": [
                {
                    "id": 804,
                    "main": "Clouds",
                    "description": "overcast clouds",
                    "icon": "04n"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1611946800,
            "temp": 281.39,
            "feels_like": 276.85,
            "pressure": 1004,
            "humidity": 86,
            "dew_point": 279.21,
            "uvi": 0,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 5.19,
            "wind_deg": 257,
            "weather": [
                {
                    "id": 804,
                    "main": "Clouds",
                    "description": "overcast clouds",
                    "icon": "04n"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1611950400,
            "temp": 281.19,
            "feels_like": 276.77,
            "pressure": 1003,
            "humidity": 88,
            "dew_point": 279.37,
            "uvi": 0,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 5.06,
            "wind_deg": 249,
            "weather": [
                {
                    "id": 804,
                    "main": "Clouds",
                    "description": "overcast clouds",
                    "icon": "04n"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1611954000,
            "temp": 281.23,
            "feels_like": 277.99,
            "pressure": 1003,
            "humidity": 89,
            "dew_point": 279.68,
            "uvi": 0,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 3.43,
            "wind_deg": 244,
            "weather": [
                {
                    "id": 804,
                    "main": "Clouds",
                    "description": "overcast clouds",
                    "icon": "04n"
                }
            ],
            "pop": 0
        },
        {
            "dt": 1611957600,
            "temp": 281.41,
            "feels_like": 278.02,
            "pressure": 1002,
            "humidity": 90,
            "dew_point": 279.93,
            "uvi": 0,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 3.76,
            "wind_deg": 217,
            "weather": [
                {
                    "id": 804,
                    "main": "Clouds",
                    "description": "overcast clouds",
                    "icon": "04n"
                }
            ],
            "pop": 0.1
        },
        {
            "dt": 1611961200,
            "temp": 281.35,
            "feels_like": 278.29,
            "pressure": 1001,
            "humidity": 91,
            "dew_point": 280.13,
            "uvi": 0,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 3.31,
            "wind_deg": 195,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10n"
                }
            ],
            "pop": 0.47,
            "rain": {
                "1h": 0.38
            }
        },
        {
            "dt": 1611964800,
            "temp": 281.13,
            "feels_like": 278.19,
            "pressure": 999,
            "humidity": 93,
            "dew_point": 280.07,
            "uvi": 0,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 3.18,
            "wind_deg": 123,
            "weather": [
                {
                    "id": 501,
                    "main": "Rain",
                    "description": "moderate rain",
                    "icon": "10n"
                }
            ],
            "pop": 0.8,
            "rain": {
                "1h": 1.11
            }
        },
        {
            "dt": 1611968400,
            "temp": 280.88,
            "feels_like": 275.59,
            "pressure": 997,
            "humidity": 92,
            "dew_point": 279.75,
            "uvi": 0,
            "clouds": 100,
            "visibility": 7266,
            "wind_speed": 6.4,
            "wind_deg": 101,
            "weather": [
                {
                    "id": 501,
                    "main": "Rain",
                    "description": "moderate rain",
                    "icon": "10n"
                }
            ],
            "pop": 1,
            "rain": {
                "1h": 1.29
            }
        },
        {
            "dt": 1611972000,
            "temp": 280.67,
            "feels_like": 274.15,
            "pressure": 996,
            "humidity": 93,
            "dew_point": 279.65,
            "uvi": 0,
            "clouds": 100,
            "visibility": 4978,
            "wind_speed": 8.14,
            "wind_deg": 108,
            "weather": [
                {
                    "id": 501,
                    "main": "Rain",
                    "description": "moderate rain",
                    "icon": "10n"
                }
            ],
            "pop": 1,
            "rain": {
                "1h": 2.44
            }
        },
        {
            "dt": 1611975600,
            "temp": 280.75,
            "feels_like": 273.78,
            "pressure": 994,
            "humidity": 95,
            "dew_point": 280.02,
            "uvi": 0,
            "clouds": 100,
            "visibility": 4095,
            "wind_speed": 8.91,
            "wind_deg": 112,
            "weather": [
                {
                    "id": 501,
                    "main": "Rain",
                    "description": "moderate rain",
                    "icon": "10n"
                }
            ],
            "pop": 1,
            "rain": {
                "1h": 3.49
            }
        },
        {
            "dt": 1611979200,
            "temp": 281.17,
            "feels_like": 274.86,
            "pressure": 993,
            "humidity": 96,
            "dew_point": 280.69,
            "uvi": 0,
            "clouds": 100,
            "visibility": 4752,
            "wind_speed": 8.16,
            "wind_deg": 115,
            "weather": [
                {
                    "id": 501,
                    "main": "Rain",
                    "description": "moderate rain",
                    "icon": "10n"
                }
            ],
            "pop": 1,
            "rain": {
                "1h": 2.81
            }
        },
        {
            "dt": 1611982800,
            "temp": 281.54,
            "feels_like": 277.05,
            "pressure": 992,
            "humidity": 97,
            "dew_point": 281.12,
            "uvi": 0,
            "clouds": 100,
            "visibility": 3367,
            "wind_speed": 5.73,
            "wind_deg": 121,
            "weather": [
                {
                    "id": 501,
                    "main": "Rain",
                    "description": "moderate rain",
                    "icon": "10n"
                }
            ],
            "pop": 1,
            "rain": {
                "1h": 1.97
            }
        },
        {
            "dt": 1611986400,
            "temp": 281.78,
            "feels_like": 279.96,
            "pressure": 992,
            "humidity": 96,
            "dew_point": 281.23,
            "uvi": 0,
            "clouds": 100,
            "visibility": 6735,
            "wind_speed": 1.94,
            "wind_deg": 129,
            "weather": [
                {
                    "id": 501,
                    "main": "Rain",
                    "description": "moderate rain",
                    "icon": "10n"
                }
            ],
            "pop": 1,
            "rain": {
                "1h": 1.1
            }
        },
        {
            "dt": 1611990000,
            "temp": 281.36,
            "feels_like": 278.83,
            "pressure": 992,
            "humidity": 95,
            "dew_point": 280.65,
            "uvi": 0,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 2.77,
            "wind_deg": 108,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10n"
                }
            ],
            "pop": 0.64,
            "rain": {
                "1h": 0.31
            }
        },
        {
            "dt": 1611993600,
            "temp": 281.02,
            "feels_like": 277.3,
            "pressure": 992,
            "humidity": 94,
            "dew_point": 280.14,
            "uvi": 0,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 4.31,
            "wind_deg": 97,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10d"
                }
            ],
            "pop": 0.81,
            "rain": {
                "1h": 0.57
            }
        },
        {
            "dt": 1611997200,
            "temp": 281.18,
            "feels_like": 276.72,
            "pressure": 992,
            "humidity": 94,
            "dew_point": 280.38,
            "uvi": 0.23,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 5.41,
            "wind_deg": 102,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10d"
                }
            ],
            "pop": 0.94,
            "rain": {
                "1h": 0.54
            }
        },
        {
            "dt": 1612000800,
            "temp": 281.48,
            "feels_like": 277.8,
            "pressure": 992,
            "humidity": 93,
            "dew_point": 280.52,
            "uvi": 0.38,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 4.34,
            "wind_deg": 113,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10d"
                }
            ],
            "pop": 0.98,
            "rain": {
                "1h": 0.37
            }
        },
        {
            "dt": 1612004400,
            "temp": 281.49,
            "feels_like": 277.19,
            "pressure": 993,
            "humidity": 92,
            "dew_point": 280.35,
            "uvi": 0.57,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 5.18,
            "wind_deg": 111,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10d"
                }
            ],
            "pop": 0.99,
            "rain": {
                "1h": 0.33
            }
        },
        {
            "dt": 1612008000,
            "temp": 281.49,
            "feels_like": 277.73,
            "pressure": 992,
            "humidity": 92,
            "dew_point": 280.36,
            "uvi": 0.67,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 4.41,
            "wind_deg": 126,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10d"
                }
            ],
            "pop": 0.99,
            "rain": {
                "1h": 0.98
            }
        },
        {
            "dt": 1612011600,
            "temp": 281.61,
            "feels_like": 279.12,
            "pressure": 992,
            "humidity": 92,
            "dew_point": 280.4,
            "uvi": 0.4,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 2.63,
            "wind_deg": 171,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10d"
                }
            ],
            "pop": 0.99,
            "rain": {
                "1h": 0.79
            }
        },
        {
            "dt": 1612015200,
            "temp": 281.42,
            "feels_like": 278.27,
            "pressure": 992,
            "humidity": 91,
            "dew_point": 280.12,
            "uvi": 0.28,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 3.47,
            "wind_deg": 116,
            "weather": [
                {
                    "id": 501,
                    "main": "Rain",
                    "description": "moderate rain",
                    "icon": "10d"
                }
            ],
            "pop": 0.98,
            "rain": {
                "1h": 1.13
            }
        },
        {
            "dt": 1612018800,
            "temp": 280.49,
            "feels_like": 274.98,
            "pressure": 992,
            "humidity": 92,
            "dew_point": 279.31,
            "uvi": 0.14,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 6.6,
            "wind_deg": 92,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10d"
                }
            ],
            "pop": 0.98,
            "rain": {
                "1h": 0.95
            }
        },
        {
            "dt": 1612022400,
            "temp": 279.72,
            "feels_like": 272.25,
            "pressure": 992,
            "humidity": 92,
            "dew_point": 278.57,
            "uvi": 0,
            "clouds": 100,
            "visibility": 10000,
            "wind_speed": 9.17,
            "wind_deg": 82,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10d"
                }
            ],
            "pop": 0.98,
            "rain": {
                "1h": 0.44
            }
        }
    ],
    "daily": [
        {
            "dt": 1611835200,
            "sunrise": 1611819957,
            "sunset": 1611852539,
            "temp": {
                "day": 284.4,
                "min": 283.12,
                "max": 284.48,
                "night": 283.38,
                "eve": 283.16,
                "morn": 284.06
            },
            "feels_like": {
                "day": 278.11,
                "night": 278.02,
                "eve": 277.58,
                "morn": 276.62
            },
            "pressure": 1005,
            "humidity": 86,
            "dew_point": 282.24,
            "wind_speed": 8.68,
            "wind_deg": 258,
            "weather": [
                {
                    "id": 502,
                    "main": "Rain",
                    "description": "heavy intensity rain",
                    "icon": "10d"
                }
            ],
            "clouds": 94,
            "pop": 1,
            "rain": 11.13,
            "uvi": 0.68
        },
        {
            "dt": 1611921600,
            "sunrise": 1611906276,
            "sunset": 1611939043,
            "temp": {
                "day": 282.4,
                "min": 281.19,
                "max": 283.21,
                "night": 281.35,
                "eve": 282.02,
                "morn": 282.22
            },
            "feels_like": {
                "day": 273.8,
                "night": 278.29,
                "eve": 277.04,
                "morn": 273.26
            },
            "pressure": 1001,
            "humidity": 82,
            "dew_point": 279.53,
            "wind_speed": 11.08,
            "wind_deg": 279,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10d"
                }
            ],
            "clouds": 85,
            "pop": 1,
            "rain": 1.3,
            "uvi": 0.79
        },
        {
            "dt": 1612008000,
            "sunrise": 1611992592,
            "sunset": 1612025548,
            "temp": {
                "day": 281.49,
                "min": 276.86,
                "max": 281.78,
                "night": 276.86,
                "eve": 278.9,
                "morn": 281.78
            },
            "feels_like": {
                "day": 277.73,
                "night": 267.06,
                "eve": 269.23,
                "morn": 279.96
            },
            "pressure": 992,
            "humidity": 92,
            "dew_point": 280.36,
            "wind_speed": 4.41,
            "wind_deg": 126,
            "weather": [
                {
                    "id": 501,
                    "main": "Rain",
                    "description": "moderate rain",
                    "icon": "10d"
                }
            ],
            "clouds": 100,
            "pop": 1,
            "rain": 21.62,
            "uvi": 0.67
        },
        {
            "dt": 1612094400,
            "sunrise": 1612078907,
            "sunset": 1612112054,
            "temp": {
                "day": 277.8,
                "min": 275.95,
                "max": 277.8,
                "night": 277.69,
                "eve": 277.24,
                "morn": 275.99
            },
            "feels_like": {
                "day": 269.94,
                "night": 269.85,
                "eve": 269,
                "morn": 268.32
            },
            "pressure": 1001,
            "humidity": 73,
            "dew_point": 273.42,
            "wind_speed": 8.44,
            "wind_deg": 107,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10d"
                }
            ],
            "clouds": 99,
            "pop": 0.86,
            "rain": 1.88,
            "uvi": 0.45
        },
        {
            "dt": 1612180800,
            "sunrise": 1612165219,
            "sunset": 1612198560,
            "temp": {
                "day": 278.54,
                "min": 277.45,
                "max": 279.28,
                "night": 279.28,
                "eve": 278.61,
                "morn": 277.93
            },
            "feels_like": {
                "day": 274.4,
                "night": 275.05,
                "eve": 274.56,
                "morn": 271.16
            },
            "pressure": 1004,
            "humidity": 82,
            "dew_point": 275.78,
            "wind_speed": 3.66,
            "wind_deg": 53,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10d"
                }
            ],
            "clouds": 93,
            "pop": 1,
            "rain": 3.45,
            "uvi": 0.29
        },
        {
            "dt": 1612267200,
            "sunrise": 1612251529,
            "sunset": 1612285066,
            "temp": {
                "day": 283.93,
                "min": 280.45,
                "max": 283.93,
                "night": 282.18,
                "eve": 282.44,
                "morn": 281.53
            },
            "feels_like": {
                "day": 276.36,
                "night": 275.85,
                "eve": 275.19,
                "morn": 274.33
            },
            "pressure": 999,
            "humidity": 91,
            "dew_point": 282.63,
            "wind_speed": 10.64,
            "wind_deg": 238,
            "weather": [
                {
                    "id": 501,
                    "main": "Rain",
                    "description": "moderate rain",
                    "icon": "10d"
                }
            ],
            "clouds": 100,
            "pop": 1,
            "rain": 10.49,
            "uvi": 1
        },
        {
            "dt": 1612353600,
            "sunrise": 1612337838,
            "sunset": 1612371573,
            "temp": {
                "day": 281.63,
                "min": 281.15,
                "max": 282.06,
                "night": 281.15,
                "eve": 281.42,
                "morn": 281.53
            },
            "feels_like": {
                "day": 277.62,
                "night": 279.03,
                "eve": 278.77,
                "morn": 277.61
            },
            "pressure": 1001,
            "humidity": 84,
            "dew_point": 279.2,
            "wind_speed": 4.39,
            "wind_deg": 175,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10d"
                }
            ],
            "clouds": 96,
            "pop": 1,
            "rain": 2,
            "uvi": 1
        },
        {
            "dt": 1612440000,
            "sunrise": 1612424144,
            "sunset": 1612458079,
            "temp": {
                "day": 279.88,
                "min": 279,
                "max": 280.82,
                "night": 279.35,
                "eve": 279.53,
                "morn": 279
            },
            "feels_like": {
                "day": 276.97,
                "night": 274.64,
                "eve": 274.41,
                "morn": 275.56
            },
            "pressure": 1001,
            "humidity": 87,
            "dew_point": 277.99,
            "wind_speed": 2.47,
            "wind_deg": 350,
            "weather": [
                {
                    "id": 500,
                    "main": "Rain",
                    "description": "light rain",
                    "icon": "10d"
                }
            ],
            "clouds": 85,
            "pop": 0.54,
            "rain": 0.33,
            "uvi": 1
        }
    ]
}
)";