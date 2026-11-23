# Architecture

GeneScope separates the pipeline into layers:

1. **Data**: validation, label parsing, and reproducible data splits.
2. **Features**: one-hot tensors and fixed-size k-mer count vectors.
3. **Models**: shared model-facing interfaces, followed by MLP and 1D CNN implementations.
4. **Evaluation**: accuracy, per-class precision/recall/F1, confusion matrices, and error analysis.

The UCI data format and data-split policy should be documented before training results are compared. Keep raw data immutable; store every generated artefact in `data/processed/` or a run-specific directory under `experiments/`.
