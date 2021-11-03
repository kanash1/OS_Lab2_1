#include "menu_maker.h"

Menu MakeMainMenu() {
	return Menu(
		"MAIN MENU",
		"Exit",
		std::vector<Item>({
			Item("Get system information", []() { MemoryManager::PrintSystemInfo(); system("pause"); }),
			Item("Determine memory status", []() { MemoryManager::PrintMemoryStatus(); system("pause"); }),
			Item("Determine region state", []() { MemoryManager::PrintSegmentState(); system("pause"); }),
			Item("Reserve region", []() { menu_process(MakeReserveMenu()); }),
			Item("Protect region", []() { menu_process(MakeProtectionMenu()); }),
			Item("Free region", []() { MemoryManager::FreeRegion(); system("pause"); }),
			Item("Write data", []() { MemoryManager::DataWritingAndOutput(); system("pause"); })
			})
	);
}

Menu MakeReserveMenu() {
	return Menu(
		"RESERVE REGION MENU",
		"Go back",
		std::vector<Item>({
			Item("Virtual auto reserve", []() { MemoryManager::AllocateRegion(true, false); system("pause"); }),
			Item("Virtual manual reserve", []() { MemoryManager::AllocateRegion(false, false); system("pause"); }),
			Item("Physical auto reserve", []() { MemoryManager::AllocateRegion(true, true); system("pause"); }),
			Item("Physical manual reserve", []() { MemoryManager::AllocateRegion(false, true); system("pause"); })
			})
	);
}

Menu MakeProtectionMenu() {
	return Menu(
		"PROTECTION MENU",
		"Go back",
		std::vector<Item>({
			Item("PAGE_EXECUTE", []() { MemoryManager::ProtectRegion(PAGE_EXECUTE); system("pause"); }),
			Item("PAGE_EXECUTE_READ", []() { MemoryManager::ProtectRegion(PAGE_EXECUTE_READ); system("pause"); }),
			Item("PAGE_EXECUTE_READWRITE", []() { MemoryManager::ProtectRegion(PAGE_EXECUTE_READWRITE); system("pause"); }),
			//Item("PAGE_EXECUTE_WRITECOPY", []() { MemoryManager::ProtectRegion(PAGE_EXECUTE_WRITECOPY); system("pause"); }),
			Item("PAGE_NOACCESS", []() { MemoryManager::ProtectRegion(PAGE_NOACCESS); system("pause"); }),
			Item("PAGE_READONLY", []() { MemoryManager::ProtectRegion(PAGE_READONLY); system("pause"); }),
			Item("PAGE_READWRITE", []() { MemoryManager::ProtectRegion(PAGE_READWRITE); system("pause"); }),
			//Item("PAGE_WRITECOPY", []() { MemoryManager::ProtectRegion(PAGE_WRITECOPY); system("pause"); }),
			//Item("PAGE_TARGETS_INVALID", []() { MemoryManager::ProtectRegion(PAGE_TARGETS_INVALID); system("pause"); })
			})
	);
}