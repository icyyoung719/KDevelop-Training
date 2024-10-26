#include "Scanner.h"
#include "util.h"
#include<string>
#include <cctype> // For isdigit function

namespace json
{
	bool Scanner::IsAtEnd() {
		return current_pos_ >= source_.size();
	}
	char Scanner::Advance() {
		return source_[current_pos_++];
	}
	char Scanner::Peek() {
		return source_[current_pos_];
	}
	char Scanner::PeekNext() {
		return source_[current_pos_ + 1];
	}
	std::string Scanner::getValueString() {
		return value_string_;
	}
	double Scanner::getValueNumber() {
		return value_number_;
	}
	void Scanner::Rollback() {
		current_pos_ = prev_pos_;
	}
	void Scanner::ScanValid(const std::string& str) {
		//the first char has been confirmed
		if (IsAtEnd())
		{
			return;
		}
		if (source_.compare(current_pos_ - 1, str.length(), str) == 0) {
			current_pos_ += str.length() - 1;
		}
		else {
			Error("Scan unqualified result: " + str);
		}
	}
	void Scanner::ScanString() {
		//"key":value
		size_t pos = current_pos_;
		while (!IsAtEnd() && Peek() != '\"') {
			Advance();
		}
		if (IsAtEnd()) {
			Error("Invalid string: missing \"");
		}
		Advance();
		value_string_ = source_.substr(pos, current_pos_ - pos - 1);
	}
	void Scanner::ScanNumber() {
		//"num":120
		size_t pos = current_pos_ - 1;
		while (std::isdigit(Peek())) {
			Advance();
		}

		if (Peek() == '.' && std::isdigit(PeekNext())) {
			Advance();
			while (std::isdigit(Peek())) {
				Advance();
			}

		}

		value_number_ = std::stod(source_.substr(pos, current_pos_ - pos));
	}

	// return next token(exclude white space)
	Scanner::TokenType Scanner::nextToken()
	{
		if (IsAtEnd())
		{
			return TokenType::END_OF_SOURCE;
		}
		prev_pos_ = current_pos_;// record the position of the previous token
		char c = Advance();
		switch (c)
		{
		case '{':
			return TokenType::BGEIN_OF_OBJECT;
		case '}':
			return TokenType::END_OF_OBJECT;
		case '[':
			return TokenType::BEGIN_OF_ARRAY;
		case ']':
			return TokenType::END_OF_ARRAY;
		case ':':
			return TokenType::KEY_VALUE_SEPARATOR;
		case ',':
			return TokenType::VALUE_SEPARATOR;
		case 't':
			ScanValid(std::string("true"));
			return TokenType::VALUE_TRUE;
		case 'f':
			ScanValid(std::string("false"));
			return TokenType::VALUE_FALSE;
		case 'n':
			ScanValid(std::string("null"));
			return TokenType::VALUE_NULL;
		case ' ':
		case '\t':
		case '\n':
		case '\r':
			return nextToken();
		case '\"':
			ScanString();
			return TokenType::VALUE_STRING;
		case '-':
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			ScanNumber();
			return TokenType::VALUE_NUMBER;
		default:
			Error("Unexpected character:" + c);
		}
		return TokenType();
	}
}
