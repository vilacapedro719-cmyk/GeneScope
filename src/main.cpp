#include "genescope/data/dna_sequence.hpp"
#include "genescope/data/splice_dataset.hpp"
#include "genescope/evaluation/metrics.hpp"
#include "genescope/features/one_hot_encoder.hpp"
#include "genescope/models/trainer.hpp"

#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

int label_index(genescope::data::SpliceClass label) {
    switch (label) {
        case genescope::data::SpliceClass::exon_intron: return 0;
        case genescope::data::SpliceClass::intron_exon: return 1;
        case genescope::data::SpliceClass::neither: return 2;
    }
    throw std::invalid_argument("unknown splice class");
}

void evaluate_test_set(
    const genescope::models::Mlp& model,
    const genescope::features::OneHotEncoder& encoder,
    const std::string& path
) {
    const auto sequences = genescope::data::SpliceDataset::load(path);

    std::vector<int> truth;
    std::vector<int> predictions;
    for (const auto& sequence : sequences) {
        if (!sequence.is_valid()) {
            continue;
        }
        const auto features = encoder.encode(sequence);
        truth.push_back(label_index(sequence.label));
        predictions.push_back(model.predict(features));
    }

    if (truth.empty()) {
        std::cerr << "No valid test sequences found.\n";
        return;
    }

    const auto report = genescope::evaluation::evaluate(truth, predictions, 3);

    std::cout << "Test accuracy: " << report.accuracy * 100.0F << "% ("
              << truth.size() << " sequences)\n";
    std::cout << "Confusion matrix (rows=true, cols=pred; EI, IE, N):\n"
              << report.confusion_matrix << '\n';
}

}  // namespace

int main() {
    const auto sequences = genescope::data::SpliceDataset::load("data/raw/train.data");
    if (sequences.empty()) {
        std::cerr << "No training sequences found.\n";
        return 1;
    }

    const genescope::features::OneHotEncoder encoder;
    std::vector<genescope::training::TrainingSample> training_data;
    training_data.reserve(sequences.size());
    for (const auto& sequence : sequences) {
        if (!sequence.is_valid()) {
            continue;
        }
        training_data.push_back({encoder.encode(sequence), label_index(sequence.label)});
    }

    if (training_data.empty()) {
        std::cerr << "No valid training sequences found.\n";
        return 1;
    }

    const auto input_size = static_cast<std::size_t>(training_data.front().features.size());
    genescope::models::Mlp model({input_size, 64, 3}, 0.05F);
    const genescope::training::Trainer trainer{100};
    const auto history = trainer.train(model, training_data);
    if (!history.training_losses.empty()) {
        std::cout << "Loss: " << history.training_losses.front() << " -> " << history.training_losses.back() << '\n';
    }

    evaluate_test_set(model, encoder, "data/raw/test.data");

    return 0;
}
