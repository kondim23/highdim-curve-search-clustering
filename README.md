# High-Dimensional Vector & Curve Search & Clustering Suite

## Table of Contents
1. [Overview](#overview)
2. [Motivation, Importance & Scope](#motivation-importance--scope)
3. [Theoretical Background](#theoretical-background)
4. [Key Features](#key-features)
5. [Algorithms & Methodology](#algorithms--methodology)
6. [Input & Output Specifications](#input--output-specifications)
7. [Build & Execution](#build--execution)
8. [Configuration & Parameters](#configuration--parameters)
9. [Results & Evaluation](#results--evaluation)
10. [Code Quality & Best Practices](#code-quality--best-practices)
11. [Directory Structure](#directory-structure)
12. [Installation & Dependencies](#installation--dependencies)
13. [Extensibility & Maintenance](#extensibility--maintenance)
14. [Authors & License](#authors--license)

---

## Overview
This repository provides a robust, extensible software suite for efficient nearest neighbor search and clustering in high-dimensional vectors and polygonal curves. The project leverages advanced algorithmic techniques (LSH, Hypercube, Fréchet) and is implemented in C/C++ for performance and scalability. It is designed for both academic research and industrial applications where large-scale, high-dimensional data analysis is required.

---

## Motivation, Importance & Scope
High-dimensional vectors and polygonal curves are central to a wide range of modern applications:
- **Finance:** Portfolio modeling, risk analysis, and market pattern recognition
- **Bioinformatics:** Protein structure comparison, gene expression analysis
- **Geospatial Analytics:** Trajectory clustering, route optimization, and movement pattern analysis
- **Computer Vision & Graphics:** Shape matching, object recognition, and gesture analysis

The analysis of such data is challenging due to the "curse of dimensionality" and the complexity of geometric similarity. Standard search and clustering algorithms become computationally prohibitive as data size and dimensionality increase, and they often fail to capture the true structure of geometric data such as curves.

This project directly addresses these challenges by providing:
- **Scalable, high-performance algorithms** for approximate nearest neighbor search and clustering in high-dimensional vector and curve spaces
- **Advanced similarity measures** (e.g., Fréchet distance) that accurately reflect geometric relationships
- **A modular, extensible C/C++ platform** suitable for both research and industrial deployment, enabling rapid experimentation and robust integration

By enabling efficient and accurate analysis of vectors and curves, this suite empowers data-driven decision-making and innovation in fields where geometric and high-dimensional data are critical.

---

## Theoretical Background
### Locality Sensitive Hashing (LSH)
Locality Sensitive Hashing is a technique for efficient approximate nearest neighbor search in high-dimensional spaces. It uses hash functions designed so that similar items are more likely to be mapped to the same hash bucket. This enables sublinear query times for similarity search, especially with metrics like L2 (Euclidean) and Fréchet distance for curves.

### Hypercube Method
The Hypercube algorithm projects high-dimensional data into the vertices of a binary hypercube using random projections. Search is performed by probing the cube's vertices in a controlled manner, balancing accuracy and efficiency. This method is particularly effective for very high-dimensional vector spaces.

### k-means++ Clustering
k-means++ is an improved initialization method for the k-means clustering algorithm. It selects initial centroids to maximize their spread, leading to better convergence and clustering quality. The assignment step can be performed exactly (Lloyd's) or approximately (using LSH or Hypercube), and the update step computes new centroids as mean vectors or curves.

### Fréchet Distance
Fréchet distance is a measure of similarity between curves, capturing both the location and ordering of points. The discrete version considers only the sequence of points, while the continuous version allows for more flexible alignment. It is particularly suited for geometric and trajectory analysis.

---

## Key Features
- **Efficient Search:** LSH and Hypercube for vectors; LSH for Discrete/Continuous Fréchet on curves
- **Advanced Clustering:** k-means++ initialization, Lloyd's, LSH, and Hypercube-based assignment; mean vector/curve updates
- **Strict Input/Output Compliance:** Adheres to well-defined formats for reproducibility and integration
- **Command-Line Tools:** Rich configuration and automation support
- **Code Quality:** Modular, maintainable, and thoroughly documented

---

## Algorithms & Methodology
- **Locality Sensitive Hashing (LSH):** Accelerates approximate nearest neighbor search in high-dimensional spaces using similarity-preserving hash functions. Supports both L2 (vector) and Fréchet (curve) metrics.
- **Hypercube Search:** Projects data into binary space for rapid search using random projections and controlled probing.
- **k-means++ Clustering:** Enhanced initialization for k-means, with assignment via Lloyd's, LSH, or Hypercube, and update via mean vector or mean curve.
- **Fréchet Distance:** Measures similarity between curves in both discrete and continuous forms, enabling advanced geometric analysis.

### Supported Search & Clustering Techniques
- **Search:**
  - LSH (L2) and Hypercube for vectors
  - LSH (Discrete/Continuous Fréchet) for curves
- **Clustering:**
  - Initialization: k-means++
  - Assignment: Lloyd's (Classic), LSH (Vector/Fréchet), Hypercube
  - Update: Mean as vector or curve (with assignment compatibility)

---

## Input & Output Specifications
### Input Format
Tab-separated, one vector or curve per line:
```
item_id1 X11 X12 ... X1d
item_id2 X21 X22 ... X2d
...
item_idN XN1 XN2 ... XNd
```

### Output Format
#### Nearest Neighbor Search
```
Query: <item_id>
Algorithm: LSH_Vector | Hypercube | LSH_Frechet_Continuous | LSH_Frechet_Discrete
Approximate Nearest Neighbor: <item_id>
True Nearest Neighbor: <item_id>
distanceApproximate: <double>
distanceTrue: <double>
tApproximateAverage: <double>
tTrueAverage: <double>
MAF: <double>
```
#### Clustering
```
Algorithm: <Assignment Method>
CLUSTER-1 {size: <int>, centroid: [coordinates or points]}
...
CLUSTER-k {size: <int>, centroid: [coordinates or points]}
clustering_time: <double>
Silhouette: [s1, ..., sk, stotal]   # if -silhouette used
CLUSTER-1 {centroid, item_idA, ...} # if -complete used
```

---

## Build & Execution
- Compile with `make` (produces executables in `bin/`):
  ```bash
  make
  ```
- Run according to assignment specifications:
  ```bash
  ./bin/search -i <input file> -q <query file> -k <int> -L <int> -M <int> -probes <int> -o <output file> -algorithm <LSH | Hypercube | Frechet> -metric <discrete | continuous> -delta <double>
  ./bin/cluster -i <input file> -c <config file> -o <output file> -update <Frechet | Vector> -assignment <Classic | LSH | Hypercube | LSH_Frechet> -complete -silhouette
  ./bin/test
  ```
- Note: Algorithms are randomized; multiple runs may be needed for expected results.

---

## Configuration & Parameters
### Clustering Configuration File (cluster.conf)
```ini
number_of_clusters: <int>
number_of_vector_hash_tables: <int>        # Default: 3
number_of_vector_hash_functions: <int>     # Default: 4
max_number_M_hypercube: <int>              # Default: 10
number_of_hypercube_dimensions: <int>      # Default: 3
number_of_probes: <int>                    # Default: 2
```

---

## Results & Evaluation
- **Clustering:** Silhouette score, cluster sizes, centroids, and assignment details
- **Search:** Approximation ratios, timing, and neighbor lists
- **Reproducibility:** Strict adherence to input/output formats for reliable benchmarking

---

## Directory Structure
```
project_1_NN_Clustering/
├── bin/        # Compiled executables (search, cluster, test)
├── obj/        # Compiled object files
├── include/    # Header files
├── src/        # Source code
├── test/       # Unit test source and headers
├── data/       # Example datasets
├── configs/    # Configuration files
├── results/    # Output/result files
├── README.md   # Project documentation
└── ...
```

---

## Installation & Dependencies
### C/C++
- GCC or Clang (C++11 or newer)
- Make

---

## Extensibility & Maintenance
- Modular codebase: add new distance metrics, clustering methods, or algorithmic modules with minimal changes
- Well-documented interfaces and configuration files
- Designed for reproducibility, benchmarking, and future research or production deployment

---