#include <iostream>
#include <vector>
#include "../Misc/Interpol.h"
#include "ClimbDataPoint.h"

/**
 * @brief The TClimbMassData class
 * On layer above TClimbDataPoint.
 * Stores a vector of TClimbDataPoints as well as an int dMass and features several functions to
 * get and set them. All get-requests targeting ClimbDataPoint-Members are basically
 * being "routet through" and simply return the return value
 */

class TClimbMassData
{
private:
    unsigned dMass;
    std::vector < TClimbDataPoint > dClimbData;

    unsigned GetMass(){
        return dMass;
    };

    //Neuen Datenpunkt hinzufügen mit einzelnen Parametern
    void AddClimbDataPoint(unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
        TClimbDataPoint tmp(fl, kias, trq, time, dist, fuel);
        AddClimbDataPoint(tmp);
    };

    //Neuen Datenpunkt hinzufügen
    void AddClimbDataPoint(TClimbDataPoint NewClimbDataPoint){
        dClimbData.push_back(NewClimbDataPoint);
    };

    //Index des Datepunktes mit FL zurückgeben
    unsigned GetClimbDataIndex(unsigned fl){
        unsigned i;
        for (i = 0; i < dClimbData.size(); i++){
            if (dClimbData.at(i).FL() == fl) break;
        }
        return i;
    };

    // Check ob Datenpunkt für FL existiert
    bool CheckClimbDataPoint(unsigned fl){
        if (GetClimbDataIndex(fl) < dClimbData.size()){
            return true;
        }
        else{
            return false;
        }
    };

    // Datapoint mit FL zurückgeben
    TClimbDataPoint GetClimbDataPoint(unsigned fl){
        if (CheckClimbDataPoint(fl)) return dClimbData.at(GetClimbDataIndex(fl));
        else return InterpolClimbData(fl);
    };

    // sucht das nächst-kleinere FL in ClimbData und gibt es zurück
    unsigned LowerFL(unsigned fl){
        unsigned result = MinFL();
        for (unsigned i = 0; i < dClimbData.size(); i++){
            if ((dClimbData.at(i).FL() <= fl) && (result < dClimbData.at(i).FL())){
                result = dClimbData.at(i).FL();
            }
        }
        return result;
    };

    // sucht das nächst-grössere FL in ClimbData und gibt es zurück
    unsigned UpperFL(unsigned fl){
        unsigned result = MaxFL();
        for (unsigned i = 0; i < dClimbData.size(); i++){
            if ((dClimbData.at(i).FL() >= fl) && (result > dClimbData.at(i).FL())){
                result = dClimbData.at(i).FL();
            }
        }
        return result;
    };

    //Sucht das grösste vorhandene  FL und gibt es zurück
    unsigned MaxFL(){
        unsigned result = 0;
        for (unsigned i = 0; i < dClimbData.size(); i++){
            if (dClimbData.at(i).FL() > result) result = dClimbData.at(i).FL();
        }
        return result;
    };

    //Sucht das kleinste vorhandene  FL und gibt es zurück
    unsigned MinFL(){
        unsigned result = MaxFL();
        for (unsigned i = 0; i < dClimbData.size(); i++){
            if (dClimbData.at(i).FL() < result) result = dClimbData.at(i).FL();
        }
        return result;
    };
    
    //Kann der Punkt interpoliert werden?
    bool IsInRange(unsigned fl){
        return (!(((fl < MinFL()) || (fl > MaxFL()))));
    };
    
    // Erstellt einen Interpolierten DatenPunkt zum gewählten FL
    TClimbDataPoint InterpolClimbData(unsigned fl){
        if (!(IsInRange(fl))){
            TClimbDataPoint tmp(0, 0, 0, 0, 0, 0);
            return tmp;
        }
        TClimbDataPoint lower = GetClimbDataPoint(LowerFL(fl));
        TClimbDataPoint upper = GetClimbDataPoint(UpperFL(fl));
        unsigned kias = InterpolateLinear1D(lower.KIAS(), upper.KIAS(), lower.FL(), upper.FL(), fl);
        double trq = InterpolateLinear1D(lower.TRQ(), upper.TRQ(), lower.FL(), upper.FL(), fl);
        double time = InterpolateLinear1D(lower.Time(), upper.Time(),lower.FL(), upper.FL(),  fl);
        double dist = InterpolateLinear1D(lower.Dist(), upper.Dist(),lower.FL(), upper.FL(),  fl);
        double fuel = InterpolateLinear1D(lower.Fuel(), upper.Fuel(),lower.FL(), upper.FL(),  fl);
        TClimbDataPoint tmp(fl, kias, trq, time, dist, fuel);
        return tmp;
    };

public:

    TClimbMassData(){};

    //Constructor mit init
    TClimbMassData(unsigned mass){
        dMass = mass;
    };

    //Destructor
    ~TClimbMassData(void){};

    //Neuen Datenpunkt hinzufügen
    void ClimbData(unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
        AddClimbDataPoint(fl, kias, trq, time, dist,fuel);
    };

    void ClimbData(TClimbDataPoint NewClimbDataPoint){
        AddClimbDataPoint(NewClimbDataPoint);
    };

    //Public Getter
    unsigned Mass(){
        return GetMass();
    };

    //Ganzen Datenpunkt zurückgeben
    TClimbDataPoint ClimbData(unsigned fl){
        return GetClimbDataPoint(fl);
    };
    
    bool IsValid(unsigned fl){
    	return IsInRange(fl);
    };
   
    //Einzelne Werte zurückgeben
    unsigned KIAS(unsigned fl){
        return GetClimbDataPoint(fl).KIAS();
    };

    double TRQ(unsigned fl){
        return GetClimbDataPoint(fl).TRQ();
    };

    double Time(unsigned fl){
        return GetClimbDataPoint(fl).Time();
    };

    double Dist(unsigned fl){
        return GetClimbDataPoint(fl).Dist();
    };

    double Fuel(unsigned fl){
        return GetClimbDataPoint(fl).Fuel();
    };
};