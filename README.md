# TinyJson
Single header file to read a JSON file into a structure of c++ containers for easy access.

The goal of the is code is to be a concise and clean implementation of a json parser. It only reads a file.
It follows the ethos of the clean code movement and so lacks some of the more expected optimizations that could be done with the use of c++17 features.
My stance is that if you need speed use rapidjson, that is the gold standard. This code is a single head for small projects parsing files recived from cloud services.

This targets c++11 to give maximum compiler support.
I could reduce the number of allocations by switching to string_view but that would increase the requirements to c++17. 
It would also make the code more complicated as I would have to modify the json string on the fly adding nulls.

The unit test code carries out the same and more tests that are leveraged against rapidjson test suit.
Despite my claim that speed was not a concern in this implementation, the code will, on a 15W cpu (AMD 4800u), parse a 2.1MB file in 65ns.

This header is a nice fit for embedded systems where the tool chain can lack behind the desktop parts.
To keep the code clean it only accepts a std::string that is the container for the json.
If you are reading from a file it is for you to read it into a string and passing into the reader. Look at the unit test code for examples.

# How to use
Copy the header TinyJson.h into your project and the LICENSE file.
Please do not remove the comment stating the original source repo location of the header file, that would be a little rude. :)

Example code:
```c++
const std::string jsonString =
R"(
    {
        "Hello World":"TheWorld",
        "empty string test":"",
        "A Null": null,
        "A True": true,
        "A False": false,
        "Int": 123456,
        "Float" : 3.14,
        "Exponent":-1.50139930144708198E18,
        "Array":[
            12,
            "fred",
            {
                "Weather":"Good"
            }
        ]
    }
)";

tinyjson::JsonProcessor json(jsonString);
const tinyjson::JsonValue& root = json.GetRoot();

std::cout << root["Hello World"].GetString() << '\n';
std::cout << root["Array"][0].GetInt() << '\n';
std::cout << root["Array"][2]["Weather"].GetString() << '\n';
```
