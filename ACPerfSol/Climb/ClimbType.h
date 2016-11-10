#include <iostream>
#include <vector>
#include "../Misc/Interpol.h"
#include "ClimbISADev.h"

class TClimbTypeData
{
  private:
	int dClimbType;
      std::vector < TClimbISADevData > dClimbData;

	int GetClimbType(){
		return dClimbType;
	};
	
	//Neuen Datenpunkt hinzufügen mit einzelnen Parametern
    void AddClimbDataPoint(signed isadev, double aicortime, double aicordist, double aicorfuel, unsigned mass, unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
		TClimbDataPoint tmp(fl, kias, trq, time, dist, fuel);
        AddClimbDataPoint(isadev, aicortime, aicordist, aicorfuel, mass, tmp);
	};
	
	//Neuen Datenpunkt hinzufügen
    void AddClimbDataPoint(signed isadev, double aicortime, double aicordist, double aicorfuel, unsigned mass, TClimbDataPoint NewClimbDataPoint){
		//1. gibt es die masse schon? sonst erstellen. Dann Datenpunkt hinzufügen
        if (!(CheckClimbData(isadev))){
            TClimbISADevData tmp(isadev, aicortime, aicordist, aicorfuel);
            dClimbData.push_back(tmp);
		}
        dClimbData.at(GetClimbDataIndex(isadev)).ClimbData(mass, NewClimbDataPoint);
	};
	
	//Index des Datepunktes mit Masse zurückgeben
    unsigned GetClimbDataIndex(signed isadev){
        unsigned i;
        for (i = 0; i < dClimbData.size(); i++){
            if (dClimbData.at(i).ISADev() == isadev) break;
		}
		return i;
	};

	// Check ob Datenpunkt für Masse existiert
    bool CheckClimbData(signed isadev){
        if (GetClimbDataIndex(isadev) < dClimbData.size()) return true;
		else return false;
	};
			
	// Datapoint mit Mass und FL zurückgeben
    TClimbDataPoint GetClimbDataPoint(signed isadev, unsigned mass, unsigned fl){
        if (CheckClimbData(isadev)){
            return dClimbData.at(GetClimbDataIndex(isadev)).ClimbData(mass, fl);
		}
		else{
			return InterpolClimbDataPoint(isadev, mass, fl);
		}
	};
	
	//Correkturfatoren für AntiIce ausgeben oder interpolieren
    double GetAICorTime(signed isadev){
        if (CheckClimbData(isadev)){
            return dClimbData.at(GetClimbDataIndex(isadev)).AICorTime();
		}
		else{
            return InterpolAICorTime(isadev);
		}
	};
	
        double GetAICorDist(signed isadev){
        if (CheckClimbData(isadev)){
            return dClimbData.at(GetClimbDataIndex(isadev)).AICorDist();
		}
		else{
            return InterpolAICorDist(isadev);
		}
	};
	
        double GetAICorFuel(signed isadev){
        if (CheckClimbData(isadev)){
            return dClimbData.at(GetClimbDataIndex(isadev)).AICorFuel();
		}
		else{
            return InterpolAICorFuel(isadev);
		}
	};
	
    // sucht die nächst-kleinere ISADev in ClimbData und gibt sie zurück
    int LowerISADev(signed isadev){
        int result = MinISADev();
        for (unsigned i = 0; i < dClimbData.size(); i++){
            if ((dClimbData.at(i).ISADev() <= isadev) && (result <= dClimbData.at(i).ISADev())){
                result = dClimbData.at(i).ISADev();
				}
		}	
	return result;
	};

    // sucht die nächst-grössere ISADev in ClimbData und gibt sie zurück
    int UpperISADev(signed isadev){
        int result = MaxISADev();
        for (unsigned i = 0; i < dClimbData.size(); i++){
            if ((dClimbData.at(i).ISADev() >= isadev) && (result >= dClimbData.at(i).ISADev())){
                result = dClimbData.at(i).ISADev();
				}
		}
		return result;
	};
	
	//Sucht die grösste vorhandene ISADev und gibt sie zurück
    int MaxISADev(){
        int result = 0;
        for (unsigned i = 0; i < dClimbData.size(); i++){
            if (dClimbData.at(i).ISADev() > result) result = dClimbData.at(i).ISADev();
		}
	return result;
	};

	//Sucht die kleinste vorhandene ISADev und gibt sie zurück
    int MinISADev(){
        int result = MaxISADev();
        for (unsigned i = 0; i < dClimbData.size(); i++){
            if (dClimbData.at(i).ISADev() < result) result = dClimbData.at(i).ISADev();
		}
	return result;
	};
	
	bool IsInRange(signed isadev){
		return (!(((isadev < MinISADev()) || (isadev > MaxISADev()))));
	};
	
