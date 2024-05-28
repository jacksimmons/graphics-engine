#pragma once

#ifdef PLATFORM_WINDOWS
#	ifdef BUILD_DLL
#		define TANK_API __declspec(dllexport)
#	else
#		define TANK_API __declspec(dllimport)
#	endif
#else
# error Only Windows supported.
#endif