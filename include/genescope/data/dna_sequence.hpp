#pragma once

#include <string>
#include <string_view>

namespace genescope::data {

enum class SpliceClass { exon_intron, intron_exon, neither };

struct DnaSequence {
    std::string bases;
    SpliceClass label{SpliceClass::neither};
    std::string identifier;

    [[nodiscard]] bool is_valid() const;
};

[[nodiscard]] bool is_valid_base(char base);
[[nodiscard]] std::string normalize_bases(std::string_view bases);
[[nodiscard]] std::string_view to_string(SpliceClass label);

}  // namespace genescope::data
