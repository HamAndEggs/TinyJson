/*
   Copyright (C) 2021, Richard e Collins.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  
   
   Original code base is at https://github.com/HamAndEggs/TinyJson
*/
#ifndef TINY_JSON_H
#define TINY_JSON_H

#include <stdexcept>
#include <exception>
#include <memory>
#include <map>
#include <vector>
#include <set>
#include <string>
#include <utility>
#include <iostream>
#include <fstream>

#include <assert.h>

#define THROW_JSON_EXCEPTION(THE_MESSAGE__)	{throw std::runtime_error("Tiny Json EXCEPTION At: " + std::to_string(__LINE__) + " In " + std::string(__FILE__) + " : " + std::string(THE_MESSAGE__));}

/**
 * @brief Throws an exception if the type is not a match. Is a #define so we get the real file position and name.
 */
#define TINYJSON_ASSERT_TYPE(THE_TYPE__) {if( mType != THE_TYPE__ ){THROW_JSON_EXCEPTION("Json Type is not what is expected, the type is " + JsonValueTypeToString(mType) +" looking for " + JsonValueTypeToString(THE_TYPE__));}}

namespace tinyjson{ // Using a namespace to try to prevent name clashes as my class names are kind of obvious :)
///////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TINYJSON_VERSION "1.0.0"

/**
 * @brief Different types of json value.
 */
#define JSON_TYPES              \
    DEF_TYPE(STRING,"String")   \
    DEF_TYPE(NUMBER,"Number")   \
    DEF_TYPE(OBJECT,"Object")   \
    DEF_TYPE(ARRAY,"Array")     \
    DEF_TYPE(BOOLEAN,"Boolean") \
    DEF_TYPE(NULL_VALUE,"NULL")
    
enum struct JsonValueType
{
    INVALID,
#define DEF_TYPE(JSON_ENUM__,JSON_NAME__) JSON_ENUM__,
    JSON_TYPES
#undef DEF_TYPE
};

/**
 * @brief Fetches the string human readable name for a just type.
 * 
 * @param pType 
 * @return std::string 
 */
inline std::string JsonValueTypeToString(JsonValueType pType)
{
    switch(pType)
    {
#define DEF_TYPE(JSON_ENUM__,JSON_NAME__) case JsonValueType::JSON_ENUM__:return JSON_NAME__;
    JSON_TYPES
#undef DEF_TYPE
    case JsonValueType::INVALID:
        return "INVALID";
    };
    THROW_JSON_EXCEPTION("JsonValueTypeToString passed an unknown type value");
}

/**
 * @brief The key value pairs of a json object.
 */
typedef std::map<std::string,struct JsonValue> JsonKeyValue;

/**
 * @brief This represents the core data structure that drives Json.
 * It forms the second part of 1 or more the key value pair that represents a Json object.
 */
struct JsonValue
{
    /**
     * @brief This holds the true or false value if the json value is TRUE or FALSE
     * The json spec defines types, one for false and one for true. That is daft.
     * So I define a boolean type and set my type to BOOLEAN and store the value.
     */
    bool mBoolean = false;

	JsonValueType mType;   //<! The type of the json value.

	/**
	 * @brief I hold all number values as a string, this is because until the user asks I do not know what type they want it as.
	 * I also put the strings in here.
     * I am not using any memory fancy tricks. I prefer the code to be ledgable and maintainable.
     * Also, it has been shown, some tricks that should work, like, polymorphic memory resources are slower.
     * https://stackoverflow.com/questions/55028447/why-is-pmrstring-so-slow-in-these-benchmarks
	 */
	std::string mValue;

    /**
     * @brief A json object is a list of 1 or more key, value pairs.
     */
    JsonKeyValue mObject;

    /**
     * @brief The storage for an array, which is just an array of json values.
     */
	std::vector<struct JsonValue> mArray;


    JsonValue() = default;
    JsonValue(JsonValueType pType):mType(pType){}

    JsonValue(bool pValue):mType(JsonValueType::BOOLEAN){mValue = pValue?"true":"false";}
    JsonValue(const std::string& pValue):mType(JsonValueType::STRING){mValue = pValue;}

