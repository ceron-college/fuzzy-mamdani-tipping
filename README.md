# Mamdani fuzzy inference Tipping System

This project implements a fuzzy logic system to infer tipping amounts based on service and food quality, using various membership functions and Mamdani inference.

## Key Features

- **Membership Functions**: Triangular, Trapezoidal, Saturation, Gaussian.
- **Fuzzy Inference**: Mamdani inference for combining fuzzy rules.
- **Fuzzy Sets**: Handles input (e.g., service, food) and output (e.g., tip) fuzzy sets.

## Rule and Fuzzy Set Format

For future use in similar projects, the system can handle rules and fuzzy set definitions in the following format:

### Example Rules

```
IF Service_Poor AND Food_Poor THEN Tip_Low
IF Service_Average AND Food_Average THEN Tip_Medium
IF Service_Excellent AND Food_Excellent THEN Tip_High
```

### Example Fuzzy Sets

```
Service_Poor SAT 0 50
Service_Average TRIANG 0 50 100
Service_Excellent SAT 50 100
Tip_Low
Tip_Medium
Tip_High
```

A help for this project's Mandani implementation was the code made by [Pablohrdz](https://github.com/Pablohrdz/Fuzzy-Engine).

## How to Run

Compile and run the program:

```bash
g++ -o fuzzy_tipping main.cpp -lm
./fuzzy_tipping
```

Ensure `variables.txt` and `rules.txt` files are in the directory.
