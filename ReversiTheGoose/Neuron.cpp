#include "Neuron.h"
#include "NeuralNetwork.h"
#include <algorithm>
#include <iostream>

Neuron::Neuron(int new_neuron_index, int output_num) : index_(new_neuron_index), value_(0.0), gradient_(0.0)
{
	this->output_weights_.assign(output_num, Connection());
}

void Neuron::SetValue(double new_value)
{
	this->value_ = new_value;
}
double Neuron::GetValue() const
{
	return this->value_;
}


void Neuron::FeedForwardFrom(const Layer& previous_layer)
{
	this->value_ = 0.0;

	for (const Neuron& prev_neuron : previous_layer)
	{
		this->value_ += prev_neuron.value_ * prev_neuron.output_weights_[this->index_].weight_;
	}

	this->value_ = Neuron::TransferFunction(this->value_);
}

void Neuron::UpdateOutputLayerGradient(double target_value)
{
	double loss = target_value - this->value_;
	this->gradient_ = loss * Neuron::TransferFunctionDerv(this->value_);
}

void Neuron::UpdateHiddenLayerGradient(const Layer& next_layer)
{
	//sum of derivative of weight
	double sum_DOW = 0.0;
	for (int neuron_index = 0; neuron_index < next_layer.size() - 1; ++neuron_index)
	{
		sum_DOW += this->output_weights_[neuron_index].weight_ * next_layer[neuron_index].gradient_;
	}

	this->gradient_ = sum_DOW * Neuron::TransferFunctionDerv(this->value_);
}

void Neuron::UpdateLayerWeight(Layer& prev_layer)
{
	for (Neuron& prev_neuron : prev_layer)
	{
		double old_diff_weight = prev_neuron.output_weights_[this->index_].diff_weight_;
		double new_diff_weight = prev_neuron.value_ * this->gradient_ * LEARNING_RATE + old_diff_weight * MOMENTUM_RATE;

		prev_neuron.output_weights_[this->index_].diff_weight_ = new_diff_weight;
		prev_neuron.output_weights_[this->index_].weight_ += new_diff_weight;
	}
}

double Neuron::TransferFunction(double value)
{
	//sigmoid 1.0 / (1.0 - std::exp(-value))
	return tanh(value);
}

double Neuron::TransferFunctionDerv(double value)
{
	double tanh_value = tanh(value);
	return 1.0 - tanh_value * tanh_value;
}