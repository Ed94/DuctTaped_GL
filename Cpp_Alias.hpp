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


// Aliases

// Fundamental

using uInt64 = unsigned long long int;


// Pointers

using std::shared_ptr;
using std::unique_ptr;

using std::make_shared;
using std::make_unique;

using std::make_pair;


// Delegating

using std::function;


// Strings

using std::cout; 
using std::endl;

using std::ifstream    ;
using std::ios         ;
using std::string      ;
using std::stringstream;

using std::size_t;
using std::thread;

using std::tuple;

using std::type_index;

using std::vector;


void DoNothing() {};
