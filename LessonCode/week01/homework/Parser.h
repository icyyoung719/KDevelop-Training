#pragma once
#include "Scanner.h"
#include "JsonElement.h"

namespace json {
    class Parser {
    public:
        Parser(const Scanner& scanner) : scanner_(scanner) {}
        std::unique_ptr<JsonElement> Parse();

    private:
        std::unique_ptr<JsonObject> ParseObject();
        std::unique_ptr<JsonArray> ParseArray();

    private:
        Scanner scanner_;
    };

}