    #define MAKE_COPY_CONSTRUCTOR(CTYPE__,JTYPE__)  JsonValue(CTYPE__ pValue):mType(JsonValueType::JTYPE__){mValue = std::to_string(pValue);}
        MAKE_COPY_CONSTRUCTOR(float,NUMBER)
        MAKE_COPY_CONSTRUCTOR(double,NUMBER)
        MAKE_COPY_CONSTRUCTOR(int,NUMBER)
        MAKE_COPY_CONSTRUCTOR(uint64_t,NUMBER)
        MAKE_COPY_CONSTRUCTOR(int64_t,NUMBER)
    #undef MAKE_COPY_CONSTRUCTOR

    JsonValue(const std::vector<std::string>& pStrings):mType(JsonValueType::ARRAY)
    {
        for( const auto& str : pStrings )
        {
            mArray.push_back(str);
        }
    }

    JsonValue(const std::set<std::string>& pStrings):mType(JsonValueType::ARRAY)
    {
        for( const auto& str : pStrings )
        {
            mArray.push_back(str);
        }
    }

    JsonValue(const std::map<std::string,std::string>& pStrings):mType(JsonValueType::OBJECT)
    {
        for( const auto& str : pStrings )
        {
            mObject[str.first] = str.second;
        }
    }

    JsonValue& operator = (const std::string& pString)
    {
        // Can only assign when the type has not yet been set.
        TINYJSON_ASSERT_TYPE(JsonValueType::INVALID);

        mType = JsonValueType::STRING;
        mValue = pString;
        return *this;
    }

    JsonValue& operator = (const std::vector<std::string>& pStrings)
    {
        // Can only assign when the type has not yet been set.
        TINYJSON_ASSERT_TYPE(JsonValueType::INVALID);

        mType = JsonValueType::ARRAY;
        for( const auto& s : pStrings )
        {
            mArray.push_back(s);
        }
        return *this;
    }

    JsonValue& operator = (const std::set<std::string>& pStrings)
    {
        // Can only assign when the type has not yet been set.
        TINYJSON_ASSERT_TYPE(JsonValueType::INVALID);

        mType = JsonValueType::ARRAY;
        for( const auto& s : pStrings )
        {
            mArray.push_back(s);
        }
        return *this;
    }

    JsonValue& operator = (const std::map<std::string,std::string>& pStrings)
    {
        // Can only assign when the type has not yet been set.
        TINYJSON_ASSERT_TYPE(JsonValueType::INVALID);

        mType = JsonValueType::OBJECT;
        for( const auto& str : pStrings )
        {
            mObject[str.first] = str.second;
        }
        return *this;        
    }

    /**
     * @brief This is a handy overload that allows you to do ["key1"]["key2"]["key3"].GetInt() type of thing.
     * throws std::runtime_error if key not found.
     * @param pKey The key string that represents the value you want.
     * @return const JsonValue& The value.
     */
    const JsonValue& operator [](const std::string& pKey)const
    {
        TINYJSON_ASSERT_TYPE(JsonValueType::OBJECT);
        const auto found = mObject.find(pKey);
        if( found != mObject.end() )
            return found->second;
        THROW_JSON_EXCEPTION("Json value for key " + pKey + " not found");
    }
    const JsonValue& operator [](const char* pKey)const{assert(pKey);return (*this)[std::string(pKey)];}

    JsonValue& operator [](const std::string& pKey)
    {
        TINYJSON_ASSERT_TYPE(JsonValueType::OBJECT);
        return mObject[pKey];
    }
    JsonValue& operator [](const char* pKey){assert(pKey);return (*this)[std::string(pKey)];}


    /**
     * @brief Allows you to access the array type with an index and not have to add a ".Array"
     * This means MyJson["songs"][10]["name"].GetString() is possible.
     * @param pIndex 
     * @return const JsonValue& 
     */
    const JsonValue& operator [](size_t pIndex)const
    {
        TINYJSON_ASSERT_TYPE(JsonValueType::ARRAY);
        return mArray[pIndex];
    }
    const JsonValue& operator [](int pIndex)const{TINYJSON_ASSERT_TYPE(JsonValueType::ARRAY);return mArray[pIndex];}

