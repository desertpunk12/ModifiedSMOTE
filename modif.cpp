#pragma once

#include "modif.hpp"
#include "mmain.hpp"
#include "modifcomputes.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

//TODO: the link error was in function prodModif()


//Useda a comparator for sorting vd datas
struct VDComparator {
	int indexCriteria;
	VDComparator(int indexCriteria) { this->indexCriteria = indexCriteria; }
	bool operator () (vd a, vd b) {
		return a[indexCriteria] < b[indexCriteria];
	}
};


vvd prodModifAllCluster(vvd dataset, int N, int numOfVars, int knn) {
	#pragma region Declaration of vvds
	vvd syntheticDatas;
	vvd cluster5;
	vvd cluster6;
	vvd cluster7;
	vvd cluster8;
	#pragma endregion
			

	#pragma region Remove class with 0 as value
	dataset = remove0Classes(dataset);

	/*
	cout << "Removed classes with 0 as values" << endl;
	cout << "---------------------------------------------------------" << endl;
	printDatas(dataset);
	*/
	#pragma endregion

	#pragma region Separate each cluster and put in to teach vvd
	int indexOfCluster = 8;
	for (int i = 0; i < dataset.size(); i++) {
		if (dataset[i][indexOfCluster] == 5)
			cluster5.push_back(dataset[i]);
		else if (dataset[i][indexOfCluster] == 6)
			cluster6.push_back(dataset[i]);
		else if (dataset[i][indexOfCluster] == 7)
			cluster7.push_back(dataset[i]);
		else if (dataset[i][indexOfCluster] == 8)
			cluster8.push_back(dataset[i]);
	}

	/*
	cout << "Separated each cluster" << endl;
	cout << "---------------------------------------------------------" << endl;
	cout << "Cluster 8" << endl;
	printDatas(cluster8);
	cout << "Cluster 9" << endl;
	printDatas(cluster9);
	cout << "Cluster 10" << endl;
	printDatas(cluster10);
	*/
	#pragma endregion 

	#pragma region Computes Sparsity Factor of each Cluster
	double sparFC5 = computeClusterSparsityFactor(cluster5, numOfVars);
	double sparFC6 = computeClusterSparsityFactor(cluster6, numOfVars);
	double sparFC7 = computeClusterSparsityFactor(cluster7, numOfVars);
	double sparFC8 = computeClusterSparsityFactor(cluster8, numOfVars);
	/*
	cout << "Sparsity Factors:" << endl;
	cout << "C8: " << sparFC8 << " | C9: " << sparFC9 
		<< " | C10: " << sparFC10 << endl;
	*/
	#pragma endregion

	#pragma region Computes Sparsity Sum of all Cluster
	double sparSum = sparFC5 + sparFC6 + sparFC7 + sparFC8;
	//cout << "Sparsity Sum : " << sparSum << endl;
	
	#pragma endregion

	#pragma region Computes Sparsity Weight of each Cluster
	double samplingWeightC5 = sparFC5 / sparSum;
	double samplingWeightC6 = sparFC6 / sparSum;
	double samplingWeightC7 = sparFC7 / sparSum;
	double samplingWeightC8 = sparFC8 / sparSum;
	/*
	cout << "Sampling Weights    ";
	cout << samplingWeightC8 << " || ";
	cout << samplingWeightC9 << " || ";
	cout << samplingWeightC10;
	*/
	#pragma endregion
	
	space;

#pragma region Computing the number of samples for each cluster
	int numSamplesC5 = round(N * samplingWeightC5);
	int numSamplesC6 = round(N * samplingWeightC6);
	int numSamplesC7 = round(N * samplingWeightC7);
	int numSamplesC8 = round(N * samplingWeightC8);
	cout << "Number of Samples    ";
	cout << "N: " << N << "  :  ";
	cout << "Cluster5:" << numSamplesC5 << " || ";
	cout << "Cluster6:" << numSamplesC6 << " || ";
	cout << "Cluster7:" << numSamplesC7 << " || ";
	cout << "Cluster8:" << numSamplesC8;
	cout << "    Total: " << numSamplesC5 + numSamplesC6 + numSamplesC7 + numSamplesC8;
#pragma endregion

	space;
		
	//Using cluster 8 for now for testign
	vvd synthC5 = prodModif(cluster5, numSamplesC5, numOfVars,2,knn);
	cout << "Finally " << synthC5.size() << endl;
	// printDatas(synthC5);

	//All clusters
	space;
	space;

	
	vvd synthC6 = prodModif(cluster6, numSamplesC6, numOfVars,2,knn);
	cout << "Finally " << synthC6.size() << endl;
	// printDatas(synthC6);

	//All clusters
	space;
	space;

	
	vvd synthC7 = prodModif(cluster7, numSamplesC7, numOfVars,2,knn);
	cout << "Finally " << synthC7.size() << endl;
	// printDatas(synthC7);

	//All clusters
	space;
	space;

	
	vvd synthC8 = prodModif(cluster8, numSamplesC8, numOfVars,2,knn);
	cout << "Finally " << synthC8.size() << endl;
	// printDatas(synthC8);

	//All clusters
	space;
	space;


	vvd combined = combineAllClusters(synthC5, synthC6, synthC7, synthC8);

	syntheticDatas = combined;


	//syntheticDatas = synthC8;
	
	return syntheticDatas;
}

