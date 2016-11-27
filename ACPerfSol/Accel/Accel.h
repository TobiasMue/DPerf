#include <iostream>
#include <fstream>
#include <string>
#include "AccelRPM.h"
/**
 * @brief The TAccelDataSet class
 * Outer Shell containing all AccelData.
 */
class TAccelDataSet{
	int dSource;
    std::vector < TAccelRPMData > dAccelData;

	private:
		int GetSource(){
			return dSource;
		};
		
		//Neuen Datenpunkt hinzufügen mit einzelnen Parametern
        void AddAccelDataPoint(unsigned rpm, unsigned climbtype, unsigned toc, unsigned cruisetype, double time, double dist, double fuel){
			TAccelDataPoint tmp(cruisetype, time, dist, fuel);
            AddAccelDataPoint(rpm, climbtype, toc, tmp);
		};
	
		//Neuen Datenpunkt hinzufügen
        void AddAccelDataPoint(unsigned rpm, unsigned climbtype, unsigned toc, TAccelDataPoint NewAccelDataPoint){
            if (!(CheckAccelData(rpm))){
				TAccelRPMData tmp(rpm);
                dAccelData.push_back(tmp);
			}
            dAccelData.at(GetAccelDataIndex(rpm)).AccelData(climbtype, toc, NewAccelDataPoint);
		};
	
		//Index des Datepunktes mit RPM zurückgeben
        unsigned GetAccelDataIndex(unsigned rpm){
            unsigned i;
            for (i = 0; i < dAccelData.size(); i++){
                if (dAccelData.at(i).RPM() == rpm) break;
			}
			return i;
		};

		// Check ob Datenpunkt für RPM existiert
        bool CheckAccelData(unsigned rpm){
            if (GetAccelDataIndex(rpm) < dAccelData.size()) return true;
			else return false;
		};
			
		// Datapoint zurückgeben
        TAccelDataPoint GetAccelDataPoint(unsigned rpm, unsigned climbtype,  unsigned toc, unsigned cruisetype){
            if (CheckAccelData(rpm)) return dAccelData.at(GetAccelDataIndex(rpm)).AccelData(climbtype, toc, cruisetype);
			else return InterpolAccelDataPoint(rpm, climbtype, toc, cruisetype);
		};
	
		// sucht die nächst-kleinere RPM in AccelData und gibt es zurück
        unsigned LowerRPM(unsigned rpm){
            unsigned result = MinRPM();
            for (unsigned i = 0; i < dAccelData.size(); i++){
                if ((dAccelData.at(i).RPM() <= rpm) && (result < dAccelData.at(i).RPM())) result = dAccelData.at(i).RPM();
			}	
			return result;
		};

		// sucht das nächst-grössere FL in AccelData und gibt es zurück 
        unsigned UpperRPM(unsigned rpm){
            unsigned result = MaxRPM();
            for (unsigned i = 0; i < dAccelData.size(); i++){
                if ((dAccelData.at(i).RPM() >= rpm) && (result > dAccelData.at(i).RPM())) result = dAccelData.at(i).RPM();
			}
			return result;
		};
	
		//Sucht das grösste vorhandene  FL und gibt es zurück
        unsigned MaxRPM(){
            unsigned result = 0;
            for (unsigned i = 0; i < dAccelData.size(); i++){
                if (dAccelData.at(i).RPM() > result) result = dAccelData.at(i).RPM();
			}
		return result;
		};

		//Sucht das kleinste vorhandene  FL und gibt es zurück
        unsigned MinRPM(){
            unsigned result = MaxRPM();
            for (unsigned i = 0; i < dAccelData.size(); i++){
                if (dAccelData.at(i).RPM() < result) result = dAccelData.at(i).RPM();
			}
			return result;
		};
		
		bool IsInRange(unsigned rpm){
			return (!(((rpm < MinRPM()) || (rpm > MaxRPM()))));
		};
	
		// Erstellt einen Interpolierten DatenPunkt zum gewählten FL
        TAccelDataPoint InterpolAccelDataPoint(unsigned rpm, unsigned climbtype, unsigned toc, unsigned cruisetype){
			if (!(IsValid(rpm))){
				TAccelDataPoint tmp(INVAL_CRUISETYPE, INVAL_TIME, INVAL_DIST, INVAL_FUEL);
				return tmp;
			}
			TAccelDataPoint lower = GetAccelDataPoint(LowerRPM(rpm), climbtype, toc, cruisetype);
			TAccelDataPoint upper = GetAccelDataPoint(UpperRPM(rpm), climbtype, toc, cruisetype);
            double time = InterpolateLinear1D(lower.Time(), upper.Time(), LowerRPM(rpm), UpperRPM(rpm), rpm);
			double dist = InterpolateLinear1D(lower.Dist(), upper.Dist(),LowerRPM(rpm), UpperRPM(rpm), rpm);
			double fuel = InterpolateLinear1D(lower.Fuel(), upper.Fuel(), LowerRPM(rpm), UpperRPM(rpm), rpm);
			TAccelDataPoint tmp(cruisetype, time, dist, fuel);
			return tmp;
		};
		