    /**
     * If the value is an object then you can use it in a for loop.
     * E.G or(const auto &res : child.second)
     */
    const JsonKeyValue::const_iterator begin()const{TINYJSON_ASSERT_TYPE(JsonValueType::OBJECT);return mObject.cbegin();}
    const JsonKeyValue::iterator begin(){TINYJSON_ASSERT_TYPE(JsonValueType::OBJECT);return mObject.begin();}

    const JsonKeyValue::const_iterator end()const{TINYJSON_ASSERT_TYPE(JsonValueType::OBJECT);return mObject.cend();}
    const JsonKeyValue::iterator end(){TINYJSON_ASSERT_TYPE(JsonValueType::OBJECT);return mObject.end();}

    operator float()const{return GetFloat();} 
    operator double()const{return GetDouble();} 

    operator bool()const{return GetBoolean();} 
    operator int32_t()const{return GetInt32();} 
    operator uint32_t()const{return GetUInt32();} 
    operator int64_t()const{return GetInt64();} 
    operator uint64_t()const{return GetUInt64();} 
    operator const std::string&()const{return GetString();} 
    operator tinyjson::JsonValueType()const{return GetType();} 

    /**
     * Object has to either already be an object or be unitialsed for this to be valid. 
     */
    void Emplace(const std::string& pKey,const JsonValue& pValue)
    {
        if( mType == JsonValueType::INVALID )
        {
            mType = JsonValueType::OBJECT;
        }
        else
        {
            TINYJSON_ASSERT_TYPE(JsonValueType::OBJECT);
        }

        mObject.emplace(pKey,pValue);
    }

    void Emplace(const std::string& pKey,const std::string& pValue)
    {
        if( mType == JsonValueType::INVALID )
        {
            mType = JsonValueType::OBJECT;
        }
        else
        {
            TINYJSON_ASSERT_TYPE(JsonValueType::OBJECT);
        }

        mObject.emplace(pKey,pValue);
    }

    void Emplace(const std::string& pKey,const std::vector<std::string>& pValues)
    {
        if( mType == JsonValueType::INVALID )
        {
            mType = JsonValueType::OBJECT;
        }
        else
        {
            TINYJSON_ASSERT_TYPE(JsonValueType::OBJECT);
        }

        mObject.emplace(pKey,pValues);
    }

    void Emplace(const std::string& pKey,const std::set<std::string>& pValues)
    {
        if( mType == JsonValueType::INVALID )
        {
            mType = JsonValueType::OBJECT;
        }
        else
        {
            TINYJSON_ASSERT_TYPE(JsonValueType::OBJECT);
        }

        mObject.emplace(pKey,pValues);
    }

    void Emplace(const std::string& pKey,const std::map<std::string,std::string>& pValues)
    {
        if( mType == JsonValueType::INVALID )
        {
            mType = JsonValueType::OBJECT;
        }
        else
        {
            TINYJSON_ASSERT_TYPE(JsonValueType::OBJECT);
        }

        mObject.emplace(pKey,pValues);
    }

   /**
     * @brief Checks that the key passed in exists without throwing an exception.
     * If you do MyJson["scores"][10].GetInt() and "scores" was not in the root the code will throw an exception.
     */
    bool HasValue(const std::string& pKey)const
    {
        if( mType == JsonValueType::OBJECT )
        {
            const auto found = mObject.find(pKey);
            if( found != mObject.end() )
                return true;
        }

        return false;
    }

    /**
     * @brief Get the Type of the jason value
     */
    JsonValueType GetType()const
    {
        return mType;
    }

    /**
     * @brief Fetches the size of the array, if the type is an array, else zero.
     */
    size_t GetArraySize()const
    {
        if( mType == JsonValueType::ARRAY )
            return mArray.size();
        return 0;
    }

    /**
     * @brief Gets the value as a string, if it is a string type. Else throws an exception.
     */
    const std::string& GetString()const
    {
        TINYJSON_ASSERT_TYPE(JsonValueType::STRING);
        return mValue;
    }

    /**
     * @brief Gets the value as a double, if it is a number type. Else throws an exception.
     */
    double GetDouble()const
    {
        TINYJSON_ASSERT_TYPE(JsonValueType::NUMBER);
        return std::stod(mValue);
    }

