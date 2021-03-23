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
