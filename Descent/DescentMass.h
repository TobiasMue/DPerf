#include <vector>
#include "../Misc/Interpol.h"
#include "DescentDataPoint.h"

/**
 * @brief The TDescentMassData class
 * On layer above TDescentDataPoint.
 * Stores a vector of TDescentDataPoints as well as an int dMass and features several functions to
 * get and set them. All get-requests targeting DescentDataPoint-Members are basically
 * being "routet through" and sinply return the return value
 */

class TDescentMassData
{
private:
    unsigned dMass;
    std::vector < TDescentDataPoint > dDescentData;

    unsigned GetMass(){
        return dMass;};

    //Neuen Datenpunkt hinzufügen mit einzelnen Parametern
    void AddDescentDataPoint(unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
        TDescentDataPoint tmp(fl, kias, trq, time, dist, fuel);
        AddDescentDataPoint(tmp);
    };

    //Neuen Datenpunkt hinzufügen
    void AddDescentDataPoint(TDescentDataPoint NewDescentDataPoint){
        dDescentData.push_back(NewDescentDataPoint);
    };

    //Index des Datepunktes mit FL zurückgeben
    unsigned GetDescentDataIndex(unsigned fl){
        unsigned i;
        for (i = 0; i < dDescentData.size(); i++){
            if (dDescentData.at(i).FL() == fl) break;
        }
        return i;
    };

    // Check ob Datenpunkt für FL existiert
    bool CheckDescentDataPoint(unsigned fl){
        if (GetDescentDataIndex(fl) < dDescentData.size()){
            return true;
        }
        else{
            return false;
        }
    };

    // Datapoint mit FL zurückgeben
    TDescentDataPoint GetDescentDataPoint(unsigned fl){
        if (CheckDescentDataPoint(fl)) return dDescentData.at(GetDescentDataIndex(fl));
        else return InterpolDescentData(fl);
    };

    // sucht das nächst-kleinere FL in DescentData und gibt es zurück
    unsigned LowerFL(unsigned fl){
        unsigned result = MinFL();
        for (unsigned i = 0; i < dDescentData.size(); i++){
            if ((dDescentData.at(i).FL() <= fl) && (result < dDescentData.at(i).FL())){
                result = dDescentData.at(i).FL();
            }
        }
        return result;
    };

    // sucht das nächst-grössere FL in DescentData und gibt es zurück
    unsigned UpperFL(unsigned fl){
        unsigned result = MaxFL();
        for (unsigned i = 0; i < dDescentData.size(); i++){
            if ((dDescentData.at(i).FL() >= fl) && (result > dDescentData.at(i).FL())){
                result = dDescentData.at(i).FL();
            }
        }
        return result;
    };

    //Sucht das grösste vorhandene  FL und gibt es zurück
    unsigned MaxFL(){
        unsigned result = 0;
        for (unsigned i = 0; i < dDescentData.size(); i++){
            if (dDescentData.at(i).FL() > result) result = dDescentData.at(i).FL();
        }
        return result;
    };

    //Sucht das kleinste vorhandene  FL und gibt es zurück
    unsigned MinFL(){
        unsigned result = MaxFL();
        for (unsigned i = 0; i < dDescentData.size(); i++){
            if (dDescentData.at(i).FL() < result) result = dDescentData.at(i).FL();
        }
        return result;
    };
    
    bool IsInRange(unsigned fl){
    	return (!(((fl < MinFL()) || (fl > MaxFL()))));
    };
    	
    // Erstellt einen Interpolierten DatenPunkt zum gewählten FL
    TDescentDataPoint InterpolDescentData(unsigned fl){
        if (!(IsValid(fl))){
            TDescentDataPoint tmp(999, INVAL_KIAS, INVAL_TRQ, INVAL_TIME, INVAL_DIST, INVAL_FUEL);
            return tmp;
        }
        TDescentDataPoint lower = GetDescentDataPoint(LowerFL(fl));
        TDescentDataPoint upper = GetDescentDataPoint(UpperFL(fl));
        unsigned kias = InterpolateLinear1D(lower.KIAS(), upper.KIAS(), lower.FL(), upper.FL(), fl);
        double trq = InterpolateLinear1D(lower.TRQ(), upper.TRQ(), lower.FL(), upper.FL(), fl);
        double time = InterpolateLinear1D(lower.Time(), upper.Time(),lower.FL(), upper.FL(),  fl);
        double dist = InterpolateLinear1D(lower.Dist(), upper.Dist(),lower.FL(), upper.FL(),  fl);
        double fuel = InterpolateLinear1D(lower.Fuel(), upper.Fuel(),lower.FL(), upper.FL(),  fl);
        TDescentDataPoint tmp(fl, kias, trq, time, dist, fuel);
        return tmp;
    };

public:

    TDescentMassData(){};

    //Constructor mit init
    TDescentMassData(unsigned mass){
        dMass = mass;
    };

    //Destructor
    ~TDescentMassData(void){};

    //Neuen Datenpunkt hinzufügen
    void DescentData(unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
        AddDescentDataPoint(fl, kias, trq, time, dist,fuel);
    };

    void DescentData(TDescentDataPoint NewDescentDataPoint){
        AddDescentDataPoint(NewDescentDataPoint);
    };

    //Public Getter
    unsigned Mass(){
        return GetMass();
    };

    //Ganzen Datenpunkt zurückgeben
    TDescentDataPoint DescentData(unsigned fl){
        return GetDescentDataPoint(fl);
    };
    
    bool IsValid(unsigned fl){
    	return IsInRange(fl);
    };

    //Einzelne Werte zurückgeben
    int KIAS(unsigned fl){
        return GetDescentDataPoint(fl).KIAS();
    };

    double TRQ(unsigned fl){
        return GetDescentDataPoint(fl).TRQ();
    };

    double Time(unsigned fl){
        return GetDescentDataPoint(fl).Time();
    };

    double Dist(unsigned fl){
        return GetDescentDataPoint(fl).Dist();
    };

    double Fuel(unsigned fl){
        return GetDescentDataPoint(fl).Fuel();
    };
};