    /**
     * @brief Gets the value as a float, if it is a number type. Else throws an exception.
     */
    float GetFloat()const
    {
        TINYJSON_ASSERT_TYPE(JsonValueType::NUMBER);
        return std::stof(mValue);
    }

    /**
     * @brief Gets the value as an int, if it is a number type. Else throws an exception.
     */
    int GetInt()const
    {
        return GetInt32();
    }

    /**
     * @brief Gets the value as an uint64_t, if it is a number type. Else throws an exception.
     */
    uint64_t GetUInt64()const
    {
        TINYJSON_ASSERT_TYPE(JsonValueType::NUMBER);
        return std::stoull(mValue);
    }

    /**
     * @brief Gets the value as an uint32_t, if it is a number type. Else throws an exception.
     */
    uint32_t GetUInt32()const
    {
        TINYJSON_ASSERT_TYPE(JsonValueType::NUMBER);
        return std::stoul(mValue);
    }

    /**
     * @brief Gets the value as an int64_t, if it is a number type. Else throws an exception.
     */
    int64_t GetInt64()const
    {
        TINYJSON_ASSERT_TYPE(JsonValueType::NUMBER);
        return std::stoll(mValue);
    }

    /**
     * @brief Gets the value as an int32_t, if it is a number type. Else throws an exception.
     */
    int32_t GetInt32()const
    {
        TINYJSON_ASSERT_TYPE(JsonValueType::NUMBER);
        return std::stol(mValue);
    }

    /**
     * @brief Gets the value as an boolean, if it is a boolean type. Else throws an exception.
     */
    bool GetBoolean()const
    {
        TINYJSON_ASSERT_TYPE(JsonValueType::BOOLEAN);
        return mBoolean;
    }

    const std::vector<struct JsonValue>& GetArray()const
    {
        TINYJSON_ASSERT_TYPE(JsonValueType::ARRAY);
        return mArray;
    }

    const JsonKeyValue& GetObject()const
    {
        TINYJSON_ASSERT_TYPE(JsonValueType::OBJECT);
        return mObject;
    }

    bool IsString()const{return GetType() == JsonValueType::STRING;}
    bool IsNumber()const{return GetType() == JsonValueType::NUMBER;}
    bool IsObject()const{return GetType() == JsonValueType::OBJECT;}
    bool IsArray()const{return GetType() == JsonValueType::ARRAY;}
    bool IsBool()const{return GetType() == JsonValueType::BOOLEAN;}
    bool IsNULL()const{return GetType() == JsonValueType::NULL_VALUE;}

    /***************************************************
     * Following set of functions are more robust and allow you to supply a default if the key is missing or the expected type is wrong.
     * But beware uses these, although convenient, will hide errors in code. These are best used when you do not have control over the
     * Json being read.
     * If it's one of your files that you generate it is best to use the ones above as they will warn you about errors in the data.
     * 
     * @brief These functions will return the value found if the key is present and the value type is correct, else will return the default.
     * @param pKey The Key of the value to look for.
     * @param pDefault The default value to use if there was a problem.
     * @return The value if there or the default
     ***************************************************/
#define MAKE_SAFE_FUNCTION(FUNC_NAME__,FUNC_TYPE__,DEFAULT_VALUE__)                                 \
    FUNC_TYPE__ FUNC_NAME__(const std::string& pKey,FUNC_TYPE__ pDefault = DEFAULT_VALUE__,bool pVerbose = false)const    \
    {                                                                                               \
        try{return (*this)[pKey].FUNC_NAME__();}                                                    \
        catch(const std::exception& e)                                                              \
        {                                                                                           \
            if( pVerbose ){std::cerr << e.what() << "\n";}                                          \
        }/* Ignore exception and return the default.*/                                              \
        return pDefault;                                                                            \
    }

