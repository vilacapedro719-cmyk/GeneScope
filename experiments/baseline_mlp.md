# Baseline MLP experiment

## Goal

Establish a reproducible baseline using flattened one-hot input.

## Suggested initial configuration

- Input: flattened 60 × 4 one-hot vector
- Hidden layers: 128, 64 ReLU units
- Output: 3-class softmax
- Loss: cross-entropy
- Optimizer: mini-batch gradient descent
- Metrics: macro F1, per-class precision/recall, confusion matrix

Record the data-split seed, training seed, batch size, learning rate, epochs, parameter count, and validation-selected checkpoint with every run.
