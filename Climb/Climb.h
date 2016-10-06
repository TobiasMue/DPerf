#include <iostream>
#include <fstream>
#include <string>
#include "ClimbRPM.h"
#include "../Misc/ISA.h"

/**
 * @brief The TClimbDataSet class
 * Outer Shell containing all ClimbData.
 */
class TClimbDataSet{
	int dSource;
    std::vector < TClimbRPMData > dClimbData;

	private:
		int GetSource(){
			return dSource;
		};
		
		//Neuen Datenpunkt hinzufügen mit einzelnen Parametern
        void AddClimbDataPoint(unsigned rpm, unsigned climbtype, signed isadev, double aicortime, double aicordist, double aicorfuel, unsigned mass, unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
			TClimbDataPoint tmp(fl, kias, trq, time, dist, fuel);
            AddClimbDataPoint(rpm, climbtype, isadev, aicortime, aicordist, aicorfuel, mass, tmp);
		};
	
		//Neuen Datenpunkt hinzufügen
        void AddClimbDataPoint(unsigned rpm, unsigned climbtype, signed isadev, double aicortime, double aicordist, double aicorfuel, unsigned mass, TClimbDataPoint NewClimbDataPoint){
			//1. gibt es die RPM schon? sonst erstellen. Dann Datenpunkt hinzufügen
            if (!(CheckClimbData(rpm))){
				TClimbRPMData tmp(rpm);
                dClimbData.push_back(tmp);
			}
            dClimbData.at(GetClimbDataIndex(rpm)).ClimbData(climbtype, isadev, aicortime, aicordist, aicorfuel, mass, NewClimbDataPoint);
		};
	
		//Index des Datepunktes mit RPM zurückgeben
        unsigned GetClimbDataIndex(unsigned rpm){
            unsigned i;
            for (i = 0; i < dClimbData.size(); i++){
                if (dClimbData.at(i).RPM() == rpm) break;
			}
			return i;
		};

		// Check ob Datenpunkt für RPM existiert
        bool CheckClimbData(unsigned rpm){
            return (GetClimbDataIndex(rpm) < dClimbData.size());
		};
			
		// Datapoint zurückgeben
        TClimbDataPoint GetClimbDataPoint(unsigned rpm, unsigned climbtype,  signed isadev, unsigned mass, unsigned fl){
            if (CheckClimbData(rpm)) return dClimbData.at(GetClimbDataIndex(rpm)).ClimbData(climbtype, isadev, mass, fl);
			else return InterpolClimbDataPoint(rpm, climbtype, isadev, mass, fl);
		};
	
		// sucht die nächst-kleinere RPM in ClimbData und gibt es zurück
        unsigned LowerRPM(unsigned rpm){
            unsigned result = MinRPM();
            for (unsigned i = 0; i < dClimbData.size(); i++){
                if ((dClimbData.at(i).RPM() <= rpm) && (result < dClimbData.at(i).RPM())) result = dClimbData.at(i).RPM();
			}	
			return result;
		};

		// sucht das nächst-grössere FL in ClimbData und gibt es zurück 
        unsigned UpperRPM(unsigned rpm){
            unsigned result = MaxRPM();
            for (unsigned i = 0; i < dClimbData.size(); i++){
                if ((dClimbData.at(i).RPM() >= rpm) && (result > dClimbData.at(i).RPM())) result = dClimbData.at(i).RPM();
			}
			return result;
		};
	
		//Sucht das grösste vorhandene  FL und gibt es zurück
        unsigned MaxRPM(){
            unsigned result = 0;
            for (unsigned i = 0; i < dClimbData.size(); i++){
                if (dClimbData.at(i).RPM() > result) result = dClimbData.at(i).RPM();
			}
		return result;
		};

		//Sucht das kleinste vorhandene  FL und gibt es zurück
        unsigned MinRPM(){
            unsigned result = MaxRPM();
            for (unsigned i = 0; i < dClimbData.size(); i++){
                if (dClimbData.at(i).RPM() < result) result = dClimbData.at(i).RPM();
			}
			return result;
		};
	
		bool IsInRange(unsigned rpm){
			return (!(((rpm < MinRPM()) || (rpm > MaxRPM()))));
		};
	
