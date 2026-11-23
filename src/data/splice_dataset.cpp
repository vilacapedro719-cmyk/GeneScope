#include "genescope/data/splice_dataset.hpp"

#include <cctype>
#include <fstream>
#include <stdexcept>
#include <string>
#include <string_view>

namespace genescope::data {

namespace {

std::string_view trim(std::string_view value) {
    while (!value.empty() && std::isspace(static_cast<unsigned char>(value.front()))) {
        value.remove_prefix(1);
    }
    while (!value.empty() && std::isspace(static_cast<unsigned char>(value.back()))) {
        value.remove_suffix(1);
    }
    return value;
}

SpliceClass parse_label(std::string_view label) {
    if (label == "EI") {
        return SpliceClass::exon_intron;
    }
    if (label == "IE") {
        return SpliceClass::intron_exon;
    }
    if (label == "N") {
        return SpliceClass::neither;
    }
    throw std::runtime_error("Unknown splice label: " + std::string(label));
}

}  // namespace

std::vector<DnaSequence> SpliceDataset::load(const std::filesystem::path& path) {
    std::ifstream file(path);
    if (!file) {
        throw std::runtime_error("Failed to open file: " + path.string());
    }
    std::vector<DnaSequence> sequences;
    std::string line;
    std::size_t line_number = 0;
    while (std::getline(file, line)) {
        ++line_number;
        const std::string_view row = trim(line);
        if (row.empty()) {
            continue;
        }

        const std::size_t first_comma = row.find(',');
        const std::size_t second_comma = first_comma == std::string_view::npos
            ? std::string_view::npos
            : row.find(',', first_comma + 1);
        if (second_comma == std::string_view::npos) {
            throw std::runtime_error("Malformed dataset row at line " + std::to_string(line_number));
        }

        const auto label = trim(row.substr(0, first_comma));
        const auto identifier = trim(row.substr(first_comma + 1, second_comma - first_comma - 1));
        const auto bases = trim(row.substr(second_comma + 1));
        SpliceClass parsed_label;
        try {
            parsed_label = parse_label(label);
        } catch (const std::runtime_error& error) {
            throw std::runtime_error(
                "Invalid dataset row at line " + std::to_string(line_number) + ": " + error.what());
        }
        sequences.push_back({normalize_bases(bases), parsed_label, std::string(identifier)});
    }

    return sequences;
}

}  // namespace genescope::data
