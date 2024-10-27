#pragma once
#include<string>
#include<map>
#include<vector>
#include<sstream>
#include<memory>
#include "util.h"

namespace json {
	class JsonElement;
	using JsonObject = std::map<std::string, std::unique_ptr<JsonElement>>;
	using JsonArray = std::vector<std::unique_ptr<JsonElement>>;

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
		explicit JsonElement(const Type& type) ;
		JsonElement(std::unique_ptr<JsonObject> object) : type_(Type::JSON_OBJECT) { value(std::move(object)); }
		JsonElement(std::unique_ptr<JsonArray> array) : type_(Type::JSON_ARRAY) { value(std::move(array)); }
		JsonElement(std::string str) : type_(Type::JSON_STRING) { value(std::make_unique<std::string>(str)); }
		JsonElement(float number) : type_(Type::JSON_NUMBER) { value(number); }
		JsonElement(bool val) : type_(Type::JSON_BOOL) { value(val); }

		//constructer for =

		Type getType();
		void value(std::unique_ptr<JsonObject> value_object);
		void value(std::unique_ptr<JsonArray> value_array);
		void value(std::unique_ptr<std::string> value_string);
		void value(double value_number);
		void value(bool value_bool);

		//return C-like pointer to ensure unique_ptr's uniqueness
		JsonObject* asObject();
		JsonArray* asArray();
		const std::string& asString();
		double asNumber();
		bool asBool();
		std::string toString() const;

		// Overloading << for JSON_OBJECT and JSON_ARRAY
		friend std::ostream& operator<<(std::ostream& os, const JsonObject& object);
		friend std::ostream& operator<<(std::ostream& os, const JsonArray& array);

		JsonElement* getObject (const std::string& key);
		JsonElement* getArrayElement(const int& index);

	private:
		Type type_;
		struct Value
		{
			std::unique_ptr<JsonObject> value_object;
			std::unique_ptr<JsonArray> value_array;

			std::unique_ptr<std::string> value_string;
			double value_number;
			bool value_bool;
			Value() {}
			~Value() {}
		};
		Value value_;
	};
}
