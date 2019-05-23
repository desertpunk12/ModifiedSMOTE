#pragma once

#include "mmain.hpp"

vvd prodModif(vvd, int, int, int, int);
vvd prodModifAllCluster(vvd, int, int, int);

vvd pairMinorityAndEDs(vvd, vvd);
vvvd near5Neighbors(vvd);
vvd rank5NearFromEx(vvd,int);
vvd lowest5(vvd, int);
vd roundDatas(vd);

vvvd getMinorityWithEDs(vvd);
vvd allEDs(vvd, int);


vvd produceSyntheticData(vvvd, int, int);



vvd combineAllClusters(vvd, vvd, vvd);


vvd remove0Classes(vvd);


vvd rankFromExemplar(vvd,int);