		// Erstellt einen Interpolierten DatenPunkt zum gewählten FL
        TClimbDataPoint InterpolClimbDataPoint(unsigned rpm, unsigned climbtype, signed isadev, unsigned mass, unsigned fl){
			if (!(IsInRange(rpm))){
				TClimbDataPoint tmp(0, 0, 0, 0, 0, 0);
				return tmp;
			}
			TClimbDataPoint lower = GetClimbDataPoint(LowerRPM(rpm), climbtype, isadev, mass, fl);
			TClimbDataPoint upper = GetClimbDataPoint(UpperRPM(rpm), climbtype, isadev, mass, fl);
            unsigned kias = InterpolateLinear1D(lower.KIAS(), upper.KIAS(), LowerRPM(rpm), UpperRPM(rpm), rpm);
			double trq = InterpolateLinear1D(lower.TRQ(), upper.TRQ(), LowerRPM(rpm), UpperRPM(rpm), rpm);
			double time = InterpolateLinear1D(lower.Time(), upper.Time(), LowerRPM(rpm), UpperRPM(rpm), rpm);
			double dist = InterpolateLinear1D(lower.Dist(), upper.Dist(),LowerRPM(rpm), UpperRPM(rpm), rpm);
			double fuel = InterpolateLinear1D(lower.Fuel(), upper.Fuel(), LowerRPM(rpm), UpperRPM(rpm), rpm);
			TClimbDataPoint tmp(fl, kias, trq, time, dist, fuel);
			return tmp;
		};
		
		//Coorekturfatoren für AntiIce ausgeben. Wenn climbtype nicht vorhanden, dann 0 zurückgeben
        double GetAICorTime(unsigned rpm, unsigned climbtype, signed isadev){
            if (CheckClimbData(rpm)) return dClimbData.at(GetClimbDataIndex(rpm)).AICorTime(climbtype, isadev);
			else return 0;
		};
	
        double GetAICorDist(unsigned rpm, unsigned climbtype, signed isadev){
            if (CheckClimbData(rpm)) return dClimbData.at(GetClimbDataIndex(rpm)).AICorDist(climbtype, isadev);
			else return 0;
		};
			
        double GetAICorFuel(unsigned rpm, unsigned climbtype, signed isadev){
            if (CheckClimbData(rpm)) return dClimbData.at(GetClimbDataIndex(rpm)).AICorFuel(climbtype, isadev);
			else return 0;
		};
	
	public:
		TClimbDataSet(){};
	
	bool IsValid(unsigned rpm){
		return IsInRange(rpm);
	};
	
   bool IsValid(unsigned rpm, unsigned climbtype){
   	if (!(IsValid(rpm)))return false;
        return (dClimbData.at(GetClimbDataIndex(LowerRPM(rpm))).IsValid(climbtype) && dClimbData.at(GetClimbDataIndex(UpperRPM(rpm))).IsValid(climbtype));
	};

    bool IsValid(unsigned rpm, unsigned climbtype, signed isadev){
   	if (!(IsValid(rpm))) return false;
        return (dClimbData.at(GetClimbDataIndex(LowerRPM(rpm))).IsValid(climbtype, isadev) && dClimbData.at(GetClimbDataIndex(UpperRPM(rpm))).IsValid(climbtype, isadev));
	};

	bool IsValid(unsigned rpm, unsigned climbtype, signed isadev, unsigned mass){
   	if (!(IsValid(rpm))) return false;
        return (dClimbData.at(GetClimbDataIndex(LowerRPM(rpm))).IsValid(climbtype, isadev, mass) && dClimbData.at(GetClimbDataIndex(UpperRPM(rpm))).IsValid(climbtype, isadev, mass));
	};
		
    bool IsValid(unsigned rpm, unsigned climbtype, signed isadev, unsigned mass, unsigned fl){
    	if (!(IsValid(rpm))) return false;
        return (dClimbData.at(GetClimbDataIndex(LowerRPM(rpm))).IsValid(climbtype, isadev, mass, fl) && dClimbData.at(GetClimbDataIndex(UpperRPM(rpm))).IsValid(climbtype, isadev, mass, fl));
	};
	
		 //Constructor mit init
		TClimbDataSet(int source){
			dSource = source;
		};
	
		//Destructor
		~TClimbDataSet(void){};
		
		int Source(){
			return GetSource();
		};
				
