#ifndef TINY_JSON_H
#define TINY_JSON_H

#include <stdexcept>
#include <memory>
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <optional>
#include <assert.h>

namespace tinyjson{ // Using a namespace to try to prevent name clashes as my class names are kind of obvious :)
///////////////////////////////////////////////////////////////////////////////////////////////////////////

enum JsonValueType
{
	JTYPE_STRING,
	JTYPE_NUMBER,
	JTYPE_OBJECT,
	JTYPE_ARRAY,
	JTYPE_TRUE,
	JTYPE_FALSE,
	JTYPE_NULL
};

struct JsonValue;
typedef std::map<std::string,JsonValue> JsonObject;

struct JsonValue
{
	JsonValueType mType;
	
	/**
	 * @brief I hold all number values as a string, this is because until the user asks I do not know what type they want it as.
	 * I also put the strings in here.
	 * I do not put objects or arrays as could take up a lot of space and also not needed.
	 */
	std::string mValue;
    JsonObject mObject;
	std::vector<JsonValue> mArray;

    const JsonValue& operator [](const std::string& pKey)const
    {
        const auto found = mObject.find(pKey);
        if( found != mObject.end() )
            return found->second;
        throw std::runtime_error("Json value for key " + pKey + " not found");
    }
    
    const std::string& GetString()const
    {
        assert( mType == JTYPE_STRING );
        return mValue;
    }

    double GetDouble()const
    {
        assert( mType == JTYPE_NUMBER );
        return std::stod(mValue);
    }

    float GetFloat()const
    {
        assert( mType == JTYPE_NUMBER );
        return std::stof(mValue);
    }

    int GetInt()const
    {
        return GetSigned32Int();
    }

    uint64_t GetUnsigned64Int()const
    {
        assert( mType == JTYPE_NUMBER );
        return std::stoull(mValue);
    }

    uint32_t GetUnsigned32Int()const
    {
        assert( mType == JTYPE_NUMBER );
        return std::stoul(mValue);
    }

    int64_t GetSigned64Int()const
    {
        assert( mType == JTYPE_NUMBER );
        return std::stoll(mValue);
    }

    int32_t GetSigned32Int()const
    {
        assert( mType == JTYPE_NUMBER );
        return std::stol(mValue);
    }

};

struct JsonProcessor
{
	JsonProcessor(const std::string& pJsonString):
        mJson(pJsonString.c_str()),
        mJsonEnd(pJsonString.c_str() + pJsonString.size() + 1)
    {
        if( pJsonString.size() < 2 )
        {
            throw std::runtime_error("Empty string passed into ParseJson");
        }

        MakeObject(mRoot);// This function will leave json pointing to the next non white space.

        // Skip white space that maybe after it.
        SkipWhiteSpace();

        // Now should be at the end
        if( mJson >= mJsonEnd )
        {
            throw std::runtime_error("Data found after root object, invalid Json");
        }
    }

    JsonObject mRoot;

private:
    const char* mJson;
    const char* const mJsonEnd;

    void AssertMoreData(const char* pErrorString)
    {
        if( mJson >= mJsonEnd )
        {
            throw std::runtime_error(pErrorString);
        }
    }

    void MakeObject(JsonObject& rObject)
    {
        // Search for the start of the object.
        FindNextControlChar('{',"Start of object not found, invalid Json");
        do
        {
            mJson++;// Skip object start char or comma for more key value pairs.
            const std::string objKey = ReadString();
            // Now parse it's value.
            FindNextControlChar(':',"Json format error detected, seperator character ':'");
            mJson++;

            MakeValue(rObject[objKey]);

            // Now see if there are more key valuer pairs to add to the object or if we're done.
            // Skip white space, then see if next char is a } (for end of object) or a , for more key value pairs.
            // We leave the pointer there and drop out the function.
            // Down to the caller to test if they carry on or finish the current object.
            SkipWhiteSpace();
            if( *mJson != '}' && *mJson != ',' )
            {
                throw std::runtime_error("Json format error detected, did you forget a comma between key value pairs? For key " + objKey);
            }
        }while (*mJson == ',');

        // Validate end of object.
        if( *mJson == '}' )
        {
            mJson++;
        }
        else
        {
            throw std::runtime_error("End of root object not found, invalid Json");
        }
    }

