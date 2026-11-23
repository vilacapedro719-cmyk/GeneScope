#include "genescope/evaluation/metrics.hpp"

#include <stdexcept>

namespace genescope::evaluation {

ClassificationReport evaluate(const std::vector<int>& truth, const std::vector<int>& predictions, int class_count) {
    if (truth.size() != predictions.size() || class_count <= 0) throw std::invalid_argument("invalid evaluation inputs");
    ClassificationReport report{.confusion_matrix = Eigen::MatrixXi::Zero(class_count, class_count)};
    for (std::size_t i = 0; i < truth.size(); ++i) {
        if (truth[i] < 0 || truth[i] >= class_count || predictions[i] < 0 || predictions[i] >= class_count) throw std::invalid_argument("class index out of range");
        ++report.confusion_matrix(truth[i], predictions[i]);
        report.accuracy += truth[i] == predictions[i] ? 1.0F : 0.0F;
    }
    if (!truth.empty()) report.accuracy /= static_cast<float>(truth.size());
    return report;
}

}  // namespace genescope::evaluation
