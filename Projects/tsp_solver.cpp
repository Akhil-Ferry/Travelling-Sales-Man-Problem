#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <ctime>
#include <cstdlib>

using namespace std;

struct City {
    int id;
    double x, y;
};

class TSPSolver {
private:
    vector<City> cities;

    double euclideanDistance(const City &a, const City &b) {
        return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
    }

    double calculateTourCost(const vector<int> &tour) {
        double cost = 0;
        int n = tour.size();
        for (int i = 0; i < n; i++) {
            cost += euclideanDistance(cities[tour[i]], cities[tour[(i + 1) % n]]);
        }
        return cost;
    }

    vector<int> generateInitialTour() {
        int n = cities.size();
        vector<int> tour(n);
        for (int i = 0; i < n; ++i) {
            tour[i] = i;
        }
        random_shuffle(tour.begin(), tour.end());
        return tour;
    }

    vector<int> getNeighbor(const vector<int> &tour) {
        vector<int> newTour = tour;
        int n = tour.size();

        // 2-opt move
        int i = rand() % n;
        int j = rand() % n;

        while (i == j) {
            j = rand() % n;
        }

        if (i > j) swap(i, j);
        reverse(newTour.begin() + i, newTour.begin() + j + 1);

        return newTour;
    }

public:
    TSPSolver() {
        srand(time(0));
    }

    void loadCities(const string &filename) {
        ifstream file(filename);
        string line;
        int id;
        double x, y;

        while (getline(file, line)) {
            if (isdigit(line[0])) {
                stringstream ss(line);
                ss >> id >> x >> y;
                cities.push_back({id - 1, x, y});
            }
        }
    }

    vector<int> solve() {
        const int MAX_ITERATIONS = 100000;
        const double INITIAL_TEMP = 10000.0;
        const double COOLING_RATE = 0.9995;

        vector<int> currentTour = generateInitialTour();
        vector<int> bestTour = currentTour;
        double currentCost = calculateTourCost(currentTour);
        double bestCost = currentCost;
        double temperature = INITIAL_TEMP;

        for (int iter = 0; iter < MAX_ITERATIONS; iter++) {
            vector<int> newTour = getNeighbor(currentTour);
            double newCost = calculateTourCost(newTour);

            double delta = newCost - currentCost;
            if (delta < 0 || exp(-delta / temperature) > ((double) rand() / RAND_MAX)) {
                currentTour = newTour;
                currentCost = newCost;

                if (currentCost < bestCost) {
                    bestTour = currentTour;
                    bestCost = currentCost;
                }
            }

            temperature *= COOLING_RATE;
        }

        cout << "Best tour cost: " << bestCost << endl;
        return bestTour;
    }
};

int main() {
    TSPSolver solver;

    cout << "Enter the TSP filename: ";
    string filepath;
    cin >> filepath;

    solver.loadCities(filepath);
    vector<int> optimalTour = solver.solve();

    cout << "Optimal tour sequence: ";
    for (int city : optimalTour) {
        cout << (city + 1) << " ";
    }
    cout << endl;

    return 0;
}
