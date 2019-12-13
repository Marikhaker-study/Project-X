// Network.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include "matrix.h"
#include <ctime>

class matrix;

class Network
{
private:
	int in_num;	 // Num of in neurons
	int hid_num; // Num of hidden neurons
	int out_num; // Num of out neurons
	float lern_rate; // Learning rate
	matrix wih; // in->hid weights 
	matrix who; // hid->out weights
	matrix in;
	matrix hid;
	matrix out;
	
public:

	friend class matrix;

	Network(int input, int hidden, int output, float lern_rate)
		: in_num(input), hid_num(hidden), out_num(output)
	{
		this->lern_rate = lern_rate;
	}

	void train(matrix input_list, matrix target_list)
	{

	}

	void generate_weights(matrix wih, matrix who)
	{
		double InputtedWeight;
		double *rand_mas = new double[hid_num*in_num];
		srand(time(0));
		for (int i = 0; i < hid_num; ++i)
		{
			for (int j = 0; j < in_num; ++j)
			{
				InputtedWeight = (rand() % 10000);
				rand_mas[i] = InputtedWeight / 10000;
			};
		}
		wih.set(in_num, hid_num, rand_mas);
		delete[] rand_mas;
	}
		
};

double sigmoid(double x)
{
	double exp_v;
	double return_value;
	exp_v = exp(-x);
	return_value = 1 / (1 + exp_v);

	return return_value;
}

double sigmoid_derivative(double x)
{
	double return_value;
	return_value = sigmoid(x)*(1 - sigmoid(x));
	return return_value;
}

int main()
{
	matrix input;

    return 0;
}

