#pragma once

/* Blind Engine Precomiled Header*/

// This PCH includes all standard library headers that are in use by blind
// MSVC is the used C++ Compiler for Blind so a PCH source file is also created.

#ifdef BLIND_PLATFORM_WINDOWS
	#include <Windows.h>
#endif

// STD LIB includes
#include <sstream>
#include <iostream>
#include <functional>
#include <algorithm>
#include <utility>

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <array>

#include <Blind/Core/Log.h>
#include <Blind/Debug/Instrumentor.h>