vvd prodModif(vvd clusterDataset, int N, int numOfVars, int indexOfExemplar,int knn) {
	/*
		1. Rank nearest neighbors of exemplar
		2. Get the 4 nearest neighbors
		3. From the exemplar move to the nearest neighbor to be the basis for
			creating the new sythetic data
		4. Move to the next in the rank
		
	*/
	vvd producedSyntheticDatas;

	// cout << "cluster dataset" << endl;
	//printDatas(clusterDataset);

	//Computes the eucliedan distances
	vvd eds = computeEDs(clusterDataset, numOfVars);
	//printDatas(eds);

	//Paired the eculidean distaces with the actual data so that its easier to see and compare
	vvd paired = pairMinorityAndEDs(clusterDataset, eds);
	//printDatas(paired); 


	//Ranks all the EDs from the exemplar
	//vvd rank = rankFromExemplar(paired, indexOfExemplar);
	paired = addIndexValueToVVD(paired);
	//printDatas(paired);

	vd rank = getIndexOfRanksFromExemplar(paired, indexOfExemplar);
	//cout << "EDs ranked from exemplar" << endl;
	//printDatas(rank);
	


	//Loop Untill it reaches the number of produced syntethic data 
	//	needed for the cluster
	for (int i = 0; i < N; i++) {
		int indexOfTheCurrentExemplar = rank[i%rank.size()];
		//cout << "Index of Current Exempalar: " << indexOfTheCurrentExemplar << endl;
		vvd n4 = get4Neighbor(paired, indexOfTheCurrentExemplar);
		//printDatas(n4);
		vd randOversample = n4[r0to3()];
		vd exemplar = paired[indexOfTheCurrentExemplar];
		vd synthetic;
		for (int i = 0; i < numOfVars; i++) {
			double value = exemplar[i] + (r0to1()*(randOversample[i] - exemplar[i]));
			synthetic.push_back(value);
		}
		// 1. Find the 4 nearest neightbor of the exemplar
		// 2. Select 1 random neighbor to be the basis for creating the new synthetic data
		// 2.1. In creating synthetic data do
		// 2.2	exemplarVariable+(r0to1()*(oversampleVar-exemplarVar))
		// 3. change the exemplar to the next in rank


		producedSyntheticDatas.push_back(synthetic);
	}

	return producedSyntheticDatas;
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

//Returns the sets of 5 nearest neighbors of exemplar
vvd rank5NearFromEx(vvd clusterData, int indexOfExemplar) {
	vvd low5 = clusterData;
	sort(low5.begin(), low5.end(), VDComparator(indexOfExemplar));
	int resizeTo = min(6, (int)low5.size());
	low5.resize(resizeTo);
	for (int i = 0; i < low5.size(); i++) {
		for (int j = 8; j < low5[i].size(); j++) {
			if (j == indexOfExemplar)
				continue;
			low5[i][j] = 0;

		}
	}

	return low5;
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
vvd producedSyntheticData(vvvd data, int N, int numOfVars) {
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
vvd combineAllClusters(vvd c5, vvd c6, vvd c7, vvd c8) {
	vvd all;
	for (int i = 0; i < c5.size(); i++) {
		all.push_back(c5[i]);
	}

	for (int i = 0; i < c6.size(); i++) {
		all.push_back(c6[i]);
	}
	
	for (int i = 0; i < c7.size(); i++) {
    		all.push_back(c7[i]);
	}

	for (int i = 0; i < c8.size(); i++) {
    		all.push_back(c8[i]);
	}

	return all;
}


//Removes rows with 0 class values
vvd remove0Classes(vvd dataset) {
	for(int i = 0; i < dataset.size(); i++){
		int indexOfClass = 5;
		if (dataset[i][indexOfClass] == 0) {
			dataset.erase(dataset.begin() + i);
			i--;
		}
	}
	return dataset;
}

vvd rankFromExemplar(vvd pairedData, int indexOfExemplar) {
	vvd ranked = pairedData;
	int indexEDStarts = 8;
	indexOfExemplar += indexEDStarts; //because index 8 and below are the actual data an not the eds
	sort(ranked.begin(), ranked.end(), VDComparator(indexOfExemplar));
	ranked = removeExessDataFromPaired(ranked, indexOfExemplar);

	return ranked;
}

vvd removeExessDataFromPaired(vvd paired, int indexOfImportantData) {
	int indexEDStarts = 8;
	for (int i = 0; i < paired.size(); i++) {
		if (indexOfImportantData == indexEDStarts) {
			paired[i].erase(paired[i].begin() + 1 + 8,paired[i].end());
		}
		else {
			//left of exemplar deletion
			paired[i].erase(paired[i].begin()+indexEDStarts, paired[i].begin() + indexOfImportantData  );

			//right of exemplar deletion
			paired[i].erase(paired[i].begin()+ indexEDStarts + 1, paired[i].end()-1);
		}
	}

	return paired;
}

vvd get4Neighbor(vvd paired, int indexOfExemplar) {
	vvd n4 = paired;
	int indexEDStarts = 8;
	sort(n4.begin(), n4.end(), VDComparator(indexOfExemplar+indexEDStarts));
	n4.erase(n4.begin());
	return n4;
}



vd getIndexOfRanksFromExemplar(vvd paired, int indexOfExemplar) {

	cout << "Start Paired Datas:" << endl;
	printDatas(paired);
	cout << "END Paired Datas:" << endl;
	vd ranks;

	int indexEDStarts = 8;
	indexOfExemplar += indexEDStarts; //because index 8 and below are the actual data an not the eds
	sort(paired.begin(), paired.end(), VDComparator(indexOfExemplar));
	
	for (int i = 0; i < paired.size(); i++) {
		ranks.push_back(paired[i][paired[i].size() - 1]);
	}

	return ranks;
}


vvd addIndexValueToVVD(vvd paired) {
	for (int i = 0; i < paired.size(); i++) {
		paired[i].push_back(i);
	}

	return paired;
}