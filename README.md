# OCR - Optical Character Recognition Neural Network

A simple neural network implementation in C for Optical Character Recognition (OCR) tasks. This project demonstrates the fundamentals of neural networks through a practical XOR function learning example.

## Features

- Multi-layer perceptron neural network implementation
- Backpropagation learning algorithm
- Sigmoid activation function
- Weight persistence (save/load)
- Interactive testing mode
- SDL2 integration for potential image processing

## Project Structure

```
OCR/
├── NN/
│   └── NN/
│       ├── Reseau/
│       │   ├── main.c          # Main program entry point
│       │   ├── network.c       # Neural network implementation
│       │   ├── network.h       # Network header definitions
│       │   └── weights.txt     # Saved network weights
│       └── lib/
│           └── SDL2-*          # SDL2 libraries for Windows
```

## Technical Details

### Neural Network Architecture
- **Input Layer**: 2 neurons
- **Hidden Layer**: 4 neurons  
- **Output Layer**: 1 neuron
- **Learning Rate**: 0.1
- **Activation Function**: Sigmoid

### Current Implementation
The network is currently configured to learn the XOR function as a proof of concept:
- Training continues until error < 0.0004
- Supports 4 training examples
- Uses backpropagation for weight updates

## Building

### Prerequisites
- C compiler (GCC/MinGW for Windows)
- SDL2 libraries (included for Windows)

### Compilation
```bash
gcc -o main.exe main.c network.c -lm
```

## Usage

1. **Training Phase**: The program automatically trains the network on the XOR function
2. **Weight Saving**: Trained weights are saved to `weights.txt`
3. **Testing Mode**: Interactive prompt to test the learned XOR function

```bash
./main.exe
```

Example interaction:
```
Apprentissage de la fonction XOR...
Sauvegarde des poids dans weights.txt ...
Vous pouvez maintenant tester la fonction XOR
x0 = 1
x1 = 0
Output: ~1.0
```

## API Functions

### Core Functions
- `allocNet()` - Allocate memory for network structure
- `propagation()` - Forward propagation through the network
- `back_propagation()` - Backpropagation for error correction
- `MAJweights()` - Update network weights
- `saveWeights()` - Save weights to file
- `utilisation()` - Use trained network for predictions

### Utility Functions
- `fonctionActivation()` - Sigmoid activation function
- `fonctionActivationDerivative()` - Derivative for backpropagation
- `sommePondere()` - Calculate weighted sum
- `initDeltas()` - Initialize delta values
- `initNodes()` - Initialize node values

## Future Improvements

- [ ] Implement actual OCR functionality for character recognition
- [ ] Add image preprocessing using SDL2
- [ ] Support for multiple character recognition
- [ ] GUI for drawing/testing characters
- [ ] Variable network architecture configuration
- [ ] Support for different activation functions
- [ ] Batch training implementation
- [ ] Cross-validation support

## License

This project is open source and available for educational purposes.

## Author

Personal OCR project implementation for learning neural network fundamentals.