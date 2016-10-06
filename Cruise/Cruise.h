#include <iostream>
#include <fstream>
#include <string>
#include "CruiseType.h"
#include "../Misc/ISA.h"

/**
 * @brief The TCruiseDataSet class
 * Outer Shell containing all CruiseData.
 */
class TCruiseDataSet{
	int dSource;
    std::vector < TCruiseTypeData > dCruiseData;

	private:
		int GetSource(){
			return dSource;
		};
		
		//Neuen Datenpunkt hinzufügen mit einzelnen Parametern
        void AddCruiseDataPoint(unsigned cruisetype, signed isadev, double aicortrq, double aicorff, unsigned mass, unsigned fl, unsigned kias, double trq, unsigned ktas, double ff){
			TCruiseDataPoint tmp(fl, kias, trq, ktas, ff);
            AddCruiseDataPoint(cruisetype, isadev, aicortrq, aicorff, mass, tmp);
		};
	
		//Neuen Datenpunkt hinzufügen
        void AddCruiseDataPoint(unsigned cruisetype, signed isadev, double aicortrq, double aicorff, unsigned mass, TCruiseDataPoint NewCruiseDataPoint){
			//1. gibt es die CruiseType schon? sonst erstellen. Dann Datenpunkt hinzufügen
            if (!(CheckCruiseData(cruisetype))){
				TCruiseTypeData tmp(cruisetype);
                dCruiseData.push_back(tmp);
			}
            dCruiseData.at(GetCruiseDataIndex(cruisetype)).CruiseData(isadev, aicortrq, aicorff, mass, NewCruiseDataPoint);
		};
	
		//Index des Datepunktes mit CruiseType zurückgeben
        unsigned GetCruiseDataIndex(unsigned cruisetype){
            unsigned i;
            for (i = 0; i < dCruiseData.size(); i++){
                if (dCruiseData.at(i).CruiseType() == cruisetype) break;
			}
			return i;
		};

		// Check ob Datenpunkt für CruiseType existiert
        bool CheckCruiseData(unsigned cruisetype){
            return (GetCruiseDataIndex(cruisetype) < dCruiseData.size());
		};
			
		// Datapoint zurückgeben
        TCruiseDataPoint GetCruiseDataPoint(unsigned cruisetype,  signed isadev, unsigned mass, unsigned fl){
            if (!(IsValid(cruisetype))){
				TCruiseDataPoint tmp(INVAL_FL, INVAL_KIAS, INVAL_TRQ, INVAL_KTAS, INVAL_FF);
				return tmp;
			} 
            return dCruiseData.at(GetCruiseDataIndex(cruisetype)).CruiseData(isadev, mass, fl); 
		};
			
		//Correkturfatoren für AntiIce ausgeben. Wenn climbtype nicht vorhanden, dann -999 zurückgeben
        double GetAICorTRQ(unsigned cruisetype, signed isadev){
            if (CheckCruiseData(cruisetype)) return dCruiseData.at(GetCruiseDataIndex(cruisetype)).AICorTRQ(isadev);
			else return 0;
		};
	
        double GetAICorFF(unsigned cruisetype, signed isadev){
            if (CheckCruiseData(cruisetype)) return dCruiseData.at(GetCruiseDataIndex(cruisetype)).AICorFF(isadev);
			else return 0;
		};
			
	public:
		TCruiseDataSet(){};

	bool IsValid(unsigned cruisetype){
		return CheckCruiseData(cruisetype);
	};
	
	bool IsValid(unsigned cruisetype, unsigned isadev){
		if (!(IsValid(cruisetype))) return false;
		return dCruiseData.at(GetCruiseDataIndex(cruisetype)).IsValid(isadev);
	};
	
	bool IsValid(unsigned cruisetype, signed isadev, unsigned mass){
		if (!(IsValid(cruisetype))) return false;
		return dCruiseData.at(GetCruiseDataIndex(cruisetype)).IsValid(isadev, mass);	
	};
				
    bool IsValid(unsigned cruisetype, signed isadev, unsigned mass, unsigned fl){
		if (!(IsValid(cruisetype))) return false;
		return dCruiseData.at(GetCruiseDataIndex(cruisetype)).IsValid(isadev, mass, fl);
	};
	
		 //Constructor mit init
		TCruiseDataSet(int source){
			dSource = source;
		};
	
