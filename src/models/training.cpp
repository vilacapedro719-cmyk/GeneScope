#include "genescope/models/trainer.hpp"

#include <algorithm>
#include <iostream>
#include <random>

namespace genescope::training {

Trainer::Trainer(std::size_t num_epochs) : num_epochs_(num_epochs) {}

TrainingHistory Trainer::train(models::Mlp& model, const std::vector<TrainingSample>& training_data) const {
    TrainingHistory history;
    history.training_losses.reserve(num_epochs_);

    // shuffling the data for each epoch to ensure better generalization
    std::vector<TrainingSample> shuffled_data = training_data;
    std::mt19937 generator{42};

    for (std::size_t epoch = 0; epoch < num_epochs_; ++epoch) {
        std::shuffle(shuffled_data.begin(), shuffled_data.end(), generator);

        float total_loss = 0.0F;
        for (const auto& sample : shuffled_data) {
            total_loss += model.train_step(sample.features, sample.label);
        }

        const float average_loss = shuffled_data.empty() ? 0.0F : total_loss / static_cast<float>(shuffled_data.size());

        history.training_losses.push_back(average_loss);

        std::cout << "Epoch " << epoch + 1 << '/' << num_epochs_ << " - average loss: " << average_loss << '\n';
    }

    return history;
}

} // namespace genescope::training
