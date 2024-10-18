#pragma once

#include <cstddef>
#include <utility>

// Custom hash function for std::pair<int, int>
// This allows us to use pairs as keys in unordered_map and unordered_set
struct PairHash {
	// This is a function object (functor) that can be called like a function
	// It takes a const reference to a std::pair<T1, T2> and returns a size_t
	template <class T1, class T2>
	std::size_t operator() (const std::pair<T1, T2>& pair) const {
		auto hash1 = std::hash<T1>{}(pair.first);
		auto hash2 = std::hash<T2>{}(pair.second);
		// Combine 2 hashes -> Use bitwise XOR (^) and left shift (<<) to create a unique hash
		return hash1 ^ (hash2 << 1);
	}
};