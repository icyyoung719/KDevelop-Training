#include "JsonElement.h"

namespace json {
	JsonElement::JsonElement(const Type& type) : type_(type) {
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

	JsonElement::~JsonElement() {
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

	void JsonElement::value(JsonObject* value_object) {
		type_ = Type::JSON_OBJECT;
		value_.value_object = value_object;
	}
	void JsonElement::value(JsonArray* value_array) {
		type_ = Type::JSON_ARRAY;
		value_.value_array = value_array;
	}
	void JsonElement::value(std::string* value_string) {
		type_ = Type::JSON_STRING;
		value_.value_string = value_string;
	}
	void JsonElement::value(double value_number) {
		type_ = Type::JSON_NUMBER;
		value_.value_number = value_number;
	}
	void JsonElement::value(bool value_bool) {
		type_ = Type::JSON_BOOL;
		value_.value_bool = value_bool;
	}
	JsonElement::Type JsonElement::getType() {
		return type_;
	}

	JsonObject* JsonElement::asObject() {
		if (type_ == Type::JSON_OBJECT)
			return value_.value_object;
		Error("Type of JsonElement isn't JSON_OBJECT");
	}
	JsonArray* JsonElement::asArray() {
		if (type_ == Type::JSON_ARRAY)
			return value_.value_array;
		Error("Type of JsonElement isn't JSON_ARRAY");
	}
	std::string* JsonElement::asString() {
		if (type_ == Type::JSON_STRING)
			return value_.value_string;
		Error("Type of JsonElement isn't JSON_STRING");
	}
	double JsonElement::asNumber() {
		if (type_ == Type::JSON_NUMBER)
			return value_.value_number;
		Error("Type of JsonElement isn't JSON_NUMBER");
	}
	bool JsonElement::asBool() {
		if (type_ == Type::JSON_BOOL)
			return value_.value_bool;
		Error("Type of JsonElement isn't JSON_BOOL");
	}

	std::string JsonElement::toString() const {
		std::stringstream ss;
		std::string indent(g_indentLevel, '\t');

		switch (type_) {
		case Type::JSON_STRING:
			ss << "\"" << *(value_.value_string) << "\"";
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

	std::ostream& operator<<(std::ostream& os, const JsonObject& object) {
		os << "{" << std::endl;
		g_indentLevel++;
		std::string indent(g_indentLevel, '\t');
		for (auto iter = object.begin(); iter != object.end(); ++iter) {
			os << indent << '\"' << iter->first << "\": " << iter->second->toString();
			if (std::next(iter) != object.end()) {
				os << ", " << std::endl;
			}
		}
		indent.pop_back();
		g_indentLevel--;
		os << std::endl << indent << "}";
		return os;
	}
	std::ostream& operator<<(std::ostream& os, const JsonArray& array) {
		os << "[" << std::endl;
		g_indentLevel++;
		std::string indent(g_indentLevel, '\t');
		for (size_t i = 0; i < array.size(); i++) {
			os << indent << array[i]->toString();
			if (i != array.size() - 1) {
				os << ", " << std::endl;
			}
		}
		indent.pop_back();
		g_indentLevel--;
		os << std::endl << indent << "]";
		return os;
	}

	JsonElement* JsonElement::getObject(const std::string& key) {
		if (type_ != Type::JSON_OBJECT) {
			Error("Type of JsonElement isn't JSON_OBJECT");
		}
		return value_.value_object->at(key);
	}
	JsonElement* JsonElement::getArrayElement(const int& index) {
		if (type_ != Type::JSON_ARRAY) {
			Error("Type of JsonElement isn't JSON_ARRAY");
		}
		return value_.value_array->at(index);
	}
}