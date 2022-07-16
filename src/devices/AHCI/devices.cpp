#include "AHCI.hpp"
#include <rpc>

static UUIDmap atapis;

std::vector<std::UUID> AHCI::probeATAPI() {
	if(!resolve())
		return {};

	std::vector<std::UUID> ret;
	size_t devices = std::rpc(ahci, std::AHCI::GET_ATAPIS);
	ret.reserve(devices);

	for(size_t i=0; i<devices; ++i) {
		std::UUID uuid = randomUUID(); // TODO: Figure it out somehow
		atapis[uuid.a][uuid.b] = (uint64_t)(new AHCIATAPI(i));
		ret.push_back(uuid);
	}
	return ret;
}

bool AHCI::readATAPI(const std::UUID& uuid, std::SMID smid, size_t start, size_t sz) {
	AHCIATAPI* dev = (AHCIATAPI*)(atapis[uuid.a][uuid.b]);
	return dev->read(smid, start, sz);
}
