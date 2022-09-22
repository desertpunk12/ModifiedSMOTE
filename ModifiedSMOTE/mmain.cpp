#pragma once

#include "mmain.hpp"
#include <fstream>
#include <iostream>
#include "modif.hpp"
#include <cstring>
#include <cstdlib>
#include <map>
#include <iterator>

using namespace std;


int main(int argc, char **argv) {
	
	// int N = 146;
	int N = 114;

	//Initialize random seed
	srand(5);
		
	//Location of the dataset
	string fileData;
	if (argc > 1 && string(argv[1]) == string("d")) {
		fileData = "clustered_sampledata.csv";
		if (argc > 2)
			N = atoi(argv[2]);
	}else{
		//fileData = "new_datasets/clustered_sampledata.csv";
		// fileData = "new_datasets/clustered_sampledata.csv";
		fileData = "new_datasets/clustered_sampledata.csv";
		printf("using new datasetttt!!!");
	}
		
	//Reads the file of dataset and stores it in vector
	vvs datasetRaw = getData(fileData);
	//printDatas(datasetRaw);

	//Convert raw string data to double skip the first index becuase its the label
	vvd dataset = vvsTovvd(datasetRaw,1);
	//printDatas(dataset);
	
	//Get the number of vars
	int numOfVars = dataset[0][0];
	// cout << "Num of Vars:" << numOfVars << endl;
	int startIndexOfDatas = 2;
	
	int indexOfClass = 7;

	int indexOfCluster = 8;

	//The KNN
	int knn = 4;

	vvd datasetSynthetic = prodModifAllCluster(dataset,N,numOfVars, startIndexOfDatas, indexOfClass, indexOfCluster,knn);
	//cout << "DOOOOONNNNEEEE size :" << datasetSynthetic.size() << endl;
	//printDatas(datasetSynthetic);
	space;
	space;
	string fname = "Synthetic_Data_of_Modif.csv";
	csvExporter(fname, datasetSynthetic);

	getchar();
	return 0;
}

//Converts (vvs)vector<vector<string>> to (vvd)vector<vector<double>>
//Parameters: from=vvs dataset, start= index from data to start
vvd vvsTovvd(vvs from, int start) {
	vvd to;
	for (int i = start; i < from.size(); i++) {
		vd temp;
		for (int j = 0; j < from[i].size(); j++) {
			temp.push_back(stod(from[i][j]));
		}
		to.push_back(temp);
	}
	return to;
}

//Reads the file "filename", stores it in 2d vector of string and returns it
vvs getData(string filename){
	vvs dataset;
	int c=0;
	string line;
	ifstream myfile;
	myfile.open(filename);
	if(myfile.is_open()){
		while(getline(myfile,line)){
			dataset.push_back(splitLines(line,','));
		}
	}else cout << "can't open file error";
	myfile.close();
	return dataset;
}

//Splits the line of string by the delimiter and returns a vector of strings
vs splitLines(string line, char delimiter){
	vs x;
	size_t pos = 0;
	string token;
	
	while( (pos=line.find(delimiter)) != string::npos ){
		token = line.substr(0,pos);
		x.push_back(token);
		line.erase(0,pos+1);
	}
	x.push_back(line);

	return x;
}

//Prints vvs datas
void printDatas(vvs datas){
	for (int i = 0; i < datas.size(); i++) {
		for (int j = 0; j < datas[i].size(); j++) {
			cout << datas[i][j] << "\t";
		}
		cout << endl;
	}
	space;

}

//Prints vvd datas
void printDatas(vvd datas) {
	for (int i = 0; i < datas.size(); i++) {
		for (int j = 0; j < datas[i].size(); j++) {
			cout << datas[i][j] << "\t";
		}
		cout << endl;
	}
	space;
}

//Prints vvd datas
void printDatas(vvvd datas) {
	for (int i = 0; i < datas.size(); i++) {
		printDatas(datas[i]);
		space;
	}
	space;
}

//Prints vd datas
void printDatas(vd datas) {
	for (int i = 0; i < datas.size(); i++) {
		cout << datas[i] << "\t";
	}
	space;
}


void printDatas(map<int, vvd> datas) {
	for (auto itr = datas.begin(); itr != datas.end(); ++itr) {
		cout << "Clutster: " << itr->first << endl;
		printDatas(itr->second);
		space;
	}
}


void csvExporter(string fname,vvd data) {
	ofstream outFile(fname);
	outFile << "v1,v2,v3,v4,v5" << endl;
	for (int i = 0; i < data.size(); i++) {
		for (int j = 0; j < data[i].size(); j++) {
			outFile << data[i][j];
			if (j != data[i].size() - 1)
				outFile << ",";
		}
		outFile << endl;
	}
}
