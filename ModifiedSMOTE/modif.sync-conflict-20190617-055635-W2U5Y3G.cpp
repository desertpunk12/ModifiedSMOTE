#pragma once

#include "modif.hpp"
#include "mmain.hpp"
#include "modifcomputes.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

//TODO: the link error was in function prodModif()

vvd prodModifAllCluster(vvd dataset, int N, int numOfVars, int knn) {
	vvd syntheticDatas;
	vvd cluster8;
	vvd cluster9;
	vvd cluster10;
			

	//Remove class with 0 as value
	dataset = remove0Classes(dataset);

	cout << "Removed classes with 0 as values" << endl;
	printDatas(dataset);
	

	//Separate each cluster and put in to teach vvd
	for (int i = 0; i < dataset.size(); i++) {
		if (dataset[i][7] == 8)
			cluster8.push_back(dataset[i]);
		else if (dataset[i][7] == 9)
			cluster9.push_back(dataset[i]);
		else if (dataset[i][7] == 10)
			cluster10.push_back(dataset[i]);
	}


	//Computes Sparsity Factor of each Cluster
	double sparFC8 = computeClusterSparsityFactor(cluster8, 5);
	double sparFC9 = computeClusterSparsityFactor(cluster9, 5);
	double sparFC10 = computeClusterSparsityFactor(cluster10, 5);


	//Computes Sparsity Sum of all Cluster
	double sparSum = sparFC8 + sparFC9 + sparFC10;

	//Computes Sparsity Weight of each Cluster
	double samplingWeightC8 = sparFC8 / sparSum;
	double samplingWeightC9 = sparFC9 / sparSum;
	double samplingWeightC10 = sparFC10 / sparSum;
	cout << "Sampling Weights    ";
	cout << samplingWeightC8 << " || ";
	cout << samplingWeightC9 << " || ";
	cout << samplingWeightC10;
	space;

	int numSamplesC8 = round(N * samplingWeightC8);
	int numSamplesC9 = round(N * samplingWeightC9);
	int numSamplesC10 = round(N * samplingWeightC10);
	cout << "Number of Samples    ";
	cout << "N: " << N << "  :  ";
	cout << numSamplesC8 << " || ";
	cout << numSamplesC9 << " || ";
	cout << numSamplesC10;
	cout << "    Total: " << numSamplesC8 + numSamplesC9 + numSamplesC10;
	space;
	
	//old
	//vvd synthC8 = prodModif(cluster8, numSamplesC8, numOfVars,1,knn);
	
	//new fix
	vvd synthC8 = prodModif(cluster8, numSamplesC8, numOfVars,1,knn);

	space;
	space;
	//vvd synthC9 = prodModif(cluster9, numSamplesC9, numOfVars);
	space;
	space;
	//vvd synthC10 = prodModif(cluster10, numSamplesC10, numOfVars);

	//vvd combined = combineAllClusters(synthC8, synthC9, synthC10);

	//syntheticDatas = combined;
	syntheticDatas = synthC8;
	return syntheticDatas;
}

vvd prodModif(vvd dataset, int N, int numOfVars, int indexOfExemplar,int knn) {

	//Step 1: compute average of EDs by summing all eds with 
	//			respect to their minority sample and dividing 
	//			it the minority samples

	//Step 2: compute for density factor 
	//			density_factor = minortiy_count/(average_minority^num_of_vars)

	//Step 3: compute for sparsity_factor = 1 / density_factor


	//Gets the minority data
	vvd minority = getMinority(dataset, numOfVars);
	//printDatas(minority);


	//Computes the eucliedan distances
	vvd eds = computeEDs(minority, numOfVars);
	//printDatas(eds);


	vvd paired = pairMinorityAndEDs(minority, eds);
	//printDatas(paired); 


	//Gets the 5 nearest Neighbors / 5 lowest eds
	vvvd bottom5 = near5Neighbors(paired);
	printDatas(bottom5);

	vvd oversampled = computeExemplarNeighbors(paired,indexOfExemplar,knn);


	//vvvd alleds = getMinorityWithEDs(paired);
	//printDatas(bottom5);

	//vd sumOfEDs = sumOfVVVDs(bottom5);
	//vd sumOfEDs = sumOfVVVDs(alleds);
	//cout << "\t\t\t\t\t\t\t";
	//printDatas(sumOfEDs);

	//vd neds = computeNEDs(sumOfEDs);
	//printDatas(neds);

	//vd rneds = computeRNEDs(neds);
	//printDatas(rneds);

	//vd wmatrix = computeWeightMatrix(rneds);
	//printDatas(wmatrix);

	//vd smotegenmatrix = computeSmoteGenerationMatrix(wmatrix, N);
	//printDatas(smotegenmatrix);

	//Rounded the SMOTE Generation Matrix to get the Ns of each
	//minority data in consideration
	//vd Ns = roundDatas(smotegenmatrix);
	//printDatas(Ns);


//	vvd syntheticData = produceSyntheticData(bottom5,Ns);

	//TODO: Get the summations of the 5 nearest neghbors

	//vvd syntheticWithoutV6 = produceSyntheticData(bottom5, N, numOfVars);
	//printDatas(syntheticWithoutV6);

	//vvd synthetic = addV6(syntheticWithoutV6);
	//printDatas(synthetic);
	//return syntheticWithoutV6;
	return dataset;
	//return synthetic;
}

