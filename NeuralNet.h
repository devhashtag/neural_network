#ifndef NEURALNET_H
#define NEURALNET_H

#include <vector>
#include "Matrix.h"

class NeuralNet {
public:
	// the layout of the network (nodes per layer)
	vector<int> size;

	// weights
	vector<Matrix> synapses;

	// biases
	vector<Matrix> bias;

	// delta for the weights (backpropagation)
	vector<Matrix> delta_s;

	// delta for the biases (backprop)
	vector<Matrix> delta_b;
	
	// outputs
	vector<Matrix> layer_outputs;

	// constructor with layout as argument
	NeuralNet(const vector<int>& size);

	// constructor with file as argument
	NeuralNet(const char * filename);

	// save the network to a file
	int saveNet(const char * filename);

	// activation function
	static double activate(double x, int deriv = false);

	//error function
	static double error(Matrix& out, Matrix& target);

	// forward propagation
	Matrix predict(Matrix& input);

	// backward propagation
	void back_propagate(Matrix& target, int print = false);

	//void back_propagate(const vector<double>& expected);
};
#endif