		void LoadData(std::string path, char valsep, char lineend){
            std::ifstream file;
            std::string tmp;
			unsigned rpm;
			unsigned  climbtype;
			signed isadev;
            double aicortime;
            double aicordist;
            double aicorfuel;
			unsigned mass;
			unsigned fl;
			unsigned kias;
			double trq;
			double time;
			double dist;
			double fuel;
            file.open(path);
            if(file){
            	//Achtung! while(!ile.eof()) ist unzuverlässig, u.u. wird die letzte zeile zweimal geladen und prog bleibt hängen
                while(std::getline(file, tmp, valsep)){
                	rpm = std::stoi(tmp);
                    std::getline(file, tmp, valsep);
                    climbtype = std::stoi(tmp);
                    std::getline(file, tmp, valsep);
                    isadev  = std::stoi(tmp);
                    std::getline(file,  tmp, valsep);
                    aicortime  = std::stof(tmp);
                    std::getline(file,  tmp, valsep);
                    aicordist = std::stof(tmp);
                    std::getline(file,  tmp, valsep);
                    aicorfuel  = std::stof(tmp);
                    std::getline(file,  tmp, valsep);
                    mass  = std::stoi(tmp);
                    std::getline(file,  tmp, valsep);
                    fl = std::stoi(tmp);
                    std::getline(file,  tmp, valsep);
                    kias = std::stoi(tmp);
                    std::getline(file,  tmp, valsep);
                    trq  = std::stof(tmp);
                    std::getline(file,  tmp, valsep);
                    time   = std::stof(tmp);
                    std::getline(file,  tmp, valsep);
                    dist  = std::stof(tmp);
                    std::getline(file,  tmp, lineend);
                    fuel = std::stof(tmp);
                    AddClimbDataPoint(rpm, climbtype, isadev, aicortime, aicordist, aicorfuel, mass, fl, kias, trq, time, dist, fuel);
                }

            }
            else std::cout << "File not found." << std::endl;
            file.close();
        };
	
		//Neuen Datenpunkt hinzufügen
        void ClimbData(unsigned rpm, unsigned climbtype, signed isadev, double aicortime, double aicordist, double aicorfuel,  unsigned mass, unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
            AddClimbDataPoint(rpm, climbtype, isadev, aicortime, aicordist, aicorfuel, mass, fl, kias, trq, time, dist, fuel);
		};
	
        void ClimbData(unsigned rpm, unsigned climbtype, signed isadev, double aicortime, double aicordist, double aicorfuel,  unsigned mass, TClimbDataPoint NewClimbDataPoint){
            AddClimbDataPoint(rpm, climbtype, isadev, aicortime, aicordist, aicorfuel, mass, NewClimbDataPoint);
		};
	
		//Korrekturfaktoren für AntiIce
        double AICorTime(unsigned rpm, unsigned climbtype, signed isadev){
            return GetAICorTime(rpm, climbtype, isadev);
		};
	
        double AICorDist(unsigned rpm, unsigned climbtype, signed isadev){
            return GetAICorDist(rpm, climbtype, isadev);
		};
	
        double AICorFuel(unsigned rpm, unsigned climbtype, signed isadev){
            return GetAICorFuel(rpm, climbtype, isadev);
		};

		TClimbDataPoint ClimbData (unsigned rpm, unsigned climbtype, signed isadev, unsigned mass, unsigned fl){
			return GetClimbDataPoint(rpm, climbtype, isadev, mass, fl);
		};
		
		//Einzelne Werte zurückgeben
        unsigned KIAS(unsigned rpm, unsigned climbtype, signed isadev, unsigned mass, unsigned fl){
			return GetClimbDataPoint(rpm, climbtype, isadev, mass, fl).KIAS();
		};

        double TRQ(unsigned rpm, unsigned climbtype, signed isadev, unsigned mass, unsigned fl){
			return GetClimbDataPoint(rpm, climbtype, isadev, mass, fl).TRQ();
		};
	
        double Time(unsigned rpm, unsigned climbtype, signed isadev, unsigned mass, unsigned fl){
			return GetClimbDataPoint(rpm, climbtype, isadev, mass, fl).Time();
		};

        double Dist(unsigned rpm, unsigned climbtype, signed isadev, unsigned mass, unsigned fl){
			return GetClimbDataPoint(rpm, climbtype, isadev, mass, fl).Dist();
		};

        double Fuel(unsigned rpm, unsigned climbtype, signed isadev, unsigned mass, unsigned fl){
			return GetClimbDataPoint(rpm, climbtype, isadev, mass, fl).Fuel();
		};
};