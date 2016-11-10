/**
 * @brief The TDescentDataPoint class
 * Central Data Container storing ONE Datapoint of the AOM-Table
 * Implements some Get and Set routines for each value as well as a constructor to init an instance
 * with all parameters set. In current project only Init-Constructor for one-time settup is being used
 * "inbound", all "Get" functions, linked by the simple name of the parameter is being used "outbound"
 */
 
 #define INVAL_KIAS 0
 #define INVAL_TRQ 0
 #define INVAL_TIME 0
 #define INVAL_DIST 0
 #define INVAL_FUEL 0

class TDescentDataPoint{
private:
    unsigned dFL, dKIAS;
    double dTime, dDist, dFuel, dTRQ;

    //Private Setter
    void SetFL(unsigned fl){
        dFL = fl;
    };
    void SetKIAS(unsigned kias){
        dKIAS = kias;
    };
    void SetTime(double time){
        dTime = time;
    };
    void SetDist(double dist){
        dDist = dist;
    };
    void SetFuel(double fuel){
        dFuel = fuel;
    };
    void SetTRQ(double trq){
        dTRQ = trq;
    };
    
    //Private Getter
    unsigned GetFL(){
        return dFL;
    };
    unsigned GetKIAS(){
        return dKIAS;
    };
    double GetTime(void){
        return dTime;
    };
    double GetDist(){
        return dDist;
    };
    double GetFuel(){
        return dFuel;
    };
    double GetTRQ(){
        return dTRQ;
    };

public:

    //Public Setter
    void FL(unsigned fl){
        SetFL(fl);
    };
    void KIAS(unsigned kias){
        SetKIAS(kias);
    };
    void Time(double time){
        SetTime(time);
    };
    void Dist(double dist){
        SetDist(dist);
    };
    void Fuel(double fuel){
        SetFuel(fuel);
    };
    void TRQ(double trq){
        SetTRQ(trq);
    };
    //Public Getter
    unsigned FL(){
        return GetFL();
    };
    unsigned KIAS(){
        return GetKIAS();
    };
    double Time(void){
        return GetTime();
    };
    double Dist(){
        return GetDist();
    };
    double Fuel(){
        return GetFuel();
    };
    double TRQ(){
        return GetTRQ();
    };
    //Constructor
    TDescentDataPoint(){};

    //Init Constructor
    TDescentDataPoint(unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
        dFL = fl;
        dKIAS = kias;
        dTime = time;
        dDist = dist;
        dFuel = fuel;
        dTRQ = trq;
    };

    //Destructor
    ~TDescentDataPoint(){};
}; 