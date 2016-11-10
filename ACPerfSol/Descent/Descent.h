#include <iostream>
#include <fstream>
#include <string>
#include "DescentRPM.h"
#include "../Misc/ISA.h"

/**
 * @brief The TDescentDataSet class
 * Outer Shell containing all DescentData.
 */
class TDescentDataSet{
	int dSource;
    std::vector < TDescentRPMData > dDescentData;

	private:
		int GetSource(){
			return dSource;
		};
		
		//Neuen Datenpunkt hinzufügen mit einzelnen Parametern
        void AddDescentDataPoint(unsigned rpm, unsigned descenttype, signed isadev, unsigned mass, unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
			TDescentDataPoint tmp(fl, kias, trq, time, dist, fuel);
            AddDescentDataPoint(rpm, descenttype, isadev, mass, tmp);
		};
	
		//Neuen Datenpunkt hinzufügen
        void AddDescentDataPoint(unsigned rpm, unsigned descenttype, signed isadev, unsigned mass, TDescentDataPoint NewDescentDataPoint){
			//1. gibt es die RPM schon? sonst erstellen. Dann Datenpunkt hinzufügen
            if (!(CheckDescentData(rpm))){
				TDescentRPMData tmp(rpm);
                dDescentData.push_back(tmp);
			}
            dDescentData.at(GetDescentDataIndex(rpm)).DescentData(descenttype, isadev, mass, NewDescentDataPoint);
		};
	
		//Index des Datepunktes mit RPM zurückgeben
        unsigned GetDescentDataIndex(unsigned rpm){
            unsigned i;
            for (i = 0; i < dDescentData.size(); i++){
                if (dDescentData.at(i).RPM() == rpm) break;
			}
			return i;
		};

		// Check ob Datenpunkt für RPM existiert
        bool CheckDescentData(unsigned rpm){
            if (GetDescentDataIndex(rpm) < dDescentData.size()) return true;
			else return false;
		};
			
		// Datapoint zurückgeben
        TDescentDataPoint GetDescentDataPoint(unsigned rpm, unsigned descenttype,  signed isadev, unsigned mass, unsigned fl){
            if (CheckDescentData(rpm)) return dDescentData.at(GetDescentDataIndex(rpm)).DescentData(descenttype, isadev, mass, fl);
			else return InterpolDescentDataPoint(rpm, descenttype, isadev, mass, fl);
		};
	
		// sucht die nächst-kleinere RPM in DescentData und gibt es zurück
        unsigned LowerRPM(unsigned rpm){
            unsigned result = MinRPM();
            for (unsigned i = 0; i < dDescentData.size(); i++){
                if ((dDescentData.at(i).RPM() <= rpm) && (result < dDescentData.at(i).RPM())) result = dDescentData.at(i).RPM();
			}	
			return result;
		};

		// sucht das nächst-grössere FL in DescentData und gibt es zurück 
        unsigned UpperRPM(unsigned rpm){
            unsigned result = MaxRPM();
            for (unsigned i = 0; i < dDescentData.size(); i++){
                if ((dDescentData.at(i).RPM() >= rpm) && (result > dDescentData.at(i).RPM())) result = dDescentData.at(i).RPM();
			}
			return result;
		};
	
		//Sucht das grösste vorhandene  FL und gibt es zurück
        unsigned MaxRPM(){
            unsigned result = 0;
            for (unsigned i = 0; i < dDescentData.size(); i++){
                if (dDescentData.at(i).RPM() > result) result = dDescentData.at(i).RPM();
			}
		return result;
		};

		//Sucht das kleinste vorhandene  FL und gibt es zurück
        unsigned MinRPM(){
            unsigned result = MaxRPM();
            for (unsigned i = 0; i < dDescentData.size(); i++){
                if (dDescentData.at(i).RPM() < result) result = dDescentData.at(i).RPM();
			}
			return result;
		};
		
		bool IsInRange(unsigned rpm){
			return (!(((rpm < MinRPM()) || (rpm > MaxRPM()))));
		};
	
		// Erstellt einen Interpolierten DatenPunkt zum gewählten FL
        TDescentDataPoint InterpolDescentDataPoint(unsigned rpm, unsigned descenttype, signed isadev, unsigned mass, unsigned fl){
			if (!(IsValid(rpm))){
				TDescentDataPoint tmp(0, INVAL_KIAS, INVAL_TRQ, INVAL_TIME, INVAL_DIST, INVAL_FUEL);
				return tmp;
			}
			TDescentDataPoint lower = GetDescentDataPoint(LowerRPM(rpm), descenttype, isadev, mass, fl);
			TDescentDataPoint upper = GetDescentDataPoint(UpperRPM(rpm), descenttype, isadev, mass, fl);
            unsigned kias = InterpolateLinear1D(lower.KIAS(), upper.KIAS(), LowerRPM(rpm), UpperRPM(rpm), rpm);
			double trq = InterpolateLinear1D(lower.TRQ(), upper.TRQ(), LowerRPM(rpm), UpperRPM(rpm), rpm);
			double time = InterpolateLinear1D(lower.Time(), upper.Time(), LowerRPM(rpm), UpperRPM(rpm), rpm);
			double dist = InterpolateLinear1D(lower.Dist(), upper.Dist(),LowerRPM(rpm), UpperRPM(rpm), rpm);
			double fuel = InterpolateLinear1D(lower.Fuel(), upper.Fuel(), LowerRPM(rpm), UpperRPM(rpm), rpm);
			TDescentDataPoint tmp(fl, kias, trq, time, dist, fuel);
			return tmp;
		};
		
		
	public:
		TDescentDataSet(){};
		
