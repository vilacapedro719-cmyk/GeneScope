#pragma once

#include <Eigen/Dense>

#include <vector>

namespace genescope::evaluation {

struct ClassificationReport {
    float accuracy{};
    Eigen::MatrixXi confusion_matrix;
};

[[nodiscard]] ClassificationReport evaluate(
    const std::vector<int>& truth, const std::vector<int>& predictions, int class_count);

}  // namespace genescope::evaluation
