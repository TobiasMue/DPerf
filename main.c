#include <iostream>
#include "Accel/Accel.h"
#include "Climb/Climb.h"
#include "Cruise/Cruise.h"
#include "Descent/Descent.h"
#include "Misc/RouteFunctions.h"
#include "Misc/Wind.h"
#include "Misc/ISA.h"


/**
* Main für GCC-Compiler. Nur zum testen ausserhalb von Qt
* Wie geht Cost Index? FlightTime  Cost/ FuelCost, ist das linear? 
* Man könnte einmal min time und einmal min fuel rechnen und dann je nach CI das oder die Perf.-Sets suchen die der Ratio entsprechen
* 0 ist min fuel, 999 ist min time. Ratio ist zwischen 0 und 1, daher müste CI15 heissen dass 15/1000 mehr fuel eingesetzt werden darf als min fuel?
* Bzw. dass (Mehr Zeit als min Time) / (Mehr Fuel als Min Fuel) = 0,015
*/

int main(){
	TClimbDataSet ClimbData(1);
	ClimbData.LoadData("/sdcard/QT Projects/movefiles/Data/Climbdata.dat", ';', '\n');
	std::cout << "Climb Loaded..." << std::endl;
	TAccelDataSet AccelData(1);
	AccelData.LoadData("/sdcard/QT Projects/movefiles/Data/Acceldata.dat", ';', '\n');
	std::cout << "Accel Loaded..." << std::endl;
	TCruiseDataSet CruiseData(1);
	CruiseData.LoadData("/sdcard/QT Projects/movefiles/Data/Cruisedata.dat", ';', '\n');
	std::cout << "Cruise Loaded..." << std::endl;
	TDescentDataSet DescentData(1);
	DescentData.LoadData("/sdcard/QT Projects/movefiles/Data/Descentdata.dat", ';', '\n');
	std::cout << "Descent Loaded..." << std::endl;

	TWindProfile ClimbWind;
		for(unsigned iFL = 0; iFL < 26; iFL += 2){
			ClimbWind.WindData(iFL * 10, (45 + 2*iFL)%360, 0 , 15 - iFL);
		}
	std::cout << "ClimbWind Loaded..." << std::endl;

	TWindProfile CruiseWind;
		for(unsigned iFL = 0; iFL < 25; iFL += 2){
			CruiseWind.WindData(iFL * 10, (45 + 2*iFL)%360, 0, 15 - iFL);
		}
	std::cout << "CruiseWind Loaded..." << std::endl;

	TWindProfile DescentWind;
		for(unsigned iFL = 0; iFL < 25; iFL += 2){
			DescentWind.WindData(iFL * 10, (45 + 2*iFL)%360, 0, 15 - iFL);
		}
	std::cout << "DescentWind Loaded..." << std::endl;
	
	unsigned fpdist = 150;
	std::cout << "fp Dist = " << fpdist << std::endl;
	unsigned avgtrack = 90;
	std::cout << "Av Track = " << avgtrack << std::endl;
	unsigned mass = 20500;
	std::cout << "Mass = " << mass << std::endl;
	unsigned CITarget = 1;
	std::cout << "Target Cost Index = " << CITarget << std::endl;
	/*Min Fuel*/
	unsigned ClimbTypeMinFuel = 0;
	unsigned CruiseTypeMinFuel = 0;
	unsigned DescentTypeMinFuel = 0;
	unsigned ClimbRPMMinFuel = 0;
	unsigned CruiseRPMMinFuel = 0;
	unsigned DescentRPMMinFuel = 0;
	unsigned FLMinFuel = 0;
	double TotalTimeMinFuel = 0;
	double ClbTimeMinFuel = 0;
	double AccelTimeMinFuel = 0;
	double CrsTimeMinFuel = 0;	
	double DescTimeMinFuel = 0;
	double TotalFuelMinFuel = 999999;
	double ClbFuelMinFuel = 0;
	double AccelFuelMinFuel = 0;
	double CrsFuelMinFuel = 0;
	double DescFuelMinFuel = 0;
	double TotalAirDistMinFuel = 0;
	/*Min Time*/
	unsigned ClimbTypeMinTime = 0;
	unsigned CruiseTypeMinTime = 0;
	unsigned DescentTypeMinTime = 0;
	unsigned ClimbRPMMinTime = 0;
	unsigned CruiseRPMMinTime = 0;
	unsigned DescentRPMMinTime = 0;
	unsigned FLMinTime = 0;
	double TotalFuelMinTime = 0;
	double ClbFuelMinTime = 0;
	double AccelFuelMinTime = 0;
	double CrsFuelMinTime = 0;
	double DescFuelMinTime = 0;
	double TotalTimeMinTime = 999999;
	double ClbTimeMinTime = 0;
	double AccelTimeMinTime = 0;
	double CrsTimeMinTime = 0;
	double DescTimeMinTime = 0;
	double TotalAirDistMinTime = 0;
	/*Cost Index*/
	unsigned ClimbTypeCI = 0;
	unsigned CruiseTypeCI = 0;
	unsigned DescentTypeCI = 0;
	unsigned ClimbRPMCI = 0;
	unsigned CruiseRPMCI = 0;
	unsigned DescentRPMCI = 0;
	unsigned FLCI = 0;
	double TotalFuelCI = 0;
	double ClbFuelCI = 0;
	double AccelFuelCI = 0;
	double CrsFuelCI = 0;
	double DescFuelCI = 0;
	double TotalTimeCI = 0;
	double ClbTimeCI = 0;
	double AccelTimeCI = 0;
	double CrsTimeCI = 0;
	double DescTimeCI = 0;
	double TotalAirDistCI = 0;
	
	double TotalAirDist = 0;
	double TotalFuel = 0;
	double TotalTime = 0;
	
	double DiffFuelToMinFuel = 0;
	double DiffTimeToMinTime = 0;
	double CIActual = 0;
	double CIAppr = 0;

	std::cout << "Running First Round" << std::endl;
	for (unsigned climbtype = 1; climbtype <= 5; climbtype++){
		for (unsigned cruisetype = 1; cruisetype < 5; cruisetype++){
			for (unsigned descenttype = 1; descenttype < 5; descenttype++){
				for (unsigned ClbRPM = 850; ClbRPM <= 900; ClbRPM += 50){
					for (unsigned CrsRPM = 850; CrsRPM <= 900; CrsRPM += 50){
						for (unsigned DescRPM = 850; DescRPM <= 900; DescRPM += 50){
							for (unsigned fl = 0; fl <= 250; fl+=10){   //Achtung, es gibt Datenpunkte für FL0 mit Time/Dist/Fuel 0/0/0, GL10 wird damit falsch interpoliert!!! Allerdings gibt es für Accel nur Fl von 100 - 250, daher wird alles unter FL100 sowieso verworfen
								/*******Climb*******/
								TWindDataPoint AvgClimbWind = ClimbWind.AvgWindData(0, fl, 20);
								signed clbisadev = ISADev(AvgClimbWind.FL, AvgClimbWind.OAT);
								signed ClbHeadWind = CalcHeadWindComp(avgtrack, AvgClimbWind.WindDir, AvgClimbWind.WindSpeed);
								if (!(ClimbData.IsValid(ClbRPM, climbtype, clbisadev, mass, fl))) continue;
								TClimbDataPoint ClbDataPoint = ClimbData.ClimbData(ClbRPM, climbtype, clbisadev, mass, fl);
								double ClbTime = ClimbData.Time(ClbRPM, climbtype, clbisadev, mass, fl);
								double ClbAirDist = ClimbData.Dist(ClbRPM, climbtype, clbisadev, mass, fl);
								unsigned ClbTas = ClbAirDist  / ClbTime * 60;
								double ClbGndDist = GetGndDist(ClbAirDist, ClbTas, ClbHeadWind);
								double ClbFuel = ClimbData.Fuel(ClbRPM, climbtype, clbisadev, mass, fl);
								/*********Accel**********/
								signed AccelHeadWind = CalcHeadWindComp(avgtrack, ClimbWind.WindDir(fl), ClimbWind.WindSpeed(fl));
								if (!(AccelData.IsValid(ClbRPM, climbtype, fl, cruisetype))) continue;
								double AccelTime = AccelData.Time(ClbRPM, climbtype, fl, cruisetype);
								double AccelAirDist = AccelData.Dist(ClbRPM, climbtype, fl, cruisetype);
								unsigned AccelAvgTas = AccelAirDist / AccelTime * 60;
								double AccelGndDist = GetGndDist(AccelAirDist, AccelAvgTas, AccelHeadWind);
								double AccelFuel = AccelData.Fuel(ClbRPM, climbtype, fl, cruisetype);			
								/*******Descent*******/
								TWindDataPoint AvgDescWind = DescentWind.AvgWindData(0, fl, 20);
								double descisadev = ISADev(AvgDescWind.FL, AvgDescWind.OAT);
								signed DescHeadWind = CalcHeadWindComp(avgtrack, AvgDescWind.WindDir, AvgDescWind.WindSpeed);
								unsigned DescMass = mass - (ClbFuel + AccelFuel + fpdist * 2); //Crs Fuel angenommen mit 200kg pro 100 nM 
								if (!(DescentData.IsValid(DescRPM, descenttype, descisadev, DescMass, fl))) continue;
								double DescAirDist = DescentData.Dist(DescRPM, descenttype, descisadev, DescMass, fl);
								double DescTime = DescentData.Time(DescRPM, descenttype, descisadev, DescMass, fl);
								unsigned DescTas = DescAirDist / DescTime * 60;
								double DescGndDist = GetGndDist(DescAirDist, DescTas, DescHeadWind);
								double DescFuel = DescentData.Fuel(DescRPM, descenttype, descisadev, mass, fl);
								/*********Cruise**********/
								unsigned CrsMass = mass - (ClbFuel + AccelFuel);
								signed CrsHeadWind = CalcHeadWindComp(avgtrack, CruiseWind.WindDir(fl), CruiseWind.WindSpeed(fl));								
								double CrsISADev = ISADev(fl, CruiseWind.OAT(fl));
								if (!(CruiseData.IsValid(cruisetype, CrsISADev, CrsMass, fl))) continue;
								unsigned CrsTAS = CruiseData.KTAS(cruisetype, CrsISADev, CrsMass, fl);
								unsigned CrsGndDist = fpdist - (ClbGndDist + AccelGndDist + DescGndDist);
								unsigned CrsAirDist = GetAirDist(CrsGndDist, CrsTAS, CrsHeadWind);
								double CrsTime = 60* CrsAirDist / CrsTAS;
								double CrsFF = CruiseData.FF(cruisetype,  CrsISADev, CrsMass, fl);
								double CrsFuel = (CrsFF * CrsTime) / 60;

								
								TotalTime = ClbTime + AccelTime + CrsTime + DescTime;
								TotalFuel = ClbFuel + AccelFuel + CrsFuel + DescFuel;
								TotalAirDist = ClbAirDist + AccelAirDist + CrsAirDist + DescAirDist;
								
								if (TotalTime < TotalTimeMinTime){
									TotalTimeMinTime = TotalTime;
									ClbTimeMinTime = ClbTime;
									AccelTimeMinTime = AccelTime;
									CrsTimeMinTime = CrsTime;	
									DescTimeMinTime = DescTime;			
									TotalAirDistMinTime = TotalAirDist;
									TotalFuelMinTime = TotalFuel;
									ClbFuelMinTime = ClbFuel;
									AccelFuelMinTime = AccelFuel;
									CrsFuelMinTime = CrsFuel;	
									DescFuelMinTime = DescFuel;
									ClimbTypeMinTime = climbtype;
									CruiseTypeMinTime = cruisetype;
									DescentTypeMinTime = descenttype;
									ClimbRPMMinTime = ClbRPM;
									CruiseRPMMinTime = CrsRPM;
									DescentRPMMinTime = DescRPM;
									FLMinTime = fl;
								}
								if (TotalFuel < TotalFuelMinFuel){
									TotalFuelMinFuel = TotalFuel;
									ClbFuelMinFuel = ClbFuel;
									AccelFuelMinFuel = AccelFuel;
									CrsFuelMinFuel = CrsFuel;	
									DescFuelMinFuel = DescFuel;
									TotalTimeMinFuel = TotalTime;
									ClbTimeMinFuel = ClbTime;
									AccelTimeMinFuel = AccelTime;
									CrsTimeMinFuel = CrsTime;	
									DescTimeMinFuel = DescTime;			
									TotalAirDistMinFuel = TotalAirDist;
									ClimbTypeMinFuel = climbtype;
									CruiseTypeMinFuel = cruisetype;
									DescentTypeMinFuel = descenttype;
									ClimbRPMMinFuel = ClbRPM;
									CruiseRPMMinFuel = CrsRPM;
									DescentRPMMinFuel = DescRPM;
									FLMinFuel = fl;								
								} 
							}
						}
					}
				}
			}
		}
	}
	std::cout  << std::endl;
	std::cout << "*************Summary****************" << std::endl  << std::endl;
	std::cout << "*************Min Time****************" << std::endl  << std::endl;
	std::cout << " * General | Cruise FL: " << FLMinTime << " | Total Air Dist: " << TotalAirDistMinTime  << " nM" << std::endl 
	<< " * Times | Total Time: " << TotalTimeMinTime << " min"
	<< " | Climb Time: " << ClbTimeMinTime << " min"
	<< " | Accel Time: " << AccelTimeMinTime << " min"
	<< " | Cruise Time: " << CrsTimeMinTime << " min"
	<< " | Descent Time: " << DescTimeMinTime << " min"<< std::endl	
	<< " * Fuel | Total Fuel: " << TotalFuelMinTime << " kg"
	<< " | Climb Fuel: " << ClbFuelMinTime << " kg"
	<< " | Accel Fuel: " << AccelFuelMinTime << " kg"
	<< " | Cruise Fuel: " << CrsFuelMinTime << " kg"
	<< " | Descent Fuel: " << DescFuelMinTime << " kg" << std::endl    	
	<< " * Perf. Types | ClimbType: " << ClimbTypeMinTime 
	<< " | Cruise Type: " <<  CruiseTypeMinTime
	<< " | Descent Type: " << DescentTypeMinTime << std::endl 
	<< " * RPM | Climb RPM: " << ClimbRPMMinTime
	<< " | Cruise RPM: " << CruiseRPMMinTime 
	<< " | Descent RPM: " << DescentRPMMinTime 
	<< std::endl  << std::endl;
	std::cout << "*************Min Fuel****************" << std::endl  << std::endl;
	std::cout << " * General | Cruise FL: " << FLMinFuel << " | Total Air Dist: " << TotalAirDistMinFuel << " nM" << std::endl 
	<< " * Times | Total Time: " << TotalTimeMinFuel << " min"
	<< " | Climb Time: " << ClbTimeMinFuel << " min"
	<< " | Accel Time: " << AccelTimeMinFuel << " min"
	<< " | Cruise Time: " << CrsTimeMinFuel << " min"
	<< " | Descent Time: " << DescTimeMinFuel << " min" << std::endl
	<< " * Fuel | Total Fuel: " << TotalFuelMinFuel << " kg"
	<< " | Climb Fuel: " << ClbFuelMinFuel << " kg" 
	<< " | Accel Fuel: " << AccelFuelMinFuel << " kg" 
	<< " | Cruise Fuel: " << CrsFuelMinFuel << " kg" 
	<< " | Descent Fuel: " << DescFuelMinFuel  << " kg" << std::endl
	<< " * Perf. Types | ClimbType: " << ClimbTypeMinFuel 
	<< " | Cruise Type: " <<  CruiseTypeMinFuel
	<< " | Descent Type: " << DescentTypeMinFuel << std::endl 
	<< " * RPM | Climb RPM: " << ClimbRPMMinFuel 
	<< " | Cruise RPM: " << CruiseRPMMinFuel
	<< " | Descent RPM: " << DescentRPMMinFuel
	<< std::endl << std::endl;;
	std::cout << "*************Differences****************" << std::endl  << std::endl;
	std::cout << " * Time: " << (TotalTimeMinFuel - TotalTimeMinTime) << " min" << std::endl
	<< " * Fuel: " << (TotalFuelMinTime - TotalFuelMinFuel) << " kg" << std::endl;

	std::cout << "Running Second Round" << std::endl;
	for (unsigned climbtype = 1; climbtype <= 3; climbtype++){
		for (unsigned cruisetype = 1; cruisetype < 4; cruisetype++){
			for (unsigned descenttype = 1; descenttype < 3; descenttype++){
				for (unsigned ClbRPM = 850; ClbRPM <= 900; ClbRPM += 50){
					for (unsigned CrsRPM = 850; CrsRPM <= 900; CrsRPM += 50){
						for (unsigned DescRPM = 850; DescRPM <= 900; DescRPM += 50){
							for (unsigned fl = 0; fl <= 250; fl+=10){   //Achtung, es gibt Datenpunkte für FL0 mit Time/Dist/Fuel 0/0/0, GL10 wird damit falsch interpoliert!!! Allerdings gibt es für Accel nur Fl von 100 - 250, daher wird alles unter FL100 sowieso verworfen
								/*******Climb*******/
								TWindDataPoint AvgClimbWind = ClimbWind.AvgWindData(0, fl, 20);
								signed clbisadev = ISADev(AvgClimbWind.FL, AvgClimbWind.OAT);
								signed ClbHeadWind = CalcHeadWindComp(avgtrack, AvgClimbWind.WindDir, AvgClimbWind.WindSpeed);
								if (!(ClimbData.IsValid(ClbRPM, climbtype, clbisadev, mass, fl))) continue;
								TClimbDataPoint ClbDataPoint = ClimbData.ClimbData(ClbRPM, climbtype, clbisadev, mass, fl);
								double ClbTime = ClimbData.Time(ClbRPM, climbtype, clbisadev, mass, fl);
								double ClbAirDist = ClimbData.Dist(ClbRPM, climbtype, clbisadev, mass, fl);
								unsigned ClbTas = ClbAirDist  / ClbTime * 60;
								double ClbGndDist = GetGndDist(ClbAirDist, ClbTas, ClbHeadWind);
								double ClbFuel = ClimbData.Fuel(ClbRPM, climbtype, clbisadev, mass, fl);
								/*********Accel**********/
								signed AccelHeadWind = CalcHeadWindComp(avgtrack, ClimbWind.WindDir(fl), ClimbWind.WindSpeed(fl));
								if (!(AccelData.IsValid(ClbRPM, climbtype, fl, cruisetype))) continue;
								double AccelTime = AccelData.Time(ClbRPM, climbtype, fl, cruisetype);
								double AccelAirDist = AccelData.Dist(ClbRPM, climbtype, fl, cruisetype);
								unsigned AccelAvgTas = AccelAirDist / AccelTime * 60;
								double AccelGndDist = GetGndDist(AccelAirDist, AccelAvgTas, AccelHeadWind);
								double AccelFuel = AccelData.Fuel(ClbRPM, climbtype, fl, cruisetype);			
								/*******Descent*******/
								TWindDataPoint AvgDescWind = DescentWind.AvgWindData(0, fl, 20);
								double descisadev = ISADev(AvgDescWind.FL, AvgDescWind.OAT);
								signed DescHeadWind = CalcHeadWindComp(avgtrack, AvgDescWind.WindDir, AvgDescWind.WindSpeed);
								unsigned DescMass = mass - (ClbFuel + AccelFuel + fpdist * 2); //Crs Fuel angenommen mit 200kg pro 100 nM 
								if (!(DescentData.IsValid(DescRPM, descenttype, descisadev, DescMass, fl))) continue;
								double DescAirDist = DescentData.Dist(DescRPM, descenttype, descisadev, DescMass, fl);
								double DescTime = DescentData.Time(DescRPM, descenttype, descisadev, DescMass, fl);
								unsigned DescTas = DescAirDist / DescTime * 60;
								double DescGndDist = GetGndDist(DescAirDist, DescTas, DescHeadWind);
								double DescFuel = DescentData.Fuel(DescRPM, descenttype, descisadev, mass, fl);
								/*********Cruise**********/
								unsigned CrsMass = mass - (ClbFuel + AccelFuel);
								signed CrsHeadWind = CalcHeadWindComp(avgtrack, CruiseWind.WindDir(fl), CruiseWind.WindSpeed(fl));								
								double CrsISADev = ISADev(fl, CruiseWind.OAT(fl));
								if (!(CruiseData.IsValid(cruisetype, CrsISADev, CrsMass, fl))) continue;
								unsigned CrsTAS = CruiseData.KTAS(cruisetype, CrsISADev, CrsMass, fl);
								unsigned CrsGndDist = fpdist - (ClbGndDist + AccelGndDist + DescGndDist);
								unsigned CrsAirDist = GetAirDist(CrsGndDist, CrsTAS, CrsHeadWind);
								double CrsTime = 60* CrsAirDist / CrsTAS;
								double CrsFF = CruiseData.FF(cruisetype,  CrsISADev, CrsMass, fl);
								double CrsFuel = (CrsFF * CrsTime) / 60;
								
								TotalTime = ClbTime + AccelTime + CrsTime + DescTime;
								DiffTimeToMinTime = TotalTime - TotalTimeMinTime;
								TotalFuel = ClbFuel + AccelFuel + CrsFuel + DescFuel;
								DiffFuelToMinFuel = TotalFuel - TotalFuelMinFuel;
								TotalAirDist = ClbAirDist + AccelAirDist + CrsAirDist + DescAirDist;
								if (DiffFuelToMinFuel == 0) CIActual = 0;
								else CIActual = 1000 * (DiffTimeToMinTime) / (DiffFuelToMinFuel);
								
								if (abs(CIActual - CITarget) < abs(CIAppr - CITarget)){
									CIAppr = CIActual;
									TotalTimeCI = TotalTime;
									ClbTimeCI = ClbTime;
									AccelTimeCI = AccelTime;
									CrsTimeCI = CrsTime;	
									DescTimeCI = DescTime;			
									TotalAirDistCI = TotalAirDist;
									TotalFuelCI = TotalFuel;
									ClbFuelCI = ClbFuel;
									AccelFuelCI = AccelFuel;
									CrsFuelCI = CrsFuel;	
									DescFuelCI = DescFuel;
									ClimbTypeCI = climbtype;
									CruiseTypeCI = cruisetype;
									DescentTypeCI = descenttype;
									ClimbRPMCI = ClbRPM;
									CruiseRPMCI = CrsRPM;
									DescentRPMCI = DescRPM;
									FLCI = fl;
								}
							}
						}
					}
				}
			}
		}
	}
	std::cout << "*************Cost Index****************" << std::endl  << std::endl;
	std::cout << " * General | Cruise FL: " << FLCI << " | Actual Cost Index: " << CIAppr << " | Total Air Dist: " << TotalAirDistCI << " nM" << std::endl
	<< " * Times | Total Time: " << TotalTimeCI << " min"
	<< " | Climb Time: " << ClbTimeCI << " min"
	<< " | Accel Time: " << AccelTimeCI << " min"
	<< " | Cruise Time: " << CrsTimeCI << " min"
	<< " | Descent Time: " << DescTimeCI << " min" << std::endl
	<< " * Fuel | Total Fuel: " << TotalFuelCI << " kg"
	<< " | Climb Fuel: " << ClbFuelCI << " kg" 
	<< " | Accel Fuel: " << AccelFuelCI << " kg" 
	<< " | Cruise Fuel: " << CrsFuelCI << " kg" 
	<< " | Descent Fuel: " << DescFuelCI  << " kg" << std::endl
	<< " * Perf. Types | ClimbType: " << ClimbTypeCI 
	<< " | Cruise Type: " <<  CruiseTypeCI
	<< " | Descent Type: " << DescentTypeCI << std::endl 
	<< " * RPM | Climb RPM: " << ClimbRPMCI 
	<< " | Cruise RPM: " << CruiseRPMCI
	<< " | Descent RPM: " << DescentRPMCI
	<< std::endl << std::endl;;
	std::cout << "*************Differences****************" << std::endl  << std::endl;
	std::cout << " * Time: " << (TotalTimeCI - TotalTimeMinTime) << " min" << std::endl
	<< " * Fuel: " << (TotalFuelCI - TotalFuelMinFuel) << " kg" << std::endl;
}