		//Destructor
		~TCruiseDataSet(void){};
		
		int Source(){
			return GetSource();
		};
				
		void LoadData(std::string path, char valsep, char lineend){
            std::ifstream file;
			std::string tmp;
			 unsigned cruisetype;
			 signed isadev;
			 double aicortrq;
			 double aicorff;
			 unsigned mass;
			 unsigned fl;
			 unsigned kias;
			 double trq;
			 unsigned ktas;
			 double ff;

	        file.open(path);
            if(file){
            	//Achtung! while(!ile.eof()) ist unzuverlässig, u.u. wird die letzte zeile zweimal geladen und prog bleibt hängen
                while(std::getline(file, tmp, valsep)){
                    cruisetype = std::stoi(tmp);
					std::getline(file, tmp, valsep);
					isadev = std::stoi(tmp);
                    std::getline(file, tmp, valsep);
					aicortrq = std::stof(tmp);
                    std::getline(file, tmp, valsep);
					aicorff = stof(tmp);
                    std::getline(file, tmp, valsep);
					mass = stoi(tmp);
                    std::getline(file, tmp, valsep);
					fl = stoi(tmp);
                    std::getline(file, tmp, valsep);
                    if(tmp == "N/A"){
						std::getline(file, tmp, lineend);		//Bis linend weiterlesendamit nächstezeile geladen wird
						continue;
					}
					else{
						 kias = stoi(tmp);
					}
					
                    std::getline(file, tmp, valsep);
                    if(tmp  == "N/A") {
						std::getline(file, tmp, lineend);		//Bis linend weiterlesendamit nächstezeile geladen wird
						continue;
					}
					else trq = stof(tmp);
                	std::getline(file, tmp, valsep);
                	if(tmp == "N/A"){
						std::getline(file, tmp, lineend);		//Bis linend weiterlesendamit nächstezeile geladen wird
						continue;
					}
					else ktas = stoi(tmp);
	   		     std::getline(file, tmp, lineend);
 	               if(tmp  == "N/A"){
						std::getline(file, tmp, lineend);		//Bis linend weiterlesendamit nächstezeile geladen wird
						continue;
					}
					else ff = stof(tmp);
										
					AddCruiseDataPoint(cruisetype, isadev, aicortrq, aicorff, mass, fl, kias, trq, ktas, ff);
                }
            }
            else std::cout << "File not found." << std::endl;
            file.close();
        };
	
		//Neuen Datenpunkt hinzufügen
        void CruiseData(unsigned cruisetype, signed isadev, double aicortrq, double aicorff, unsigned mass, unsigned fl, unsigned kias, double trq, unsigned ktas, double ff){
            AddCruiseDataPoint(cruisetype, isadev, aicortrq, aicorff, mass, fl, kias, trq, ktas, ff);
		};
	
        void CruiseData(unsigned cruisetype, signed isadev, double aicortrq, double aicorff, unsigned mass, TCruiseDataPoint NewCruiseDataPoint){
            AddCruiseDataPoint(cruisetype, isadev, aicortrq, aicorff, mass, NewCruiseDataPoint);
		};
	
		//Korrekturfaktoren für AntiIce
        double AICorTRQ(unsigned cruisetype, signed isadev){
            return GetAICorTRQ(cruisetype, isadev);
		};
	
        double AICorFF(unsigned cruisetype, signed isadev){
            return GetAICorFF(cruisetype, isadev);
		};
	
		//Einzelne Werte zurückgeben
        unsigned KIAS(unsigned cruisetype, signed isadev, unsigned mass, unsigned fl){
			return GetCruiseDataPoint(cruisetype, isadev, mass, fl).KIAS();
		};

        double TRQ(unsigned cruisetype, signed isadev, unsigned mass, unsigned fl){
			return GetCruiseDataPoint(cruisetype, isadev, mass, fl).TRQ();
		};
	
        double KTAS(unsigned cruisetype, signed isadev, unsigned mass, unsigned fl){
			return GetCruiseDataPoint(cruisetype, isadev, mass, fl).KTAS();
		};

        double FF(unsigned cruisetype, signed isadev, unsigned mass, unsigned fl){
			return GetCruiseDataPoint(cruisetype, isadev, mass, fl).FF();
		};
};