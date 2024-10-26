#include "parser.h"

namespace json {
    using TokenType = Scanner::TokenType;
    JsonElement* Parser::Parse() {
        JsonElement* elem = new JsonElement();
        TokenType token_type = scanner_.nextToken();

        switch (token_type)
        {
        case TokenType::END_OF_SOURCE: {
            break;
        }
        case TokenType::BGEIN_OF_OBJECT: {
            JsonObject* object = ParseObject();
            elem->value(object);
            break;
        }
        case TokenType::BEGIN_OF_ARRAY: {
            JsonArray* array = ParseArray();
            elem->value(array);
            break;
        }
        case TokenType::VALUE_STRING: {
            std::string* value = new std::string(scanner_.getValueString());
            elem->value(value);
            break;
        }
        case TokenType::VALUE_NUMBER: {
            elem->value(scanner_.getValueNumber());
            break;
        }
        case TokenType::VALUE_TRUE: {
            elem->value(true);
            break;
        }
        case TokenType::VALUE_FALSE: {
            elem->value(false);
            break;
        }
        case TokenType::VALUE_NULL: {
            break;
        }
        default:
            break;
        }
        return elem;
    }
    JsonObject* Parser::ParseObject() {
        JsonObject* res = new JsonObject();
        TokenType next = scanner_.nextToken();
        if (next == TokenType::END_OF_OBJECT) {
            return res;
        }
        scanner_.Rollback();

        while (true) {
            next = scanner_.nextToken();
            if (next != TokenType::VALUE_STRING) {
                Error("Error: Object key must be string");
            }
            std::string key = scanner_.getValueString();
            next = scanner_.nextToken();
            if (next != TokenType::KEY_VALUE_SEPARATOR) {
                Error("Error: Object key must be followed by ':'");
            }
            (*res)[key] = Parse();
            next = scanner_.nextToken();
            if (next == TokenType::END_OF_OBJECT) {
                break;
            }
            if (next == TokenType::END_OF_SOURCE) {
                break;
            }
            if (next != TokenType::VALUE_SEPARATOR) {
                Error("Error: Object values must be separated by ','");
            }
        }

        return res;
    }
    JsonArray* Parser::ParseArray() {
        JsonArray* res = new JsonArray();
        TokenType next = scanner_.nextToken();
        if (next == TokenType::END_OF_ARRAY) {
            return res;
        }
        scanner_.Rollback();

        while (true) {
            res->push_back(Parse());
            next = scanner_.nextToken();
            if (next == TokenType::END_OF_ARRAY) {
                break;
            }
            if (next == TokenType::END_OF_OBJECT) {
                break;
            }
            if (next != TokenType::VALUE_SEPARATOR) {
                Error("Error: Array values must be separated by ','");
            }
        }
        return res;
    }

}