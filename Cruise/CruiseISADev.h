#include <iostream>
#include <vector>
#include "../Misc/Interpol.h"
#include "CruiseMass.h"

class TCruiseISADevData
{
  private:
    int dISADev;
    double dAICorTRQ, dAICorFF; //IsaDev und AntiIce on delta KTAS / FF
      std::vector < TCruiseMassData > dCruiseData;

    int GetISADev(){
		return dISADev;};
				
    double GetAICorTRQ(){
        return dAICorTRQ;};
		
    double GetAICorFF(){
        return dAICorFF;};
		
	//Neuen Datenpunkt hinzufügen mit einzelnen Parametern
    void AddCruiseDataPoint(unsigned mass, unsigned fl, unsigned kias, double trq, double ktas, double ff){
		TCruiseDataPoint tmp(fl, kias, trq, ktas, ff);
		AddCruiseDataPoint(mass, tmp);
	};
	
	//Neuen Datenpunkt hinzufügen
    void AddCruiseDataPoint(unsigned mass, TCruiseDataPoint NewCruiseDataPoint){
	//1. gibt es die masse schon? sonst erstellen. Dann Datenpunkt hinzufügen	
	if (!(CheckCruiseMassData(mass))){
			TCruiseMassData tmp(mass);
            dCruiseData.push_back(tmp);
		}
        dCruiseData.at(GetCruiseDataIndex(mass)).CruiseData(NewCruiseDataPoint);
	};

	//Index des Datepunktes mit Masse zurückgeben
    unsigned GetCruiseDataIndex(unsigned mass){
        unsigned i;
        for (i = 0; i < dCruiseData.size(); i++){
            if (dCruiseData.at(i).Mass() == mass) break;
		}
		return i;
	};

	// Check ob Datenpunkt für Masse existiert
    bool CheckCruiseMassData(unsigned mass){
        if (GetCruiseDataIndex(mass) < dCruiseData.size()){
			return true;
		}
		else{
			return false;
		}
	};
			
	// Datapoint mit Mass und FL zurückgeben
    TCruiseDataPoint GetCruiseDataPoint(unsigned mass, unsigned fl){
		if (CheckCruiseMassData(mass)){
            return dCruiseData.at(GetCruiseDataIndex(mass)).CruiseData(fl);
		}
		else{
			return InterpolCruiseDataPoint(mass, fl);
		}
	};
	 	
	// sucht die nächst-kleinere Mass in CruiseMassData und gibt sie zurück
    unsigned LowerMass(unsigned mass){
        unsigned result = MinMass();
        for (unsigned i = 0; i < dCruiseData.size(); i++){
            if ((dCruiseData.at(i).Mass() <= mass) && (result < dCruiseData.at(i).Mass())){
                result = dCruiseData.at(i).Mass();
				}
		}	
	return result;
	};

	// sucht die nächst-grössere Mass in CruiseMassData und gibt sie zurück 
    unsigned UpperMass(unsigned mass){
        unsigned result = MaxMass();
        for (unsigned i = 0; i < dCruiseData.size(); i++){
            if ((dCruiseData.at(i).Mass() >= mass) && (result > dCruiseData.at(i).Mass())){
                result = dCruiseData.at(i).Mass();
				}
		}
		return result;
	};
	
	//Sucht die grösste vorhandene  Masse und gibt sie zurück
    unsigned MaxMass(){
        unsigned result = 0;
        for (unsigned i = 0; i < dCruiseData.size(); i++){
            if (dCruiseData.at(i).Mass() > result) result = dCruiseData.at(i).Mass();
		}
	return result;
	};

	//Sucht die kleinste vorhandene  Masse und gibt sie zurück
    unsigned MinMass(){
        unsigned result = MaxMass();
        for (unsigned i = 0; i < dCruiseData.size(); i++){
            if (dCruiseData.at(i).Mass() < result) result = dCruiseData.at(i).Mass();
		}
	return result;
	};
	
	bool IsInRange(unsigned mass){
		return (!(((mass < MinMass()) || (mass > MaxMass()))));	
	};
	
	// Erstellt einen Interpolierten DatenPunkt zum gewählten FL
    TCruiseDataPoint InterpolCruiseDataPoint(unsigned mass, unsigned fl){
		if (!(IsValid(mass))){
			TCruiseDataPoint tmp(INVAL_FL, INVAL_KIAS, INVAL_TRQ, INVAL_KTAS, INVAL_FF);
			return tmp;
		}
		TCruiseDataPoint lower = GetCruiseDataPoint(LowerMass(mass), fl);
		TCruiseDataPoint upper = GetCruiseDataPoint(UpperMass(mass), fl);
        unsigned kias = InterpolateLinear1D(lower.KIAS(), upper.KIAS(), LowerMass(mass), UpperMass(mass), mass);
		double trq = InterpolateLinear1D(lower.TRQ(), upper.TRQ(), LowerMass(mass), UpperMass(mass), mass);
		double ktas = InterpolateLinear1D(lower.KTAS(), upper.KTAS(), LowerMass(mass), UpperMass(mass), mass);
		double ff = InterpolateLinear1D(lower.FF(), upper.FF(), LowerMass(mass), UpperMass(mass), mass);
		TCruiseDataPoint tmp(fl, kias, trq, ktas, ff);
		return tmp;
	};

  public:
  
	TCruiseISADevData(){};
	
  //Constructor mit init
    TCruiseISADevData(int isadev, double aicortrq, double aicorff){
		dISADev = isadev;
        dAICorTRQ = aicortrq;
        dAICorFF = aicorff;
	};
	
	//Destructor
	~TCruiseISADevData(void){};
	
	//Neuen Datenpunkt hinzufügen
    void CruiseData(unsigned mass, unsigned fl, unsigned kias, double trq, double ktas, double ff){
		AddCruiseDataPoint(mass, fl, kias, trq, ktas, ff);
	};
	
    void CruiseData(unsigned mass, TCruiseDataPoint NewCruiseDataPoint){
		AddCruiseDataPoint(mass, NewCruiseDataPoint);
	};

	//Public Getter
    int ISADev(){
		return GetISADev();
	};
	
    double AICorTRQ(){
        return GetAICorTRQ();
	};
	
    double AICorFF(){
        return GetAICorFF();
	};
	
	//Ganzen Datenpunkt zurückgeben	
    TCruiseDataPoint CruiseData(unsigned mass, unsigned fl){
		return GetCruiseDataPoint(mass, fl);
	};

	bool IsValid(unsigned mass){
		return IsInRange(mass);
	};
			
    bool IsValid(unsigned mass, unsigned fl){
    	if (!(IsValid(mass))) return false;
        return (dCruiseData.at(GetCruiseDataIndex(LowerMass(mass))).IsValid(fl) && dCruiseData.at(GetCruiseDataIndex(UpperMass(mass))).IsValid(fl));
	};
	
	//Einzelne Werte zurückgeben
    unsigned KIAS(unsigned mass, unsigned fl){
		return GetCruiseDataPoint(mass, fl).KIAS();
	};

    double TRQ(unsigned mass, unsigned fl){
		return GetCruiseDataPoint(mass, fl).TRQ();
	};
	
    double KTAS(unsigned mass, unsigned fl){
		return GetCruiseDataPoint(mass, fl).KTAS();
	};

    double FF(unsigned mass, unsigned fl){
		return GetCruiseDataPoint(mass, fl).FF();
	};
};