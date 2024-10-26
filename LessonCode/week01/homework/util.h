#pragma once
#include<string>
#include<stdexcept>

namespace json{
    inline void Error(std::string message){
        throw std::logic_error(message);
    }
}