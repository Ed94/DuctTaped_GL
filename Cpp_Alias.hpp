/*
Title: C++ Aliasing Library
Author: Edward R. Gonzalez
Date: 

Description:
This is a segment of the C++ Assist Library I use for other code.

This merely removes the need to use operators I don't like and wraps them in easier to manage typedefs, functions or macros
*/


#pragma once

// Macros

#define sfn \
auto

#define deduce \
auto

#define Ref(_type) \
_type&

#define rRef(_type) \
_type&&


// Aliases

// Fundamental

using uInt64 = unsigned long long int;

// Pointers

template<typename Type>
using ptr = Type*;

template<typename ReturnType, typename... ParamTypes>
using FnPtr = ReturnType(*)(ParamTypes...);

// Strings

template<typename CharType>
using RawString = ptr<CharType>;


// Enum

enum class ExitCode
{
	Success = EXIT_SUCCESS,
	Failed  = EXIT_FAILURE
};


// Functions

// Ptr

template<typename Type>
sfn Address(Ref(Type) _instance) -> ptr<Type>
{
	return &_instance;
}

template<typename Type>
sfn Dref(ptr<Type> _type) -> Ref(Type)
{
	return *_type;
}


// Exit

sfn Exit(ExitCode _code)
{
	exit(int(_code));
}
