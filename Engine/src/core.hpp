#pragma once

#ifdef BUILD_DLL
#	define TANK_API __declspec(dllexport)
#else
#	define TANK_API __declspec(dllimport)
#endif