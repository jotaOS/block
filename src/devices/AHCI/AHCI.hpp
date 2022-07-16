#ifndef AHCI_HPP
#define AHCI_HPP

#include <vector>
#include "ATAPI/ATAPI.hpp"
#include <UUID.hpp>
#include <userspace/AHCI.hpp>

namespace AHCI {
	extern std::PID ahci;

	bool resolve();

	std::vector<std::UUID> probeATAPI();
	bool readATAPI(const std::UUID&, std::SMID smid, size_t start, size_t sz);
}

#endif
