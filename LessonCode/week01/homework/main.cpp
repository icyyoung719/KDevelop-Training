#include<iostream>
#include <regex>
#include "Scanner.h"
#include "Parser.h"
#include "JsonElement.h"
#include "util.h"

int main() {
    using namespace json;
    auto source = R"(
        {
            "name": "John",
            "age": 30,
            "age2": true,
            "player":{
                "name":"Tom",
                "age": 20
            },
            "scores":[
                1,
                2,
                3,
            ]
        }
    )";


    Scanner scanner(source);
    Parser  parser(scanner);
    //now the member scanner_ of parser is identical to scanner

    JsonElement* element = parser.Parse();
    //We use the map container, which will automatically sort the key-value pairs according to the key
    std::cout << element->toString() << std::endl;

    //output the value of "name" inputed in JsonElement
    //for JsonObjects in the root Json, inputs like ["player"]["name"]
    //for JsonArray in the root Json, inputs like ["scores"][0]
    std::cout << element->getObject(std::string("name"))->toString() << std::endl;
    std::cout << element->getObject(std::string("scores"))->getArrayElement(0)->toString() << std::endl;

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
                    current = current->getObject(key);
                }
                else if (match[2].matched) { // Match for [index]
                    int index = std::stoi(match[2].str());
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
                std::cout << "Result: " << current->toString() << std::endl;
            }
        }
    }

    delete element;
    return 0;
}