	// Erstellt einen Interpolierten DatenPunkt zum gewählten FL
    TClimbDataPoint InterpolClimbDataPoint(signed isadev, unsigned mass, unsigned fl){
		if (!(IsInRange(isadev))){
			TClimbDataPoint tmp(0, 0, 0, 0, 0, 0);
			return tmp;
		}
		TClimbDataPoint lower = GetClimbDataPoint(LowerISADev(isadev), mass, fl);
		TClimbDataPoint upper = GetClimbDataPoint(UpperISADev(isadev), mass, fl);
        unsigned kias = InterpolateLinear1D(lower.KIAS(), upper.KIAS(), LowerISADev(isadev), UpperISADev(isadev), isadev);
		double trq = InterpolateLinear1D(lower.TRQ(), upper.TRQ(), LowerISADev(isadev), UpperISADev(isadev), isadev);
		double time = InterpolateLinear1D(lower.Time(), upper.Time(), LowerISADev(isadev), UpperISADev(isadev), isadev);
		double dist = InterpolateLinear1D(lower.Dist(), upper.Dist(), LowerISADev(isadev), UpperISADev(isadev), isadev);
		double fuel = InterpolateLinear1D(lower.Fuel(), upper.Fuel(), LowerISADev(isadev), UpperISADev(isadev), isadev);
		TClimbDataPoint tmp(fl, kias, trq, time, dist, fuel);
		return tmp;
	};
	
	//Korrekturfaktoren für AntiIce interpolieren
    double InterpolAICorTime(signed isadev){
		if (!(IsInRange(isadev))) return 0;
        int lowerisadev = LowerISADev(isadev);
        int upperisadev = UpperISADev(isadev);
        double lowercor = dClimbData.at(GetClimbDataIndex(lowerisadev)).AICorTime();
        double uppercor = dClimbData.at(GetClimbDataIndex(upperisadev)).AICorTime();
		return InterpolateLinear1D(lowercor, uppercor, lowerisadev, upperisadev, isadev);
	};
	
    double InterpolAICorDist(signed isadev){
		if (!(IsInRange(isadev))) return 0;
        int lowerisadev = LowerISADev(isadev);
        int upperisadev = UpperISADev(isadev);
        double lowercor = dClimbData.at(GetClimbDataIndex(lowerisadev)).AICorDist();
        double uppercor = dClimbData.at(GetClimbDataIndex(upperisadev)).AICorDist();
		return InterpolateLinear1D(lowercor, uppercor, lowerisadev, upperisadev, isadev);
	};
	
    double InterpolAICorFuel(signed isadev){
		if (!(IsInRange(isadev))) return 0;
        int lowerisadev = LowerISADev(isadev);
        int upperisadev = UpperISADev(isadev);
        double lowercor = dClimbData.at(GetClimbDataIndex(lowerisadev)).AICorFuel();
        double uppercor = dClimbData.at(GetClimbDataIndex(upperisadev)).AICorFuel();
		return InterpolateLinear1D(lowercor, uppercor, lowerisadev, upperisadev, isadev);	
	};
	
  public:
	
  TClimbTypeData(){};
	
  //Constructor mit init
    TClimbTypeData(unsigned climbtype){
		dClimbType = climbtype;
	};
	
	//Destructor
	~TClimbTypeData(void){};
	
	//Neuen Datenpunkt hinzufügen
    void ClimbData(signed isadev, double aicortime, double aicordist, double aicorfuel, unsigned mass, unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
        AddClimbDataPoint(isadev, aicortime, aicordist, aicorfuel, mass, fl, kias, trq, time, dist, fuel);
	};
	
	//Punkt für ISADev muss bereits bestehen
    void ClimbData(signed isadev, double aicortime, double aicordist, double aicorfuel, unsigned mass, TClimbDataPoint NewClimbDataPoint){
        AddClimbDataPoint(isadev, aicortime, aicordist, aicorfuel, mass, NewClimbDataPoint);
	};

	//Public Getter
    unsigned ClimbType(){
		return GetClimbType();
	};

	//Ganzen Datenpunkt zurückgeben	
    TClimbDataPoint ClimbData(signed isadev, unsigned mass, unsigned fl){
		return GetClimbDataPoint(isadev, mass, fl);
	};

	bool IsValid(signed isadev){
		return IsInRange(isadev);
	};
	
    bool IsValid(signed isadev, unsigned mass){    	
    	if (!(IsValid(isadev))) return false;
        return (dClimbData.at(GetClimbDataIndex(LowerISADev(isadev))).IsValid(mass) && dClimbData.at(GetClimbDataIndex(UpperISADev(isadev))).IsValid(mass));
	};
		
	bool IsValid(signed isadev, unsigned mass, unsigned fl){
		if (!(IsValid(isadev))) return false;
        return (dClimbData.at(GetClimbDataIndex(LowerISADev(isadev))).IsValid(mass, fl) && dClimbData.at(GetClimbDataIndex(UpperISADev(isadev))).IsValid(mass, fl));
	};

	
	//Korrekturfaktoren für AntiIce
    double AICorTime(signed isadev){
        return GetAICorTime(isadev);
	};
	
    double AICorDist(signed isadev){
        return GetAICorDist(isadev);
	};
    double AICorFuel(signed isadev){
        return GetAICorFuel(isadev);
	};
		
	//Einzelne Werte zurückgeben
    unsigned KIAS(signed isadev, unsigned mass, unsigned fl){
		return GetClimbDataPoint(isadev, mass, fl).KIAS();
	};

    double TRQ(signed isadev, unsigned mass, unsigned fl){
		return GetClimbDataPoint(isadev, mass, fl).TRQ();
	};
	
    double Time(signed isadev, unsigned mass, unsigned fl){
		return GetClimbDataPoint(isadev, mass, fl).Time();
	};

    double Dist(signed isadev, unsigned mass, unsigned fl){
		return GetClimbDataPoint(isadev, mass, fl).Dist();
	};

    double Fuel(signed isadev, unsigned mass, unsigned fl){
		return GetClimbDataPoint(isadev, mass, fl).Fuel();
	};
};