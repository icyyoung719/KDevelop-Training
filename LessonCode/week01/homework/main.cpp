#include<iostream>
#include <regex>
#include<fstream>
#include<filesystem>
#include <memory>
#include <algorithm> //for std::transform
#include "Scanner.h"
#include "Parser.h"
#include "JsonElement.h"

int main() {
    using namespace json;

    std::string file_path;
    std::cout << "Please enter the JSON file path: "<<std::endl;
    std::cout<<R"(Example: C:\Users\icyyoung\Desktop\C++\JsonFile.json)"<<std::endl;
    std::getline(std::cin, file_path);

    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << file_path << std::endl;
        return 1;
    }
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();
    file.close();

    Scanner scanner(source);
    Parser  parser(std::move(scanner));
    //now the member scanner_ of parser is identical to scanner
    std::unique_ptr<JsonElement> element;

    try {
        element = parser.Parse();
    } catch (const std::logic_error& e) {
        std::cout << "Pre-defined exception caught when parsing: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cout << "Unexpected exception caught when parsing: " << e.what() << std::endl;
        return 1;
    }
    //We use the map container, which will automatically sort the key-value pairs according to the key
    std::cout << element->toString() << std::endl;

    std::string input_string{ "" };


    while (true) {
        std::cout << "----------------------------------------------------" << std::endl;
        std::cout << "Enter a command: "<<std::endl;
        std::cout<<R"(Enter "exit" or "q" to exit)"<<std::endl;
        std::cout<<R"(Enter "1" to output the whole Json)"<<std::endl;
        std::cout<<R"(Enter "2" to output the whole Json to a new file)"<<std::endl;
        std::cout<<R"(Enter "2" + blankspace + file_path(includeing file_name) to output the whole Json to a new file)"<<std::endl;
        std::cout<<R"(For JsonObjects in the root Json, inputs like ["player"]["name"] or ["scores"])"<<std::endl;
        std::cout<<R"(For JsonArray in the root Json, inputs like ["scores"][0])"<<std::endl;
        std::cout << R"(To change a value, inputs like ["player"]["name"]=new_value)"<<std::endl<<std::endl;
        std::getline(std::cin, input_string);  // scan the input string 

        if (input_string == "exit" || input_string == "q") {
            std::cout<<std::endl<<"Exiting..." << std::endl;
            break;
        }
        else if (input_string == "1") {
            std::cout << element->toString() << std::endl;
        }
        else if (input_string[0] == '2') {
            std::string output_file_path;
            if (input_string.size() != 1) {
                //if there is a file path
                output_file_path = input_string.substr(2);
            }
            else {
                //if there is no file path, use the same file name with "_out" appended
                std::regex file_regex_pattern("(.*[/\\\\])([^/\\\\]+)\\.json");
                output_file_path = std::regex_replace(file_path, file_regex_pattern, "$1$2_out.json");
            }
            std::ofstream outputFile(output_file_path);
            if (!outputFile.is_open()) {
                std::cerr << "Failed to open file for writing." << std::endl;
                return 1;
            }

            std::string output = element->toString();
            outputFile << output;
            outputFile.close();
            std::cout << "Json has been output to " << output_file_path << std::endl;
        }
        else {
            // TODO :match set mode 
            // Use std::regex to match the JSON path syntax
            std::regex re(R"(\[\"([^\"]+)\"\]|\[(\d+)\])");
            std::smatch match;
            std::sregex_iterator it(input_string.begin(), input_string.end(), re);
            std::sregex_iterator end;

            //current is used to trace current JsonElement, to output wanted JsonElement
            JsonElement* current = element.get();
            bool validPath = true;

            //no match
            if (it == end) {
                std::cout << "Invalid path format." << std::endl;
                validPath = false;
            }

            // Iterate over all matches in the input string
            for (; it != end; ++it) {
                std::smatch match = *it;

                if (match[1].matched) { // Match for ["key"]
                    std::string key = match[1].str();
                    if (current->getType() != JsonElement::Type::JSON_OBJECT) {
                        std::cout << "Invalid key, not a JsonObject." << std::endl;
                        validPath = false;
                        break;
                    }
                    if(current->asObject()->find(key) == current->asObject()->end()) {
                        std::cout << "Key not found." << std::endl;
                        validPath = false;
                        break;
                    }
                    current = current->getObject(key);
                }
                else if (match[2].matched) { // Match for [index]
                    int index = std::stoi(match[2].str());
                    if (current->getType() != JsonElement::Type::JSON_ARRAY) {
                        std::cout<<"Invalid index, not a JsonArray."<<std::endl;
                        validPath = false;
                        break;
                    }
                    if (index >= current->asArray()->size()) {
                        std::cout << "Index out of range." << std::endl;
                        validPath = false;
                        break;
                    }
                    current = current->getArrayElement(index);
                }
            }

            // If the path is valid, output the result
            if (validPath) {
                bool read_mode = input_string.back() == ']';
                //write_value is right behind =
                std::string write_value = input_string.substr(input_string.find('=') + 1);
                if (read_mode) {
                    std::cout << "Value: " << current->toString() << std::endl;
                }
                else {
                    std::string write_value = input_string.substr(input_string.find('=') + 1);
                    //rid the " in write_value
                    if (write_value.front() == '\"' && write_value.back() == '\"') {
                        write_value = write_value.substr(1, write_value.size() - 2);
                    }
                    JsonElement::Type type = current->getType();
                    auto isTrue = [](const std::string& s) -> bool {
                        std::string lowerStr = s;
                        std::transform(lowerStr.begin(), lowerStr.end(), lowerStr.begin(),
                            [](unsigned char c) { return std::tolower(c); });

                        return lowerStr == "true";
                        };
                    switch (type) {
                        case JsonElement::Type::JSON_STRING:
                            current->value(std::make_unique <std::string>(write_value));
                            break;
                        case JsonElement::Type::JSON_INT:
                            current->value(std::stoi(write_value));
                            break;
                        case JsonElement::Type::JSON_DOUBLE:
                            current->value(std::stod(write_value));
                            break;
                        case JsonElement::Type::JSON_BOOL:
                            current->value(isTrue(write_value));
                            break;
                        case JsonElement::Type::JSON_NULL:
                            current->value(std::make_unique < std::string >("null"));
                            break;
                        default:
                            std::cout << "Not suported type." << std::endl;
                    }
                    std::cout << "Value has been set to " << write_value << std::endl;
                }
            }
        }
    }

    return 0;
}