#include "genescope/data/dna_sequence.hpp"
#include "genescope/features/kmer_extractor.hpp"
#include "genescope/features/one_hot_encoder.hpp"
#include "genescope/models/mlp.hpp"

#include <cassert>

int main() {
    using namespace genescope;
    const data::DnaSequence sequence{"ATCG", data::SpliceClass::neither, "test"};
    assert(sequence.is_valid());
    assert(!data::DnaSequence{"ATNG"}.is_valid());
    assert(data::normalize_bases("a tcg\n") == "ATCG");

    const features::OneHotEncoder encoder;
    const auto encoded = encoder.encode_matrix(sequence);
    assert(encoded.rows() == 4 && encoded.cols() == 4);
    assert(encoded.row(0).sum() == 1.0F);

    const auto kmers = features::extract_kmers(sequence, 2);
    assert(kmers.at("AT") == 1 && kmers.at("TC") == 1 && kmers.at("CG") == 1);
    assert(features::canonical_kmers(2).size() == 16);

    // A linear softmax classifier should learn this separable two-class problem.
    models::Mlp classifier({2, 3}, 0.1F);
    Eigen::Vector2f first;
    first << 1.0F, 0.0F;
    Eigen::Vector2f second;
    second << 0.0F, 1.0F;
    for (int epoch = 0; epoch < 200; ++epoch) {
        classifier.train_step(first, 0);
        classifier.train_step(second, 1);
    }
    assert(classifier.predict(first) == 0);
    assert(classifier.predict(second) == 1);
}
