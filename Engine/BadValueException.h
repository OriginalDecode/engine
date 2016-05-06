#pragma once

#include <stdexcept>
#include <DL_Debug.h>
#define _CRT_SECURE_NO_WARNINGS
class BadValueException : public std::runtime_error
{
public:
	BadValueException(int value )
		: std::runtime_error("Bad Value Exception")
		, myValue(value)
		, message(new char[200]) {
		what();
	};

	~BadValueException() { delete[] message; message = nullptr; }

	virtual const char* what() const throw()
	{
		sprintf(message, "%s. Value: %i", std::runtime_error::what(), GetValue());
		DL_MESSAGE("%s", message);
		return message;
	}

	int GetValue() const
	{
		return myValue;
	}
private:
	int myValue;
	char* message;

};

#define BAD_VALUE(statement, value) if(statement) throw BadValueException(value)