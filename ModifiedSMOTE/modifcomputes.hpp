#pragma once
#include "modif.hpp"
#include "mmain.hpp"

//Used a comparator for sorting vd datas
struct VDComparator {
	int indexCriteria;
	VDComparator(int indexCriteria) { this->indexCriteria = indexCriteria; }
	bool operator () (vd a, vd b) {
		return a[indexCriteria] < b[indexCriteria];
	}
};



//Returns the summation of 5 nearest neighbors of each variable
vd sumOfVVVDs(vvvd);


vd computeNEDs(vd);
vd computeRNEDs(vd);
vd computeWeightMatrix(vd);
vd computeSmoteGenerationMatrix(vd, int);


//Computes a row of Euclidean Distances
vvd computeEDs(vvd, int, int);

//Computes a single Euclidean Distance
double computeED(vd, vd, int, int);

//Computes the sparsity factor of given cluster
double computeClusterSparsityFactor(vvd, int, int);

//Computer the average Eculidean Distance
double computeAverageED(vvd);


//Computes the what??
vvd computeExemplarNeighbors(vvd, int, int);



double computeDensityFactor(double, double, double);
double computeSparsityFactor(double);
double computeSparsitySum(vd);

vd computeSamplingWeights(vd, double);


//Adds v6 with 1 as their value
vvd addV6(vvd);

//Generates random number from 0 to 1
double r0to1();

//Generates random number from 1 to 5
double r1to5();

//Generates random number from 1 to 5
double r1ton(int);

//Generates random number from 1 to 4
double r1to4();


//Generates random number from 1 to 4
double r0to3();