//Returns the minorty from the dataset
vvd getMinority(vvd dataset, int classIndex) {
	vvd minority;
	for (int i = 0; i < dataset.size(); i++) {
		if (dataset[i][classIndex] == 1) {
			minority.push_back(dataset[i]);
		}
	}
	return minority;
}
//Pairs the minority data from their corresponding EDs
vvd pairMinorityAndEDs(vvd minoritydata, vvd eds) {
	for (int i = 0; i < eds.size(); i++) {
		for (int j = 0; j < eds[i].size(); j++) {
			minoritydata[i].push_back(eds[i][j]);
		}
	}
	return minoritydata;
}

//Returns the sets of 5 nearest neghbors
vvvd near5Neighbors(vvd pair) {
	vvvd low5s;
	for (int i = 8; i < 8 + pair.size(); i++) {
		low5s.push_back(lowest5(pair, i));
	}

	return low5s;
}

//Returns the minority datas with EDs
vvvd getMinorityWithEDs(vvd pair) {
	vvvd low5s;
	for (int i = 7; i < 7 + pair.size(); i++) {
		low5s.push_back(allEDs(pair, i));
	}

	return low5s;
}



//Useda a comparator for sorting vd datas
struct VDComparator {
	int indexCriteria;
	VDComparator(int indexCriteria) { this->indexCriteria = indexCriteria; }
	bool operator () (vd a, vd b) {
		return a[indexCriteria] < b[indexCriteria];
	}
};


//Returns 5 lowest eds for a single minority in consideration
vvd lowest5(vvd data, int index_criteria) {
	vvd low5 = data;
	sort(low5.begin(), low5.end(), VDComparator(index_criteria));
	int resizeTo = min(6, (int)low5.size());
	low5.resize(resizeTo);
	for (int i = 0; i < low5.size(); i++) {
		for (int j = 8; j < low5[i].size(); j++) {
			if (j == index_criteria)
				continue;
			low5[i][j] = 0;

		}
	}

	return low5;
}


//Returns 
vvd allEDs(vvd data, int index_criteria) {
	vvd low5 = data;
	sort(low5.begin(), low5.end(), VDComparator(index_criteria));
	for (int i = 0; i < low5.size(); i++) {
		for (int j = 7; j < low5[i].size(); j++) {
			if (j == index_criteria)
				continue;
			low5[i][j] = 0;

		}
	}

	return low5;
}

vd roundDatas(vd datas) {
	vd rounded;
	for (int i = 0; i < datas.size(); i++) {
		double xx = round(datas[i]);
		rounded.push_back(xx);
	}
	return rounded;
}

//Produces Synthetic Data using Weighted SMOTE
vvd produceSyntheticData(vvvd data, int N, int numOfVars) {
	vvd syntheticDatas;
	for (int i = 0; i < N; i++) {
		int r1t5 = min((int)data[i].size()-1,(int)r1to5());
		vd sd;
		for (int j = 0; j < numOfVars; j++) {
			double ddd = data[i][0][j] + (r0to1()*(data[i][r1t5][j] - data[i][0][j]));
			sd.push_back(ddd);
		}
		syntheticDatas.push_back(sd);
	}
	return syntheticDatas;
}


//Combines all vvd clusters into one vvd adds new attribute called cluster
vvd combineAllClusters(vvd c8, vvd c9, vvd c10) {
	vvd all;
	for (int i = 0; i < c8.size(); i++) {
		all.push_back(c8[i]);
	}

	for (int i = 0; i < c9.size(); i++) {
		all.push_back(c9[i]);
	}


	for (int i = 0; i < c10.size(); i++) {
		all.push_back(c10[i]);
	}

	return all;
}


//Removes rows with 0 class values
vvd remove0Classes(vvd dataset) {
	for(int i = 0; i < dataset.size(); i++){
		if (dataset[i][5] == 0) {
			dataset.erase(dataset.begin() + i);
			i--;
		}
	}
	return dataset;
}
