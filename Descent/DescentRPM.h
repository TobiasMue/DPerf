#include <iostream>
#include <vector>
#include "../Misc/Interpol.h"
#include "DescentType.h"

class TDescentRPMData
{
  private:
    unsigned dRPM;
      std::vector < TDescentTypeData > dDescentData;

    unsigned GetRPM(){
		return dRPM;
	};

	// Neuen Datenpunkt hinzufügen mit einzelnen Parametern
    void AddDescentDataPoint(unsigned descenttype, signed isadev, unsigned mass, unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
		TDescentDataPoint tmp(fl, kias, trq, time, dist, fuel);
		AddDescentDataPoint(descenttype, isadev, mass, tmp);
	};

	// Neuen Datenpunkt hinzufügen
    void AddDescentDataPoint(unsigned descenttype, signed isadev, unsigned mass, TDescentDataPoint NewDescentDataPoint){
		// 1. gibt es die masse schon? sonst erstellen. Dann Datenpunkt
		// hinzufügen
        if (!(CheckDescentData(descenttype))){
			TDescentTypeData tmp(descenttype);
            dDescentData.push_back(tmp);
		}
        dDescentData.at(GetDescentDataIndex(descenttype)).DescentData(isadev, mass, NewDescentDataPoint);
	};

	// Index des Datepunktes mit DescentType zurückgeben
    unsigned GetDescentDataIndex(unsigned descenttype){
        unsigned i;
        for (i = 0; i < dDescentData.size(); i++){
            if (dDescentData.at(i).DescentType() == descenttype)
				break;
		}
		return i;
	};

	// Check ob Datenpunkt für Masse existiert
    bool CheckDescentData(unsigned descenttype){
        if (GetDescentDataIndex(descenttype) < dDescentData.size())
			return true;
		else
			return false;
	};

	// Datapoint mit Parametern
    TDescentDataPoint GetDescentDataPoint(unsigned descenttype, signed isadev, unsigned mass, unsigned fl){
        if (!(IsValid(descenttype))){
			TDescentDataPoint tmp(0, INVAL_KIAS, INVAL_TRQ, INVAL_TIME, INVAL_DIST, INVAL_FUEL);
			return tmp;
		}
		else
            return dDescentData.at(GetDescentDataIndex(descenttype)).DescentData(isadev, mass, fl);
	};

  public:

	TDescentRPMData(){
	};

	// Constructor mit init
    TDescentRPMData(unsigned rpm){
		dRPM = rpm;
	};

	// Destructor
	~TDescentRPMData(void){
	};

	// Neuen Datenpunkt hinzufügen
    void DescentData(unsigned descenttype, signed isadev, unsigned mass, unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
		AddDescentDataPoint(descenttype, isadev, mass, fl, kias, trq,
						  time, dist, fuel);
	};

	// Punkt für ISADev muss bereits bestehen
    void DescentData(unsigned descenttype, signed isadev, unsigned mass, TDescentDataPoint NewDescentDataPoint){
		AddDescentDataPoint(descenttype, isadev, mass,
						  NewDescentDataPoint);
	};

	// Public Getter
    unsigned RPM(){
		return GetRPM();
	};

	// Ganzen Datenpunkt zurückgeben 
    TDescentDataPoint DescentData(unsigned descenttype, signed isadev, unsigned mass, unsigned fl){
		return GetDescentDataPoint(descenttype, isadev, mass, fl);
	};
	
	bool IsValid(unsigned descenttype){
		return CheckDescentData(descenttype);
	};

    bool IsValid(unsigned descenttype, signed isadev){
    	if (!(IsValid(descenttype))) return false;
        return dDescentData.at(GetDescentDataIndex(descenttype)).IsValid(isadev);
	};

    bool IsValid(unsigned descenttype, signed isadev, unsigned mass){
    	if (!(IsValid(descenttype))) return false;
        return dDescentData.at(GetDescentDataIndex(descenttype)).IsValid(isadev, mass);
	};
	
	bool IsValid(unsigned descenttype, signed isadev, unsigned mass, unsigned fl){
    	if (!(IsValid(descenttype))) return false;
        return dDescentData.at(GetDescentDataIndex(descenttype)).IsValid(isadev, mass, fl);
	};

	// Einzelne Werte zurückgeben
    unsigned KIAS(unsigned descenttype, signed isadev, unsigned mass, unsigned fl){
		return GetDescentDataPoint(descenttype, isadev, mass, fl).KIAS();
	};

    double TRQ(unsigned descenttype, signed isadev, unsigned mass, unsigned fl){
		return GetDescentDataPoint(descenttype, isadev, mass, fl).TRQ();
	};

    double Time(unsigned descenttype, signed isadev, unsigned mass, unsigned fl){
		return GetDescentDataPoint(descenttype, isadev, mass, fl).Time();
	};

    double Dist(unsigned descenttype, signed isadev, unsigned mass, unsigned fl){
		return GetDescentDataPoint(descenttype, isadev, mass, fl).Dist();
	};

    double Fuel(unsigned descenttype, signed isadev, unsigned mass, unsigned fl){
		return GetDescentDataPoint(descenttype, isadev, mass, fl).Fuel();
	};
};