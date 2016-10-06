#include <iostream>
#include <vector>
#include "../Misc/Interpol.h"
#include "ClimbType.h"

class TClimbRPMData
{
  private:
    unsigned dRPM;
      std::vector < TClimbTypeData > dClimbData;

    unsigned GetRPM(){
		return dRPM;
	};

	// Neuen Datenpunkt hinzufügen mit einzelnen Parametern
    void AddClimbDataPoint(unsigned climbtype, signed isadev, double aicortime, double aicordist,
                           double aicorfuel, unsigned mass, unsigned fl, unsigned kias, double trq,
						   double time, double dist, double fuel){
		TClimbDataPoint tmp(fl, kias, trq, time, dist, fuel);
		AddClimbDataPoint(climbtype, isadev, aicortime, aicordist, aicorfuel, mass, tmp);
	};

	// Neuen Datenpunkt hinzufügen
    void AddClimbDataPoint(unsigned climbtype, signed isadev, double aicortime, double aicordist,
                           double aicorfuel, unsigned mass, TClimbDataPoint NewClimbDataPoint){
		// 1. gibt es die masse schon? sonst erstellen. Dann Datenpunkt
		// hinzufügen
        if (!(CheckClimbData(climbtype))){
			TClimbTypeData tmp(climbtype);
            dClimbData.push_back(tmp);
		}
        dClimbData.at(GetClimbDataIndex(climbtype)).ClimbData(isadev, aicortime, aicordist,
																	  aicorfuel, mass,
																	  NewClimbDataPoint);
	};

	// Index des Datepunktes mit ClimbType zurückgeben
    unsigned GetClimbDataIndex(unsigned climbtype){
        unsigned i;
        for (i = 0; i < dClimbData.size(); i++){
            if (dClimbData.at(i).ClimbType() == climbtype)
				break;
		}
		return i;
	};

	// Check ob Datenpunkt für Masse existiert
    bool CheckClimbData(unsigned climbtype){
        return (GetClimbDataIndex(climbtype) < dClimbData.size());
	};

	// Datapoint mit Parametern
    TClimbDataPoint GetClimbDataPoint(unsigned climbtype, signed isadev, unsigned mass, unsigned fl){
        if (!(CheckClimbData(climbtype))){
			TClimbDataPoint tmp(0, 0, 0, 0, 0, 0);
			return tmp;
		}
		else
            return dClimbData.at(GetClimbDataIndex(climbtype)).ClimbData(isadev, mass, fl);
	};

	// Coorekturfatoren für AntiIce ausgeben. Wenn klimbtype nicht vorhanden, 
	// dann -999 zurückgeben
    double GetAICorTime(unsigned climbtype, signed isadev){
        if (CheckClimbData(climbtype))
            return dClimbData.at(GetClimbDataIndex(climbtype)).AICorTime(isadev);
		else
			return 0;
	};

    double GetAICorDist(unsigned climbtype, signed isadev){
        if (CheckClimbData(climbtype))
            return dClimbData.at(GetClimbDataIndex(climbtype)).AICorDist(isadev);
		else
			return 0;
	};

    double GetAICorFuel(unsigned climbtype, signed isadev){
        if (CheckClimbData(climbtype))
            return dClimbData.at(GetClimbDataIndex(climbtype)).AICorFuel(isadev);
		else
			return 0;
	};

  public:

	TClimbRPMData(){
	};

	// Constructor mit init
    TClimbRPMData(unsigned rpm){
		dRPM = rpm;
	};

	// Destructor
	~TClimbRPMData(void){
	};

	// Neuen Datenpunkt hinzufügen
    void ClimbData(unsigned climbtype, signed isadev, double aicortime, double aicordist, double aicorfuel,
                   unsigned mass, unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
		AddClimbDataPoint(climbtype, isadev, aicortime, aicordist, aicorfuel, mass, fl, kias, trq,
						  time, dist, fuel);
	};

	// Punkt für ISADev muss bereits bestehen
    void ClimbData(unsigned climbtype, signed isadev, double aicortime, double aicordist, double aicorfuel,
                   unsigned mass, TClimbDataPoint NewClimbDataPoint){
		AddClimbDataPoint(climbtype, isadev, aicortime, aicordist, aicorfuel, mass,
						  NewClimbDataPoint);
	};

	// Public Getter
    unsigned RPM(){
		return GetRPM();
	};

	// Ganzen Datenpunkt zurückgeben 
    TClimbDataPoint ClimbData(unsigned climbtype, signed isadev, unsigned mass, unsigned fl){
		return GetClimbDataPoint(climbtype, isadev, mass, fl);
	};

	bool IsValid(unsigned climbtype){
		return CheckClimbData(climbtype);
	};

    bool IsValid(unsigned climbtype, signed isadev){
    	if (!(IsValid(climbtype))) return false;
        return dClimbData.at(GetClimbDataIndex(climbtype)).IsValid(isadev);
	};

    bool IsValid(unsigned climbtype, signed isadev, unsigned mass){
    	if (!(IsValid(climbtype))) return false;    	
        return dClimbData.at(GetClimbDataIndex(climbtype)).IsValid(isadev, mass);
	};

    bool IsValid(unsigned climbtype, signed isadev, unsigned mass, unsigned fl){
    	if (!(IsValid(climbtype))) return false;
        return dClimbData.at(GetClimbDataIndex(climbtype)).IsValid(isadev, mass, fl);
	};

	// Korrekturfaktoren für AntiIce
    double AICorTime(unsigned climbtype, signed isadev){
		return GetAICorTime(climbtype, isadev);
	};

    double AICorDist(unsigned climbtype, signed isadev){
		return GetAICorDist(climbtype, isadev);
	};

    double AICorFuel(unsigned climbtype, signed isadev){
		return GetAICorFuel(climbtype, isadev);
	};

	// Einzelne Werte zurückgeben
    unsigned KIAS(unsigned climbtype, signed isadev, unsigned mass, unsigned fl){
		return GetClimbDataPoint(climbtype, isadev, mass, fl).KIAS();
	};

    double TRQ(unsigned climbtype, signed isadev, unsigned mass, unsigned fl){
		return GetClimbDataPoint(climbtype, isadev, mass, fl).TRQ();
	};

    double Time(unsigned climbtype, signed isadev, unsigned mass, unsigned fl){
		return GetClimbDataPoint(climbtype, isadev, mass, fl).Time();
	};

    double Dist(unsigned climbtype, signed isadev, unsigned mass, unsigned fl){
		return GetClimbDataPoint(climbtype, isadev, mass, fl).Dist();
	};

    double Fuel(unsigned climbtype, signed isadev, unsigned mass, unsigned fl){
		return GetClimbDataPoint(climbtype, isadev, mass, fl).Fuel();
	};
};