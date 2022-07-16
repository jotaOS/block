#ifndef COMMON_HPP
#define COMMON_HPP

#include <UUID.hpp>
#include <userspace/block.hpp>
#include <vector>

extern UUIDmap deviceTypes;
extern std::vector<std::UUID> uuids;

bool genericRead(const std::UUID& uuid, size_t type, std::SMID smid, size_t start, size_t sz);
bool genericWrite(const std::UUID& uuid, size_t type, std::SMID smid, size_t start, size_t sz);
void exportProcedures();

#endif
