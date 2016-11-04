#include <iostream>
#include <vector>
#include "../Misc/Interpol.h"
#include "DescentISADev.h"

class TDescentTypeData
{
  private:
	int dDescentType;
      std::vector < TDescentISADevData > dDescentData;

	int GetDescentType(){
		return dDescentType;
	};
	
	//Neuen Datenpunkt hinzufügen mit einzelnen Parametern
    void AddDescentDataPoint(signed isadev, unsigned mass, unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
		TDescentDataPoint tmp(fl, kias, trq, time, dist, fuel);
        AddDescentDataPoint(isadev, mass, tmp);
	};
	
	//Neuen Datenpunkt hinzufügen
    void AddDescentDataPoint(signed isadev, unsigned mass, TDescentDataPoint NewDescentDataPoint){
		//1. gibt es die masse schon? sonst erstellen. Dann Datenpunkt hinzufügen
        if (!(CheckDescentData(isadev))){
            TDescentISADevData tmp(isadev);
            dDescentData.push_back(tmp);
		}
        dDescentData.at(GetDescentDataIndex(isadev)).DescentData(mass, NewDescentDataPoint);
	};
	
	//Index des Datepunktes mit Masse zurückgeben
    unsigned GetDescentDataIndex(signed isadev){
        unsigned i;
        for (i = 0; i < dDescentData.size(); i++){
            if (dDescentData.at(i).ISADev() == isadev) break;
		}
		return i;
	};

	// Check ob Datenpunkt für Masse existiert
    bool CheckDescentData(signed isadev){
        if (GetDescentDataIndex(isadev) < dDescentData.size()) return true;
		else return false;
	};
			
	// Datapoint mit Mass und FL zurückgeben
    TDescentDataPoint GetDescentDataPoint(signed isadev, unsigned mass, unsigned fl){
        if (CheckDescentData(isadev)){
            return dDescentData.at(GetDescentDataIndex(isadev)).DescentData(mass, fl);
		}
		else{
			return InterpolDescentDataPoint(isadev, mass, fl);
		}
	};
		
    // sucht die nächst-kleinere ISADev in DescentData und gibt sie zurück
    int LowerISADev(signed isadev){
        int result = MinISADev();
        for (unsigned i = 0; i < dDescentData.size(); i++){
            if ((dDescentData.at(i).ISADev() <= isadev) && (result <= dDescentData.at(i).ISADev())){
                result = dDescentData.at(i).ISADev();
				}
		}	
	return result;
	};

    // sucht die nächst-grössere ISADev in DescentData und gibt sie zurück
    int UpperISADev(signed isadev){
        int result = MaxISADev();
        for (unsigned i = 0; i < dDescentData.size(); i++){
            if ((dDescentData.at(i).ISADev() >= isadev) && (result >= dDescentData.at(i).ISADev())){
                result = dDescentData.at(i).ISADev();
				}
		}
		return result;
	};
	
	//Sucht die grösste vorhandene ISADev und gibt sie zurück
    int MaxISADev(){
        int result = 0;
        for (unsigned i = 0; i < dDescentData.size(); i++){
            if (dDescentData.at(i).ISADev() > result) result = dDescentData.at(i).ISADev();
		}
	return result;
	};

	//Sucht die kleinste vorhandene ISADev und gibt sie zurück
    int MinISADev(){
        int result = MaxISADev();
        for (unsigned i = 0; i < dDescentData.size(); i++){
            if (dDescentData.at(i).ISADev() < result) result = dDescentData.at(i).ISADev();
		}
	return result;
	};
	
	bool IsInRange(signed isadev){
		return (!((isadev < MinISADev()) || (isadev > MaxISADev())));
	};
	
