﻿#include <iostream>
#include <sstream>
#include <chrono>
#include <cmath>

#include "HANNA/HANNA.hpp"

float sigmoid(const float& x) { return 1.f / (1.f + std::exp(-x)); }
float sigmoidDV(const float& x) { return x * (1.f - x); }

struct normConf { float min, delta; }; // delta = max - min
void minmaxnorm(std::valarray<float>& a, normConf conf)
{
	if (conf.min == *std::max_element(&a[0], &a[a.size() - 1]))
	{
		a = 0.f;
		return;
	}

	long long int size = a.size();
	for (long long int i = 0; i < size; ++i)
		a[i] = (a[i] - conf.min) / conf.delta;
}

std::vector<std::valarray<float>> readCSV(std::string path)
{
	std::ifstream r(path, std::ios::in);
	if (!r || !r.is_open() || !r.good()) return std::vector<std::valarray<float>>(0);

	std::vector<std::valarray<float>> data(0);
	std::size_t elems = 0;

	std::string buffer("");
	std::string elem("");

	std::getline(r, buffer); // First line is header
	{
		std::stringstream header(buffer);
		while (std::getline(header, elem, ','))
			++elems;
	}
	--elems; // first is 'id'

	while (std::getline(r, buffer))
	{
		std::stringstream row(buffer);
		std::valarray<float> rowvec(elems);
		
		std::getline(row, elem, ','); // First elem is 'id'
		for (std::size_t i = 0; std::getline(row, elem, ','); ++i)
			rowvec[i] = std::stof(elem);
		data.push_back(rowvec);
	}
	return data;
}

int main()
{
	std::chrono::high_resolution_clock::time_point tp[2] = { std::chrono::high_resolution_clock::now() };
	
	std::vector<std::valarray<float>> data = readCSV("cardio-hdd.csv");
	std::size_t rows = data.size();
	std::size_t cols = data[0].size();
	
	std::vector<std::valarray<float>> input (rows, std::valarray<float>(cols - 1));
	std::vector<std::valarray<float>> output(rows, std::valarray<float>(1));
	
	// Age, Gender, Height, Weight, ap_hi, ap_lo, cholesterol, gluc, smoke, alco, active, cardio
	std::vector<bool> normalizecol({ true, false, true, true, true, true, true, true, false, false, false, false });
	
	// Col1: Gender. Nobody knows why it's 1 = Male and 2 = Female instead of 0 and 1
	for (std::size_t row = 0; row < rows; ++row)
		--data[row][1];
	
	for (std::size_t col = 0; col < cols; ++col)
	{
		if (normalizecol[col])
		{
			std::valarray<float> coldata;
			coldata.resize(rows);
			for (std::size_t row = 0; row < rows; ++row)
				coldata[row] = data[row][col];
			
			float min = *std::min_element(&(coldata[0]), &(coldata[rows - 1]));
			normConf conf = { min, *std::max_element(&(coldata[0]), &(coldata[rows - 1])) - min };
			minmaxnorm(coldata, conf);
	
			for (std::size_t row = 0; row < rows; ++row)
				data[row][col] = coldata[row];
		}
	}
	
	std::size_t inputsize = input[0].size();
	for (std::size_t row = 0; row < rows; ++row)
	{
		for (std::size_t col = 0; col < inputsize; ++col)
			input[row][col] = data[row][col];
		output[row][0] = data[row][inputsize];
	}
	data.clear();
	
	tp[1] = std::chrono::high_resolution_clock::now();
	std::cout << "Red, initialized and formatted dataset in " << std::chrono::duration_cast<std::chrono::milliseconds>(tp[1] - tp[0]).count() << "ms.\n";
	
	MLP::MLP hdm({ inputsize, inputsize, inputsize / 2, 1});
	std::size_t epochs = 10000;
	
	hdm.enableTraining();
	hdm.lr = 0.05f;
	tp[0] = std::chrono::high_resolution_clock::now();
	// hdm.train(input, output, sigmoid, sigmoidDV, epochs);
	tp[1] = std::chrono::high_resolution_clock::now();
	hdm.disableTraining();
	
	std::cout << "Took " << std::chrono::duration_cast<std::chrono::seconds>(tp[1] - tp[0]).count() << "s +- 500ms.\n";
	hdm.load("HDM.MLP");
	std::cout << "--- Lazy Test ---\n";
	
	std::size_t testspassed = 0;
	for (std::size_t i = 0; i < rows; ++i)
	{
		float out = 0.f;
		if (hdm.out(input[i], sigmoid)[0] >= 0.5) out = 1.f;
		if (out == output[i][0]) ++testspassed;
		if ((i % 1000) == 0)
			std::cout << float(float(i) / float(rows)) * 100.f << "% tested | " << float(float(testspassed) / float(rows)) * 100.f << "% accuracy.\n";
	}
	std::cout << "Passed " << testspassed << " out of " << rows << " tests." << std::endl;
	
	return 0;
	
	hdm.suicide();

	return 0;
}