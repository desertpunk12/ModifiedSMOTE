#pragma once

#include "modif.hpp"
#include "mmain.hpp"
#include "modifcomputes.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <map>


vvd prodModifAllCluster(vvd dataset, int N, int numOfVars,int indexOfClass, int indexOfCluster, int knn)
{
#pragma region Declaration of vvds
    vvd syntheticDatas;

    //Map of clusters key: cluster number, value: datas associated by the cluster
    map<int, vvd> clusters;
    //Map of Sparsity Factors key: cluster number, value: Sparsity Factor associated by each cluster
    map<int, double> sparFCs;
    //Map of Sampling Weights key: cluster number, value: Samplign Weights associated by each cluster
    map<int, double> samplWs;

    // vvd cluster8;
    // vvd cluster9;
    // vvd cluster10;
#pragma endregion


    // int indexOfClass = numOfVars + 1;
    // int indexOfCluster = indexOfClass + 1;

#pragma region Remove class with 0 as value
    dataset = remove0Classes(dataset, indexOfClass);
    /*
    cout << "Removed classes with 0 as values" << endl;
    cout << "---------------------------------------------------------" << endl;
    printDatas(dataset);
    */
#pragma endregion

#pragma region Separate each cluster and put in to each vvd

    // printDatas(dataset);
    cout << "^Dataset^" << endl;
    for (int i = 0; i < dataset.size(); i++)
    {
        int cc = dataset[i][indexOfCluster];
        auto xc = clusters.find(cc);
        if (xc == clusters.end())
        {
            std::cout << "cluster does not exist not creating cluster: " << cc << endl;
            vvd ddtt;
            ddtt.push_back(dataset[i]);
            clusters.insert(pair<int, vvd>(cc, ddtt));
        }
        else
        {
            xc->second.push_back(dataset[i]);
        }
    }

    // printDatas(clusters);

    // cout << "end of dataz!!" << endl;
#pragma endregion


#pragma region Computes Sparsity Factor of each Cluster
    for (auto itr = clusters.begin(); itr != clusters.end(); ++itr)
    {
        double spf = computeClusterSparsityFactor(itr->second, numOfVars);
        sparFCs.insert(pair<int, double>(itr->first, spf));
        cout << "Cluster: " << itr->first << "\t, Sparsity: " << spf << endl;
    }

    //Prints all the sparsity factors of each cluster
    // for (auto itr = sparFCs.begin(); itr != sparFCs.end(); ++itr)
    // {
    //     cout << "Cluster: " << itr->first << "\t, Sparsity: " << itr->second << endl;
    // }

    //double sparFC8 = computeClusterSparsityFactor(cluster8, numOfVars);
    // double sparFC9 = computeClusterSparsityFactor(cluster9, numOfVars);
    // double sparFC10 = computeClusterSparsityFactor(cluster10, numOfVars);
    /*
    cout << "Sparsity Factors:" << endl;
    cout << "C8: " << sparFC8 << " | C9: " << sparFC9 
        << " | C10: " << sparFC10 << endl;
        */

#pragma endregion

#pragma region Computes Sparsity Sum of all Cluster
    //double sparSum = sparFC8 + sparFC9 + sparFC10;
    double sparSum = 0;
    for (auto itr = sparFCs.begin(); itr != sparFCs.end(); ++itr)
    {
        sparSum += itr->second;
    }
    cout << "Sparsity Sum : " << sparSum << endl;

#pragma endregion

#pragma region Computes Sparsity Weight of each Cluster
    for (auto itr = sparFCs.begin(); itr != sparFCs.end(); ++itr)
    {
        double spw = itr->second / sparSum;
        samplWs.insert(pair<int, double>(itr->first, spw));
    }

    // double samplingWeightC8 = sparFC8 / sparSum;
    // double samplingWeightC9 = sparFC9 / sparSum;
    // double samplingWeightC10 = sparFC10 / sparSum;

    for (auto itr = samplWs.begin(); itr != samplWs.end(); ++itr)
    {
        cout << "Cluster: " << itr->first << "\t, SamplingWeight: " << itr->second << endl;
    }
    /*
    cout << "Sampling Weights    ";
    cout << samplingWeightC8 << " || ";
    cout << samplingWeightC9 << " || ";
    cout << samplingWeightC10;
*/

#pragma endregion

    space;

#pragma region Computing the number of samples for each cluster
    // int numSamplesC8 = round(N * samplingWeightC8);
    // int numSamplesC9 = round(N * samplingWeightC9);
    // int numSamplesC10 = round(N * samplingWeightC10);
    /*
    cout << "Number of Samples    ";
    cout << "N: " << N << "  :  ";
    cout << numSamplesC8 << " || ";
    cout << numSamplesC9 << " || ";
    cout << numSamplesC10;
    cout << "    Total: " << numSamplesC8 + numSamplesC9 + numSamplesC10;
    */
#pragma endregion

    space;

    //Using cluster 8 for now for testign
    // vvd synthC8 = prodModif(cluster8, numSamplesC8, numOfVars, 2, knn);
    // vvd synthC8 = prodModif(clusters[0], numSamplesC8, numOfVars, 2, knn);
    //cout << "Finally " << synthC8.size() << endl;
    //printDatas(synthC8);

    //All clusters
    space;
    space;

    // vvd synthC9 = prodModif(cluster9, numSamplesC9, numOfVars, 2, knn);
    //cout << "Finally " << synthC9.size() << endl;
    //printDatas(synthC9);
    space;
    space;

    // vvd synthC10 = prodModif(cluster10, numSamplesC10, numOfVars, 6, knn);
    //cout << "Finally " << synthC10.size() << endl;
    //printDatas(synthC10);

    // vvd combined = combineAllClusters(synthC8, synthC9, synthC10);

    // syntheticDatas = combined;


    //syntheticDatas = synthC8;
    return syntheticDatas;
}

