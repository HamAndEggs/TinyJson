#ifndef TINY_JSON_H
#define TINY_JSON_H

#include <stdexcept>
#include <memory>
#include <map>
#include <vector>
#include <string>
#include <utility>
#include <optional>

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

// I am using the shared pointer because it makes it easy to build the object and return it.
// If I did not then I would be returning by value that, depending on the contents of the value, could be very expensive!
typedef std::shared_ptr<JsonValue> JsonValuePtr;
typedef std::pair<std::string,JsonValuePtr> JsonKeyValue;
typedef std::vector<JsonKeyValue> JsonObject;

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
	std::vector<JsonValuePtr> mArray;
};


struct JsonProcessor
{
	JsonProcessor(const std::string& pJsonString)
    {
        if( pJsonString.size() < 2 )
        {
            throw std::runtime_error("Empty string passed into ParseJson");
        }

        const char* json = pJsonString.c_str();

        MakeObject(json,mRoot);// This function will leave json pointing to the next non white space.

        // Skip white space that maybe after it.
        SkipWhiteSpace(json);

        // Now should be a NULL
        if( *json != 0)
        {
            throw std::runtime_error("Data found after root object, invalid Json");
        }

    }

    JsonObject mRoot;

private:

    void MakeObject(const char* &pJson,JsonObject& rObject)const
    {
        // Search for the start of the object.
        if( FindNextControlChar(pJson,'{') )
        {
            do
            {
                pJson++;// Skip object start char or comma for more key value pairs.
                const std::string objKey = ReadString(pJson);
                // Now parse it's value.
                if( FindNextControlChar(pJson,':') )
                {
                    pJson++;
                    JsonValuePtr newValue = MakeValue(pJson);
                    if( newValue != nullptr )
                    {
                        rObject.emplace_back(objKey,newValue);
                        // Now see if there are more key valuer pairs to add to the object or if we're done.
                        // Skip white space, then see if next char is a } (for end of object) or a , for more key value pairs.
                        // We leave the pointer there and drop out the function.
                        // Down to the caller to test if they carry on or finish the current object.
                        if( SkipWhiteSpace(pJson) )
                        {
                            if( *pJson != '}' && *pJson != ',' )
                            {
                                throw std::runtime_error("Json format error detected, did you forget a comma between key value pairs? For key " + objKey);
                            }
                        }
                        else
                        {
                            throw std::runtime_error("Abrupt end to json string detected for key " + objKey);
                        }
                    }
                    else
                    {
                        throw std::runtime_error("Json format error detected, value unreadable for key " + objKey);
                    }
                }
                else
                {
                    throw std::runtime_error("Json format error detected, seperator character ':' not found for key " + objKey);
                }
            }while (*pJson == ',');

            // Validate end of object.
            if( *pJson == '}' )
            {
                pJson++;
            }
            else
            {
                throw std::runtime_error("End of root object not found, invalid Json");
            }
        }
        else
        {
            throw std::runtime_error("Start of object not found, invalid Json");
        }
    }

