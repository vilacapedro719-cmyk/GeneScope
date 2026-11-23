
#pragma once

#include "genescope/models/mlp.hpp"

#include <cstddef>
#include <vector>

namespace genescope::training {

struct TrainingSample {
    Eigen::VectorXf features;
    int label;
};

struct TrainingHistory {
    std::vector<float> training_losses;
};

class Trainer {
public:
    explicit Trainer(std::size_t num_epochs = 100);

    [[nodiscard]] TrainingHistory train(models::Mlp& model, const std::vector<TrainingSample>& training_data) const;

private:
    std::size_t num_epochs_;
};

} // namespace genescope::training

