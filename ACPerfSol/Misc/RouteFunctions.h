#ifndef RouteFunction
#define RouteFunctions

//Total distance - Cimb, Accel, Descent
double GetCruiseGndDist(double totalgnddist, double climbgnddist, double accelgnddist, double descentgnddist){
	return totalgnddist - (climbgnddist + accelgnddist + descentgnddist);
}

//Airdist from Grounddist TAS, Wind
double GetAirDist(double gnddist, unsigned cruisetas, signed windcomp ){
	return gnddist / (cruisetas + windcomp) * cruisetas;
}

//GroundDist from Airdist, TAS, Wind
double GetGndDist(double airdist, unsigned tas, signed windcomp){
	return airdist / tas * (tas + windcomp);
}

//TAS, time in Minutes, returns KTAS
unsigned GetTAS(double dist, double time){
	return (dist / time) * 60;
}

//Windcompensated
double GetCruiseGndDist(double totalgnddist, double climbairdist, unsigned climbtas, signed climbheadwind, double accelairdist, unsigned acceltas, signed accelheadwind, double descentairdist, unsigned descenttas, signed descentheadwind){
	double climbgnddist = GetGndDist(climbairdist, climbtas, climbheadwind);
	double accelgnddist = GetGndDist(accelairdist, acceltas, accelheadwind);
	double descentgnddist = GetGndDist(descentairdist, descenttas, descentheadwind);
	return GetCruiseGndDist(totalgnddist, climbgnddist, accelgnddist, descentgnddist);
} 

//Time in Minutes
double GetCruiseTime(double cruiseairdist, unsigned cruisetas ){
	return (cruiseairdist / cruisetas) * 60;
}

//Time in Minutes, Fuelflow in kg/hr
double GetCruiseFuel(double cruiseff, double cruisetime){
	return (cruiseff / 60) * cruisetime;
}

//Sum up
double GetTotalTime(double climbtime, double acceltime, double cruisetime, double descenttime){
	return (climbtime + acceltime + cruisetime + descenttime);
}

//Sum up
double GetTotalFuel(double climbfuel, double accelfuel, double cruisefuel, double descentfuel){
	return (climbfuel + accelfuel + cruisefuel + descentfuel);
}

#endif