    MAKE_SAFE_FUNCTION(GetArraySize,size_t,0);
    MAKE_SAFE_FUNCTION(GetString,const std::string&,"");
    MAKE_SAFE_FUNCTION(GetDouble,double,0.0);
    MAKE_SAFE_FUNCTION(GetFloat,float,0.0f);
    MAKE_SAFE_FUNCTION(GetInt,int,0);
    MAKE_SAFE_FUNCTION(GetUInt64,uint64_t,0);
    MAKE_SAFE_FUNCTION(GetUInt32,uint32_t,0);
    MAKE_SAFE_FUNCTION(GetInt64,int64_t,0);
    MAKE_SAFE_FUNCTION(GetInt32,int32_t,0);
    MAKE_SAFE_FUNCTION(GetBoolean,bool,false);
    MAKE_SAFE_FUNCTION(GetType,JsonValueType,JsonValueType::INVALID);
#undef MAKE_SAFE_FUNCTION

private:
};

/**
 * @brief This is the work horse that builds our data structure that mirrors the json data.
 */
class JsonProcessor
{
public:
    /**
     * @brief Construct a new Json Processor object and parse the json data.
     * throws std::runtime_error if the json is not constructed correctly.
     * If pFailOnDuplicateKeys is true and two keys at the same level are found to have the same name then we'll throw an exception.
     */
	JsonProcessor(const std::string& pJsonString,bool pFailOnDuplicateKeys = false) :
        mFailOnDuplicateKeys(pFailOnDuplicateKeys),
        mJsonEnd(pJsonString.c_str() + pJsonString.size()),
        mPos(pJsonString.c_str())
    {
        mRow = mColumn = 1;

        if( pJsonString.size() < 2 )
        {
            THROW_JSON_EXCEPTION("Empty string passed into ParseJson");
        }

        MakeValue(mRoot); // Now lets get going. :D
        SkipWhiteSpace();
        if( mPos < mJsonEnd )// Now should be at the end
        {
            THROW_JSON_EXCEPTION("Data found after root object, invalid Json");
        }
    }

    /**
     * @brief Get the Root object
     */
    const JsonValue& GetRoot()const
    {
        return mRoot;
    }

    /**
     * Convenient way to access values in the root object.
    */
    const JsonValue& operator [](const std::string& pKey)const
    {
        return GetRoot()[pKey];
    }

private:
    const bool mFailOnDuplicateKeys;    //!< If true and two keys at the same level are found to have the same name then we'll throw an exception.
    const char* const mJsonEnd;         //!< Used to detect when we're at the end of the data.
    const char* mPos;                   //!< The current position in the data that we are at.  
    JsonValue mRoot;                    //!< When all is done, this contains the json as usable c++ objects.
    uint32_t mRow,mColumn;              //!< Keeps track of where we are in the file for error reporting to the user.

    /**
     * @brief This will advance to next char and deal with line and colum tracking as we go.
     * It was optional, with a version that just incremented mPos. Made little difference to speed when reading massive 2MB file.
     * So is on all the time now as makes code cleaning and the class easier to use.
     * Remember, trust your compiler. NextChar will compile into a few instructions. This has been check in compiler explorer.  https://godbolt.org/
     */
    inline void NextChar(){mPos++;if(*mPos == '\n'){mRow++;mColumn=1;}else{mColumn++;}}

    /**
     * @brief This is used in several place whilst parsing the data to detect json data that is not complete.
     * For safety does not test for NULL but checks that the mPos pointer has not gone past the end of the data.
     */
    inline void AssertMoreData(const char* pErrorString)
    {
        if( mPos >= mJsonEnd ){THROW_JSON_EXCEPTION(pErrorString);}
    }

    /**
     * @brief Used to check that the expected charater is the correct one, if not tells the user!
     */
    inline void AssertCorrectChar(char c,const char* pErrorString)
    {
        if( *mPos != c )
        {
            THROW_JSON_EXCEPTION(GetErrorPos() + pErrorString);
        }
    }

    /**
     * @brief Returns a string used in errors to show where the error is.
     */
    inline std::string GetErrorPos()
    {
        return std::string("Error at Line ") + std::to_string(mRow) + " column " + std::to_string(mColumn) + " : ";
    }

