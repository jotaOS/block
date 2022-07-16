#include "ATAPI.hpp"
#include "../AHCI.hpp"
#include <userspace/AHCI.hpp>
#include <rpc>
#include <cstdio>
#include <shared_memory>

bool AHCIATAPI::read(std::SMID smid, size_t start, size_t sz) {
	AHCI::resolve();

	std::smAllow(smid, AHCI::ahci);
	size_t sectors = (sz + ATAPI_SECTOR_SIZE - 1) / ATAPI_SECTOR_SIZE;
	return std::rpc(AHCI::ahci,
					std::AHCI::READ_ATAPI,
					smid,
					id,
					start,
					sectors);
}
