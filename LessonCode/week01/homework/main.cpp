#include <iostream>
#include <regex>
#include <fstream>
#include <filesystem>
#include <memory>
#include "Scanner.h"
#include "Parser.h"
#include "JsonElement.h"

using namespace std;
using namespace json;

int main() {
    string file_path;
    cout << "Please enter the JSON file path: \n";
    cout << R"(Example: C:\Users\icyyoung\Desktop\C++\Simple.json)" << endl;
    getline(cin, file_path);
    ifstream file(file_path);
    if (!file.is_open()) {
        cerr << "Failed to open the file: " << file_path << endl;
        return 1;
    }

    stringstream buffer;
    buffer << file.rdbuf();
    string source = buffer.str();
    file.close();

    Scanner scanner(source);
    Parser parser(move(scanner));
    unique_ptr<JsonElement> element;
    try {
        element = parser.Parse();
    } catch (const logic_error& e) {
        cout << "Pre-defined exception caught when parsing: " << e.what() << endl;
        return 1;
    } catch (const exception& e) {
        cout << "Unexpected exception caught when parsing: " << e.what() << endl;
        return 1;
    }
    cout << element->toString() << endl;

    while (true) {
        cout << "----------------------------------------------------" << endl;
        cout << "Enter a command:" << endl;
        cout << R"(Enter "exit" or "q" to exit)" << endl;
        cout << R"(Enter "1" to output the whole Json)" << endl;
        cout << R"(Enter "2" to output the whole Json to a new file)" << endl;
        cout << R"(Enter "2" + space + file_path to output to a new file)" << endl;
        cout << R"(Use paths like ["player"]["name"] or ["scores"][0])" << endl;
        cout << R"(Set values via ["key"]=value or ["array"][index]=value)" << endl;

        string input_string;
        getline(cin, input_string);

        if (input_string == "exit" || input_string == "q") {
            cout << "Exiting...\n";
            break;
        } else if (input_string == "1") {
            cout << element->toString() << endl;
        } else if (input_string[0] == '2') {
            string output_file_path;
            if (input_string.size() > 1) {
                output_file_path = input_string.substr(2);
            } else {
                regex file_pattern(R"(^(.*[/\\])?([^/\\]+)\.json$)");
                smatch match;
                if (regex_search(file_path, match, file_pattern)) {
                    output_file_path = match.prefix().str() + match[2].str() + "_out.json";
                } else {
                    output_file_path = file_path + "_out.json";
                }
            }
            ofstream outputFile(output_file_path);
            if (!outputFile.is_open()) {
                cerr << "Failed to open file for writing.\n";
                return 1;
            }
            outputFile << element->toString();
            outputFile.close();
            cout << "JSON saved to " << output_file_path << endl;
        } else {
            regex re(R"(\[\"([^\"]+)\"\]|\[(\d+)\])");
            sregex_iterator it(input_string.begin(), input_string.end(), re);
            sregex_iterator end;
            bool validPath = true;
            JsonElement* current = element.get();
            bool isWrite = (input_string.find('=') != string::npos);

            // Calculate total matches
            size_t totalMatches = distance(it, end);
            sregex_iterator temp_it = it;
            size_t currentMatch = 0;

            // Reset iterator
            it = sregex_iterator(input_string.begin(), input_string.end(), re);

            for (; it != end; ++it, ++currentMatch) {
                smatch match = *it;
                bool isLastSegment = (currentMatch == totalMatches - 1);

                if (match[1].matched) { // Handle key ["key"]
                    string key = match[1].str();
                    if (current->getType() != JsonElement::Type::JSON_OBJECT) {
                        validPath = false;
                        cout << "Error: Current node is not an object\n";
                        break;
                    }
                    auto& obj = *current->asObject();

                    // Only create missing keys in write mode
                    if (obj.find(key) == obj.end()) {
                        if (isWrite) {
                            if (isLastSegment) {
                                obj[key] = make_unique<JsonElement>();
                                cout << "Added new key: " << key << endl;
                            } else {
                                obj[key] = make_unique<JsonElement>(make_unique<JsonObject>());
                                cout << "Created object for key: " << key << endl;
                            }
                        } else {
                            validPath = false;
                            cout << "Key not found: " << key << endl;
                            break;
                        }
                    }
                    current = current->getObject(key);
                } else if (match[2].matched) { // Handle index [index]
                    int index = stoi(match[2].str());
                    if (current->getType() != JsonElement::Type::JSON_ARRAY) {
                        validPath = false;
                        cout << "Error: Current node is not an array\n";
                        break;
                    }
                    auto& arr = *current->asArray();

                    // Only expand array in write mode
                    if (index >= arr.size()) {
                        if (isWrite) {
                            arr.resize(index + 1);
                            for (size_t i = arr.size() - 1; i <= index; ++i) {
                                arr[i] = make_unique<JsonElement>();
                            }
                            cout << "Expanded array to index " << index << endl;
                        } else {
                            validPath = false;
                            cout << "Index out of range: " << index << endl;
                            break;
                        }
                    }
                    current = current->getArrayElement(index);
                }
            }

            if (validPath) {
                if (isWrite) {
                    size_t equalsPos = input_string.find('=');
                    string writeValue = input_string.substr(equalsPos + 1);
                    writeValue = writeValue.substr(writeValue.find_last_of(']') + 1); // Remove trailing path
                    writeValue = writeValue.substr(0, writeValue.find_first_of('\n'));

                    // Remove quotes
                    if (!writeValue.empty() && writeValue.front() == '"' && writeValue.back() == '"') {
                        writeValue = writeValue.substr(1, writeValue.size() - 2);
                    }

                    // Type inference
                    JsonElement::Type targetType = JsonElement::Type::JSON_STRING;
                    if (writeValue == "true" || writeValue == "false") {
                        targetType = JsonElement::Type::JSON_BOOL;
                    } else if (isdigit(writeValue[0]) || (writeValue.size() > 1 && writeValue[0] == '-')) {
                        try {
                            stoi(writeValue);
                            targetType = JsonElement::Type::JSON_INT;
                        } catch (const invalid_argument&) {
                            try {
                                stod(writeValue);
                                targetType = JsonElement::Type::JSON_DOUBLE;
                            } catch (...) {
                                targetType = JsonElement::Type::JSON_STRING;
                            }
                        }
                    }

                    // Set the value
                    switch (targetType) {
                        case JsonElement::Type::JSON_STRING:
                            current->value(make_unique<string>(writeValue));
                            break;
                        case JsonElement::Type::JSON_BOOL:
                            current->value(writeValue == "true");
                            break;
                        case JsonElement::Type::JSON_INT:
                            current->value(stoi(writeValue));
                            break;
                        case JsonElement::Type::JSON_DOUBLE:
                            current->value(stod(writeValue));
                            break;
                        default:
                            cout << "Unsupported value type\n";
                            break;
                    }
                    cout << "Value set to: " << writeValue << endl;
                } else {
                    cout << "Value: " << current->toString() << endl;
                }
            }
        }
    }
    return 0;
}