    /**
     * @brief Builds a Json object, which is a map of key value paris.
     * Constructed in this way to reduce copy by value which is what you would get by returning the completed object. That would be horrendous.
     * @param rObject The json object that is to be built.
     */
    void MakeObject(JsonKeyValue& rObject)
    {
        // Search for the start of the object.
        SkipWhiteSpace();
        AssertCorrectChar('{',"Start of object not found, invalid Json");
        do
        {
            const char previousChar = mPos[0];
            NextChar();// Skip object start char or comma for more key value pairs.
            SkipWhiteSpace();
            // Is it an empty object?
            if( *mPos == '}' )
            {
                if( previousChar == ',' )
                {
                    THROW_JSON_EXCEPTION(GetErrorPos() + "End of root object found, invalid Json. Comma with no object defined after it");
                }
                else
                {
                    NextChar();
                    return;
                }
            }

            std::string objKey;
            ReadString(objKey);

            // Now parse it's value.
            SkipWhiteSpace();
            AssertCorrectChar(':',"Json format error detected, seperator character ':'");
            NextChar();

            if( mFailOnDuplicateKeys )
            {
                if( rObject.find(objKey) != rObject.end() )
                {
                    THROW_JSON_EXCEPTION(GetErrorPos() + "Json format error detected, two objects at the same level have the same key, " + objKey);
                }
            }

            MakeValue(rObject[objKey]);
            
            // Now see if there are more key value pairs to add to the object or if we're done.
            if( *mPos != '}' && *mPos != ',' )
            {
                THROW_JSON_EXCEPTION(GetErrorPos() + "Json format error detected, did you forget a comma between key value pairs? For key " + objKey);
            }
        }while (*mPos == ',');

        if( *mPos == '}' )    // Validate end of object.
        {
            NextChar();
        }
        else
        {
            THROW_JSON_EXCEPTION(GetErrorPos() + "End of root object not found, invalid Json");
        }
    }

    /**
     * @brief Builds the core value structure that powers Json.
     * @param pNewValue As with MakeObject, we don't return the new value but initialise the one passed in.
     */
    void MakeValue(JsonValue& pNewValue)
    {
        SkipWhiteSpace();// skip space and then see if it's an object, string, value or special state (TRUE,FALSE,NULL). We also skip white space before exit.
        switch( *mPos )
        {
        case 0:
            break;

        case '{':
            pNewValue.mType = JsonValueType::OBJECT;
            MakeObject(pNewValue.mObject);
            break;

        case '[':
            pNewValue.mType = JsonValueType::ARRAY;
            do
            {
                const char previousChar = mPos[0];
                NextChar();// skip '[' or the ','

                SkipWhiteSpace();
                // Make sure there is an object next and not the end of the array.
                if( previousChar == ',' && mPos[0] == ']' )
                {
                    THROW_JSON_EXCEPTION(GetErrorPos() + "Json format error detected, comma not follwed by a value.");
                }

                // Looks odd, but is the easiest / optimal way to reduce memory reallocations using c++14 features.
                // The problem is that we have to make the object before adding to the vector.
                // This means we can't use emplace_back as we don't have all that we need.
                // I would have to duplicate the code for MakeValue for when adding to an array. And that just makes everything hard to maintain!
                // In c++17 you can use PMR and reduce memory allocations. I may look at that.
                // But for now this code compiles for c++11 which is good for some old systems. Remember, minimun requirements!

                // Only make the value if there is one next.
                // It is posible that this is an empty array. Rather annoying that the spec allows that.
                if( *mPos != ']' )
                {
                    pNewValue.mArray.resize(pNewValue.mArray.size()+1);
                    MakeValue(pNewValue.mArray.back());
                }
            }while(*mPos == ',');

            // Check we did get to the end.
            if( *mPos != ']' )
            {
                THROW_JSON_EXCEPTION(GetErrorPos() + "Json format error detected, array not terminated with ']'");
            }
            NextChar();//skip ']'
            break;

        case '\"':
            pNewValue.mType = JsonValueType::STRING;
            ReadString(pNewValue.mValue);
            break;

        case 'T':
        case 't':
            if( tolower(mPos[1]) == 'r' && tolower(mPos[2]) == 'u' && tolower(mPos[3]) == 'e' )
            {
                mPos += 4;
                pNewValue.mType = JsonValueType::BOOLEAN;
                pNewValue.mBoolean = true;
            }
            else
            {
                THROW_JSON_EXCEPTION(GetErrorPos() + std::string("Invalid character \"") + *mPos + "\" found in json value definition reading true type");
            }
            break;

        case 'F':
        case 'f':
            if( tolower(mPos[1]) == 'a' && tolower(mPos[2]) == 'l' && tolower(mPos[3]) == 's' && tolower(mPos[4]) == 'e' )
            {
                mPos += 5;
                pNewValue.mType = JsonValueType::BOOLEAN;
                pNewValue.mBoolean = false;
            }
            else
            {
                THROW_JSON_EXCEPTION(GetErrorPos() + std::string("Invalid character \"") + *mPos + "\" found in json value definition reading false type");
            }
            break;

        case 'N':
        case 'n':
            if( tolower(mPos[1]) == 'u' && tolower(mPos[2]) == 'l' && tolower(mPos[3]) == 'l' )
            {
                mPos += 4;
                pNewValue.mType = JsonValueType::NULL_VALUE;
            }
            else
            {
                THROW_JSON_EXCEPTION(GetErrorPos() + std::string("Invalid character \"") + *mPos + "\" found in json value definition reading null type");
            }
            break;

        case '-':
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            pNewValue.mType = JsonValueType::NUMBER;
            ReadNumber(pNewValue.mValue);
            break;

        default:
            THROW_JSON_EXCEPTION(GetErrorPos() + std::string("Invalid character \"") + *mPos + "\" found at start of json value definition");
            break;
        }
        // Skip any human readble characters. We exit leaving pos on the next meaningful character.
        SkipWhiteSpace();
    }

