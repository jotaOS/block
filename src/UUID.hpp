#ifndef UUIDMAP_HPP
#define UUIDMAP_HPP

#include <unordered_map>

// This implements a sort of std::unordered_map<UUID, V>
// It's just two linked std::unordered_map<uint64_t, V>
// V is uint64_t. That should be enough
typedef uint64_t _HalfUUID;
typedef std::unordered_map<_HalfUUID, uint64_t> _InnerMap;
typedef std::unordered_map<_HalfUUID, _InnerMap> UUIDmap;

inline std::UUID genUUID() {
	// TEMOPRAL! Should figure it out somehow. TODO
	return {0x1337, 0x1337};
}

#endif
