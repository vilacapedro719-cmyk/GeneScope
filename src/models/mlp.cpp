
#include "genescope/models/mlp.hpp"

#include <algorithm>
#include <cmath>
#include <limits>
#include <random>
#include <stdexcept>

namespace genescope::models {
namespace {

Eigen::VectorXf softmax(const Eigen::VectorXf& logits) {
    const float maximum = logits.maxCoeff();

    Eigen::VectorXf probabilities =
        (logits.array() - maximum).exp();

    probabilities /= probabilities.sum();

    return probabilities;
}

void validate_input(
    const Eigen::VectorXf& input,
    Eigen::Index expected_size
) {
    if (input.size() != expected_size) {
        throw std::invalid_argument("Invalid input size");
    }
}

} // namespace


// CONSTRUCTOR

Mlp::Mlp(std::vector<std::size_t> layer_sizes, float learning_rate) : learning_rate_(learning_rate) {

    if (layer_sizes.size() != 3 || learning_rate <= 0.0F) {
        throw std::invalid_argument("Invalid MLP configuration");
    }

    for (const auto size : layer_sizes) {
        if (size == 0) {
            throw std::invalid_argument("Layers cannot be empty");
        }
    }

    std::mt19937 generator{42};

    for (std::size_t layer = 1; layer < layer_sizes.size(); ++layer) {

        const auto input_size =
            static_cast<Eigen::Index>(layer_sizes[layer - 1]);

        const auto output_size =
            static_cast<Eigen::Index>(layer_sizes[layer]);

        const float standard_deviation =
            std::sqrt(2.0F / static_cast<float>(input_size));

        std::normal_distribution<float> distribution{
            0.0F,
            standard_deviation
        };

        Eigen::MatrixXf weights = Eigen::MatrixXf::NullaryExpr(
            output_size,
            input_size,
            [&] { return distribution(generator); }
        );

        Eigen::VectorXf biases =
            Eigen::VectorXf::Zero(output_size);

        weights_.push_back(weights);
        biases_.push_back(biases);
    }
}


// FORWARD PROPAGATION

ForwardPass Mlp::forward(const Eigen::VectorXf& input) const {

    // Hidden layer
    Eigen::VectorXf z1 = weights_[0] * input + biases_[0];

    Eigen::VectorXf a1 = z1.cwiseMax(0.0F);

    // Output layer
    Eigen::VectorXf z2 = weights_[1] * a1 + biases_[1];

    Eigen::VectorXf probabilities = softmax(z2);

    return {z1, a1, probabilities};
}


// PREDICTION

Eigen::VectorXf Mlp::predict_proba(const Eigen::VectorXf& input) const {

    validate_input(input, weights_[0].cols());

    return forward(input).probabilities;
}


int Mlp::predict(const Eigen::VectorXf& input) const {

    Eigen::Index index{};

    predict_proba(input).maxCoeff(&index);

    return static_cast<int>(index);
}


// BACKPROPAGATION AND GRADIENT DESCENT

float Mlp::train_step(const Eigen::VectorXf& input, int target_class) {

    validate_input(input, weights_[0].cols());

    if (target_class < 0 || target_class >= weights_[1].rows()) {
        throw std::invalid_argument("Invalid target class");
    }


    // ==========================================
    // 1. FORWARD PROPAGATION
    // ==========================================

    ForwardPass pass = forward(input);

    const Eigen::VectorXf& probabilities = pass.probabilities;

    const Eigen::VectorXf& a1 = pass.a1;

    const Eigen::VectorXf& z1 = pass.z1;


    // ==========================================
    // 2. CALCULATE LOSS
    // ==========================================

    const float loss = -std::log(std::max(probabilities(target_class), std::numeric_limits<float>::min()));


    // ==========================================
    // 3. OUTPUT LAYER GRADIENTS
    // ==========================================

    // dL/dz2 = predicted probabilities - correct class

    Eigen::VectorXf delta2 = probabilities;

    delta2(target_class) -= 1.0F;


    // dW2 = delta2 * a1 transpose

    Eigen::MatrixXf dW2 = delta2 * a1.transpose();


    // db2 = delta2

    Eigen::VectorXf db2 = delta2;


    // ==========================================
    // 4. HIDDEN LAYER ERROR
    // ==========================================

    // Propagate output error through output weights

    Eigen::VectorXf delta1 = weights_[1].transpose() * delta2;


    // Calculate the ReLU derivative

    Eigen::VectorXf relu_derivative = (z1.array() > 0.0F).cast<float>();


    // Apply the chain rule

    delta1 = delta1.cwiseProduct(relu_derivative);


    // ==========================================
    // 5. HIDDEN LAYER GRADIENTS
    // ==========================================

    // dW1 = delta1 * input transpose

    Eigen::MatrixXf dW1 = delta1 * input.transpose();


    // db1 = delta1

    Eigen::VectorXf db1 = delta1;


    // ==========================================
    // 6. UPDATE PARAMETERS
    // ==========================================

    // Output layer

    weights_[1] -= learning_rate_ * dW2;
    biases_[1] -= learning_rate_ * db2;


    // Hidden layer

    weights_[0] -= learning_rate_ * dW1;
    biases_[0] -= learning_rate_ * db1;


    return loss;
}

} // namespace genescope::models
