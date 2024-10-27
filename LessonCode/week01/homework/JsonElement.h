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

		JsonElement(const Type& type) ;

		JsonElement(JsonObject* object) : type_(Type::JSON_OBJECT) { value(object); }
		JsonElement(JsonArray* array) : type_(Type::JSON_ARRAY) { value(array); }
		JsonElement(std::string* str) : type_(Type::JSON_STRING) { value(str); }
		JsonElement(float number) : type_(Type::JSON_NUMBER) { value(number); }
		JsonElement(bool val) : type_(Type::JSON_BOOL) { value(val); }

		~JsonElement();

		Type getType();
		void value(JsonObject* value_object);
		void value(JsonArray* value_array);
		void value(std::string* value_string);
		void value(double value_number);
		void value(bool value_bool);

		JsonObject* asObject();
		JsonArray* asArray();
		std::string* asString();
		double asNumber();
		bool asBool();
		std::string toString() const;

		// Overloading << for JSON_OBJECT and JSON_ARRAY
		friend std::ostream& operator<<(std::ostream& os, const JsonObject& object);
		friend std::ostream& operator<<(std::ostream& os, const JsonArray& array);

		JsonElement* getObject(const std::string& key);
		JsonElement* getArrayElement(const int& index);

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
