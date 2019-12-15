#include "pch.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <ctime>


struct Individ
{
	float x;
	float y;
};

float FitFunc(Individ indiv)
{
	return exp(-pow(indiv.x, 2) - pow(indiv.y, 2));
}

float MeanFitFunc(std::vector<Individ> individs)
{
	float values_sum = 0;

	for (auto indiv : individs)
		values_sum += FitFunc(indiv);

	values_sum /= individs.size();

	return values_sum;
}

bool CompareByFitFunc(Individ indiv_1, Individ indiv_2)
{
	return FitFunc(indiv_1) < FitFunc(indiv_2);
}

void PrintPopulation(int population_num, std::vector<Individ> individs)
{
	std::cout << std::fixed << std::setprecision(3);

	std::cout << "Population:" << std::endl;
	std::cout << " _________________________________________________________________________________________________________ " << std::endl;
	std::cout << "|  Population number |  Gen 1 (X)  |  Gen 2 (Y)  |   FIT value  |   Max FIT value  |   Average FIT value  |" << std::endl;
	std::cout << "|--------------------|-------------|-------------|--------------|------------------|----------------------|" << std::endl;
	for (int i = 0; i < 4; i++)
	{
		// Print population number
		if (i == 2)
		{
			std::cout << "|";
			if (population_num / 10 < 1)
				std::cout << "  ";
			else if (population_num / 100 < 1)
				std::cout << " ";
			std::cout << "        " << population_num << "         |    ";

		}
		else
			std::cout << "|                    |    ";

		// Print X and Y
		if (individs[i].x > 0)
			std::cout << " ";

		std::cout << individs[i].x << "   |    ";

		if (individs[i].y > 0)
			std::cout << " ";

		std::cout << individs[i].y << "   |     ";

		// Print Fit values
		std::cout << FitFunc(individs[i]) << "    |       ";

		// Print max and average value
		if (i == 2)
		{
			std::cout << FitFunc(*std::max_element(individs.begin(), individs.end(), CompareByFitFunc)) << "      |         ";
			std::cout << MeanFitFunc(individs) << "        |";
		}
		else
			std::cout << "           |                      |";

		std::cout << std::endl;
	}
	std::cout << "|____________________|_____________|_____________|______________|__________________|______________________|" << std::endl;
}

void PrintRoulette(std::vector<float> fit_values)
{
	std::cout << std::fixed << std::setprecision(1);

	std::cout << "Rullete sectors:" << std::endl;
	std::cout << " ___________________________________________ " << std::endl;
	std::cout << "|          |          |          |          |" << std::endl;
	std::cout << "|  ";
	for (auto value : fit_values)
	{
		if (value * 100 < 10)
			std::cout << " ";

		std::cout << value * 100 << " %  |  ";
	}
	std::cout << std::endl;
	std::cout << "|__________|__________|__________|__________| " << std::endl;
}

void Normalize(std::vector<float>& values)
{
	float summ = 0;

	for (int i = 0; i < values.size(); i++)
		summ += values[i];

	for (int i = 0; i < values.size(); i++)
		values[i] /= summ;
}

void Mutation(Individ& individ)
{
	int do_mutation = (int)rand() % 4;
	bool mutated_gen_num = 0;
	float mutation_value = 0;

	if (do_mutation == 3) // 25%
	{
		mutated_gen_num = rand() % 2;
		mutation_value = ((float)rand() / RAND_MAX)*0.6 - 0.3;

		if (mutated_gen_num == 0)
			individ.x += mutation_value;
		else
			individ.y += mutation_value;
	}
}

void Crossover(std::vector<Individ>& individs, std::vector<int> selected)
{
	std::vector<Individ> new_population;
	Individ child1, child2;

	for (auto number : selected)
		new_population.push_back(individs[number]);

	child1.x = individs[selected[0]].x;
	child1.y = individs[selected[1]].y;
	Mutation(child1);

	child2.x = individs[selected[1]].x;
	child2.y = individs[selected[0]].y;
	Mutation(child2);

	new_population.push_back(child1);
	new_population.push_back(child2);

	individs = new_population;
}

void SelectionAndCrossover(std::vector<Individ>& individs, int population_num)
{
	std::vector<float> fit_values;
	std::vector<int> selected;

	// Check values
	for (auto indiv : individs)
		fit_values.push_back(FitFunc(indiv));

	// Normalize
	Normalize(fit_values);

	// Make sectors
	std::vector<float> sectors;
	float prev = 0;

	sectors.push_back(0);
	for (auto fit_value : fit_values)
	{
		sectors.push_back(prev + fit_value);
		prev += fit_value;
	}

	// Print each 10 info
	if (population_num % 10 == 0)
	{
		// Print population info
		std::cout << std::endl;
		std::cout << "POPULATION " << population_num << std::endl;
		std::cout << "__________________________________________" << std::endl << std::endl;
		PrintPopulation(population_num, individs);
		std::cout << std::endl;

		// Print sectors
		PrintRoulette(fit_values);
		std::cout << std::endl;

		std::cout << std::endl << std::endl;
	}


	// Make rolls
	float roll_1 = (float)rand() / RAND_MAX;
	float roll_2 = (float)rand() / RAND_MAX;

	for (int i = 0; i < 4; i++)
	{
		if (sectors[i] < roll_1 && roll_1 < sectors[i + 1])
			selected.push_back(i);

		if (sectors[i] < roll_2 && roll_2 < sectors[i + 1])
			selected.push_back(i);
	}

	// Crossover
	Crossover(individs, selected);
}

int main()
{
	std::vector<Individ> population;
	const int N = 99;  // number of populations

	for (int i = 0; i < 4; i++)
	{
		Individ* curr_indiv = new Individ();
		curr_indiv->x = ((float)rand() * 4 / RAND_MAX) - 2;
		curr_indiv->y = ((float)rand() * 4 / RAND_MAX) - 2;
		population.push_back(*curr_indiv);
	}

	for (int i = 0; i < N; i++)
	{
		SelectionAndCrossover(population, i);
	}

	// Last population
	std::cout << "LAST POPULATION" << std::endl;
	std::cout << "__________________________________________" << std::endl << std::endl;
	PrintPopulation(N, population);
	std::cout << std::endl;

	std::cout << std::fixed << std::setprecision(3);
	std::cout << "Result: " << FitFunc(*std::max_element(population.begin(), population.end(), CompareByFitFunc)) << std::endl;
}