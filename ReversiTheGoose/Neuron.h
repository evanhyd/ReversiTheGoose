#pragma once
#include "Connection.h"
#include <vector>
#include <string>

class NeuralNetwork;

class Neuron
{
	using Layer = std::vector<Neuron>;

private:
	int index_;
	double value_;
	std::vector<Connection> output_weights_;
	double gradient_;

public:
	Neuron(int neuron_index, int output_num);
	void SetValue(double new_value);
	double GetValue() const;

	void FeedForwardFrom(const Layer& previous_layer);
	void UpdateOutputLayerGradient(double target_value);
	void UpdateHiddenLayerGradient(const Layer& next_layer);
	void UpdateLayerWeight(Layer& prev_layer);

private:
	static constexpr double LEARNING_RATE = 0.005; //eta
	static constexpr double MOMENTUM_RATE = 0.15; //alpha
	static double TransferFunction(double value);
	static double TransferFunctionDerv(double value);

	friend NeuralNetwork;
};