	public:
		TAccelDataSet(){};
			
	bool IsValid(unsigned rpm){
		return IsInRange(rpm);
	};
	
    bool IsValid(unsigned rpm, unsigned climbtype){
    	if (!(IsValid(rpm))) return false;
        return (dAccelData.at(GetAccelDataIndex(LowerRPM(rpm))).IsValid(climbtype) && dAccelData.at(GetAccelDataIndex(UpperRPM(rpm))).IsValid(climbtype));
	};

    bool IsValid(unsigned rpm, unsigned climbtype, unsigned toc){
    	if (!(IsValid(rpm))) return false;
        return (dAccelData.at(GetAccelDataIndex(LowerRPM(rpm))).IsValid(climbtype, toc) && dAccelData.at(GetAccelDataIndex(UpperRPM(rpm))).IsValid(climbtype, toc));
	};	
	
    bool IsValid(unsigned rpm, unsigned climbtype, unsigned toc, unsigned cruisetype){
        if (!(IsValid(rpm))) return false;
        return (dAccelData.at(GetAccelDataIndex(LowerRPM(rpm))).IsValid(climbtype, toc, cruisetype) && dAccelData.at(GetAccelDataIndex(UpperRPM(rpm))).IsValid(climbtype, toc, cruisetype));
	};
	
		 //Constructor mit init
		TAccelDataSet(int source){
			dSource = source;
		};
	
		//Destructor
		~TAccelDataSet(void){};
		
		int Source(){
			return GetSource();
		};
				
		void LoadData(std::string path, char valsep, char lineend){
            std::ifstream file;
            std::string tmp; 
			unsigned rpm;
			unsigned climbtype;
			unsigned toc;
			unsigned cruisetype;
			double time;
			double dist;
			double fuel;
            file.open(path);
            if(file){
            	//Achtung: while(!(file.eof()) schmiert ab!
                while(std::getline(file, tmp, valsep)){
                    if ((tmp == "N/A") || (tmp == "n/a") || (tmp == "")) break; //Variable prüfen, wenn Leer oder "N/A" dann durchlauf abbrechen
                	rpm = std::stoi(tmp);
                    std::getline(file, tmp, valsep);
                    if ((tmp == "N/A") || (tmp == "n/a") || (tmp == "")) break;
                    climbtype = std::stoi(tmp);
                    std::getline(file, tmp, valsep);
                    if ((tmp == "N/A") || (tmp == "n/a") || (tmp == "")) break;
                    toc = std::stoi(tmp);
                    std::getline(file, tmp, valsep);
                    if ((tmp == "N/A") || (tmp == "n/a") || (tmp == "")) break;
                    cruisetype = std::stoi(tmp);
                    std::getline(file, tmp, valsep);
                    if ((tmp == "N/A") || (tmp == "n/a") || (tmp == "")) break;
                    time = std::stof(tmp);
                    std::getline(file, tmp, valsep);
                    if ((tmp == "N/A") || (tmp == "n/a") || (tmp == "")) break;
                    dist = std::stof(tmp);
                    std::getline(file, tmp, lineend);
                    if ((tmp == "N/A") || (tmp == "n/a") || (tmp == "")) break;
                    fuel = std::stof(tmp);
                    AddAccelDataPoint(rpm, climbtype, toc, cruisetype, time, dist, fuel);
                }

            }
            else std::cout << "File not found." << std::endl;
            file.close();
        };
	
		//Neuen Datenpunkt hinzufügen
        void AccelData(unsigned rpm, unsigned climbtype, unsigned toc, unsigned cruisetype, double time, double dist, double fuel){
            AddAccelDataPoint(rpm, climbtype, toc, cruisetype, time, dist, fuel);
		};
	
        void AccelData(unsigned rpm, unsigned climbtype, unsigned toc, TAccelDataPoint NewAccelDataPoint){
            AddAccelDataPoint(rpm, climbtype, toc, NewAccelDataPoint);
		};
	
	
        double Time(unsigned rpm, unsigned climbtype, unsigned toc, unsigned cruisetype){
			return GetAccelDataPoint(rpm, climbtype, toc, cruisetype).Time();
		};

        double Dist(unsigned rpm, unsigned climbtype, unsigned toc, unsigned cruisetype){
			return GetAccelDataPoint(rpm, climbtype, toc, cruisetype).Dist();
		};

        double Fuel(unsigned rpm, unsigned climbtype, unsigned toc, unsigned cruisetype){
			return GetAccelDataPoint(rpm, climbtype, toc, cruisetype).Fuel();
		};
};
