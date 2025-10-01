#pragma once

#include <windows.h>

// Entry point for min_max thread
DWORD WINAPI findMinMax(LPVOID lpParam);

// Entry point for average thread
DWORD WINAPI calculateAverage(LPVOID lpParam);