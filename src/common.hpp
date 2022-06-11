#ifndef COMMON_HPP
#define COMMON_HPP

#include <UUID.hpp>

extern UUIDmap devices;
extern std::vector<std::UUID> uuids;

struct DeviceTypes {
	enum {
		NONE,
		AHCIATAPI,
	};
};

bool genericRead(const std::UUID& uuid, size_t type, uint8_t* data, size_t start, size_t sz);
void exportProcedures();

#endif
