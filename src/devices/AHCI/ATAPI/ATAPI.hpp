#ifndef AHCI_ATAPI_HPP
#define AHCI_ATAPI_HPP

#include <types>

#define ATAPI_SECTOR_SIZE 2048

class AHCIATAPI {
private:
	size_t id;

public:
	AHCIATAPI() = default;
	AHCIATAPI(size_t id) : id(id) {}

	bool read(std::SMID smid, size_t start, size_t sz);
};

#endif
