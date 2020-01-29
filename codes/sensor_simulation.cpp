#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <thread>
#include <semaphore.h>

using namespace std;

int max_v;
sem_t s1;
sem_t s2;
sem_t in_sem;
sem_t b1[20];
sem_t b2;
int input_counter;
int cal_counter;
ofstream file;
int file_counter;


void split_line(string line, vector <int> &numbers)
{
	vector < int > temp_line;
	string word;
	for(int i =0; i < line.size(); i++)
	{
		if(line[i] == ' ' && word != "")
		{
			temp_line.push_back(atoi(word.c_str()));
			word = "";
		}
		else
			word += line[i];
	}
	temp_line.push_back(atoi(word.c_str()));
	for(int i =0; i < temp_line.size(); i++)
	{
		numbers.push_back(temp_line[i]);
	}

}

void read_data(int index, string line)
{
	vector <int> numbers;
	split_line(line, numbers);

	for(int i =0; i < 20; i++)
	{	
		sem_wait(&in_sem);
		input_counter ++;
		sem_post(&in_sem);
		if(input_counter == 10)
		{
			max_v = 0;
			sem_init(&b2, 0, 0);
			sem_post(&b1[i]);
		}
		sem_wait(&b1[i]);
		sem_post(&b1[i]);
		input_counter = 0;
		sem_wait(&s2);
		cal_counter ++;
		if(max_v < numbers[i])
		{
			max_v = numbers[i];
		}
		sem_post(&s2);
		if(cal_counter == 10)
		{
			sem_post(&b2);
			cal_counter = 0;
		}
		sem_wait(&b2);
		sem_post(&b2);
		double temp;
		sem_wait(&s1);
		// file<< double(numbers[i]) / max_v << "    ";
		file<< double(numbers[i]) / max_v << "  " <<numbers[i] << "  " <<"max value is  " <<max_v<<"   "<< endl;

		file_counter ++;
		if(file_counter == 10)
		{
			file_counter = 0;
			file << endl <<endl;
		}

		sem_post(&s1);
	}
}


int main()
{
	vector <thread> threads;
	max_v = 0;
	file.open("output.txt");
	sem_init(&s1, 0, 1);
	sem_init(&s2, 0, 1);
	sem_init(&in_sem, 0, 1);
	for(int i=0;i<20;i++)
		sem_init(&b1[i], 0, 0);
	input_counter = 0;
	cal_counter = 0;
	file_counter = 0;


	ifstream infile("input.txt");
	string line;
	int counter = 0;
	while(getline(infile, line))
	{
		threads.push_back(thread(read_data, counter, line));
		counter ++;
	}
	// for(int i =0; i < 10; i++)
	// {
	// 	cout << threads[i].get_id() <<endl;
	// }

	for(int i =0; i < 10; i ++)
	{
		threads[i].join();
	}


	// for(int i =0; i < 20; i ++)
	// 	cout << max_v[i] <<endl;
	// double temp;
	// for(int i =0; i < 20; i++)
	// {
	// 	for(int j =0; j< 10; j ++)
	// 	{
	// 		temp = 
	// 		file << 
	// 	}
	// }

	// 799 - 167 - 183 - 
}