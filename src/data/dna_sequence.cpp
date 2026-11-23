#include "genescope/data/dna_sequence.hpp"

#include <algorithm>
#include <cctype>

namespace genescope::data {

bool is_valid_base(char base) {
    const char normalized = static_cast<char>(std::toupper(static_cast<unsigned char>(base)));
    return normalized == 'A' || normalized == 'T' || normalized == 'C' || normalized == 'G';
}

std::string normalize_bases(std::string_view bases) {
    std::string result;
    result.reserve(bases.size());
    for (const char base : bases) {
        if (!std::isspace(static_cast<unsigned char>(base))) {
            result.push_back(static_cast<char>(std::toupper(static_cast<unsigned char>(base))));
        }
    }
    return result;
}

bool DnaSequence::is_valid() const {
    return !bases.empty() && std::ranges::all_of(bases, is_valid_base);
}

std::string_view to_string(SpliceClass label) {
    switch (label) {
        case SpliceClass::exon_intron: return "EI";
        case SpliceClass::intron_exon: return "IE";
        case SpliceClass::neither: return "N";
    }
    return "N";
}

}  // namespace genescope::data
