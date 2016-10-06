#include <iostream>
#include <vector>
#include "../Misc/Interpol.h"
#include "ClimbMass.h"


class TClimbISADevData
{
  private:
    int dISADev;
    double dAICorTime, dAICorDist, dAICorFuel; //IsaDev und AntiIce on delta Time / Dist / Fuel
      std::vector < TClimbMassData > dClimbData;

    signed GetISADev(){
		return dISADev;};
		
    double GetAICorTime(){
        return dAICorTime;};
		
    double GetAICorDist(){
        return dAICorDist;};
		
    double GetAICorFuel(){
        return dAICorFuel;};
		
	//Neuen Datenpunkt hinzufügen mit einzelnen Parametern
    void AddClimbDataPoint(unsigned mass, unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
		TClimbDataPoint tmp(fl, kias, trq, time, dist, fuel);
		AddClimbDataPoint(mass, tmp);
	};
	
	//Neuen Datenpunkt hinzufügen
    void AddClimbDataPoint(unsigned mass, TClimbDataPoint NewClimbDataPoint){
	//1. gibt es die masse schon? sonst erstellen. Dann Datenpunkt hinzufügen	
	if (!(CheckClimbMassData(mass))){
			TClimbMassData tmp(mass);
            dClimbData.push_back(tmp);
		}
        dClimbData.at(GetClimbDataIndex(mass)).ClimbData(NewClimbDataPoint);
	};

	//Index des Datepunktes mit Masse zurückgeben
    unsigned GetClimbDataIndex(unsigned mass){
        unsigned i;
        for (i = 0; i < dClimbData.size(); i++){
            if (dClimbData.at(i).Mass() == mass) break;
		}
		return i;
	};

	// Check ob Datenpunkt für Masse existiert
    bool CheckClimbMassData(unsigned mass){
        if (GetClimbDataIndex(mass) < dClimbData.size()){
			return true;
		}
		else{
			return false;
		}
	};
			
	// Datapoint mit Mass und FL zurückgeben
    TClimbDataPoint GetClimbDataPoint(unsigned mass, unsigned fl){
		if (CheckClimbMassData(mass)){
            return dClimbData.at(GetClimbDataIndex(mass)).ClimbData(fl);
		}
		else{
			return InterpolClimbDataPoint(mass, fl);
		}
	};
	 	
	// sucht die nächst-kleinere Mass in ClimbMassData und gibt sie zurück
    unsigned LowerMass(unsigned mass){
        unsigned result = MinMass();
        for (unsigned i = 0; i < dClimbData.size(); i++){
            if ((dClimbData.at(i).Mass() <= mass) && (result < dClimbData.at(i).Mass())){
                result = dClimbData.at(i).Mass();
				}
		}	
	return result;
	};

	// sucht die nächst-grössere Mass in ClimbMassData und gibt sie zurück 
    unsigned UpperMass(unsigned mass){
        unsigned result = MaxMass();
        for (unsigned i = 0; i < dClimbData.size(); i++){
            if ((dClimbData.at(i).Mass() >= mass) && (result > dClimbData.at(i).Mass())){
                result = dClimbData.at(i).Mass();
				}
		}
		return result;
	};
	
	//Sucht die grösste vorhandene  Masse und gibt sie zurück
    unsigned MaxMass(){
        unsigned result = 0;
        for (unsigned i = 0; i < dClimbData.size(); i++){
            if (dClimbData.at(i).Mass() > result) result = dClimbData.at(i).Mass();
		}
	return result;
	};

	//Sucht die kleinste vorhandene  Masse und gibt sie zurück
    unsigned MinMass(){
        unsigned result = MaxMass();
        for (unsigned i = 0; i < dClimbData.size(); i++){
            if (dClimbData.at(i).Mass() < result) result = dClimbData.at(i).Mass();
		}
	return result;
	};
	
	bool IsInRange(unsigned mass){
		return (!((mass < MinMass()) || (mass > MaxMass())));
	};
		
	// Erstellt einen Interpolierten DatenPunkt zum gewählten FL
    TClimbDataPoint InterpolClimbDataPoint(unsigned mass, unsigned fl){
		if (!(IsInRange(mass))){
			TClimbDataPoint tmp(0, 0, 0, 0, 0, 0);
			return tmp;
		}
		TClimbDataPoint lower = GetClimbDataPoint(LowerMass(mass), fl);
		TClimbDataPoint upper = GetClimbDataPoint(UpperMass(mass), fl);
        unsigned kias = InterpolateLinear1D(lower.KIAS(), upper.KIAS(), LowerMass(mass), UpperMass(mass), mass);
		double trq = InterpolateLinear1D(lower.TRQ(), upper.TRQ(), LowerMass(mass), UpperMass(mass), mass);
		double time = InterpolateLinear1D(lower.Time(), upper.Time(), LowerMass(mass), UpperMass(mass), mass);
		double dist = InterpolateLinear1D(lower.Dist(), upper.Dist(), LowerMass(mass), UpperMass(mass), mass);
		double fuel = InterpolateLinear1D(lower.Fuel(), upper.Fuel(), LowerMass(mass), UpperMass(mass), mass);
		TClimbDataPoint tmp(fl, kias, trq, time, dist, fuel);
		return tmp;
	};

  public:
  
	TClimbISADevData(){};
	
  //Constructor mit init
    TClimbISADevData(double isadev, double aicortime, double aicordist, double aicorfuel){
		dISADev = isadev;
        dAICorTime = aicortime;
        dAICorDist = aicordist;
        dAICorFuel = aicorfuel;
	};
	
	//Destructor
	~TClimbISADevData(void){};
	
	//Neuen Datenpunkt hinzufügen
    void ClimbData(unsigned mass, unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
		AddClimbDataPoint(mass, fl, kias, trq, time, dist, fuel);
	};
	
    void ClimbData(unsigned mass, TClimbDataPoint NewClimbDataPoint){
		AddClimbDataPoint(mass, NewClimbDataPoint);
	};

	//Public Getter
    signed ISADev(){
		return GetISADev();
	};
	
    double AICorTime(){
        return GetAICorTime();
	};
	
    double AICorDist(){
        return GetAICorDist();
	};
	
    double AICorFuel(){
        return GetAICorFuel();
	};

	//Ganzen Datenpunkt zurückgeben	
    TClimbDataPoint ClimbData(unsigned mass, unsigned fl){
		return GetClimbDataPoint(mass, fl);
	};
	
	bool IsValid(unsigned mass){
		return IsInRange(mass);
	};
	
	bool IsValid(unsigned mass, unsigned fl){
		if (!(IsValid(mass))) return false;
		return (dClimbData.at(GetClimbDataIndex(LowerMass(mass))).IsValid(fl) && dClimbData.at(GetClimbDataIndex(UpperMass(mass))).IsValid(fl));
	};
		
	//Einzelne Werte zurückgeben
    unsigned KIAS(unsigned mass, unsigned fl){
		return GetClimbDataPoint(mass, fl).KIAS();
	};

    double TRQ(unsigned mass, unsigned fl){
		return GetClimbDataPoint(mass, fl).TRQ();
	};
	
    double Time(unsigned mass, unsigned fl){
		return GetClimbDataPoint(mass, fl).Time();
	};

    double Dist(unsigned mass, unsigned fl){
		return GetClimbDataPoint(mass, fl).Dist();
	};

    double Fuel(unsigned mass, unsigned fl){
		return GetClimbDataPoint(mass, fl).Fuel();
	};
};