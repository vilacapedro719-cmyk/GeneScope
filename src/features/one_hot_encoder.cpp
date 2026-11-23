#include "genescope/features/one_hot_encoder.hpp"

#include <stdexcept>

namespace genescope::features {
namespace {
int base_index(char base) {
    switch (base) {
        case 'A': return 0;
        case 'T': return 1;
        case 'C': return 2;
        case 'G': return 3;
        default: throw std::invalid_argument("sequence contains an invalid nucleotide");
    }
}
}  // namespace

Eigen::MatrixXf OneHotEncoder::encode_matrix(const data::DnaSequence& sequence) const {
    if (!sequence.is_valid()) {
        throw std::invalid_argument("cannot encode an invalid DNA sequence");
    }
    Eigen::MatrixXf encoded = Eigen::MatrixXf::Zero(static_cast<Eigen::Index>(sequence.bases.size()), 4);
    for (Eigen::Index i = 0; i < encoded.rows(); ++i) {
        encoded(i, base_index(sequence.bases[static_cast<std::size_t>(i)])) = 1.0F;
    }
    return encoded;
}

Eigen::VectorXf OneHotEncoder::encode(const data::DnaSequence& sequence) const {
    const auto matrix = encode_matrix(sequence);
    return Eigen::Map<const Eigen::VectorXf>(matrix.data(), matrix.size());
}

int encode_label(const data::DnaSequence& sequence) {
    if (!sequence.is_valid()) {
        throw std::invalid_argument("cannot encode an invalid DNA sequence");
    }
    if (sequence.label == data::SpliceClass::exon_intron) {
        return 0;
    } else if (sequence.label == data::SpliceClass::intron_exon) {
        return 1;
    } else if (sequence.label == data::SpliceClass::neither) {
        return 2;
    } else {
        throw std::invalid_argument("unknown splice class");
    }

}

}  // namespace genescope::features
