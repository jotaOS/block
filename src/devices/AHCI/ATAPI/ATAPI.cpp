#include "ATAPI.hpp"
#include "../AHCI.hpp"
#include <userspace/AHCI.hpp>
#include <rpc>
#include <cstdio>

bool AHCIATAPI::read(uint8_t* read, size_t start, size_t sz) {
	AHCI::connect();
	bool result = std::rpc(AHCI::ahci,
						   std::AHCI::READ_ATAPI,
						   id,
						   start,
						   (sz + ATAPI_SECTOR_SIZE - 1) / ATAPI_SECTOR_SIZE);

	if(!result)
		return false;

	memcpy(read, AHCI::buffer, sz);
	return true;
}
