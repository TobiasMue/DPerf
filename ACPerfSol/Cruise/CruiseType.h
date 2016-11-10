#include <iostream>
#include <vector>
#include "../Misc/Interpol.h"
#include "CruiseISADev.h"

class TCruiseTypeData
{
  private:
	unsigned dCruiseType;
      std::vector < TCruiseISADevData > dCruiseData;

	unsigned GetCruiseType(){
		return dCruiseType;
	};
	
	//Neuen Datenpunkt hinzufügen mit einzelnen Parametern
    void AddCruiseDataPoint(signed isadev, double aicortrq, double aicorff, unsigned mass, unsigned fl, unsigned kias, double trq, double ktas, double ff){
		TCruiseDataPoint tmp(fl, kias, trq, ktas, ff);
        AddCruiseDataPoint(isadev, aicortrq, aicorff, mass, tmp);
	};
	
	//Neuen Datenpunkt hinzufügen
    void AddCruiseDataPoint(signed isadev, double aicortrq, double aicorff, unsigned mass, TCruiseDataPoint NewCruiseDataPoint){
		//1. gibt es die masse schon? sonst erstellen. Dann Datenpunkt hinzufügen
        if (!(CheckCruiseData(isadev))){
            TCruiseISADevData tmp(isadev, aicortrq, aicorff);
            dCruiseData.push_back(tmp);
		}
        dCruiseData.at(GetCruiseDataIndex(isadev)).CruiseData(mass, NewCruiseDataPoint);
	};
	
	//Index des Datepunktes mit Masse zurückgeben
    unsigned GetCruiseDataIndex(signed isadev){
        unsigned i;
        for (i = 0; i < dCruiseData.size(); i++){
            if (dCruiseData.at(i).ISADev() == isadev) break;
		}
		return i;
	};

	// Check ob Datenpunkt für Masse existiert
    bool CheckCruiseData(signed isadev){
        if (GetCruiseDataIndex(isadev) < dCruiseData.size()) return true;
		else return false;
	};
			
	// Datapoint mit Mass und FL zurückgeben
    TCruiseDataPoint GetCruiseDataPoint(signed isadev, unsigned mass, unsigned fl){
        if (CheckCruiseData(isadev)){
            return dCruiseData.at(GetCruiseDataIndex(isadev)).CruiseData(mass, fl);
		}
		else{
			return InterpolCruiseDataPoint(isadev, mass, fl);
		}
	};
	
	//Correkturfatoren für AntiIce ausgeben oder interpolieren	
    double GetAICorTRQ(signed isadev){
        if (CheckCruiseData(isadev)){
            return dCruiseData.at(GetCruiseDataIndex(isadev)).AICorTRQ();
		}
		else{
            return InterpolAICorTRQ(isadev);
		}
	};
	
        double GetAICorFF(signed isadev){
        if (CheckCruiseData(isadev)){
            return dCruiseData.at(GetCruiseDataIndex(isadev)).AICorFF();
		}
		else{
            return InterpolAICorFF(isadev);
		}
	};
	
    // sucht die nächst-kleinere ISADev in CruiseData und gibt sie zurück
    signed LowerISADev(signed isadev){
        signed result = MinISADev();
        for (unsigned i = 0; i < dCruiseData.size(); i++){
            if ((dCruiseData.at(i).ISADev() <= isadev) && (result <= dCruiseData.at(i).ISADev())){
                result = dCruiseData.at(i).ISADev();
				}
		}	
	return result;
	};

    // sucht die nächst-grössere ISADev in CruiseData und gibt sie zurück
    signed UpperISADev(signed isadev){
        signed result = MaxISADev();
        for (unsigned i = 0; i < dCruiseData.size(); i++){
            if ((dCruiseData.at(i).ISADev() >= isadev) && (result >= dCruiseData.at(i).ISADev())){
                result = dCruiseData.at(i).ISADev();
				}
		}
		return result;
	};
	
	//Sucht die grösste vorhandene ISADev und gibt sie zurück
    signed MaxISADev(){
        signed result = 0;
        for (unsigned i = 0; i < dCruiseData.size(); i++){
            if (dCruiseData.at(i).ISADev() > result) result = dCruiseData.at(i).ISADev();
		}
	return result;
	};

	//Sucht die kleinste vorhandene ISADev und gibt sie zurück
    signed MinISADev(){
        signed result = MaxISADev();
        for (unsigned i = 0; i < dCruiseData.size(); i++){
            if (dCruiseData.at(i).ISADev() < result) result = dCruiseData.at(i).ISADev();
		}
	return result;
	};
	
	bool IsInRange(signed isadev){
		return (!(((isadev < MinISADev()) || (isadev > MaxISADev()))));
	};
	
