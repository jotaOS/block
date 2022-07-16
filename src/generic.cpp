#include <common.hpp>
#include <devices/AHCI/AHCI.hpp>
#include <devices/ramblock/ramblock.hpp>

bool genericRead(const std::UUID& uuid, size_t type, std::SMID smid, size_t start, size_t sz) {
	switch(type) {
	case std::block::DeviceTypes::AHCIATAPI:
		return AHCI::readATAPI(uuid, smid, start, sz);
	case std::block::DeviceTypes::RAMBLOCK:
		return ramblock::read(uuid, smid, start);
	default:
		return false;
	}
}

bool genericWrite(const std::UUID& uuid, size_t type, std::SMID smid, size_t start, size_t sz) {
	IGNORE(sz);

	switch(type) {
	case std::block::DeviceTypes::RAMBLOCK:
		return ramblock::write(uuid, smid, start);
	default:
		return false;
	}
}
