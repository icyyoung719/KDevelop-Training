#include<iostream>
#include <regex>
#include<fstream>
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
    Parser  parser(scanner);
    //now the member scanner_ of parser is identical to scanner

    JsonElement* element = parser.Parse();
    //We use the map container, which will automatically sort the key-value pairs according to the key
    std::cout << element->toString() << std::endl;

    //output the value of "name" inputed in JsonElement
    //for JsonObjects in the root Json, inputs like ["player"]["name"]
    //for JsonArray in the root Json, inputs like ["scores"][0]

    std::string input_string{ "" };


    while (true) {
        std::cout << "----------------------------------------------------" << std::endl;
        std::cout << "Enter a command: "<<std::endl;
        std::cout<<R"(Enter "exit" or "q" to exit)"<<std::endl;
        std::cout<<R"(Enter "1" to output the whole Json)"<<std::endl;
        std::cout<<R"(For JsonObjects in the root Json, inputs like ["player"]["name"] or ["scores"])"<<std::endl;
        std::cout<<R"(For JsonArray in the root Json, inputs like ["scores"][0])"<<std::endl<<std::endl;
        std::getline(std::cin, input_string);  // scan the input string 
        if (input_string == "exit" || input_string == "q") {
            std::cout<<std::endl<<"Exiting..." << std::endl;
            break;
        }
        else if (input_string == "1") {
            std::cout << element->toString() << std::endl;
        }
        else {
            // Use std::regex to match the JSON path syntax
            std::regex re(R"(\[\"([^\"]+)\"\]|\[(\d+)\])");
            std::smatch match;
            std::sregex_iterator it(input_string.begin(), input_string.end(), re);
            std::sregex_iterator end;

            JsonElement* current = element;
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

                // Check if current element is valid
                if (current == nullptr) {
                    std::cout << "Invalid path or element not found." << std::endl;
                    validPath = false;
                    break;
                }
            }

            // If the path is valid, output the result
            if (validPath && current != nullptr) {
                std::cout << "Value: " << current->toString() << std::endl;
            }
        }
    }

    delete element;
    return 0;
}