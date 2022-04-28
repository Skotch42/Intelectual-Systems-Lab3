#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <time.h>
#include <math.h>
using namespace std;

#define pop_size		2000		    // размер популяции
#define max_iteration	16000	    // максимальное число итераций
#define elitrate		0.10		// элитарность
#define mutation_rate	0.25		// мутации
#define mutation		RAND_MAX * mutation_rate
#define end_target		string("МИР")

// особь
struct individual
{
	string str;						// строка
	unsigned int fitness;		    // пригодность
};

typedef vector<individual> ind;

// задание изначальной популяции
void init_population(ind& population, ind& offspring)
{
	int tsize = end_target.size();

	for (int i = 0; i < pop_size; i++)
	{
		individual citizen;

		citizen.fitness = 0;
		citizen.str.erase();

		for (int j = 0; j < tsize; j++)
		{
			citizen.str += (rand() % 90) + 34;
		}

		population.push_back(citizen);
	}

	offspring.resize(pop_size);
}

// расчёт проспособленности
void calc_fitness(ind& population)
{
	string target = end_target;
	int tsize = target.size();
	unsigned int fitness;

	for (int i = 0; i < pop_size; i++)
	{
		fitness = 0;

		for (int j = 0; j < tsize; j++)
		{
			fitness += abs(int(population[i].str[j] - target[j]));
		}

		population[i].fitness = fitness;
	}
}

// сравнение приспособленности
bool fitness_sort(individual x, individual y)
{
	return (x.fitness < y.fitness);
}

// сортировка особей по их приспособленности (от наиболее к наименее)
inline void sort_by_fitness(ind& population)
{
	sort(population.begin(), population.end(), fitness_sort);
}

// передача элитарных особей в следующее поколение
void elitism(ind& population, ind& offspring, int esize)
{
	for (int i = 0; i < esize; i++)
	{
		offspring[i].str = population[i].str;
		offspring[i].fitness = population[i].fitness;
	}
}

// мутация
void mutate(individual& member)
{
	int tsize = end_target.size();
	int ipos = rand() % tsize;
	int delta = (rand() % 90) + 34;

	member.str[ipos] = ((member.str[ipos] + delta) % 122);
}

// скрещивание особей
void cross(ind& population, ind& offspring)
{
	int esize = pop_size * elitrate;
	int tsize = end_target.size(), spos, i1, i2;

	elitism(population, offspring, esize);

	for (int i = esize; i < pop_size; i++)
	{
		i1 = rand() % (pop_size / 2);
		i2 = rand() % (pop_size / 2);
		spos = rand() % tsize;

		offspring[i].str = population[i1].str.substr(0, spos) + population[i2].str.substr(spos, esize - spos);

		if (rand() < mutation) mutate(offspring[i]);
	}
}

// вывод наиболее приспособленной особи текущего поколения
inline void print_best(ind& gav)
{
	if (gav[0].fitness != 0)
	{
		cout << gav[0].str << " (" << gav[0].fitness << ")" << endl;
	}
	else
	{
		cout << gav[0].str << endl;
	}
}

inline void swap(ind*& population, ind*& offspring)
{
	ind* temp = population;
	population = offspring;
	offspring = temp;
}

int main()
{
	srand(unsigned(time(NULL)));

	ind pop_alpha, pop_beta;
	ind* population, * offspring;

	init_population(pop_alpha, pop_beta);
	population = &pop_alpha;
	offspring = &pop_beta;

	for (int i = 0; i < max_iteration; i++)
	{
		calc_fitness(*population);
		sort_by_fitness(*population);
		cout << i + 1 << " generation: ";
		print_best(*population);

		if ((*population)[0].fitness == 0) break;

		cross(*population, *offspring);
		swap(population, offspring);
	}

	return 0;
}

// В программе перебирается каждый член популяции и сравнивается с таковым в целевой строке. 
// Разницы между символами складываются и накопленная сумма используется как значение пригодности (соответственно чем меньше, тем лучше).
// При выполнении, программа выдает на экран лучшую популяцию и ее пригодность(число в скобках).