#include "memory_manager.h"

namespace MemoryManager {
	void PrintSegmentState() {
		MEMORY_BASIC_INFORMATION mbi;
		void* address;
		if (AddressInput(address) != InputState::NORMAL) {
			std::cout << "Invalid input. Request processing stopped\n";
			return;
		}
		if (VirtualQuery(address, &mbi, sizeof(mbi)) == 0) {
			std::cout << "Error " << GetLastError() << " ocurred\n";
			return;
		}
		std::cout << "Allocation base: " << mbi.AllocationBase << '\n'
			<< "Base address: " << mbi.BaseAddress << '\n'
			<< "Allocation protection: " << MemoryProtectionToString(mbi.AllocationProtect) << '\n'
			<< "Region size: " << mbi.RegionSize << '\n'
			<< "State: " << MemoryStateToString(mbi.State) << '\n'
			<< "Protection: " << MemoryProtectionToString(mbi.Protect) << '\n'
			<< "Type: " << MemoryTypeToString(mbi.Type) << '\n';
	}

	void AllocateRegion(const bool& is_auto, const bool& is_physic) {
		InputControler<size_t> input(std::cin);
		void* base_address;
		void* address = nullptr;
		DWORD state = MEM_RESERVE | ((is_physic) ? MEM_COMMIT : 0);
		size_t page_count = 1;
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		if (!is_auto) {
			if (AddressInput(address) != InputState::NORMAL) {
				std::cout << "Invalid input. Request processing stopped\n";
				return;
			}
			else if (address == nullptr)
				std::cout << "You entered zero address. The region will be reserved automatically\n";
		}
		std::cout << "Enter page count (this value will be multiplied with 4096):\n";
		input >> page_count;
		if (input.GetSate() != InputState::NORMAL) {
			std::cout << "Invalid input. Request processing stopped\n";
			input.Normalize();
			return;
		}
		base_address = VirtualAlloc(address, static_cast<DWORD>(page_count) * si.dwPageSize, state, PAGE_EXECUTE_READWRITE);
		if (base_address != nullptr) {
			std::cout << "Allocation successfull. Base address: " << base_address << '\n';
		}
		else
			std::cout << "Error " << GetLastError() << " ocurred\n";
	}

	void ProtectRegion(const DWORD& protection) {
		void* address;
		DWORD old_protection;
		MEMORY_BASIC_INFORMATION mbi;
		if (AddressInput(address) != InputState::NORMAL) {
			std::cout << "Invalid input. Request processing stopped\n";
			return;
		}
		if (VirtualQuery(address, &mbi, sizeof(mbi)) == 0) {
			std::cout << "Error " << GetLastError() << " ocurred\n";
			return;
		}
		if (VirtualProtect(address, mbi.RegionSize, protection, &old_protection))
			std::cout << "Protection changed successfully! Old protection: " << MemoryProtectionToString(old_protection) << '\n';
		else
			std::cout << "Error " << GetLastError() << " ocurred\n";
	}

	void FreeRegion() {
		void* address;
		if (AddressInput(address) != InputState::NORMAL) {
			std::cout << "Invalid input. Request processing stopped\n";
			return;
		}
		if (VirtualFree(address, 0, MEM_RELEASE))
			std::cout << "Memory free sucessfully\n";
		else
			std::cout << "Error " << GetLastError() << " ocurred\n";
	}

	std::string MemoryTypeToString(const DWORD& type) {
		switch (type) {
		case MEM_IMAGE: return "MEM_IMAGE";
		case MEM_MAPPED: return "MEM_MAPPED";
		case MEM_PRIVATE: return "MEM_PRIVATE";
		default: return "NONE";
		}
	}

	std::string MemoryStateToString(const DWORD& state) {
		switch (state) {
		case MEM_COMMIT: return "MEM_COMMIT";
		case MEM_FREE: return "MEM_FREE";
		case MEM_RESERVE: return "MEM_RESERVE";
		default: return "NONE";
		}
	}

	std::string MemoryProtectionToString(const DWORD& protection) {
		DWORD protection_ = protection;
		std::string str_protection;
		if ((protection & PAGE_GUARD) == PAGE_GUARD) {
			str_protection = " and PAGE_GUARD";
			protection_ ^= PAGE_GUARD;
		} else if ((protection & PAGE_NOCACHE) == PAGE_NOCACHE) {
			str_protection = " and PAGE_NOCACHE";
			protection_ ^= PAGE_NOCACHE;
		} else if ((protection & PAGE_WRITECOMBINE) == PAGE_WRITECOMBINE) {
			str_protection = " and PAGE_WRITECOMBINE";
			protection_ ^= PAGE_WRITECOMBINE;
		}
		switch (protection_) {
		case PAGE_EXECUTE:
			str_protection = "PAGE_EXECUTE" + str_protection;
			break;
		case PAGE_EXECUTE_READ:
			str_protection = "PAGE_EXECUTE_READ" + str_protection;
			break;
		case PAGE_EXECUTE_READWRITE:
			str_protection = "PAGE_EXECUTE_READWRITE" + str_protection;
			break;
		case PAGE_EXECUTE_WRITECOPY:
			str_protection = "PAGE_EXECUTE_WRITECOPY" + str_protection;
			break;
		case PAGE_NOACCESS:
			str_protection = "PAGE_NOACCESS" + str_protection;
			break;
		case PAGE_READONLY:
			str_protection = "PAGE_READONLY" + str_protection;
			break;
		case PAGE_READWRITE:
			str_protection = "PAGE_READWRITE" + str_protection;
			break;
		case PAGE_WRITECOPY:
			str_protection = "PAGE_WRITECOPY" + str_protection;
			break;
		case PAGE_TARGETS_INVALID:
			str_protection = "PAGE_TARGETS_INVALID" + str_protection;
			break;
		default: return "NONE";
		}
		return str_protection;
	}