	// Erstellt einen Interpolierten DatenPunkt zum gewählten FL
    TDescentDataPoint InterpolDescentDataPoint(signed isadev, unsigned mass, unsigned fl){
		if (!(IsValid(isadev))){
			TDescentDataPoint tmp(0, 0, 0, 0, 0, 0);
			return tmp;
		}
		TDescentDataPoint lower = GetDescentDataPoint(LowerISADev(isadev), mass, fl);
		TDescentDataPoint upper = GetDescentDataPoint(UpperISADev(isadev), mass, fl);
        unsigned kias = InterpolateLinear1D(lower.KIAS(), upper.KIAS(), LowerISADev(isadev), UpperISADev(isadev), isadev);
		double trq = InterpolateLinear1D(lower.TRQ(), upper.TRQ(), LowerISADev(isadev), UpperISADev(isadev), isadev);
		double time = InterpolateLinear1D(lower.Time(), upper.Time(), LowerISADev(isadev), UpperISADev(isadev), isadev);
		double dist = InterpolateLinear1D(lower.Dist(), upper.Dist(), LowerISADev(isadev), UpperISADev(isadev), isadev);
		double fuel = InterpolateLinear1D(lower.Fuel(), upper.Fuel(), LowerISADev(isadev), UpperISADev(isadev), isadev);
		TDescentDataPoint tmp(fl, kias, trq, time, dist, fuel);
		return tmp;
	};
		
  public:
	
  TDescentTypeData(){};
	
  //Constructor mit init
    TDescentTypeData(unsigned climbtype){
		dDescentType = climbtype;
	};
	
	//Destructor
	~TDescentTypeData(void){};
	
	//Neuen Datenpunkt hinzufügen
    void DescentData(signed isadev, unsigned mass, unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
        AddDescentDataPoint(isadev, mass, fl, kias, trq, time, dist, fuel);
	};
	
	//Punkt für ISADev muss bereits bestehen
    void DescentData(signed isadev, unsigned mass, TDescentDataPoint NewDescentDataPoint){
        AddDescentDataPoint(isadev, mass, NewDescentDataPoint);
	};

	//Public Getter
    unsigned DescentType(){
		return GetDescentType();
	};

	//Ganzen Datenpunkt zurückgeben	
    TDescentDataPoint DescentData(signed isadev, unsigned mass, unsigned fl){
		return GetDescentDataPoint(isadev, mass, fl);
	};

    //Gibt Anzahl der Elemente in dDescentData zurück
    unsigned Size(){
        return dDescentData.size();
	};
	
	bool IsValid(signed isadev){
		return IsInRange(isadev);
	};
		
   bool IsValid(signed isadev, unsigned mass){
   	if (!(IsValid(isadev))) return false;
   	return (dDescentData.at(GetDescentDataIndex(LowerISADev(isadev))).IsValid(mass) && dDescentData.at(GetDescentDataIndex(UpperISADev(isadev))).IsValid(mass));
	};
		
   bool IsValid(signed isadev, unsigned mass, unsigned fl){
   	if (!(IsValid(isadev))) return false;
  (dDescentData.at(GetDescentDataIndex(LowerISADev(isadev))).IsValid(mass, fl) && dDescentData.at(GetDescentDataIndex(UpperISADev(isadev))).IsValid(mass, fl));
/*FIXME*/    return (0);
   };
		
	//Einzelne Werte zurückgeben
    unsigned KIAS(signed isadev, unsigned mass, unsigned fl){
		return GetDescentDataPoint(isadev, mass, fl).KIAS();
	};

    double TRQ(signed isadev, unsigned mass, unsigned fl){
		return GetDescentDataPoint(isadev, mass, fl).TRQ();
	};
	
    double Time(signed isadev, unsigned mass, unsigned fl){
		return GetDescentDataPoint(isadev, mass, fl).Time();
	};

    double Dist(signed isadev, unsigned mass, unsigned fl){
		return GetDescentDataPoint(isadev, mass, fl).Dist();
	};

    double Fuel(signed isadev, unsigned mass, unsigned fl){
		return GetDescentDataPoint(isadev, mass, fl).Fuel();
	};
};
