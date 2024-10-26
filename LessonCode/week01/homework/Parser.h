#pragma once
#include "Scanner.h"
#include "JsonElement.h"

namespace json {
    class Parser {
    public:
        Parser(const Scanner& scanner) : scanner_(scanner) {}
        JsonElement* Parse();

    private:
        JsonObject* ParseObject();
        JsonArray* ParseArray();

    private:
        Scanner scanner_;
    };

}