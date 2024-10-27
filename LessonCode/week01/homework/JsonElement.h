#pragma once
#include<string>
#include<map>
#include<vector>
#include<sstream>
#include "util.h"

namespace json {
	class JsonElement;
	using JsonObject = std::map<std::string, JsonElement*>;
	using JsonArray = std::vector<JsonElement*>;

	namespace {
		int g_indentLevel = 0;
	}

	class JsonElement {
	public:
		enum class Type {
			JSON_STRING,
			JSON_NUMBER,
			JSON_BOOL,
			JSON_NULL,
			JSON_ARRAY,
			JSON_OBJECT
		};




		//constructer
		JsonElement() : type_(Type::JSON_NULL) {}

		JsonElement(const Type& type) : type_(type) {
			switch (type) {
			case Type::JSON_OBJECT:
				value_.value_object = new std::map<std::string, JsonElement*>();
				break;
			case Type::JSON_ARRAY:
				value_.value_array = new std::vector<JsonElement*>();
				break;
			case Type::JSON_STRING:
				value_.value_string = new std::string("");
				break;
			case Type::JSON_NUMBER:
				value_.value_number = 0;
				break;
			case Type::JSON_BOOL:
				value_.value_bool = false;
				break;
			case Type::JSON_NULL:
				break;
			default:
				break;
			}
		};


		JsonElement(JsonObject* object) : type_(Type::JSON_OBJECT) { value(object); }
		JsonElement(JsonArray* array) : type_(Type::JSON_ARRAY) { value(array); }
		JsonElement(std::string* str) : type_(Type::JSON_STRING) { value(str); }
		JsonElement(float number) : type_(Type::JSON_NUMBER) { value(number); }
		JsonElement(bool val) : type_(Type::JSON_BOOL) { value(val); }

		~JsonElement() {
			if (type_ == Type::JSON_OBJECT) {
				JsonObject* object = value_.value_object;
				for (auto iter = object->begin(); iter != object->end(); ++iter) {
					delete iter->second;
				}
				delete object;
			}
			else if (type_ == Type::JSON_ARRAY) {
				JsonArray* array = value_.value_array;
				for (auto iter = array->begin(); iter != array->end(); ++iter) {
					delete* iter;
				}
				delete array;
			}
			else if (type_ == Type::JSON_STRING) {
				std::string* val = value_.value_string;
				delete val;
			}
		}

		Type getType() {
			return type_;
		}

		void value(JsonObject* value_object) {
			type_ = Type::JSON_OBJECT;
			value_.value_object = value_object;
		}
		void value(JsonArray* value_array) {
			type_ = Type::JSON_ARRAY;
			value_.value_array = value_array;
		}
		void value(std::string* value_string) {
			type_ = Type::JSON_STRING;
			value_.value_string = value_string;
		}
		void value(double value_number) {
			type_ = Type::JSON_NUMBER;
			value_.value_number = value_number;
		}
		void value(bool value_bool) {
			type_ = Type::JSON_BOOL;
			value_.value_bool = value_bool;
		}

		JsonObject* asObject() {
			if (type_ == Type::JSON_OBJECT)
				return value_.value_object;
			Error("Type of JsonElement isn't JSON_OBJECT");
		}
		JsonArray* asArray() {
			if (type_ == Type::JSON_ARRAY)
				return value_.value_array;
			Error("Type of JsonElement isn't JSON_ARRAY");
		}
		std::string* asString() {
			if (type_ == Type::JSON_STRING)
				return value_.value_string;
			Error("Type of JsonElement isn't JSON_STRING");
		}
		double asNumber() {
			if (type_ == Type::JSON_NUMBER)
				return value_.value_number;
			Error("Type of JsonElement isn't JSON_NUMBER");
		}
		bool asBool() {
			if (type_ == Type::JSON_BOOL)
				return value_.value_bool;
			Error("Type of JsonElement isn't JSON_BOOL");
		}

		std::string toString() const {
			std::stringstream ss;
			std::string indent(g_indentLevel, '\t');

			switch (type_) {
			case Type::JSON_STRING:
				ss <<"\"" << *(value_.value_string) << "\"";
				break;
			case Type::JSON_NUMBER:
				ss << value_.value_number;
				break;
			case Type::JSON_BOOL:
				ss << (value_.value_bool ? "true" : "false");
				break;
			case Type::JSON_NULL:
				ss << "null";
				break;
			case Type::JSON_ARRAY:
				ss << *value_.value_array;
				break;
			case Type::JSON_OBJECT:
				ss << *value_.value_object;
				break;
			default:
				Error("Unknown type of JsonElement");
			}
			return ss.str();
		}

		// Overloading << for JSON_OBJECT
		friend std::ostream& operator<<(std::ostream& os, const JsonObject& object) {
			os << "{"<< std::endl;
            g_indentLevel++;
			std::string indent(g_indentLevel, '\t');
			for (auto iter = object.begin(); iter != object.end(); ++iter) {
				os << indent << '\"' << iter->first << "\": " << iter->second->toString();
				if (std::next(iter) != object.end()) {
					os << ", "<<std::endl;
				}
			}
			indent.pop_back();
            g_indentLevel--;
			os <<std::endl<< indent << "}";
			return os;
		}
		friend std::ostream& operator<<(std::ostream& os, const JsonArray& array) {
			os << "["<< std::endl;
			g_indentLevel++;
			std::string indent(g_indentLevel, '\t');
			for (size_t i = 0; i < array.size(); i++) {
				os << indent << array[i]->toString();
				if (i != array.size() - 1) {
					os << ", "<<std::endl;
				}
			}
			indent.pop_back();
			g_indentLevel--;
			os << std::endl << indent << "]";
			return os;
		}

		// getObject()
		JsonElement* getObject(const std::string& key) {
			if (type_ != Type::JSON_OBJECT) {
				Error("Type of JsonElement isn't JSON_OBJECT");
			}
            return value_.value_object->at(key);
		}
		// getArray()
		JsonElement* getArrayElement(const int& index) {
            if (type_ != Type::JSON_ARRAY) {
				Error("Type of JsonElement isn't JSON_ARRAY");
			}
			return value_.value_array->at(index);
        }

	private:
		Type type_;
		union Value
		{
			JsonObject* value_object;
			JsonArray* value_array;

			std::string* value_string;
			double value_number;
			bool value_bool;

		};
		Value value_;
	};
}
