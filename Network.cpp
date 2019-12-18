// Network.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <iostream>
#include "matrix.h"
#include <ctime>
#include <stdlib.h> 
#include <time.h> 
#include <fstream>

double sigmoid(double);
double sigmoid_derivative(double);

class matrix;

class Network
{
private:
	int in_num;	 // Num of in neurons
	int hid_num; // Num of hidden neurons
	int out_num; // Num of out neurons
	double lern_rate; // Learning rate
	matrix wih; // in->hid weights  // hid_num*in_num //input = i, weights = j
	matrix who; // hid->out weights
	matrix in; // Input data
	matrix hid_input;
	matrix hid_output;
	matrix out_input;
	matrix out_output;
	matrix out_err;
	matrix hid_err;
	matrix target_data;

public:

	friend class matrix;

	// Set num of in, hid, out layers, lern_rate
	Network(int input, int hidden, int output, double lern_rate) 
		: in_num(input), hid_num(hidden), out_num(output)
	{
		this->lern_rate = lern_rate;
		/////////////Random weights/////////////
		cout << "Weights i->h: " << endl;
		generate_weights(wih, hid_num, in_num, "weights1.txt");
		cout << endl;
		cout << "Weights h->o: " << endl;
		generate_weights(who, out_num, hid_num, "weights2.txt");
		cout << endl;
		/////////////Weights from file//////////

	}

	void print_output()
	{
		out_output.print_matrix();
	}
	void print_input()
	{
		in.print_matrix();
	}

	matrix& apply_sigmoid(matrix &target)
	{
		double *temp = new double[target.get_height()*target.get_width()];
		for (int i = 0; i < target.get_height(); ++i)
		{
			for (int j = 0; j < target.get_width(); ++j)
			{
				temp[target.get_width()*i+j] = sigmoid(target.get_val(i, j));
			}
		}

		target.set(target.get_width(),target.get_height(), temp);

		delete[] temp;

		return target;
	}

	void modif_minus_one(matrix target, matrix &modificant)
	{
		double *temp = new double[target.get_height()*target.get_width()];
		for (int i = 0; i < target.get_height(); ++i)
		{
			for (int j = 0; j < target.get_width(); ++j)
			{
				temp[target.get_width()*i + j] = 1.0 - target.get_val(i,j);
			}
		}

		modificant.set(target.get_width(), target.get_height(), temp);

		delete[] temp;

	}


	void generate_inputs(double *temp)
	{
		for (int i = 0; i < in_num; i++)
		{
			if (temp[i] == 0) temp[i] = 0.0001;
			else if (temp[i] == 1) temp[i] = 0.9999;
		}

		in.set(1, in_num, temp);
		/*cout << "Input data: " << endl;
		cout << in.get_column(1) << endl;*/
		//cout << endl;
	}

	void generate_targets(double *temp)
	{
		for (int i = 0; i < in_num; i++)
		{
			if (temp[i] == 0) temp[i] = 0.0001;
			else if (temp[i] == 1) temp[i] = 0.9999;
		}

		target_data.set(1, out_num, temp); // Choose
		/*cout << "Target data: " << endl;
		cout << target_data.get_column(1) << endl;*/
	}

	// This function propagate input through network
	void forward_propagation(double *temp_in) 
	{
		generate_inputs(temp_in);

		hid_input = wih*in; // generate hid_input weights
	/*	cout << "Hid_input weights" << endl;
		hid_input.print_matrix();*/
		//cout << endl;

		hid_output = apply_sigmoid(hid_input); // generate hid_output weights
	/*	cout << "Hid_output weights" << endl;
		hid_output.print_matrix();*/
		//cout << endl;

		out_input = who*hid_output;
		/*cout << "Final input weights" << endl;
		out_input.print_matrix();*/
		//cout << endl;

		out_output = apply_sigmoid(out_input);
		/*cout << "Final output weights" << endl;
		out_output.print_matrix();*/
		//cout << endl;
	}

