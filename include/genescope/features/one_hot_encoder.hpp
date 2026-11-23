#pragma once

#include "genescope/data/dna_sequence.hpp"

#include <Eigen/Dense>

namespace genescope::features {

class OneHotEncoder {
public:
    [[nodiscard]] Eigen::VectorXf encode(const data::DnaSequence& sequence) const;
    [[nodiscard]] Eigen::MatrixXf encode_matrix(const data::DnaSequence& sequence) const;
};

}  // namespace genescope::features
