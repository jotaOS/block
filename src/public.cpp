#include <common.hpp>
#include <vector>
#include <unordered_map>
#include <shared_memory>
#include <mutex>
#include <registry>

static std::mutex uuidsLock;

static const size_t UUIDS_PER_PAGE = PAGE_SIZE / (sizeof(std::UUID) + sizeof(size_t));
size_t listDevices(std::PID client, std::SMID smid, size_t page) {
	if(!std::registry::has(client, "BLOCK_LIST"))
		return 0;

	auto link = std::sm::link(client, smid);
	size_t npages = link.s;
	if(!npages)
		return 0;
	uint64_t* data = (uint64_t*)link.f;

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

	std::sm::unlink(smid);
	return idx - first; // How many
}

static std::mutex devicesLock;

#define TYPE_ERROR 99
static size_t getUUIDType(size_t uuida, size_t uuidb) {
	// Get UUID type, if exists
	devicesLock.acquire();
	if(!deviceTypes.has(uuida)) {
		devicesLock.release();
		return TYPE_ERROR;
	}

	auto& map = deviceTypes[uuida];
	devicesLock.release();

	if(!map.has(uuidb))
		return TYPE_ERROR;
	return map[uuidb];
}

std::unordered_map<std::PID, std::UUID> selecteds;
std::mutex selectedsLock;

bool select(std::PID client, size_t uuida, size_t uuidb) {
	if(!std::registry::has(client, "BLOCK_READ"))
		return false;

	selectedsLock.acquire();

	bool ret = false;
	if(getUUIDType(uuida, uuidb) != TYPE_ERROR) {
		selecteds[client] = {uuida, uuidb};
		ret = true;
	}

	selectedsLock.release();
	return ret;
}

bool read(std::PID client, std::SMID smid, size_t start, size_t sz) {
	if(!std::registry::has(client, "BLOCK_READ"))
		return false;

	selectedsLock.acquire();
	if(!selecteds.has(client)) {
		selectedsLock.release();
		return false;
	}
	std::UUID selected = selecteds[client];
	selectedsLock.release();

	auto link = std::sm::link(client, smid);
	if(!link.s)
		return false;

	size_t type = getUUIDType(selected.a, selected.b);
	bool ret = genericRead(selected, type, smid, start, sz);
	std::sm::unlink(smid);
	return ret;
}

bool write(std::PID client, std::SMID smid, size_t start, size_t sz) {
	if(!std::registry::has(client, "BLOCK_WRITE"))
		return false;

	selectedsLock.acquire();
	if(!selecteds.has(client)) {
		selectedsLock.release();
		return false;
	}
	std::UUID selected = selecteds[client];
	selectedsLock.release();

	auto link = std::sm::link(client, smid);
	if(!link.s)
		return false;

	size_t type = getUUIDType(selected.a, selected.b);
	bool ret = genericWrite(selected, type, smid, start, sz);
	std::sm::unlink(smid);
	return ret;
}

void exportProcedures() {
	std::exportProcedure((void*)listDevices, 2);
	std::exportProcedure((void*)select, 2);
	std::exportProcedure((void*)read, 3);
	std::exportProcedure((void*)write, 3);
}