    void MakeValue(JsonValue& pNewValue)
    {
        // skip space and then see if it's an object, string, value or special state (TRUE,FALSE,NULL).
        SkipWhiteSpace();

        const char* valueStart = mJson;
        switch( *mJson )
        {
        case 0:
            break;

        case '{':
            pNewValue.mType = JTYPE_OBJECT;
            MakeObject(pNewValue.mObject);
            break;

        case '[':
            do
            {
                mJson++;
                pNewValue.mType = JTYPE_ARRAY;
                JsonValue arrayValue;
                MakeValue(arrayValue);
                pNewValue.mArray.emplace_back(arrayValue);
                SkipWhiteSpace();
            }while(*mJson == ',');

            // Check we did get to the end.
            if( *mJson != ']' )
            {
                throw std::runtime_error("Json format error detected, array not terminated with ']'");
            }
            mJson++;
            break;

        case '\"':
            pNewValue.mType = JTYPE_STRING;
            pNewValue.mValue = ReadString();
            break;

        case 'T':
        case 't':
            if( tolower(mJson[1]) == 'u' && tolower(mJson[1]) == 'r' && tolower(mJson[1]) == 'e' )
            {
                mJson += 4;
                pNewValue.mType = JTYPE_TRUE;
            }
            else
            {
                throw std::runtime_error(std::string("Invalid character ") + std::string(mJson,10) + " found in json value definition");
            }
            break;

        case 'F':
        case 'f':
            if( tolower(mJson[1]) == 'a' && tolower(mJson[1]) == 'l' && tolower(mJson[1]) == 's' && tolower(mJson[1]) == 'e' )
            {
                mJson += 5;
                pNewValue.mType = JTYPE_FALSE;
            }
            else
            {
                throw std::runtime_error(std::string("Invalid character ") + std::string(mJson,10) + " found in json value definition");
            }
            break;

        case 'N':
        case 'n':
            if( tolower(mJson[1]) == 'u' && tolower(mJson[1]) == 'l' && tolower(mJson[1]) == 'l' )
            {
                mJson += 4;
                pNewValue.mType = JTYPE_NULL;
            }
            else
            {
                throw std::runtime_error(std::string("Invalid character ") + std::string(mJson,10) + " found in json value definition");
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
            // Scan to white space, comma or object end.
            FindEndOfNumber();
            pNewValue.mType = JTYPE_NUMBER;
            pNewValue.mValue.assign(valueStart,mJson-valueStart);
            break;

        default:
            throw std::runtime_error(std::string("Invalid character ") + std::string(mJson,10) + " found at start of json value definition");
            break;
        }
    }

    void FindNextControlChar(char pToFind,const char* pErrorString)
    {
        // Stop looking when we get to the end of the json.
        while( *mJson != pToFind )
        {
            AssertMoreData(pErrorString);
            const char c = *mJson;
            if( isspace(c) )
            {// Skip white space.
                mJson++;
            }
            else
            {
                //Invalid char, should have been pToFind.
                throw std::runtime_error(std::string("Error in parsing json, expected ") + pToFind + " found " + c);
            }
        }
    }

    void SkipWhiteSpace()
    {
        while( isspace(*mJson) )
        {// As per Json spec, look for characters that are not a space, linefeed, carrage return or horizontal tab. isspace does this.
            AssertMoreData("Abrupt end to json whilst skipping white space");
            mJson++;
        }
    }

    std::string ReadString()
    {
        // First find the start of the string
        FindNextControlChar('\"',"Json format error detected, expected start of string, did you forget to put the string in quotes?");

        mJson++; // Skip "
        const char* stringStart = mJson;

        // Now scan till we hit the next "
        while( *mJson != '\"' )
        {
        // Did we hit the end?
            AssertMoreData("Abrupt end to json whilst reading string");
            mJson++;
        }
        const size_t len = mJson - stringStart;
        mJson++; // Skip "
        if( len > 0 )
        {
            return std::string(stringStart,len);
        }
        // Empty string valid.
        return "";
    }

    void FindEndOfNumber()
    {
        // As per Json spec, keep going to we see end of accepted number components.
        // There is an order that you do this in, see https://www.json.org/json-en.html
        // for more information on reading a number.
        const char* start = mJson;// This is really for the excpetions to help finding the errors in the json string.

        if( *mJson == '-' )
        {
            mJson++;
        }

        // after accounting the - there must be a number next.
        if( isdigit(*mJson) == false )
        {
            throw std::runtime_error(std::string("Malformed number ") + std::string(start,20) );
        }

        // Scan for end of digits.
        while( isdigit(*mJson) )
        {
            mJson++;
        }

        // Do we have a decimal?
        if( *mJson == '.' )
        {
            mJson++;
            // Now scan more more digits.
            while( isdigit(*mJson) )
            {
                mJson++;
            }

            // now see if there is an exponent. 
            if( *mJson == 'E' || *mJson == 'e' )
            {
                mJson++;

                // Now must be a sign.
                if( *mJson == '-' || *mJson == '+' )
                {
                    mJson++;

                    // after accounting the - or + there must be a number next.
                    if( isdigit(*mJson) == false )
                    {
                        throw std::runtime_error(std::string("Malformed exponent in number ") + std::string(start,20) );
                    }

                    // Now scan more more digits.
                    while( isdigit(*mJson) )
                    {
                        mJson++;
                    }
                }
                else
                {
                    throw std::runtime_error(std::string("Malformed exponent in number ") + std::string(start,20) );
                }			
            }
        }
        AssertMoreData("Abrupt end to json whilst reading number");
    }
};//end of struct JsonProcessor

///////////////////////////////////////////////////////////////////////////////////////////////////////////
};// namespace tinyjson



#endif //TINY_JSON_H