vvd prodModif(vvd clusterDataset, int N, int numOfVars, int indexOfExemplar, int knn)
{
    /*
        1. Rank nearest neighbors of exemplar
        2. Get the 4 nearest neighbors
        3. From the exemplar move to the nearest neighbor to be the basis for
            creating the new sythetic data
        4. Move to the next in the rank
        
    */
    vvd producedSyntheticDatas;

    //cout << "cluster dataset" << endl;
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
    for (int i = 0; i < N; i++)
    {
        int indexOfTheCurrentExemplar = rank[i % rank.size()];
        //cout << "Index of Current Exempalar: " << indexOfTheCurrentExemplar << endl;
        vvd n4 = get4Neighbor(paired, indexOfTheCurrentExemplar);
        //printDatas(n4);
        vd randOversample = n4[r0to3()];
        vd exemplar = paired[indexOfTheCurrentExemplar];
        vd synthetic;
        for (int i = 0; i < numOfVars; i++)
        {
            double value = exemplar[i] + (r0to1() * (randOversample[i] - exemplar[i]));
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
vvd pairMinorityAndEDs(vvd minoritydata, vvd eds)
{
    for (int i = 0; i < eds.size(); i++)
    {
        for (int j = 0; j < eds[i].size(); j++)
        {
            minoritydata[i].push_back(eds[i][j]);
        }
    }
    return minoritydata;
}

//Returns the sets of 5 nearest neighbors of exemplar
vvd rank5NearFromEx(vvd clusterData, int indexOfExemplar)
{
    vvd low5 = clusterData;
    sort(low5.begin(), low5.end(), VDComparator(indexOfExemplar));
    int resizeTo = min(6, (int)low5.size());
    low5.resize(resizeTo);
    for (int i = 0; i < low5.size(); i++)
    {
        for (int j = 8; j < low5[i].size(); j++)
        {
            if (j == indexOfExemplar)
                continue;
            low5[i][j] = 0;
        }
    }

    return low5;
}


//Returns the sets of 5 nearest neghbors
vvvd near5Neighbors(vvd pair)
{
    vvvd low5s;
    for (int i = 8; i < 8 + pair.size(); i++)
    {
        low5s.push_back(lowest5(pair, i));
    }

    return low5s;
}


//Returns the minority datas with EDs
vvvd getMinorityWithEDs(vvd pair)
{
    vvvd low5s;
    for (int i = 7; i < 7 + pair.size(); i++)
    {
        low5s.push_back(allEDs(pair, i));
    }

    return low5s;
}


//Returns 5 lowest eds for a single minority in consideration
vvd lowest5(vvd data, int index_criteria)
{
    vvd low5 = data;
    sort(low5.begin(), low5.end(), VDComparator(index_criteria));
    int resizeTo = min(6, (int)low5.size());
    low5.resize(resizeTo);
    for (int i = 0; i < low5.size(); i++)
    {
        for (int j = 8; j < low5[i].size(); j++)
        {
            if (j == index_criteria)
                continue;
            low5[i][j] = 0;
        }
    }

    return low5;
}


//Returns 
vvd allEDs(vvd data, int index_criteria)
{
    vvd low5 = data;
    sort(low5.begin(), low5.end(), VDComparator(index_criteria));
    for (int i = 0; i < low5.size(); i++)
    {
        for (int j = 7; j < low5[i].size(); j++)
        {
            if (j == index_criteria)
                continue;
            low5[i][j] = 0;
        }
    }

    return low5;
}

vd roundDatas(vd datas)
{
    vd rounded;
    for (int i = 0; i < datas.size(); i++)
    {
        double xx = round(datas[i]);
        rounded.push_back(xx);
    }
    return rounded;
}

//Produces Synthetic Data using Weighted SMOTE
vvd producedSyntheticData(vvvd data, int N, int numOfVars)
{
    vvd syntheticDatas;
    for (int i = 0; i < N; i++)
    {
        int r1t5 = min((int)data[i].size() - 1, (int)r1to5());
        vd sd;
        for (int j = 0; j < numOfVars; j++)
        {
            double ddd = data[i][0][j] + (r0to1() * (data[i][r1t5][j] - data[i][0][j]));
            sd.push_back(ddd);
        }
        syntheticDatas.push_back(sd);
    }
    return syntheticDatas;
}


//Combines all vvd clusters into one vvd adds new attribute called cluster
vvd combineAllClusters(vvd c8, vvd c9, vvd c10)
{
    vvd all;
    for (int i = 0; i < c8.size(); i++)
    {
        all.push_back(c8[i]);
    }

    for (int i = 0; i < c9.size(); i++)
    {
        all.push_back(c9[i]);
    }


    for (int i = 0; i < c10.size(); i++)
    {
        all.push_back(c10[i]);
    }

    return all;
}


//Removes rows with 0 class values
vvd remove0Classes(vvd dataset, int indexOfClass)
{
    for (int i = 0; i < dataset.size(); i++)
    {
        if (dataset[i][indexOfClass] == 0)
        {
            dataset.erase(dataset.begin() + i);
            i--;
        }
    }
    return dataset;
}

vvd rankFromExemplar(vvd pairedData, int indexOfExemplar)
{
    vvd ranked = pairedData;
    int indexEDStarts = 8;
    indexOfExemplar += indexEDStarts; //because index 8 and below are the actual data an not the eds
    sort(ranked.begin(), ranked.end(), VDComparator(indexOfExemplar));
    ranked = removeExessDataFromPaired(ranked, indexOfExemplar);

    return ranked;
}

vvd removeExessDataFromPaired(vvd paired, int indexOfImportantData)
{
    int indexEDStarts = 8;
    for (int i = 0; i < paired.size(); i++)
    {
        if (indexOfImportantData == indexEDStarts)
        {
            paired[i].erase(paired[i].begin() + 1 + 8, paired[i].end());
        }
        else
        {
            //left of exemplar deletion
            paired[i].erase(paired[i].begin() + indexEDStarts, paired[i].begin() + indexOfImportantData);

            //right of exemplar deletion
            paired[i].erase(paired[i].begin() + indexEDStarts + 1, paired[i].end() - 1);
        }
    }

    return paired;
}

vvd get4Neighbor(vvd paired, int indexOfExemplar)
{
    vvd n4 = paired;
    int indexEDStarts = 8;
    sort(n4.begin(), n4.end(), VDComparator(indexOfExemplar + indexEDStarts));
    n4.erase(n4.begin());
    return n4;
}


vd getIndexOfRanksFromExemplar(vvd paired, int indexOfExemplar)
{
    vd ranks;

    int indexEDStarts = 8;
    indexOfExemplar += indexEDStarts; //because index 8 and below are the actual data an not the eds
    sort(paired.begin(), paired.end(), VDComparator(indexOfExemplar));

    for (int i = 0; i < paired.size(); i++)
    {
        ranks.push_back(paired[i][paired[i].size() - 1]);
    }

    return ranks;
}


vvd addIndexValueToVVD(vvd paired)
{
    for (int i = 0; i < paired.size(); i++)
    {
        paired[i].push_back(i);
    }

    return paired;
}

class Cluster
{
public:
    void tt();
};

map<int, Cluster*> generateClusters(vvd dataset, int ciOfCluster)
{
    map<int, Cluster*> clusters;
    for (int i = 0; i < dataset.size(); i++)
    {
        int cc = dataset[i][ciOfCluster];
        auto xc = clusters.find(cc);
        if (xc == clusters.end())
        {
            vvd ddtt;
            ddtt.push_back(dataset[i]);
            // ddtt.p
            Cluster* clust = new Cluster();
            clusters.insert(make_pair(cc, clust));
        }
        else
        {
            // xc->second->tt();
            //xc->second.push_back(dataset[i]);
        }
    }

    for (const auto& cluster : clusters)
    {
    }

    return clusters;
}
