// Copyright (C) 2020 Arno Ansems
// 
// This program is free software: you can redistribute it and/or modify 
// it under the terms of the GNU General Public License as published by 
// the Free Software Foundation, either version 3 of the License, or 
// (at your option) any later version. 
// 
// This program is distributed in the hope that it will be useful, 
// but WITHOUT ANY WARRANTY; without even the implied warranty of 
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
// GNU General Public License for more details. 
// 
// You should have received a copy of the GNU General Public License 
// along with this program.  If not, see http://www.gnu.org/licenses/
// Should already be defined in MSVC

//
// Macros
//
// PreProcessor macros and definitions.
//
#pragma once


// Calling conventions are operating system dependent
// thus __stdcall is not defined outside of a Windows environment.
// MSVC defines the CALLBACK macro for this purpose so we define it for mingw
// and non-windows compilers.

#ifndef CALLBACK
#ifdef _WIN32
#define CALLBACK __stdcall
#else
#define CALLBACK
#endif
#endif


// There is no case insensitive comparison function in the C/C++ standards.
// Microsoft and POSIX independently specify such a function tho.
// These functions however have different names but luckily have the same arguments
// so that the name can be inserted via a preprocessor variable.

// MS compiler
#if defined (_WIN32)
#define STR_CASE_CMP _strcmpi
// POSIX compliant compiler
#elif defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
#define STR_CASE_CMP strcasecmp
// This should cover all relevant compilers
// If a compiler is not covered please add an appropriate function here
#endif

