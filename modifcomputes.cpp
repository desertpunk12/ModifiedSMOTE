#pragma once
#include "modifcomputes.hpp"
#include <algorithm>



vd sumOfVVVDs(vvvd eds) {
	vd summation;
	for (int i = 0; i < eds.size(); i++) {
		double sum = 0;
		for (int j = 0; j < eds[i].size(); j++) {
			sum += eds[i][j][7 + i];
		}
		summation.push_back(sum);
	}

	return summation;
}

vd computeNEDs(vd data) {
	double min = *min_element(data.begin(), data.end());
	double max = *max_element(data.begin(), data.end());
	vd computedDatas;
	double sum = 0;
	for (int i = 0; i < data.size(); i++) {
		double xx = (data[i] - min) / (max - min);
		sum += xx;
		computedDatas.push_back(xx);
	}
	computedDatas.push_back(sum);
	return computedDatas;
}

vd computeRNEDs(vd data) {
	vd computedDatas;
	int indexOfSum = data.size() - 1;
	double sum = 0;
	for (int i = 0; i < data.size() - 1; i++) {
		double xx = data[indexOfSum] - data[i];
		sum += xx;
		computedDatas.push_back(xx);
	}
	computedDatas.push_back(sum);

	return computedDatas;
}

vd computeWeightMatrix(vd data) {
	vd computedDatas;
	int indexOfSum = data.size() - 1;

	for (int i = 0; i < data.size() - 1; i++) {
		double xx = data[i] / data[indexOfSum];
		computedDatas.push_back(xx);
	}

	return computedDatas;
}

vd computeSmoteGenerationMatrix(vd wmatrix, int N) {
	vd computedDatas;
	int T = wmatrix.size();
	for (int i = 0; i < T; i++) {
		double xx = wmatrix[i] * ((N*T) / 100);
		computedDatas.push_back(xx);
	}

	return computedDatas;
}




vvd computeEDs(vvd minortyData, int numOfVars) {
	vvd distances;
	for (int i = 0; i < minortyData.size(); i++) {
		vd dd;
		for (int j = 0; j < minortyData.size(); j++) {
			//Condition to skip checking the distance to itself
			if (i == j) {
				dd.push_back(0);
				continue;
			}
			dd.push_back(computeED(minortyData[i], minortyData[j], numOfVars));
		}
		distances.push_back(dd);
	}
	return distances;
}

double computeED(vd a, vd b, int numOfVars) {
	double res;
	double sum = 0;
	for (int i = 0; i < numOfVars; i++) {
		sum += pow(a[i] - b[i], 2);
	}
	res = sqrt(sum);
	return res;
}


double computeClusterSparsityFactor(vvd data, int numOfVars) {


	//Computes the eucliedan distances
	vvd eds = computeEDs(data, numOfVars);
	//printDatas(eds);

	//Computes the average eculidean distance of all eds
	double avgED = computeAverageED(eds);
	//cout << "Average ED:" << avgED << endl;

	//Computes the density factor
	double densityFactor = computeDensityFactor(avgED, eds.size(), numOfVars);

	double sparsityFactor = computeSparsityFactor(densityFactor);

	return sparsityFactor;
}

double computeAverageED(vvd EDs) {
	vd avgForEachVar;
	// Compute average of each variables
	for (int i = 0; i < EDs.size(); i++) {
		double x = 0;
		for(int j = 0; j < EDs[i].size(); j++) {
			x += EDs[i][j];
		}
		double y = x / (EDs[i].size()-1);
		avgForEachVar.push_back(y);
	}
	
	//cout << "Average for each var " << endl;
	//printDatas(avgForEachVar);

	double ss = 0;
	// Compute average of all variable
	for(int i = 0; i < avgForEachVar.size(); i++) {
		ss += avgForEachVar[i];
	}
	return ss/avgForEachVar.size();
}





double computeDensityFactor(double avgMinorityDistance, double minorityCount, double numOfVars) {
	return minorityCount / pow(avgMinorityDistance, numOfVars);
}

double computeSparsityFactor(double densityFactor) {
	return 1 / densityFactor;
}

double computeSparsitySum(vd sparsityFactors) {
	double sum = 0;
	for (int i = 0; i < sparsityFactors.size(); i++) {
		sum += sparsityFactors[i];
	}
	return sum;
}


vd computeSamplingWeights(vd sparsityFactors, double sparsitySum) {
	vd samplingWeights;
	for (int i = 0; i < sparsityFactors.size(); i++) {
		double weight = sparsityFactors[i] / sparsitySum;
		samplingWeights.push_back(weight);
	}

	return samplingWeights;
}

vvd computeExemplarNeighbors(vvd data, int indexOfExemplar, int knn) {
	vvd oversampled;


	return oversampled;
}



vvd addV6(vvd data) {
	for (int i = 0; i < data.size(); i++) {
		data[i].push_back(1);
	}
	return data;
}


double r0to1() {
	return ((double)rand() / RAND_MAX);
	//return 1;
}


double r1to5() {
	return (rand() % 5) + 1;
	//return 1;
}


double r1ton(int n) {
	return (rand() % n) + 1;
	//return 1;
}


double r1to4() {
	return (rand() % 4) + 1;
	//return 1;
}


double r0to3() {
	return (rand() % 4);
}