	InputState AddressInput(void*& p_address) {
		InputState state;
		InputControler<uint32_t> input(std::cin >> std::hex);
		uint32_t address = 0;
		std::cout << "Enter the address:\n";
		input >> address;
		state = input.GetSate();
		input.Normalize();
		p_address = reinterpret_cast<void*>(address);
		return state;
	}

	void PrintMemoryStatus() {
		MEMORYSTATUSEX ms;
		ms.dwLength = sizeof(ms);
		if (!GlobalMemoryStatusEx(&ms)) {
			std::cout << "Error " << GetLastError() << " ocurred\n";
			return;
		}
		std::cout << "Memory in use: " << ms.dwMemoryLoad << "%\n"
			<< "Physical memory total: " << ms.ullTotalPhys / 1024 << " KB\n"
			<< "Physical memory free: " << ms.ullAvailPhys / 1024 << " KB\n"
			<< "Paging file total: " << ms.ullTotalPageFile / 1024 << " KB\n"
			<< "Paging file free: " << ms.ullAvailPageFile / 1024 << " KB\n"
			<< "Virtual memory total: " << ms.ullTotalVirtual / 1024 << " KB\n"
			<< "Virtual memory free: " << ms.ullAvailVirtual / 1024 << " KB\n";
	}

	void DataWritingAndOutput() {
		void* address = nullptr;
		std::string data;;
		DWORD protection;
		MEMORY_BASIC_INFORMATION mbi;
		if (AddressInput(address) != InputState::NORMAL) {
			std::cout << "Invalid input. Request processing stopped\n";
			return;
		}
		if (VirtualQuery(address, &mbi, sizeof(mbi)) == 0) {
			std::cout << "Error " << GetLastError() << " ocurred\n";
			return;
		}
		protection = PAGE_READWRITE | PAGE_WRITECOPY | PAGE_EXECUTE_READWRITE | PAGE_EXECUTE_WRITECOPY;
		if ((mbi.AllocationProtect & protection) == 0) {
			std::cout << "Memory protection level doesn't allow writing\n";
			return;
		}
		std::cout << "Enter data:\n";
		std::getline(std::cin, data);
		if (static_cast<DWORD>(data.length() + 1) > mbi.RegionSize) {
			std::cout << "Region size for this data is too small\n";
			return;
		}
		if (memcpy_s(address, data.length() + 1, data.data(), data.length() + 1) != 0) {
			std::cout << "Error ocurred\n";
			return;
		}
		std::cout << "Data written successfully\n\nData written to memory:\n";
		std::cout << static_cast<char*>(address) << '\n';
	}

	void PrintSystemInfo() {
		SYSTEM_INFO si;
		GetSystemInfo(&si);
		std::cout << "Processor architecture: " << ArchitectureToString(si.wProcessorArchitecture) << '\n'	// ok
			<< "Page size: " << si.dwPageSize << '\n'														// ok
			<< "Min app address: " << si.lpMinimumApplicationAddress << '\n'								// ok
			<< "Max app address: " << si.lpMaximumApplicationAddress << '\n'								// ok
			<< "Active processor mask: " << std::bitset<32>(si.dwActiveProcessorMask) << '\n'				// maybe some fixes are needed
			<< "Number of processors: " << si.dwNumberOfProcessors << '\n'									// ok
			<< "Allocation granularity: " << si.dwAllocationGranularity << '\n'								// ok
			<< "Processor level: " << si.wProcessorLevel << '\n'											// ok
			<< "Processor revision: " << si.wProcessorRevision << '\n';										// maybe some fixes are needed
	}

	std::string ArchitectureToString(const WORD& arch) {
		switch (arch) {
		case PROCESSOR_ARCHITECTURE_INTEL: return "x86";
		case PROCESSOR_ARCHITECTURE_ARM: return "ARM";
		case PROCESSOR_ARCHITECTURE_IA64: return "Intel Itanium-based";
		case PROCESSOR_ARCHITECTURE_AMD64: return "x64";
		case PROCESSOR_ARCHITECTURE_ARM64: return "ARM64";
		default: return "Unknown architecture";
		}
	}
}