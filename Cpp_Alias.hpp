/*
Title: C++ Aliasing Library
Author: Edward R. Gonzalez
Date: 

Description:
This is a segment of the C++ Assist Library I use for other code.

This merely removes the need to use operators I don't like and wraps them in easier to manage typedefs, functions or macros
*/


#pragma once

#include <algorithm   >
#include <chrono      >
#include <cstdarg     >   
#include <cstddef     >
#include <exception   >
#include <forward_list>
#include <fstream     >
#include <functional  >
#include <iostream    >
#include <cmath       >
#include <map         >
#include <memory      >
#include <queue       >
#include <sstream     >
#include <stdexcept   >
#include <string      >
#include <vector      >
#include <thread      >
#include <tuple       >
#include <typeindex   >
#include <utility     >



// Macros

#define sfn \
auto

#define deduce \
auto

#define ro \
const

#define Ref(...) \
__VA_ARGS__&

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

template<typename Type>
using SPtr = std::shared_ptr<Type>;

template<typename Type>
using UPtr = std::unique_ptr<Type>;


// Delegating

template<typename FnType>
using Delegate = std::function<FnType>;

template<typename ReturnType, typename... ParamTypes>
using Func = ReturnType(ParamTypes...);



// Strings

template<typename CharType>
using RawString = ptr<CharType>;


using std::cout; 
using std::endl;

using std::ifstream    ;
using std::ios         ;
using std::string      ;
using std::stringstream;

using DataSize = std::size_t;
using Thread   = std::thread;

template<typename... ObjectTypes>
using Tuple = std::tuple<ObjectTypes...>;



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
sfn Dref(ro ptr<Type> _type) -> Ref(Type)
{
	return *_type;
}

template<typename Type, typename... ParamTypes>
sfn MakeUPtr(rRef(ParamTypes)... _params) -> UPtr<Type>
{
	return std::make_unique<Type>(_params...);
}

template<typename Type, typename... ParamTypes>
sfn MakeSPtr(rRef(ParamTypes)... _params) -> SPtr<Type>
{
	return std::make_shared<Type>(_params...);
}



// Exit

sfn Exit(ro ExitCode _code)
{
	exit(int(_code));
}

// Error Stuff

sfn ErrorRuntime(ro Ref(std::runtime_error) _error)
{
	std::cout << "Runtime error occurred: " << _error.what() << std::endl;

	return;
}

