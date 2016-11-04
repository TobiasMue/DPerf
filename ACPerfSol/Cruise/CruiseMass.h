#include <vector>
#include "../Misc/Interpol.h"
#include "CruiseDataPoint.h"

/**
 * @brief The TCruiseMassData class
 * On layer above TCruiseDataPoint.
 * Stores a vector of TCruiseDataPoints as well as an int dMass and features several functions to
 * get and set them. All get-requests targeting CruiseDataPoint-Members are basically
 * being "routet through" and sinply return the return value
 */

class TCruiseMassData
{
private:
    unsigned dMass;
    std::vector < TCruiseDataPoint > dCruiseData;

    unsigned GetMass(){
        return dMass;};

    //Neuen Datenpunkt hinzufügen mit einzelnen Parametern
    void AddCruiseDataPoint(unsigned fl, unsigned kias, double trq, double ktas, double ff){
        TCruiseDataPoint tmp(fl, kias, trq, ktas, ff);
        AddCruiseDataPoint(tmp);
    };

    //Neuen Datenpunkt hinzufügen
    void AddCruiseDataPoint(TCruiseDataPoint NewCruiseDataPoint){
        dCruiseData.push_back(NewCruiseDataPoint);
    };

    //Index des Datepunktes mit FL zurückgeben
    unsigned GetCruiseDataIndex(unsigned fl){
        unsigned i;
        for (i = 0; i < dCruiseData.size(); i++){
            if (dCruiseData.at(i).FL() == fl) break;
        }
        return i;
    };

    // Check ob Datenpunkt für FL existiert
    bool CheckCruiseDataPoint(unsigned fl){
        return (GetCruiseDataIndex(fl) < dCruiseData.size());
    };

    // Datapoint mit FL zurückgeben
    TCruiseDataPoint GetCruiseDataPoint(unsigned fl){
        if (CheckCruiseDataPoint(fl)) return dCruiseData.at(GetCruiseDataIndex(fl));
        else return InterpolCruiseData(fl);
    };

    // sucht das nächst-kleinere FL in CruiseData und gibt es zurück
    unsigned LowerFL(unsigned fl){
        unsigned result = MinFL();
        for (unsigned i = 0; i < dCruiseData.size(); i++){
            if ((dCruiseData.at(i).FL() <= fl) && (result < dCruiseData.at(i).FL())){
                result = dCruiseData.at(i).FL();
            }
        }
        return result;
    };

    // sucht das nächst-grössere FL in CruiseData und gibt es zurück
    unsigned UpperFL(unsigned fl){
        unsigned result = MaxFL();
        for (unsigned i = 0; i < dCruiseData.size(); i++){
            if ((dCruiseData.at(i).FL() >= fl) && (result > dCruiseData.at(i).FL())){
                result = dCruiseData.at(i).FL();
            }
        }
        return result;
    };

    //Sucht das grösste vorhandene  FL und gibt es zurück
    unsigned MaxFL(){
        unsigned result = 0;
        for (unsigned i = 0; i < dCruiseData.size(); i++){
            if (dCruiseData.at(i).FL() > result) result = dCruiseData.at(i).FL();
        }
        return result;
    };

    //Sucht das kleinste vorhandene  FL und gibt es zurück
    unsigned MinFL(){
        unsigned result = MaxFL();
        for (unsigned i = 0; i < dCruiseData.size(); i++){
            if (dCruiseData.at(i).FL() < result) result = dCruiseData.at(i).FL();
        }
        return result;
    };
    
    bool IsInRange(unsigned fl){
    	return (!((fl < MinFL()) || (fl > MaxFL())));
    };
    
    // Erstellt einen Interpolierten DatenPunkt zum gewählten FL
    TCruiseDataPoint InterpolCruiseData(unsigned fl){
        if (!(IsValid(fl))){
            TCruiseDataPoint tmp(INVAL_FL, INVAL_KIAS, INVAL_TRQ, INVAL_KTAS, INVAL_FF);
            return tmp;
        }
        TCruiseDataPoint lower = GetCruiseDataPoint(LowerFL(fl));
        TCruiseDataPoint upper = GetCruiseDataPoint(UpperFL(fl));
        unsigned kias = InterpolateLinear1D(lower.KIAS(), upper.KIAS(), lower.FL(), upper.FL(), fl);
        double trq = InterpolateLinear1D(lower.TRQ(), upper.TRQ(), lower.FL(), upper.FL(), fl);
        double ktas = InterpolateLinear1D(lower.KTAS(), upper.KTAS(),lower.FL(), upper.FL(),  fl);
        double ff = InterpolateLinear1D(lower.FF(), upper.FF(),lower.FL(), upper.FL(),  fl);
        TCruiseDataPoint tmp(fl, kias, trq, ktas, ff);
        return tmp;
    };

public:

    TCruiseMassData(){};

    //Constructor mit init
    TCruiseMassData(unsigned mass){
        dMass = mass;
    };

    //Destructor
    ~TCruiseMassData(void){};

    //Neuen Datenpunkt hinzufügen
    void CruiseData(unsigned fl, unsigned kias, double trq, double ktas, double ff){
        AddCruiseDataPoint(fl, kias, trq, ktas, ff);
    };

    void CruiseData(TCruiseDataPoint NewCruiseDataPoint){
        AddCruiseDataPoint(NewCruiseDataPoint);
    };

    //Public Getter
    unsigned Mass(){
        return GetMass();
    };

    //Ganzen Datenpunkt zurückgeben
    TCruiseDataPoint CruiseData(unsigned fl){
        return GetCruiseDataPoint(fl);
    };
    
    //Wird interpolliert, daher nur schauen ob innnerhalb min - max
    bool IsValid(unsigned fl){
    	return IsInRange(fl);
    };

    //Einzelne Werte zurückgeben
    int KIAS(unsigned fl){
        return GetCruiseDataPoint(fl).KIAS();
    };

    double TRQ(unsigned fl){
        return GetCruiseDataPoint(fl).TRQ();
    };

    double KTAS(unsigned fl){
        return GetCruiseDataPoint(fl).KTAS();
    };

    double FF(unsigned fl){
        return GetCruiseDataPoint(fl).FF();
    };
};