    //Gibt Anzahl der Elemente in dDescentData zurück
    unsigned Size(){
        return dDescentData.size();
	};
	
	bool IsValid(unsigned rpm){
		return IsInRange(rpm);
	};
	
    bool IsValid(unsigned rpm, unsigned descenttype){
    	if (!(IsValid(rpm))) return false;
        return (dDescentData.at(GetDescentDataIndex(LowerRPM(rpm))).IsValid(descenttype) && (dDescentData.at(GetDescentDataIndex(UpperRPM(rpm))).IsValid(descenttype)));
	};

    bool IsValid(unsigned rpm, unsigned descenttype, signed isadev){
    	if (!(IsValid(rpm))) return false;
        return (dDescentData.at(GetDescentDataIndex(LowerRPM(rpm))).IsValid(descenttype, isadev) && (dDescentData.at(GetDescentDataIndex(UpperRPM(rpm))).IsValid(descenttype, isadev)));
	};

    bool IsValid(unsigned rpm, unsigned descenttype, signed isadev, unsigned mass){
    	if (!(IsValid(rpm))) return false;
        return (dDescentData.at(GetDescentDataIndex(LowerRPM(rpm))).IsValid(descenttype, isadev, mass) && (dDescentData.at(GetDescentDataIndex(UpperRPM(rpm))).IsValid(descenttype, isadev, mass)));
	};
	
	bool IsValid(unsigned rpm, unsigned descenttype, signed isadev, unsigned mass, unsigned fl){
		if (!(IsValid(rpm))) return false;
		return (dDescentData.at(GetDescentDataIndex(LowerRPM(rpm))).IsValid(descenttype, isadev, mass, fl) && (dDescentData.at(GetDescentDataIndex(UpperRPM(rpm))).IsValid(descenttype, isadev, mass, fl)));
	};

	
	
		 //Constructor mit init
		TDescentDataSet(int source){
			dSource = source;
		};
	
		//Destructor
		~TDescentDataSet(void){};
		
		int Source(){
			return GetSource();
		};
				
		void LoadData(std::string path, char valsep, char lineend){
            std::ifstream file;
			std::string tmp;
			unsigned rpm;
			unsigned descenttype;
			signed isadev;
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
										descenttype = std::stoi(tmp);
                    std::getline(file, tmp, valsep);
										isadev = std::stoi(tmp);
                    std::getline(file, tmp, valsep);
										mass = std::stoi(tmp);
                    std::getline(file, tmp, valsep);
										fl = std::stoi(tmp);
                    std::getline(file, tmp, valsep);
										kias = stoi(tmp);
                    std::getline(file, tmp, valsep);
										trq = stof(tmp);
                    std::getline(file, tmp, valsep);
										time = stof(tmp);
                    std::getline(file, tmp, valsep);
										dist = stof(tmp);
                    std::getline(file, tmp, lineend);
										fuel = stof(tmp);
                    AddDescentDataPoint(rpm, descenttype, isadev, mass, fl, kias, trq, time, dist, fuel);
                }

            }
            else std::cout << "File not found." << std::endl;
            file.close();
        };
	
		//Neuen Datenpunkt hinzufügen
        void DescentData(unsigned rpm, unsigned descenttype, signed isadev, unsigned mass, unsigned fl, unsigned kias, double trq, double time, double dist, double fuel){
            AddDescentDataPoint(rpm, descenttype, isadev, mass, fl, kias, trq, time, dist, fuel);
		};
	
        void DescentData(unsigned rpm, unsigned descenttype, signed isadev, unsigned mass, TDescentDataPoint NewDescentDataPoint){
            AddDescentDataPoint(rpm, descenttype, isadev, mass, NewDescentDataPoint);
		};
		
		//Einzelne Werte zurückgeben
        unsigned KIAS(unsigned rpm, unsigned descenttype, signed isadev, unsigned mass, unsigned fl){
			return GetDescentDataPoint(rpm, descenttype, isadev, mass, fl).KIAS();
		};

        double TRQ(unsigned rpm, unsigned descenttype, signed isadev, unsigned mass, unsigned fl){
			return GetDescentDataPoint(rpm, descenttype, isadev, mass, fl).TRQ();
		};
	
        double Time(unsigned rpm, unsigned descenttype, signed isadev, unsigned mass, unsigned fl){
			return GetDescentDataPoint(rpm, descenttype, isadev, mass, fl).Time();
		};

        double Dist(unsigned rpm, unsigned descenttype, signed isadev, unsigned mass, unsigned fl){
			return GetDescentDataPoint(rpm, descenttype, isadev, mass, fl).Dist();
		};

        double Fuel(unsigned rpm, unsigned descenttype, signed isadev, unsigned mass, unsigned fl){
			return GetDescentDataPoint(rpm, descenttype, isadev, mass, fl).Fuel();
		};
};