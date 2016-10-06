#include <vector>
#include "../Misc/Interpol.h"
#include "AccelClimbType.h"

/**
 * @brief The TAccelClimbTypeData class
 * On layer above TAccelDataPoint.
 * Stores a vector of TAccelDataPoints as well as an int dMass and features several functions to
 * get and set them. All get-requests targeting AccelDataPoint-Members are basically
 * being "routet through" and sinply return the return value
 */

class TAccelRPMData
{
private:
    unsigned dRPM;
    std::vector < TAccelClimbTypeData > dAccelData;

    unsigned GetRPM(){
        return dRPM;
    };

    //Neuen Datenpunkt hinzufügen mit einzelnen Parametern
    void AddAccelDataPoint(unsigned climbtype, unsigned toc, unsigned cruisetype, double time, double dist, double fuel){
    	TAccelDataPoint tmp(cruisetype, time, dist, fuel);
        AddAccelDataPoint(climbtype, toc, tmp);
    };

    //Neuen Datenpunkt hinzufügen
    void AddAccelDataPoint(unsigned climbtype, unsigned toc, TAccelDataPoint NewAccelDataPoint){
        if(!(CheckAccelData(climbtype))){
        	TAccelClimbTypeData tmp(climbtype);
        	dAccelData.push_back(tmp);
        }
        dAccelData.at(GetAccelDataIndex(climbtype)).AccelData(toc, NewAccelDataPoint);
    };

    //Index des Datepunktes mit TOC zurückgeben
    unsigned GetAccelDataIndex(unsigned climbtype){
        unsigned i;
        for (i = 0; i < dAccelData.size(); i++){
            if (dAccelData.at(i).ClimbType() == climbtype) break;
        }
        return i;
    };

    // Check ob Datenpunkt für TOC existiert
    bool CheckAccelData(unsigned climbtype){
        if (GetAccelDataIndex(climbtype) < dAccelData.size()){
            return true;
        }
        else{
            return false;
        }
    };

    // Datapoint mit TOC zurückgeben
    TAccelDataPoint GetAccelDataPoint(unsigned climbtype, unsigned toc, unsigned cruisetype){
        if (!(IsValid(climbtype))){
        	TAccelDataPoint tmp(INVAL_CRUISETYPE, INVAL_TIME, INVAL_DIST, INVAL_FUEL);
        	return tmp;
        }
        return dAccelData.at(GetAccelDataIndex(climbtype)).AccelData(toc, cruisetype);
    };
    
public:

    TAccelRPMData(){};

    //Constructor mit init
    TAccelRPMData(unsigned rpm){
        dRPM = rpm;
    };

    //Destructor
    ~TAccelRPMData(void){};

    //Neuen Datenpunkt hinzufügen
    void AccelData(unsigned climbtype, unsigned toc, unsigned cruisetype, double time, double dist, double fuel){
        AddAccelDataPoint(climbtype, toc, cruisetype, time, dist,fuel);
    };

    void AccelData(unsigned climbtype, unsigned toc, TAccelDataPoint NewAccelDataPoint){
        AddAccelDataPoint(climbtype, toc, NewAccelDataPoint);
    };

    //Public Getter
    unsigned RPM(){
        return GetRPM();
    };

    //Ganzen Datenpunkt zurückgeben
    TAccelDataPoint AccelData(unsigned climbtype, unsigned toc, unsigned cruisetype){
        return GetAccelDataPoint(climbtype, toc, cruisetype);
    };
    
    bool IsValid(unsigned climbtype){
    	return CheckAccelData(climbtype);
    };
	
   bool IsValid(unsigned climbtype, unsigned toc){
   	if (!(IsValid(climbtype))) return false;
   	return dAccelData.at(GetAccelDataIndex(climbtype)).IsValid(toc);
	};

    bool IsValid(unsigned climbtype, unsigned toc, unsigned cruisetype){
    	if (!(IsValid(climbtype))) return false;
        return dAccelData.at(GetAccelDataIndex(climbtype)).IsValid(toc, cruisetype);
	};

    //Einzelne Werte zurückgeben
    double Time(unsigned climbtype, unsigned toc, unsigned cruisetype){
        return GetAccelDataPoint(climbtype, toc, cruisetype).Time();
    };

    double Dist(unsigned climbtype, unsigned toc, unsigned cruisetype){
        return GetAccelDataPoint(climbtype, toc, cruisetype).Dist();
    };

    double Fuel(unsigned climbtype, unsigned toc, unsigned cruisetype){
        return GetAccelDataPoint(climbtype, toc, cruisetype).Fuel();
    };
};