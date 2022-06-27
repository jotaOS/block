#include <common.hpp>
#include <vector>
#include <unordered_map>
#include <shared_memory>

static std::unordered_map<std::PID, void*> shared;

size_t connect(std::PID client, std::SMID smid) {
	// Already connected?
	if(!std::smRequest(client, smid))
		return false;

	void* ptr = std::smMap(smid);
	if(!ptr)
		return false;

	// TODO: unmap previous, release SMID
	shared[client] = ptr;
	return true;
}

static const size_t UUIDS_PER_PAGE = PAGE_SIZE / sizeof(std::UUID);
size_t listDevices(std::PID client, size_t page) {
	if(shared.find(client) == shared.end())
		return 0;

	uint64_t* data = (uint64_t*)(shared[client]);

	// Which UUID is first in this page
	size_t first = page * UUIDS_PER_PAGE;
	// And which is last
	size_t last = (page + 1) * UUIDS_PER_PAGE - 1;

	// Run through them
	size_t idx = first;
	for(; idx < last && idx < uuids.size(); ++idx) {
		*(data++) = uuids[idx].a;
		*(data++) = uuids[idx].b;
	}

	if(idx < last) {
		// Fill the rest with zeros
		size_t remaining = last - idx;
		memset(data, 0, remaining);
	}

	return idx - first; // How many
}

bool read(std::PID client, size_t uuida, size_t uuidb, size_t start, size_t sz) {
	if(shared.find(client) == shared.end())
		return false;

	uint8_t* data = (uint8_t*)(shared[client]);

	// Get UUID type, if exists
	if(devices.find(uuida) == devices.end())
		return false;
	auto& map = devices[uuida];
	if(map.find(uuidb) == map.end())
		return false;
	size_t type = map[uuidb];

	// Perform the read
	std::UUID uuid(uuida, uuidb);
	return genericRead(uuid, type, data, start, sz);
}

void exportProcedures() {
	std::exportProcedure((void*)connect, 1);
	std::exportProcedure((void*)listDevices, 1);
	std::exportProcedure((void*)read, 4);
}
