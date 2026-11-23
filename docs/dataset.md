# Dataset notes

Place the UCI Splice-junction source file in `data/raw/`. Do not modify it in place.

The original records contain a class label, an identifier, and a fixed-length DNA sequence. The loader will normalize bases to upper-case and reject records containing bases outside `A`, `T`, `C`, and `G` unless an explicit ambiguous-base policy is added.

Recommended initial split: stratified 70% train, 15% validation, and 15% test using a fixed random seed. Never tune model settings against the test set.
