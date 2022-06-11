#ifndef AHCI_HPP
#define AHCI_HPP

#include <vector>
#include "ATAPI/ATAPI.hpp"
#include <UUID.hpp>
#include <userspace/AHCI.hpp>

namespace AHCI {
	extern std::PID ahci;
	extern char* buffer;

	bool resolve();
	bool connect();

	std::vector<std::UUID> probeATAPI();
	bool readATAPI(const std::UUID&, uint8_t* data, size_t start, size_t sz);
}

#endif
