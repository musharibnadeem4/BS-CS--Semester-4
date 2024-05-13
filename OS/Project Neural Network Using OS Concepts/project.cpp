//20i-1764

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <fstream>
#include <cstring>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>
#include <vector>
#include "lock.h"

using namespace std;

int pipeid_g = 0;

string pipename = "pipe";

lockClass pipelock;

bool secondPass = 0;

//pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
//pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;

//sem_t sem1;
//sem_t sem2;
//sem_t sem3;

struct neuronAgrs
{
	vector<double> &input;
	vector<vector<double>> &weights;

	neuronAgrs(vector<double> &i, vector<vector<double>> &w) : input(i), weights(w)
	{
	}
};

struct neuronArgs2
{
	neuronAgrs *data;
	int id;
};

double outputFunc1(double x)
{
	return ((x * x) + x + 1) / 2.0;
}

double outputFunc2(double x)
{
	return ((x * x) - x) / 2.0;
}

void *Neuron(void *arg)
{

	int myid = ((neuronArgs2 *)arg)->id;

	neuronAgrs *data = ((neuronArgs2 *)arg)->data;

	vector<double> &input = data->input;
	vector<vector<double>> &weights = data->weights;

	delete (neuronArgs2 *)arg;

	double myvalue = 0;

	for (int i = 0; i < input.size(); ++i)
	{
		myvalue += input[i] * weights[i][myid];
	}

	string towrite = to_string(myvalue);

	char neuronValue[25] = {'\0'};

	for (int i = 0; i < towrite.length() && i < 24; i++)
	{
		neuronValue[i] = towrite[i];
	}

	while (pipelock.check(myid))
		;

	cout << "Neuron " << myid << " writing to pipe" << endl;

	write(pipeid_g, neuronValue, sizeof(neuronValue));

	pipelock.increment();

	pthread_exit(NULL);
}

void ForkCode(int Layer, int numNeurons, int wr_open, fstream &infile, int backpass_wr_open)
{
	cout << "Layer " << Layer << " ";

	vector<double> inputs;

	char token[25] = {'\0'};

	for (int i = 0; i < numNeurons; i++)
	{
		read(wr_open, token, sizeof(token));

		inputs.push_back(atof(token));

		for (int k = 0; k < 25; k++)
			token[k] = '\0';
	}

	cout << "Inputs: ";

	for (int i = 0; i < numNeurons; i++)
	{
		cout << inputs[i] << " ";
	}

	cout << "\n";

	string singleLine, singleLine2;

	stringstream sstr;

	vector<vector<double>> weights;
	vector<double> temp;

	bool test1 = true;

	while (!infile.eof())
	{
		getline(infile, singleLine2);

		if (singleLine2 == "Output Weights:")
		{
			test1 = false;
		}

		if ((singleLine2[0] >= 65 && singleLine2[0] <= 90) || (singleLine2[0] >= 97 && singleLine2[0] <= 122))
		{
			cout << singleLine2 << endl;
			break;
		}
	}

	while (!infile.eof())
	{
		singleLine.clear();
		getline(infile, singleLine);

		if (singleLine == "")
		{
			break;
		}

		sstr.clear();
		sstr << singleLine;
		singleLine2.clear();
		temp.clear();
		while (getline(sstr, singleLine2, ','))
		{
			int x = 0;

			for (int i = 0; i < singleLine2.length() && x < 24; i++)
			{

				if (singleLine2[i] != ' ')
				{
					token[x] = singleLine2[i];
					x++;
				}
			}

			temp.push_back(atof(token));
			for (int k = 0; k < 25; k++)
				token[k] = '\0';
		}
		weights.push_back(temp);
	}

	neuronAgrs toPassvals(inputs, weights);

	int numThreads = weights[0].size();

	pthread_t threads[numThreads];

	for (int i = 0; i < numThreads; ++i)
	{
		neuronArgs2 *arg = new neuronArgs2;

		arg->id = i;

		arg->data = &toPassvals;

		pthread_create(&threads[i], NULL, Neuron, (void *)arg);
	}

	for (int i = 0; i < numThreads; ++i)
	{
		pthread_join(threads[i], NULL);
	}

	pipelock.clear();

	if (test1 == false)
	{
		temp.clear();

		for (int i = 0; i < numThreads; i++)
		{
			read(wr_open, token, sizeof(token));

			temp.push_back(atof(token));

			for (int k = 0; k < 25; k++)
				token[k] = '\0';
		}

		cout << "Outputs Are: ";

		for (int i = 0; i < temp.size(); i++)
		{
			cout << temp[i] << " ";
		}

		if (secondPass == 0)
		{
			vector<double> outputs;

			for (int i = 0; i < numThreads; i++)
			{
				double currentvalue = temp[i];

				outputs.push_back(outputFunc1(currentvalue));
				outputs.push_back(outputFunc2(currentvalue));
			}

			string outputVals;

			for (int i = 0; i < outputs.size(); i++)
			{
				if (i < outputs.size() - 1)
				{
					outputVals += to_string(outputs[i]) + ", ";
				}
				else if (i == outputs.size() - 1)
				{
					outputVals += to_string(outputs[i]);
				}
			}

			cout << "\nValues Generated After functions: ";

			cout << outputVals << endl;

			write(backpass_wr_open, outputVals.c_str(), outputVals.length());
		}

		return;
	}

	Layer++;

	int backpasspipe = 0;

	int backpass_wr_open_next = 0;

	string tempstr;

	if (secondPass == 0)
	{
		tempstr = pipename;

		tempstr += to_string(Layer);

		backpasspipe = mkfifo(tempstr.c_str(), 0666);

		backpass_wr_open_next = open(tempstr.c_str(), O_RDWR);
	}

	int pid = fork();

	if (pid >= 0)
	{
		if (pid == 0)
		{
			if (secondPass == 0)
			{
				ForkCode(Layer, numThreads, wr_open, infile, backpass_wr_open_next);
			}
			else
			{
				ForkCode(Layer, numThreads, wr_open, infile, 0);
			}
		}
		else
		{
			if (secondPass == 0)
			{
				char token2[100] = {'\0'};

				read(backpass_wr_open_next, token2, 100);

				unlink(tempstr.c_str());

				write(backpass_wr_open, token2, 100);
			}
		}
	}
}

