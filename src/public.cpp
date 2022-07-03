#include <common.hpp>
#include <vector>
#include <unordered_map>
#include <shared_memory>
#include <mutex>

size_t connect(std::PID client, std::SMID smid) {
	return std::sm::connect(client, smid);
}

static std::mutex uuidsLock;

static const size_t UUIDS_PER_PAGE = PAGE_SIZE / (sizeof(std::UUID) + sizeof(size_t));
size_t listDevices(std::PID client, size_t page) {
	uint64_t* data = (uint64_t*)std::sm::get(client);
	if(!data)
		return 0;

	// Which UUID is first in this page
	size_t first = page * UUIDS_PER_PAGE;
	// And which is last
	size_t last = (page + 1) * UUIDS_PER_PAGE - 1;

	// Run through them
	size_t idx = first;
	uuidsLock.acquire();
	for(; idx < last && idx < uuids.size(); ++idx) {
		auto uuid = uuids[idx];
		*(data++) = uuid.a;
		*(data++) = uuid.b;
		*(data++) = deviceTypes[uuid.a][uuid.b];
	}
	uuidsLock.release();

	if(idx < last) {
		// Fill the rest with zeros
		size_t remaining = last - idx;
		memset(data, 0, remaining);
	}

	return idx - first; // How many
}

static std::mutex devicesLock;

#define TYPE_ERROR 99
static size_t getUUIDType(size_t uuida, size_t uuidb) {
	// Get UUID type, if exists
	devicesLock.acquire();
	if(deviceTypes.find(uuida) == deviceTypes.end()) {
		devicesLock.release();
		return TYPE_ERROR;
	}

	auto& map = deviceTypes[uuida];
	devicesLock.release();

	if(map.find(uuidb) == map.end())
		return TYPE_ERROR;
	return map[uuidb];
}

bool read(std::PID client, size_t uuida, size_t uuidb, size_t start, size_t sz) {
	uint8_t* data = std::sm::get(client);
	if(!data)
		return false;

	size_t type = getUUIDType(uuida, uuidb);
	if(type == TYPE_ERROR)
		return false;

	// Perform the read
	std::UUID uuid(uuida, uuidb);
	return genericRead(uuid, type, data, start, sz);
}

bool write(std::PID client, size_t uuida, size_t uuidb, size_t start, size_t sz) {
	uint8_t* data = std::sm::get(client);
	if(!data)
		return false;

	size_t type = getUUIDType(uuida, uuidb);
	if(type == TYPE_ERROR)
		return false;

	// Perform the write
	std::UUID uuid(uuida, uuidb);
	return genericWrite(uuid, type, data, start, sz);
}

void exportProcedures() {
	std::exportProcedure((void*)connect, 1);
	std::exportProcedure((void*)listDevices, 1);
	std::exportProcedure((void*)read, 4);
	std::exportProcedure((void*)write, 4);
}
