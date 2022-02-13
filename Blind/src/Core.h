#pragma once
/* Blind Engine Core Header */

/* This header defines major defines about the building and linking of the engine.
   The core defines for using Blind on engine side are defined here.*/
// ENGINE SIDE HEADER

#ifdef BLIND_PLATFORM_WINDOWS // Engine will only build without errors with windows
	#ifdef BLIND_DLL
		#define BLIND_API __declspec(dllexport) // DLL Exporting
	#else
		#define BLIND_API __declspec(dllimport) // DlL Importing
	#endif
#else 
	#error Blind Engine currently does only support Windows OS!
#endif