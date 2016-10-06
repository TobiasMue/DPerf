#include <vector>
#include "../Misc/Interpol.h"
#include "AccelTOC.h"

/**
 * @brief The TAccelClimbTypeData class
 * On layer above TAccelDataPoint.
 * Stores a vector of TAccelDataPoints as well as an int dMass and features several functions to
 * get and set them. All get-requests targeting AccelDataPoint-Members are basically
 * being "routet through" and sinply return the return value
 */

class TAccelClimbTypeData
{
private:
    unsigned dClimbType;
    std::vector < TAccelTOCData > dAccelData;

    unsigned GetClimbType(){
        return dClimbType;
    };

    //Neuen Datenpunkt hinzufügen mit einzelnen Parametern
    void AddAccelDataPoint(unsigned toc, unsigned cruisetype, double time, double dist, double fuel){
    	TAccelDataPoint tmp(cruisetype, time, dist, fuel);
        AddAccelDataPoint(toc, tmp);
    };

    //Neuen Datenpunkt hinzufügen
    void AddAccelDataPoint(unsigned toc, TAccelDataPoint NewAccelDataPoint){
        if(!(CheckAccelData(toc))){
        	TAccelTOCData tmp(toc);
        	dAccelData.push_back(tmp);
        }
        dAccelData.at(GetAccelDataIndex(toc)).AccelData(NewAccelDataPoint);
    };

    //Index des Datepunktes mit TOC zurückgeben
    unsigned GetAccelDataIndex(unsigned toc){
        unsigned i;
        for (i = 0; i < dAccelData.size(); i++){
            if (dAccelData.at(i).TOC() == toc) break;
        }
        return i;
    };

    // Check ob Datenpunkt für TOC existiert
    bool CheckAccelData(unsigned toc){
        if (GetAccelDataIndex(toc) < dAccelData.size()){
            return true;
        }
        else{
            return false;
        }
    };

    // Datapoint mit TOC zurückgeben
    TAccelDataPoint GetAccelDataPoint(unsigned toc, unsigned cruisetype){
        if (CheckAccelData(toc)) return dAccelData.at(GetAccelDataIndex(toc)).ClimbData(cruisetype);
        else return InterpolAccelData(toc, cruisetype);
    };

    // sucht das nächst-kleinere TOC in AccelData und gibt es zurück
    unsigned LowerTOC(unsigned toc){
        unsigned result = MinTOC();
        for (unsigned i = 0; i < dAccelData.size(); i++){
            if ((dAccelData.at(i).TOC() <= toc) && (result < dAccelData.at(i).TOC())){
                result = dAccelData.at(i).TOC();
            }
        }
        return result;
    };

    // sucht das nächst-grössere TOC in AccelData und gibt es zurück
    unsigned UpperTOC(unsigned toc){
        unsigned result = MaxTOC();
        for (unsigned i = 0; i < dAccelData.size(); i++){
            if ((dAccelData.at(i).TOC() >= toc) && (result > dAccelData.at(i).TOC())){
                result = dAccelData.at(i).TOC();
            }
        }
        return result;
    };

    //Sucht das grösste vorhandene  TOC und gibt es zurück
    unsigned MaxTOC(){
        unsigned result = 0;
        for (unsigned i = 0; i < dAccelData.size(); i++){
            if (dAccelData.at(i).TOC() > result) result = dAccelData.at(i).TOC();
        }
        return result;
    };

    //Sucht das kleinste vorhandene  TOC und gibt es zurück
    unsigned MinTOC(){
        unsigned result = MaxTOC();
        for (unsigned i = 0; i < dAccelData.size(); i++){
            if (dAccelData.at(i).TOC() < result) result = dAccelData.at(i).TOC();
        }
        return result;
    };
    
    bool IsInRange(unsigned toc){
    	return (!(((toc < MinTOC()) || (toc > MaxTOC()))));
    };
    
    // Erstellt einen Interpolierten DatenPunkt zum gewählten TOC
    TAccelDataPoint InterpolAccelData(unsigned toc, unsigned cruisetype){
        if (!(IsValid(toc))){
            TAccelDataPoint tmp(INVAL_CRUISETYPE, INVAL_TIME, INVAL_DIST, INVAL_FUEL);
            return tmp;
        }
        TAccelDataPoint lower = GetAccelDataPoint(LowerTOC(toc), cruisetype);
        TAccelDataPoint upper = GetAccelDataPoint(UpperTOC(toc), cruisetype);
        double time = InterpolateLinear1D(lower.Time(), upper.Time(), LowerTOC(toc), UpperTOC(toc),  toc);
        double dist = InterpolateLinear1D(lower.Dist(), upper.Dist(), LowerTOC(toc), UpperTOC(toc),  toc);
        double fuel = InterpolateLinear1D(lower.Fuel(), upper.Fuel(), LowerTOC(toc), UpperTOC(toc),  toc);
        TAccelDataPoint tmp(cruisetype, time, dist, fuel);
        return tmp;
    };

public:

    TAccelClimbTypeData(){};

    //Constructor mit init
    TAccelClimbTypeData(unsigned climbtype){
        dClimbType = climbtype;
    };

    //Destructor
    ~TAccelClimbTypeData(void){};

    //Neuen Datenpunkt hinzufügen
    void AccelData(unsigned toc, unsigned cruisetype, double time, double dist, double fuel){
        AddAccelDataPoint(toc, cruisetype, time, dist,fuel);
    };

    void AccelData(unsigned toc, TAccelDataPoint NewAccelDataPoint){
        AddAccelDataPoint(toc, NewAccelDataPoint);
    };

    //Public Getter
    unsigned ClimbType(){
        return GetClimbType();
    };

    //Ganzen Datenpunkt zurückgeben
    TAccelDataPoint AccelData(unsigned toc, unsigned cruisetype){
        return GetAccelDataPoint(toc, cruisetype);
    };
   
    bool IsValid(unsigned toc){
    	return IsInRange(toc);
	};

	bool IsValid(unsigned toc, unsigned cruisetype){
		if (!(IsValid(toc))) return false;
        return (dAccelData.at(GetAccelDataIndex(LowerTOC(toc))).IsValid(cruisetype) && dAccelData.at(GetAccelDataIndex(UpperTOC(toc))).IsValid(cruisetype));
	};   

    //Einzelne Werte zurückgeben
    double Time(unsigned toc, unsigned cruisetype){
        return GetAccelDataPoint(toc, cruisetype).Time();
    };

    double Dist(unsigned toc, unsigned cruisetype){
        return GetAccelDataPoint(toc, cruisetype).Dist();
    };

    double Fuel(unsigned toc, unsigned cruisetype){
        return GetAccelDataPoint(toc, cruisetype).Fuel();
    };
};