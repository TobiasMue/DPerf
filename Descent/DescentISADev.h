#include <iostream>
#include <vector>
#include "../Misc/Interpol.h"
#include "DescentMass.h"

class TDescentISADevData
{
  private:
    int dISADev;
      std::vector < TDescentMassData > dDescentData;

    int GetISADev(){
		return dISADev;
	};
		
	//Neuen Datenpunkt hinzufügen mit einzelnen Parametern
    void AddDescentDataPoint(unsigned mass, unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
		TDescentDataPoint tmp(fl, kias, trq, time, dist, fuel);
		AddDescentDataPoint(mass, tmp);
	};
	
	//Neuen Datenpunkt hinzufügen
    void AddDescentDataPoint(unsigned mass, TDescentDataPoint NewDescentDataPoint){
	//1. gibt es die masse schon? sonst erstellen. Dann Datenpunkt hinzufügen	
	if (!(CheckDescentMassData(mass))){
			TDescentMassData tmp(mass);
            dDescentData.push_back(tmp);
		}
        dDescentData.at(GetDescentDataIndex(mass)).DescentData(NewDescentDataPoint);
	};

	//Index des Datepunktes mit Masse zurückgeben
    unsigned GetDescentDataIndex(unsigned mass){
        unsigned i;
        for (i = 0; i < dDescentData.size(); i++){
            if (dDescentData.at(i).Mass() == mass) break;
		}
		return i;
	};

	// Check ob Datenpunkt für Masse existiert
    bool CheckDescentMassData(unsigned mass){
        if (GetDescentDataIndex(mass) < dDescentData.size()){
			return true;
		}
		else{
			return false;
		}
	};
			
	// Datapoint mit Mass und FL zurückgeben
    TDescentDataPoint GetDescentDataPoint(unsigned mass, unsigned fl){
		if (CheckDescentMassData(mass)){
            return dDescentData.at(GetDescentDataIndex(mass)).DescentData(fl);
		}
		else{
			return InterpolDescentDataPoint(mass, fl);
		}
	};
	 	
	// sucht die nächst-kleinere Mass in DescentMassData und gibt sie zurück
    unsigned LowerMass(unsigned mass){
        unsigned result = MinMass();
        for (unsigned i = 0; i < dDescentData.size(); i++){
            if ((dDescentData.at(i).Mass() <= mass) && (result < dDescentData.at(i).Mass())){
                result = dDescentData.at(i).Mass();
				}
		}	
	return result;
	};

	// sucht die nächst-grössere Mass in DescentMassData und gibt sie zurück 
    unsigned UpperMass(unsigned mass){
        unsigned result = MaxMass();
        for (unsigned i = 0; i < dDescentData.size(); i++){
            if ((dDescentData.at(i).Mass() >= mass) && (result > dDescentData.at(i).Mass())){
                result = dDescentData.at(i).Mass();
				}
		}
		return result;
	};
	
	//Sucht die grösste vorhandene  Masse und gibt sie zurück
    unsigned MaxMass(){
        unsigned result = 0;
        for (unsigned i = 0; i < dDescentData.size(); i++){
            if (dDescentData.at(i).Mass() > result) result = dDescentData.at(i).Mass();
		}
	return result;
	};

	//Sucht die kleinste vorhandene  Masse und gibt sie zurück
    unsigned MinMass(){
        unsigned result = MaxMass();
        for (unsigned i = 0; i < dDescentData.size(); i++){
            if (dDescentData.at(i).Mass() < result) result = dDescentData.at(i).Mass();
		}
	return result;
	};
	
	bool IsInRange(unsigned mass){
		return(!((mass < MinMass()) || (mass > MaxMass())));
	};
	
	// Erstellt einen Interpolierten DatenPunkt zum gewählten FL
    TDescentDataPoint InterpolDescentDataPoint(unsigned mass, unsigned fl){
		if (!(IsValid(mass))){
			TDescentDataPoint tmp(0, INVAL_KIAS, INVAL_TRQ, INVAL_TIME, INVAL_DIST, INVAL_FUEL);
			return tmp;
		}
		TDescentDataPoint lower = GetDescentDataPoint(LowerMass(mass), fl);
		TDescentDataPoint upper = GetDescentDataPoint(UpperMass(mass), fl);
        unsigned kias = InterpolateLinear1D(lower.KIAS(), upper.KIAS(), LowerMass(mass), UpperMass(mass), mass);
		double trq = InterpolateLinear1D(lower.TRQ(), upper.TRQ(), LowerMass(mass), UpperMass(mass), mass);
		double time = InterpolateLinear1D(lower.Time(), upper.Time(), LowerMass(mass), UpperMass(mass), mass);
		double dist = InterpolateLinear1D(lower.Dist(), upper.Dist(), LowerMass(mass), UpperMass(mass), mass);
		double fuel = InterpolateLinear1D(lower.Fuel(), upper.Fuel(), LowerMass(mass), UpperMass(mass), mass);
		TDescentDataPoint tmp(fl, kias, trq, time, dist, fuel);
		return tmp;
	};

  public:
  
	TDescentISADevData(){};
	
  //Constructor mit init
    TDescentISADevData(int isadev){
		dISADev = isadev;
	};
	
	//Destructor
	~TDescentISADevData(void){};
	
	//Neuen Datenpunkt hinzufügen
    void DescentData(unsigned mass, unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
		AddDescentDataPoint(mass, fl, kias, trq, time, dist, fuel);
	};
	
    void DescentData(unsigned mass, TDescentDataPoint NewDescentDataPoint){
		AddDescentDataPoint(mass, NewDescentDataPoint);
	};

	//Public Getter
    int ISADev(){
		return GetISADev();
	};
	
	//Ganzen Datenpunkt zurückgeben	
    TDescentDataPoint DescentData(unsigned mass, unsigned fl){
		return GetDescentDataPoint(mass, fl);
	};
	
	bool IsValid(unsigned mass){
		return IsInRange(mass);
	};
		
    bool IsValid(unsigned mass, unsigned fl){
    	if (!(IsValid(mass))) return false;
        return (dDescentData.at(GetDescentDataIndex(LowerMass(mass))).IsValid(fl) && dDescentData.at(GetDescentDataIndex(LowerMass(mass))).IsValid(fl));
	};
	
	//Einzelne Werte zurückgeben
    unsigned KIAS(unsigned mass, unsigned fl){
		return GetDescentDataPoint(mass, fl).KIAS();
	};

    double TRQ(unsigned mass, unsigned fl){
		return GetDescentDataPoint(mass, fl).TRQ();
	};
	
    double Time(unsigned mass, unsigned fl){
		return GetDescentDataPoint(mass, fl).Time();
	};

    double Dist(unsigned mass, unsigned fl){
		return GetDescentDataPoint(mass, fl).Dist();
	};

    double Fuel(unsigned mass, unsigned fl){
		return GetDescentDataPoint(mass, fl).Fuel();
	};
};