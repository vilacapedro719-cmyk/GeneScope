#include "genescope/features/kmer_extractor.hpp"

#include <stdexcept>
#include <utility>

namespace genescope::features {

KmerCounts extract_kmers(const data::DnaSequence& sequence, std::size_t k) {
    if (!sequence.is_valid() || k == 0) {
        throw std::invalid_argument("k-mer extraction requires valid bases and k > 0");
    }
    KmerCounts counts;
    if (k > sequence.bases.size()) return counts;
    for (std::size_t i = 0; i + k <= sequence.bases.size(); ++i) ++counts[sequence.bases.substr(i, k)];
    return counts;
}

std::vector<std::string> canonical_kmers(std::size_t k) {
    if (k == 0) return {};
    std::vector<std::string> result{""};
    constexpr char bases[] = {'A', 'T', 'C', 'G'};
    for (std::size_t position = 0; position < k; ++position) {
        std::vector<std::string> next;
        next.reserve(result.size() * 4);
        for (const auto& prefix : result) for (char base : bases) next.push_back(prefix + base);
        result = std::move(next);
    }
    return result;
}

}  // namespace genescope::features