    JsonValuePtr MakeValue(const char* &pJson)const
    {
        // skip space and then see if it's an object, string, value or special state (TRUE,FALSE,NULL).
        if( SkipWhiteSpace(pJson) )
        {
            JsonValuePtr newValue = std::make_shared<JsonValue>();
            const char* valueStart = pJson;
            switch( *pJson )
            {
            case 0:
                break;

            case '{':
                newValue->mType = JTYPE_OBJECT;
                MakeObject(pJson,newValue->mObject);
                return newValue;

            case '[':
                do
                {
                    newValue->mType = JTYPE_ARRAY;
                    newValue->mArray.emplace_back(MakeValue(pJson));
                    SkipWhiteSpace(pJson);
                }while(',');

                // Check we did get to the end.
                if( *pJson != ']' )
                {
                    throw std::runtime_error("Json format error detected, array not terminated with ']'");
                }
                pJson++;
                return newValue;
    
            case '\"':
                newValue->mType = JTYPE_STRING;
                newValue->mValue = ReadString(pJson);
                return newValue;

            case 'T':
            case 't':
                if( tolower(pJson[1]) == 'u' && tolower(pJson[1]) == 'r' && tolower(pJson[1]) == 'e' )
                {
                    pJson += 4;
                    newValue->mType = JTYPE_TRUE;
                    return newValue;
                }
                else
                {
                    throw std::runtime_error(std::string("Invalid character ") + std::string(pJson,10) + " found in json value definition");
                }
                break;

            case 'F':
            case 'f':
                if( tolower(pJson[1]) == 'a' && tolower(pJson[1]) == 'l' && tolower(pJson[1]) == 's' && tolower(pJson[1]) == 'e' )
                {
                    pJson += 5;
                    newValue->mType = JTYPE_FALSE;
                    return newValue;
                }
                else
                {
                    throw std::runtime_error(std::string("Invalid character ") + std::string(pJson,10) + " found in json value definition");
                }
                break;

            case 'N':
            case 'n':
                if( tolower(pJson[1]) == 'u' && tolower(pJson[1]) == 'l' && tolower(pJson[1]) == 'l' )
                {
                    pJson += 4;
                    newValue->mType = JTYPE_NULL;
                    return newValue;
                }
                else
                {
                    throw std::runtime_error(std::string("Invalid character ") + std::string(pJson,10) + " found in json value definition");
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
                if( FindEndOfNumber(pJson) )
                {
                    newValue->mType = JTYPE_NUMBER;
                    newValue->mValue.assign(valueStart,pJson-valueStart);
                    return newValue;
                }
                break;

            default:
                throw std::runtime_error(std::string("Invalid character ") + std::string(pJson,10) + " found at start of json value definition");
            }
        }
        return nullptr;
    }

    bool FindNextControlChar(const char* &pJson,char pToFind)const
    {
//        std::cout << "FindNextControlChar " << pJson << '\n';
        // Stop looking when we get to the end of the json.
        while( *pJson != 0 )
        {
            const char c = *pJson;
            if( isspace(c) )
            {// Skip white space.
                pJson++;
            }
            else if( c == pToFind )
            {// We found it! :)
                return true;
            }
            else
            {
                //Invalid char, should have been pToFind.
                throw std::runtime_error(std::string("Error in parsing json, expected ") + pToFind + " found " + c);
            }
        }
        return false;
    }

    bool SkipWhiteSpace(const char* &pJson)const
    {
//        std::cout << "SkipWhiteSpace " << pJson << '\n';

        while( *pJson != 0 )
        {// As per Json spec, look for characters that are not a space, linefeed, carrage return or horizontal tab. isspace does this.
            if( isspace(*pJson) == 0 )
            {
                return true;
            }
            pJson++;
        }
        return false;
    }

    std::string ReadString(const char* &pJson)const
    {
//        std::cout << "ReadString " << pJson << '\n';
        // First find the start of the string
        if( FindNextControlChar(pJson,'\"') )
        {
            pJson++; // Skip "
            const char* stringStart = pJson;
            // Now scan till we hit the next "
            while( *pJson != '\"' )
            {
            // Did we hit the end?
                if( *pJson == 0 )
                {
                    throw std::runtime_error("Abrupt end to json whilst reading string");
                }
                pJson++;
            }
            const size_t len = pJson - stringStart;
            pJson++; // Skip "
            if( len > 0 )
            {
                return std::string(stringStart,len);
            }
            // Empty string valid.
            return "";
        }

        throw std::runtime_error("Json format error detected, expected start of string, did you forget to put the string in quotes?");
        // Should never get here, no return needed.
    }

    bool FindEndOfNumber(const char* &pJson)const
    {
        // As per Json spec, keep going to we see end of accepted number components.
        // There is an order that you do this in, see https://www.json.org/json-en.html
        // for more information on reading a number.
        const char* start = pJson;// This is really for the excpetions to help finding the errors in the json string.

        if( *pJson == 0 )
        {
            return false;
        }

        if( *pJson == '-' )
        {
            pJson++;
        }

        // after accounting the - there must be a number next.
        if( isdigit(*pJson) == false )
        {
            throw std::runtime_error(std::string("Malformed number ") + std::string(start,20) );
        }

        // Scan for end of digits.
        while( isdigit(*pJson) )
        {
            pJson++;
        }

        // Do we have a decimal?
        if( *pJson == '.' )
        {
            pJson++;
            // Now scan more more digits.
            while( isdigit(*pJson) )
            {
                pJson++;
            }

            // now see if there is an exponent. 
            if( *pJson == 'E' || *pJson == 'e' )
            {
                pJson++;

                // Now must be a sign.
                if( *pJson == '-' || *pJson == '+' )
                {
                    pJson++;

                    // after accounting the - or + there must be a number next.
                    if( isdigit(*pJson) == false )
                    {
                        throw std::runtime_error(std::string("Malformed exponent in number ") + std::string(start,20) );
                    }

                    // Now scan more more digits.
                    while( isdigit(*pJson) )
                    {
                        pJson++;
                    }
                }
                else
                {
                    throw std::runtime_error(std::string("Malformed exponent in number ") + std::string(start,20) );
                }			
            }
        }

        // All good?
        return *pJson != 0; // Can never end in a null, last part of json is always a } character.
    }
};//endof struct JsonObject

///////////////////////////////////////////////////////////////////////////////////////////////////////////
};// namespace tinyjson



#endif //TINY_JSON_H