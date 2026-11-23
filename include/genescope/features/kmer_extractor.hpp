#pragma once

#include "genescope/data/dna_sequence.hpp"

#include <string>
#include <unordered_map>
#include <vector>

namespace genescope::features {

using KmerCounts = std::unordered_map<std::string, std::size_t>;

[[nodiscard]] KmerCounts extract_kmers(const data::DnaSequence& sequence, std::size_t k);
[[nodiscard]] std::vector<std::string> canonical_kmers(std::size_t k);

}  // namespace genescope::features
