
#pragma once

#include <Eigen/Dense>

#include <cstddef>
#include <vector>

namespace genescope::models {

struct ForwardPass {
    Eigen::VectorXf z1;
    Eigen::VectorXf a1;
    Eigen::VectorXf probabilities;
};

class Mlp {
public:
    Mlp(std::vector<std::size_t> layer_sizes, float learning_rate = 0.01F);

    [[nodiscard]] Eigen::VectorXf predict_proba(
        const Eigen::VectorXf& input
    ) const;

    [[nodiscard]] int predict(
        const Eigen::VectorXf& input
    ) const;

    float train_step(
        const Eigen::VectorXf& input,
        int target_class
    );

private:
    [[nodiscard]] ForwardPass forward(
        const Eigen::VectorXf& input
    ) const;

    std::vector<Eigen::MatrixXf> weights_;
    std::vector<Eigen::VectorXf> biases_;
    float learning_rate_;
};

} // namespace genescope::models
