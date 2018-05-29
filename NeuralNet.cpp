#include "NeuralNet.h"
#include "Matrix.h"
#include <vector>
#include <cmath>
#include <iostream>
#include <fstream>
#include <string>

NeuralNet::NeuralNet(const vector<int>& setup) {
	size = setup;

	// create the synapses
	for (int i = 0; i < (int)size.size() - 1; i++) {
		synapses.push_back(Matrix(size[i], size[i + 1]));
		delta_s.push_back(Matrix(size[i], size[i + 1]));

		bias.push_back(Matrix(1, size[i + 1]));
		delta_b.push_back(Matrix(size[i], size[i + 1]));

		layer_outputs.push_back(Matrix(1, 1));
	}
	layer_outputs.push_back(Matrix(1, 1));
}

NeuralNet::NeuralNet(const char * filename) {
	ifstream file(filename);
	if (!file.is_open())
		throw "CAN'T OPEN FILE TO CONFIGURE NEURAL NETWORK";

	int s;
	file >> s;

	for (int i = 0; i < s; i++) {
		size.push_back(0);
		file >> size[i];
	}

	for (int i = 0; i < s - 1; i++) {
		synapses.push_back(Matrix(size[i], size[i + 1]));
		delta_s.push_back(Matrix(size[i], size[i + 1]));

		for (int j = 0; j < synapses[i].size(); j++) {
			file >> synapses[i](j); 
		}
	}

	for (int i = 0; i < s - 1; i++) {
		bias.push_back(Matrix(1, size[i + 1]));
		delta_b.push_back(Matrix(size[i], size[i + 1]));
		layer_outputs.push_back(Matrix(1, 1));

		for (int j = 0; j < bias[i].size(); j++) {
			file >> bias[i](j); 
		}
	}
	layer_outputs.push_back(Matrix(1, 1));

	file.close();
}

int NeuralNet::saveNet(const char * filename) {
	ofstream file(filename);
	if (!file.is_open())
		return 1;

	file << size.size() << "\n";
	for (unsigned int i = 0; i < size.size(); i++) {
		file << size[i] << "\n";
	}

	for (unsigned int i = 0; i < synapses.size(); i++) {
		for (int j = 0; j < synapses[i].size(); j++) {
			file << synapses[i](j) << "\n";
		}
	}


	for (unsigned int i = 0; i < bias.size(); i++) {
		for (int j = 0; j < bias[i].size(); j++) {
			file << bias[i](j) << "\n";
		}
	}

	file.close();
  
	return 0;
}

double NeuralNet::activate(double x, int deriv) {
	return deriv ? x * (1 - x) : 1 / (1 + exp(-x));
}

double NeuralNet::error(Matrix& output, Matrix& target) {
	double err = 0.0;
	for (int i = 0; i < output.size(); i++) {
		err += 0.5 * (target(i) - output(i)) * (target(i) - output(i));
	}
		//err += 0.5 * (target(i) - output(i)) * (target(i) - output(i));

	return err;
}

Matrix NeuralNet::predict(Matrix& input) {
	if (input.size() != size[0])
		throw "Input size does not match network input layer";

	layer_outputs[0] = input;

	for (int i = 0; i < (int)synapses.size(); i++) {
		
		// multiply weights, add bias
		layer_outputs[i + 1] = (synapses[i] ^ layer_outputs[i]) + bias[i];

		// run activation function
		for (int j = 0; j < layer_outputs[i + 1].size(); j++)
			layer_outputs[i + 1](j) = NeuralNet::activate(layer_outputs[i + 1](j));
	}


	// return last output layer
	return layer_outputs[layer_outputs.size() - 1];
}

void NeuralNet::back_propagate(Matrix& target, int print) {
	Matrix temp = layer_outputs[layer_outputs.size() - 1] - target;
	

	for (int i = synapses.size() - 1; i >= 0; i--) {
		for (int j = 0; j < temp.size(); j++)
			temp(j) = temp(j) * NeuralNet::activate(layer_outputs[i + 1](j), true);
		
		delta_s[i] = temp ^ layer_outputs[i].T();
		delta_b[i] = temp * 1.0;

		temp = synapses[i].T() ^ temp;
	}

	double learning_rate = 1;
	for (unsigned int i = 0; i < synapses.size(); i++) {
		synapses[i] = synapses[i] - delta_s[i] * learning_rate;
		bias[i] = bias[i] - delta_b[i] * learning_rate;
	}
}