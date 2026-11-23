# GeneScope

**DNA Splice-Junction Classification using a Multilayer Perceptron (MLP)**

GeneScope is a C++ machine learning project that classifies DNA sequences into three categories:

- **EI:** Exon-to-Intron boundary
- **IE:** Intron-to-Exon boundary
- **N:** Neither (no splice junction)

## Overview

The project implements a Multilayer Perceptron (MLP) from scratch using C++ and Eigen, exploring the fundamentals of neural network training and classification.

DNA sequences are converted into numerical representations using one-hot encoding, producing 240 input features per sequence.

The neural network consists of:

- **Input layer:** 240 features
- **Hidden layer:** 64 neurons with ReLU activation
- **Output layer:** 3 neurons with Softmax activation

Training is performed using backpropagation, gradient descent and categorical cross-entropy loss.

## Technologies

- C++20
- Eigen
- CMake

## Results

The model achieved **97.065% test accuracy** on 477 sequences.

Evaluation included a confusion matrix to examine classification performance across the three classes.

## Key Concepts

- Neural network architecture
- Forward propagation
- Backpropagation
- Gradient descent
- ReLU and Softmax activation functions
- Cross-entropy loss
- Model training and evaluation
- DNA sequence encoding

## Purpose

This project was developed to gain practical experience with machine learning fundamentals and neural network implementations

# Dataset

The project uses the Splice-junction Gene Sequences dataset from the UCI Machine Learning Repository.

Download the dataset: https://archive.ics.uci.edu/dataset/69/molecular+biology+splice+junction+gene+sequences

Place the downloaded dataset files in the project's data/ directory.

Build and Run

# Clone the repository
git clone <repository-url>
cd GeneScope

# Configure the project
cmake -S . -B build

# Build
cmake --build build

# Run
./build/genescope