    /**
     * @brief Skips to the next char that is not white space.
     */
    void SkipWhiteSpace()
    {
        while( isspace(*mPos) )
        {// As per Json spec, look for characters that are not a space, linefeed, carrage return or horizontal tab. isspace does this.
            AssertMoreData("Abrupt end to json whilst skipping white space");
            NextChar();
        }
    }

    /**
     * @brief Reads a string value.
     */
    void ReadString(std::string& rString)
    {
        // First find the start of the string
        SkipWhiteSpace();
        AssertCorrectChar('\"',"Json format error detected, expected start of string, did you forget to put the string in quotes?");
        NextChar(); // Skip "
        const char* stringStart = mPos;
        // Now scan till we hit the next "
        while( *mPos != '\"' )
        {
            // Did we hit the end?
            AssertMoreData("Abrupt end to json whilst reading string");            

            // Special case, if we find a \ and then a special character code.
            if( mPos[0] == '\\' )
            {
                // Test data says \x should be a failure. For safety I agree. I may have to add an option for the user.
                if( mPos[1] == '0' || mPos[1] == 'x' )
                {
                    THROW_JSON_EXCEPTION(std::string("Escape value \\") + mPos[1] + " not allowed " + std::string(mPos-1,20) );
                }


                if( mPos[1] == 'a' || mPos[1] == 'b' || mPos[1] == 't' ||
                    mPos[1] == 'n' || mPos[1] == 'v' || mPos[1] == 'f' || mPos[1] == 'r' ||
                    mPos[1] == 'e' || mPos[1] == '\"' || mPos[1] == '\\' )
                {
                    NextChar();
                }
            }

            // Test for illegal characters.
            if( mPos[0] == '\t' )
            {
                THROW_JSON_EXCEPTION(std::string("illegal character, tab not allowed ") + std::string(mPos-1,20) );
            }
            if( mPos[0] == '\n' )
            {
                THROW_JSON_EXCEPTION(std::string("illegal character, newline not allowed ") + std::string(mPos-1,20) );
            }

            NextChar();
        }
   
        const size_t len = mPos - stringStart;
        if( len > 0 ) // Did we read a string?
        {
            rString.assign(stringStart,len);
        }
        NextChar(); // Skip "
    }

