#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>

using namespace std;

// Functia de maximizat
double f(double x) {
    return x * sin(x);
}

// Parametrii algoritmului genetic
const int POPULATION_SIZE = 50;
const int GENERATIONS = 100;
const double CROSSOVER_RATE = 0.7;
const double MUTATION_RATE = 0.01;
const double INTERVAL_START = 0.0;
const double INTERVAL_END = 10.0;

// Generarea unui individ aleator in intervalul [INTERVAL_START, INTERVAL_END]
double randomIndividual() {
    return INTERVAL_START + static_cast<double>(rand()) / RAND_MAX * (INTERVAL_END - INTERVAL_START);
}

// Evaluarea fitness-ului unui individ
double fitness(double individual) {
    return f(individual);
}

// Selectia prin turneu
double tournamentSelection(const vector<double>& population) {
    int k = 3; // dimensiunea turneului
    double best = population[rand() % POPULATION_SIZE];
    for (int i = 1; i < k; ++i) {
        double competitor = population[rand() % POPULATION_SIZE];
        if (fitness(competitor) > fitness(best)) {
            best = competitor;
        }
    }
    return best;
}

// Crossover cu un singur punct
pair<double, double> crossover(double parent1, double parent2) {
    if (static_cast<double>(rand()) / RAND_MAX < CROSSOVER_RATE) {
        double alpha = static_cast<double>(rand()) / RAND_MAX;
        double child1 = alpha * parent1 + (1 - alpha) * parent2;
        double child2 = alpha * parent2 + (1 - alpha) * parent1;
        return {child1, child2};
    } else {
        return {parent1, parent2};
    }
}

// Mutatie
double mutate(double individual) {
    if (static_cast<double>(rand()) / RAND_MAX < MUTATION_RATE) {
        double mutationAmount = (static_cast<double>(rand()) / RAND_MAX - 0.5) * 0.1; // modificare mica
        individual += mutationAmount;
        // Asiguram ca individul ramane in interval
        if (individual < INTERVAL_START) individual = INTERVAL_START;
        if (individual > INTERVAL_END) individual = INTERVAL_END;
    }
    return individual;
}

int main() {
    /*
    Explicatia codului:

    1. Initializarea populatiei: Generam o populatie initiala de indivizi aleatori in intervalul [0,10][0,10].
    2. Evaluarea fitness-ului: Calculam fitness-ul fiecarui individ folosind functia f(x)=x⋅sin⁡(x)f(x)=x⋅sin(x).
    3. Selectia: Selectam parintii folosind selectia prin turneu.
    4. Incrucisarea (Crossover): Combinam parintii pentru a genera copii folosind crossover cu un singur punct.
    5. Mutatia: Aplicam mutatii aleatorii copiilor pentru a mentine diversitatea populatiei.
    6. Inlocuirea: Inlocuim vechea populatie cu noua generatie de copii.
    7. Determinarea celui mai bun individ: Dupa ce toate generatiile au fost evaluate, determinam cel mai bun individ din populatie.
    */

    srand(time(0));

    // Initializarea populatiei
    vector<double> population(POPULATION_SIZE);
    for (double& individual : population) {
        individual = randomIndividual();
    }

    // Evolutia populatiei
    for (int generation = 0; generation < GENERATIONS; ++generation) {
        vector<double> newPopulation;
        while (newPopulation.size() < POPULATION_SIZE) {
            // Selectia parintilor
            double parent1 = tournamentSelection(population);
            double parent2 = tournamentSelection(population);

            // Crossover
            auto [child1, child2] = crossover(parent1, parent2);

            // Mutatie
            child1 = mutate(child1);
            child2 = mutate(child2);

            // Adaugarea copiilor in noua populatie
            newPopulation.push_back(child1);
            newPopulation.push_back(child2);
        }

        // Inlocuirea vechii populatii cu noua generatie
        population = newPopulation;
    }

    // Gasirea celui mai bun individ din populatie
    double bestIndividual = *max_element(population.begin(), population.end(), [](double a, double b) {
        return fitness(a) < fitness(b);
    });

    cout << "Best individual: " << bestIndividual << " with fitness: " << fitness(bestIndividual) << endl;
}
