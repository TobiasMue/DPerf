/**
 * @brief The TClimbDataPoint class
 * Central Data Container storing ONE Datapoint of the AOM-Table
 * Implements some Get and Set routines for each value as well as a constructor to init an instance
 * with all parameters set. In current project only Init-Constructor for one-time settup is being used
 * "inbound", all "Get" functions, linked by the simple name of the parameter is being used "outbound"
 */
 
/*
struct TClimbDataPoint{
    unsigned dKIAS;
    double dTRQ;
		double dTime;
		double dDist;
		double dFuel;	
}
*/ 

/*Würde dann TClimbFLData heissen*/
class TClimbDataPoint{
private:
/*
	unsigned dFL;
	TClimbDataPoint dClimbDataPoint;
*/
    unsigned dFL, dKIAS;
    double dTRQ, dTime, dDist, dFuel;

	/*Können weg, werden ja nie geändert*/
    //Private Setter
    void SetFL(unsigned fl){
        dFL = fl;};
    void SetKIAS(unsigned kias){
        dKIAS = kias;};
    void SetTRQ(double trq){
        dTRQ = trq;};
    void SetTime(double time){
        dTime = time;};
    void SetDist(double dist){
        dDist = dist;};
    void SetFuel(double fuel){
        dFuel = fuel;};

    //Private Getter
    unsigned GetFL(){
        return dFL;};
    unsigned GetKIAS(){
        return dKIAS;};
    double GetTRQ(){
        return dTRQ;};
    double GetTime(void){
        return dTime;};
    double GetDist(){
        return dDist;};
    double GetFuel(){
        return dFuel;};

public:
	/*Können auch weg*/
    //Public Setter
    void FL(unsigned fl){
        SetFL(fl);};
    void KIAS(unsigned kias){
        SetKIAS(kias);};
    void TRQ(double trq){
        SetTRQ(trq);};
    void Time(double time){
        SetTime(time);};
    void Dist(double dist){
        SetDist(dist);};
    void Fuel(double fuel){
        SetFuel(fuel);};

		/*Gäbe nur noch FL und ClimbDataPoint*/
    //Public Getter
    unsigned FL(){
        return GetFL();};
    double TRQ(){
        return GetTRQ();};
    unsigned KIAS(){
        return GetKIAS();};
    double Time(void){
        return GetTime();};
    double Dist(){
        return GetDist();};
    double Fuel(){
        return GetFuel();};

    //Constructor
    TClimbDataPoint(){};

    //Init Constructor
    TClimbDataPoint(unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
		/*dClimbDataPoint.kias = kias; ...*/
        dFL = fl;
        dKIAS = kias;
        dTRQ = trq;
        dTime = time;
        dDist = dist;
        dFuel = fuel;
    };

    //Destructor
    ~TClimbDataPoint(){};
}; 