	// Erstellt einen Interpolierten DatenPunkt zum gewählten FL
    TCruiseDataPoint InterpolCruiseDataPoint(signed isadev, unsigned mass, unsigned fl){
		if (!(IsValid(isadev))){
			TCruiseDataPoint tmp(INVAL_FL, INVAL_KIAS, INVAL_TRQ, INVAL_KTAS, INVAL_FF);
			return tmp;
		}
		TCruiseDataPoint lower = GetCruiseDataPoint(LowerISADev(isadev), mass, fl);
		TCruiseDataPoint upper = GetCruiseDataPoint(UpperISADev(isadev), mass, fl);
        unsigned kias = InterpolateLinear1D(lower.KIAS(), upper.KIAS(), LowerISADev(isadev), UpperISADev(isadev), isadev);
		double trq = InterpolateLinear1D(lower.TRQ(), upper.TRQ(), LowerISADev(isadev), UpperISADev(isadev), isadev);
		double ktas = InterpolateLinear1D(lower.KTAS(), upper.KTAS(), LowerISADev(isadev), UpperISADev(isadev), isadev);
		double ff = InterpolateLinear1D(lower.FF(), upper.FF(), LowerISADev(isadev), UpperISADev(isadev), isadev);
		TCruiseDataPoint tmp(fl, kias, trq, ktas, ff);
		return tmp;
	};
	
	//Korrekturfaktoren für AntiIce interpolieren
    double InterpolAICorTRQ(signed isadev){
		if (!(IsInRange(isadev))) return 0;
        signed lowerisadev = LowerISADev(isadev);
        signed upperisadev = UpperISADev(isadev);
        double lowercor = dCruiseData.at(GetCruiseDataIndex(lowerisadev)).AICorTRQ();
        double uppercor = dCruiseData.at(GetCruiseDataIndex(upperisadev)).AICorTRQ();
		return InterpolateLinear1D(lowercor, uppercor, lowerisadev, upperisadev, isadev);
	};
	
    double InterpolAICorFF(signed isadev){
		if (!(IsInRange(isadev))) return 0;
        signed lowerisadev = LowerISADev(isadev);
        signed upperisadev = UpperISADev(isadev);
        double lowercor = dCruiseData.at(GetCruiseDataIndex(lowerisadev)).AICorFF();
        double uppercor = dCruiseData.at(GetCruiseDataIndex(upperisadev)).AICorFF();
		return InterpolateLinear1D(lowercor, uppercor, lowerisadev, upperisadev, isadev);
	};
		
  public:
	
  TCruiseTypeData(){};
	
  //Constructor mit init
    TCruiseTypeData(unsigned climbtype){
		dCruiseType = climbtype;
	};
	
	//Destructor
	~TCruiseTypeData(void){};
	
	//Neuen Datenpunkt hinzufügen
    void CruiseData(signed isadev, double aicortrq, double aicorff, unsigned mass, unsigned fl, unsigned kias, double trq, double ktas, double ff){
        AddCruiseDataPoint(isadev, aicortrq, aicorff, mass, fl, kias, trq, ktas, ff);
	};
	
	//Punkt für ISADev muss bereits bestehen
    void CruiseData(signed isadev, double aicortrq, double aicorff, unsigned mass, TCruiseDataPoint NewCruiseDataPoint){
        AddCruiseDataPoint(isadev, aicortrq, aicorff, mass, NewCruiseDataPoint);
	};

	//Public Getter
    unsigned CruiseType(){
		return GetCruiseType();
	};

	//Ganzen Datenpunkt zurückgeben	
    TCruiseDataPoint CruiseData(signed isadev, unsigned mass, unsigned fl){
		return GetCruiseDataPoint(isadev, mass, fl);
	};

	bool IsValid(signed isadev){
		return IsInRange(isadev);	
	};	
	
   bool IsValid(signed isadev, unsigned mass){
   	if (!(IsValid(isadev))) return false;
   	return (dCruiseData.at(GetCruiseDataIndex(LowerISADev(isadev))).IsValid(mass) && dCruiseData.at(GetCruiseDataIndex(UpperISADev(isadev))).IsValid(mass));
	};
				
  bool IsValid(signed isadev, unsigned mass, unsigned fl){
  	   if (!(IsValid(isadev))) return false;
  	   return (dCruiseData.at(GetCruiseDataIndex(LowerISADev(isadev))).IsValid(mass, fl) && dCruiseData.at(GetCruiseDataIndex(UpperISADev(isadev))).IsValid(mass, fl));
	};
	
	//Korrekturfaktoren für AntiIce
    double AICorTRQ(signed isadev){
        return GetAICorTRQ(isadev);
	};
	
    double AICorFF(signed isadev){
        return GetAICorFF(isadev);
	};
	
	//Einzelne Werte zurückgeben
    unsigned KIAS(signed isadev, unsigned mass, unsigned fl){
		return GetCruiseDataPoint(isadev, mass, fl).KIAS();
	};

    double TRQ(signed isadev, unsigned mass, unsigned fl){
		return GetCruiseDataPoint(isadev, mass, fl).TRQ();
	};
	
    double KTAS(signed isadev, unsigned mass, unsigned fl){
		return GetCruiseDataPoint(isadev, mass, fl).KTAS();
	};

    double FF(signed isadev, unsigned mass, unsigned fl){
		return GetCruiseDataPoint(isadev, mass, fl).FF();
	};
};