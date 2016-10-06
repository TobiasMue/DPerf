/**
 * @brief The TCruiseDataPoint class
 * Central Data Container storing ONE Datapoint of the AOM-Table
 * Implements some Get and Set routines for each value as well as a constructor to init an instance
 * with all parameters set. In current project only Init-Constructor for one-ktas settup is being used
 * "inbound", all "Get" functions, linked by the simple name of the parameter is being used "outbound"
 */

#define INVAL_FL 0
#define INVAL_KIAS 0
#define INVAL_TRQ 0
#define INVAL_KTAS 0
#define INVAL_FF 0


class TCruiseDataPoint{
private:
    unsigned dFL, dKIAS, dKTAS;
    double dTRQ, dFF;

    //Private Setter
    void SetFL(unsigned fl){
        dFL = fl;
    };
    void SetKIAS(unsigned kias){
        dKIAS = kias;
    };
    void SetTRQ(double trq){
        dTRQ = trq;
    };
    void SetKTAS(double ktas){
        dKTAS = ktas;
    };
    void SetFF(double ff){
        dFF = ff;
    };

    //Private Getter
    unsigned GetFL(){
        return dFL;
    };
    unsigned GetKIAS(){
        return dKIAS;
    };
    double GetTRQ(){
        return dTRQ;
    };
    double GetKTAS(void){
        return dKTAS;
    };
    double GetFF(){
        return dFF;
    };

public:

    //Public Setter
    void FL(unsigned fl){
        SetFL(fl);
    };
    void KIAS(unsigned kias){
        SetKIAS(kias);
    };
    void TRQ(double trq){
        SetTRQ(trq);
    };
    void KTAS(double ktas){
        SetKTAS(ktas);
    };
    void FF(double ff){
        SetFF(ff);
    };
   
    //Public Getter
    unsigned FL(){
        return GetFL();
    };
    unsigned KIAS(){
        return GetKIAS();
    };
    double TRQ(){
        return GetTRQ();
    };
    unsigned KTAS(void){
        return GetKTAS();
    };
    double FF(){
        return GetFF();
    };
    
    //Constructor
    TCruiseDataPoint(){};

    //Init Constructor
    TCruiseDataPoint(unsigned fl, unsigned kias, double trq, unsigned ktas, double ff){
        dFL = fl;
        dKIAS = kias;
        dTRQ = trq;
        dKTAS = ktas;
        dFF = ff;
    };

    //Destructor
    ~TCruiseDataPoint(){};
}; 