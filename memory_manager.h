#pragma once

#ifndef MEMORY_MANAGER_H
#define MEMORY_MANAGER

#include <Windows.h>
#include <cstdint>
#include <iostream>
#include <string>
#include <cstring>
#include <bitset>

#undef max

#include "incontrol.h"

namespace MemoryManager {
	void PrintSegmentState();
	void FreeRegion();
	InputState AddressInput(void*&);
	void AllocateRegion(const bool&, const bool&);
	void ProtectRegion(const DWORD&);
	std::string MemoryTypeToString(const DWORD&);
	std::string MemoryStateToString(const DWORD&);
	std::string MemoryProtectionToString(const DWORD&);
	void PrintMemoryStatus();
	void DataWritingAndOutput();
	void PrintSystemInfo();
	std::string ArchitectureToString(const WORD&);
}

#endif // !MEMORY_MANAGER_H