	// actually error propagation
	void back_propagation(matrix target_data)
	{
		out_err = target_data - out_output;
		/*cout << "Output errors" << endl;
		out_err.print_matrix();*/
		//cout << endl;

		hid_err = who.transpose() * out_err;
		/*cout << "Hidden errors" << endl;
		hid_err.print_matrix();*/
		//cout << endl;
	}

	//void train(double **input_list, double **target_list, int size_of_list)
	void train(double *input_list[], double *target_list[], int size_of_list)
	{

		//Global cycle to train on all input list
		for (int count = 0; count < size_of_list; count++)
		{
			forward_propagation(input_list[count]); // generate_inputs() inside
			generate_targets(target_list[count]);

			back_propagation(target_data);

			matrix out_output_modif; // for 1 - out_output
			modif_minus_one(out_output, out_output_modif);		

			who = who + lern_rate*((out_err*out_output*(out_output_modif))*hid_output.transpose());

			matrix hid_output_modif; // for 1 - hid_output
			modif_minus_one(hid_output, hid_output_modif);

			wih = wih + lern_rate*((hid_err*hid_output*(hid_output_modif))*in.transpose());

			/*cout << "Output" << endl;
			print_output();*/
			
		}

		// Writing wih
		
		// Writing who
	}

	void save_weights()
	{
		wih.file_write_matrix("weights1.txt");
		who.file_write_matrix("weights2.txt");
	}

	void generate_weights(matrix &target, int a_num, int b_num, char* filename) // Done
	{

		cout << "Enter 1 if you want randomize weights; 2 to read from file" << endl;
		int ck;

		do
		{
			cin >> ck;
			if (ck == 1)
			{
				double InputtedWeight;
				double *rand_mas = new double[a_num*b_num];
				for (int i = 0; i < a_num*b_num; ++i)
				{
					InputtedWeight = (rand() % 1000);
					rand_mas[i] = InputtedWeight / 1000;
				}
				target.set(b_num, a_num, rand_mas);

				target.print_matrix();

				delete[] rand_mas;
				break;
			}
			else if (ck == 2)
			{
				target.file_read_matrix(filename);
				target.print_matrix();
				break;
			}

		} while (ck != 1 || ck != 2);
		
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
	srand(time(0));
	double learn_rate = 0.01;/////////////////
	int in_num = 2;////////////////////
	int EPOCHS = 6000;///////////////

	Network net(in_num,2,1,learn_rate);
	
	double input[]{ 0,1 }; // enter inputs


	double ** input_list = new double*[4];

	input_list[0] = new double[in_num] {0, 0};
	input_list[1] = new double[in_num] {0, 1};
	input_list[2] = new double[in_num] {1, 0}; 
	input_list[3] = new double[in_num] {1, 1};

	double ** target_list = new double*[4];

	target_list[0] = new double[in_num] { 0 };
	target_list[1] = new double[in_num] { 1 };
	target_list[2] = new double[in_num] { 1 };
	target_list[3] = new double[in_num] { 0 };

	
	cout << "Enter 1 if you want to analyze input; 2 to teach network" << endl;
	int ck;

	do
	{
		cin >> ck;
		if (ck == 1)
		{
			net.forward_propagation(input);
			break;
		}
		else if (ck == 2)
		{
			for (int i = 0; i < EPOCHS; i++)
			{
				net.train(input_list, target_list, 4);
			}
			net.save_weights();
			break;
		}
	} while (ck != 1 || ck != 2);

	

	cout << "Input" << endl;
	net.print_input();
	cout << endl;
	cout << "Output" << endl;
	net.print_output();
	

	for (int i = 0; i < 4; i++)
	{
		delete[] target_list[i];
	}
	delete[] target_list;

	for (int i = 0; i < 4; i++)
	{
		delete[] input_list[i];
	}
	delete[] input_list;

    return 0;
}