int main()
{
	int pipd = mkfifo("pipd", 0666);

	if (pipd < 0)
	{
		unlink("pipd");
		pipd = mkfifo("pipd", 0666);
	}

	int wr_open = open("pipd", O_RDWR);

	pipeid_g = wr_open;

	fstream infile;

	infile.open("input.txt", ios::in);

	string singleLine, singleLine2;

	stringstream sstr;

	int numNeurons = 0;

	char token[25] = {'\0'};

	getline(infile, singleLine);
	getline(infile, singleLine);
	sstr << singleLine;

	while (getline(sstr, singleLine2, ','))
	{
		int x = 0;
		for (int i = 0; i < singleLine2.length() && x < 24; i++)
		{
			if (singleLine2[i] != ' ')
			{
				token[x] = singleLine2[i];
				x++;
			}
		}

		write(wr_open, token, sizeof(token));
		numNeurons++;

		for (int k = 0; k < 25; k++)
			token[k] = '\0';
	}

	int layer = 0;

	int pid = fork();

	layer++;

	string tempstr = pipename;

	tempstr += to_string(layer);

	int backpasspipe = mkfifo(tempstr.c_str(), 0666);

	int backpass_wr_open = open(tempstr.c_str(), O_RDWR);

	if (pid >= 0)
	{
		if (pid == 0)
		{
			ForkCode(layer, numNeurons, wr_open, infile, backpass_wr_open);
		}
		else
		{
			char token2[100] = {'\0'};

			read(backpass_wr_open, token2, sizeof(token2));

			unlink(tempstr.c_str());

			numNeurons = 0;

			string tempOut(token2);
			stringstream ss;
			ss << tempOut;
			while (getline(ss, tempOut, ','))
			{
				int x = 0;

				for (int i = 0; i < tempOut.length(); i++)
				{
					if (tempOut[i] != ' ')
					{
						token[x] = tempOut[i];
						x++;
					}
				}
				write(wr_open, token, sizeof(token));
				numNeurons++;

				for (int k = 0; k < 25; k++)
					token[k] = '\0';
			}

			infile.close();

			infile.open("input.txt", ios::in);

			getline(infile, singleLine);
			getline(infile, singleLine);

			layer = 1;

			secondPass = 1;

			ForkCode(layer, numNeurons, wr_open, infile, 0);
		}
	}

	unlink("pipd");

	return 0;
}