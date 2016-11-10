#include <vector>
#include "AccelDataPoint.h"

class TAccelTOCData{
	private: 
		unsigned dTOC;
		std::vector <TAccelDataPoint> dAccelData;
		
		unsigned GetTOC(){
			return dTOC;
		};

    //Neuen Datenpunkt hinzufügen mit einzelnen Parametern
    void AddAccelDataPoint(unsigned cruisetype, double time, double dist, double fuel){
        TAccelDataPoint tmp(cruisetype, time, dist, fuel);
        AddAccelDataPoint(tmp);
    };

    //Neuen Datenpunkt hinzufügen
    void AddAccelDataPoint(TAccelDataPoint NewAccelDataPoint){
        dAccelData.push_back(NewAccelDataPoint);
    };

    //Index des Datepunktes mit TOC zurückgeben
    unsigned GetAccelDataIndex(unsigned cruisetype){
        unsigned i;
        for (i = 0; i < dAccelData.size(); i++){
            if (dAccelData.at(i).CruiseType() == cruisetype) break;
        }
        return i;
    };

    // Check ob Datenpunkt für TOC existiert
    bool CheckAccelData(unsigned cruisetype){
        return (GetAccelDataIndex(cruisetype) < dAccelData.size());
    };

    // Datapoint mit TOC zurückgeben
    TAccelDataPoint GetAccelDataPoint(unsigned cruisetype){
        if (!(IsValid(cruisetype))) {
        	TAccelDataPoint tmp(INVAL_CRUISETYPE, INVAL_TIME, INVAL_DIST, INVAL_FUEL);
        	return tmp;
        }
        return dAccelData.at(GetAccelDataIndex(cruisetype));
    };
    
 public:

    TAccelTOCData(){};

    //Constructor mit init
    TAccelTOCData(unsigned toc){
        dTOC = toc;
    };

    //Destructor
    ~TAccelTOCData(void){};

    //Neuen Datenpunkt hinzufügen
    void AccelData(unsigned cruisetype, double time, double dist, double fuel){
        AddAccelDataPoint(cruisetype, time, dist,fuel);
    };

    void AccelData(TAccelDataPoint NewAccelDataPoint){
        AddAccelDataPoint(NewAccelDataPoint);
    };

    //Public Getter
    unsigned TOC(){
        return dTOC;
    };

    //Ganzen Datenpunkt zurückgeben
    TAccelDataPoint ClimbData(unsigned cruisetype){
        return GetAccelDataPoint(cruisetype);
    };
    
    bool IsValid(unsigned cruisetype){
    	return CheckAccelData(cruisetype);
    };

    //Einzelne Werte zurückgeben
    double Time(unsigned cruisetype){
        return GetAccelDataPoint(cruisetype).Time();
    };

    double Dist(unsigned cruisetype){
        return GetAccelDataPoint(cruisetype).Dist();
    };

    double Fuel(unsigned cruisetype){
        return GetAccelDataPoint(cruisetype).Fuel();
    };
};