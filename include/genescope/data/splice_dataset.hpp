#pragma once

#include "genescope/data/dna_sequence.hpp"

#include <filesystem>
#include <vector>

namespace genescope::data {

class SpliceDataset {
public:
    [[nodiscard]] static std::vector<DnaSequence> load(const std::filesystem::path& path);
};

}  // namespace genescope::data
