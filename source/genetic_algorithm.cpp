// genetic_algorithm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

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
	std::cout << " _________________________________________________________________________________________________________ \n";
	std::cout << "|  Population number |  Gen 1 (X)  |  Gen 2 (Y)  |   FIT value  |   Max FIT value  |   Average FIT value  |\n";
	std::cout << "|--------------------|-------------|-------------|--------------|------------------|----------------------|\n";
	for (int i = 0; i < 4; i++)
	{
		// Print population number
		if (i == 2)
		{
			std::cout << "|";
			if (population_num / 10 < 1)
				std::cout << " ";
			std::cout << "         " << population_num << "         |    ";
			
		}
		else
			std::cout << "|                    |    ";

		// Print X and Y
		if (individs[i].x > 0)
			std::cout << " ";

		std::cout << individs[i].x << "   |    ";

		if (individs[i].y > 0)
			std::cout << " "; 

		std::cout<< individs[i].y << "   |     ";

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
	std::cout << "|____________________|_____________|_____________|______________|__________________|______________________|\n";
}

void PrintRoulette(std::vector<float> sectors)
{
	std::cout << std::fixed << std::setprecision(2);

	std::cout << "Rullete sectors:" << std::endl;
	std::cout << " ___________________________________ " << std::endl;
	std::cout << "|        |        |        |        |" << std::endl;
	std::cout << "|  ";
	for (int i = 0; i < 5; i++)
		std::cout << sectors[i] << "  |  ";
	std::cout << std::endl;
	std::cout << "|________|________|________|________| " << std::endl;
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
	int do_mutation = (int)rand()%4;
	bool mutated_gen_num = 0;
	float mutation_value = 0;

	if (do_mutation == 3) // 25%
	{
		mutated_gen_num = rand() % 2;
		mutation_value = ((float)rand() / RAND_MAX)*0.2 - 0.1;
		
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

	for (int i = 0; i < 4; i++)
		if (std::find(selected.begin(), selected.end(), i) == selected.end()) // Not found
			new_population.push_back(individs[i]);

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
	for (int i = 0; i < individs.size(); i++)
		fit_values.push_back(FitFunc(individs[i]));

	// Normalize
	Normalize(fit_values);
	
	// Make sectors
	std::vector<float> sectors;
	float prev = 0;

	sectors.push_back(0);
	for (int i = 0; i < 4; i++)
	{
		std::cout << "Prev: " << prev << "; FIT value: " << fit_values[i] << std::endl;
		sectors.push_back(prev + fit_values[i]);
		prev += fit_values[i];
	}

	//Print population info
	std::cout << "POPULATION " << population_num << std::endl;
	std::cout << "__________________________________________" << std::endl;
	PrintPopulation(population_num, individs);
	std::cout << std::endl;
	// Print sectors
	PrintRoulette(sectors);
	std::cout << std::endl;

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
	std::cout << std::endl << std::endl << std::endl << std::endl;

}


int main()
{
	std::vector<Individ> population;
	const int N = 50;  // number of populations
	
	//srand(time(NULL));
	
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
	std::cout << "Best:" << FitFunc(population[0]) << std::endl;
}
