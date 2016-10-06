/**
 * @brief The TAccelDataPoint class
 * Central Data Container storing ONE Datapoint of the AOM-Table
 * Implements some Get and Set routines for each value as well as a constructor to init an instance
 * with all parameters set. In current project only Init-Constructor for one-time settup is being used
 * "inbound", all "Get" functions, linked by the simple name of the parameter is being used "outbound"
 */

#define INVAL_CRUISETYPE 0
#define INVAL_TIME 0
#define INVAL_DIST 0
#define INVAL_FUEL 0

class TAccelDataPoint{
private:
	unsigned dCruiseType;
    double dTime, dDist, dFuel;
    
    //Private Setter
	void SetCruiseType(unsigned cruisetype){
		dCruiseType = cruisetype;};
    void SetTime(double time){
        dTime = time;};
    void SetDist(double dist){
        dDist = dist;};
    void SetFuel(double fuel){
        dFuel = fuel;};

    //Private Getter
	unsigned GetCruiseType(void){
		return dCruiseType;};
    double GetTime(void){
        return dTime;};
    double GetDist(){
        return dDist;};
    double GetFuel(){
        return dFuel;};

public:

    //Public Setter
    void CruiseType(unsigned cruisetype){
    	SetCruiseType(cruisetype);};
    void Time(double time){
        SetTime(time);};
    void Dist(double dist){
        SetDist(dist);};
    void Fuel(double fuel){
        SetFuel(fuel);};

    //Public Getter
    unsigned  CruiseType(void){
    	return GetCruiseType();};
    double Time(void){
        return GetTime();};
    double Dist(){
        return GetDist();};
    double Fuel(){
        return GetFuel();};

    //Constructor
    TAccelDataPoint(){};

    //Init Constructor
    TAccelDataPoint(unsigned cruisetype, double time, double dist, double fuel){
    	dCruiseType = cruisetype;
        dTime = time;
        dDist = dist;
        dFuel = fuel;
    };

	//Constructor for invalid point
	TAccelDataPoint(bool invalid){
		if(!(invalid)){
	    	dCruiseType = INVAL_CRUISETYPE;
	        dTime = INVAL_TIME;
	        dDist = INVAL_DIST;
	        dFuel = INVAL_FUEL;
		}
    };
    
    //Destructor
    ~TAccelDataPoint(){};
}; 