    /**
     * @brief Scans for the end of the number that we just found the start too.
     * mPos is set to the end of the number.
     */
    void ReadNumber(std::string& rString)
    {
        const char* valueStart = mPos;
        // As per Json spec, keep going to we see end of accepted number components.
        // There is an order that you do this in, see https://www.json.org/json-en.html
        if( *mPos == '-' )
        {
            NextChar();
        }

        // after accounting the - there must be a number next.
        if( isdigit(*mPos) == false )
        {
            THROW_JSON_EXCEPTION(std::string("Malformed number ") + std::string(mPos-1,20) );
        }

        // Not allowed to start with a zero.
        if( *mPos == '0' && isdigit(mPos[1]) )
        {
            THROW_JSON_EXCEPTION(std::string("Malformed number, not allowed to start with zero.") + std::string(mPos-1,20) );
        }

        // Scan for end of digits.
        while( isdigit(*mPos) )
        {
            NextChar();
        }

        // The next character should not be alpha.
        if( isalpha(*mPos) && *mPos != 'e' && *mPos != 'E' )
        {
            THROW_JSON_EXCEPTION(std::string("Malformed number, embedded characters ") + std::string(mPos-1,20) );
        }

        // Do we have a decimal?
        if( *mPos == '.' )
        {
            NextChar();
            // Now scan more more digits.
            while( isdigit(*mPos) )
            {
                NextChar();
            }
        }
        // now see if there is an exponent. 
        if( *mPos == 'E' || *mPos == 'e' )
        {
            NextChar();
            // Now must be a sign or a number
            if( *mPos == '-' || *mPos == '+' || std::isdigit(*mPos) )
            {
                if( *mPos == '-' || *mPos == '+' )
                {// Must be followed by a number.
                    NextChar();
                    if( isdigit(*mPos) == false )// after accounting the - or + there must be a number next.
                    {
                        THROW_JSON_EXCEPTION(GetErrorPos() + std::string("Malformed exponent in number ") + std::string(mPos-1,20) );
                    }
                }

                // Now scan more more digits.
                while( isdigit(*mPos) )
                {
                    NextChar();
                }
            }
            else
            {
                THROW_JSON_EXCEPTION(GetErrorPos() + std::string("Malformed exponent in number ") + std::string(mPos-1,20) );
            }
        }

        // This is a big win in the reading. I don't convert the type now, it is done when the user needs it.
        // I've tested string_view. It reduces memory allocations by 40% but complicates the code as I have to insert NULLs into the data.
        // Reading is so fast, even on low end arm chips, it's a pointless optimisation! Remember, I'm going for clean code.
        rString.assign(valueStart,mPos-valueStart);
    }
};//end of struct JsonProcessor

/**
 * @brief Writes the Json object tree passed in to the file passed. Use pPretty to set if you want tabs and newlines.
 */
inline void JsonWriter(std::ofstream& pFile,const JsonValue& pRoot,bool pPretty,int pTabCount = 0)
{
    const char* quote = "\"";
    const char* NewLine = pPretty?"\n":"";
    const std::string OBJ_TAB(4 * (pTabCount+1) * pPretty,' ');

    const std::string TAB(4 * pTabCount * pPretty,' ');

    switch (pRoot.mType)
    {
    case JsonValueType::STRING:
        pFile << quote << pRoot.mValue << quote;
        break;
    
    case JsonValueType::NUMBER:
        pFile << pRoot.mValue;
        break;

    case JsonValueType::OBJECT:
        {
            const char* StartObj = NewLine;
            pFile << NewLine << TAB << "{";
            for(const auto& o : pRoot.mObject )
            {
                pFile << StartObj << OBJ_TAB << quote << o.first << quote << ":";
                JsonWriter(pFile,o.second,pPretty,pTabCount+1);
                StartObj = pPretty?",\n":",";
            }
            pFile << NewLine << TAB << "}";
        }
        break;

    case JsonValueType::ARRAY:
        {
            const char* StartObj = NewLine;
            pFile << NewLine << TAB << "[";
            for(const auto& o : pRoot.mArray )
            {
                pFile << StartObj << OBJ_TAB;
                JsonWriter(pFile,o,pPretty,pTabCount+1);
                StartObj = pPretty?",\n":",";
            }
            pFile << NewLine << TAB << "]";
        }
        break;

    case JsonValueType::BOOLEAN:
        pFile << (pRoot.mBoolean?"true":"false");
        break;

    case JsonValueType::NULL_VALUE:
    case JsonValueType::INVALID:
        pFile << "null";
        break;
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////
};// namespace tinyjson
#endif //TINY_JSON_H