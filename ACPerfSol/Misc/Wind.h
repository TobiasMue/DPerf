#ifndef Wind
#define Wind
#include <vector>
#include <math.h>
#include "interpol.h"

#define INVAL_WINDDIR 999
#define INVAL_WINDSPEED 999
#define INVAL_OAT 999
#define INVAL_HEADWIND 999

const double pi = 3.1415926535;

struct TWindDataPoint{
	unsigned FL;
	unsigned WindDir;
	unsigned WindSpeed;
	signed OAT;
};

//WindAngle aus Tack und Winddir
unsigned CalcWindAngle(unsigned track, unsigned winddir){
	double result = ((winddir - track + 360) % 360);
	return result;
}

//WindAngle aus Komponenten (atan2 gibt 180 - (-180) zurück
unsigned CalcWindDir(double xcomp, double ycomp){
	double result = ((atan2(ycomp, xcomp)) * 180 / pi);
	result += (360 + 0.5); //+0.5 damit richtig gerundet wird
	return fmod(result, 360); 
}

//WIndspeed aus Komponenten
unsigned CalcWindSpeed(double xcomp, double ycomp){
	double result = sqrt(pow(xcomp, 2) + pow(ycomp, 2)) + 0.5; //+0.5 damit er richtig runded. Sonst werden nur die Dezimalstellen abgeschnitten 0,7 -> 0 <> 0,7 + 0.5 -> 1
	return result;
};

//SinComp
double CalcYComp(unsigned angle, unsigned windspeed){
	double tmp = (sin(angle * pi / 180)) * windspeed;
	return tmp;
}

//CosComp
double CalcXComp(unsigned angle, unsigned windspeed){
	double tmp = (cos(angle * pi /180)) * windspeed;
	return tmp;
}

//Headwind (Wind um 180° gedreht, da wind AUS der richtung, Track IN die Richtung
signed CalcHeadWindComp(unsigned track, unsigned winddir, unsigned windspeed){
	return CalcXComp(CalcWindAngle((winddir + 180) %  360, track), windspeed);
}

//Crosswind
signed CalcCrossWindComp(unsigned track, unsigned winddir, unsigned windspeed){
	return CalcYComp(CalcWindAngle((winddir + 180) % 360, track), windspeed);	
};


class TWindProfile{
	private:
		std::vector <TWindDataPoint> dWindData;
		
		unsigned GetWindDataIndex(unsigned fl){
			unsigned i;
			for (i = 0; i < dWindData.size(); i++){
   	         if (dWindData.at(i).FL == fl) break;
  	      }
   	     return i;
		};
		
		bool CheckWindData(unsigned fl){
  	    if (GetWindDataIndex(fl) < dWindData.size()){
        return true;
      }
      else{
       return false;
        }
    };
		
	  // Datapoint mit FL zur�ckgeben
    TWindDataPoint GetWindDataPoint(unsigned fl){
        if (CheckWindData(fl)){
        	 return dWindData.at(GetWindDataIndex(fl));
        }
        else return InterpolWindData(fl);
    };
	
    // sucht das n�chst-kleinere FL in WindData und gibt es zur�ck
    unsigned LowerFL(unsigned fl){
        unsigned result = MinFL();
        for (unsigned i = 0; i < dWindData.size(); i++){
            if ((dWindData.at(i).FL <= fl) && (result < dWindData.at(i).FL)){
                result = dWindData.at(i).FL;
            }
        }
        return result;
    };

    // sucht das n�chst-gr�ssere FL in WindData und gibt es zur�ck
    unsigned UpperFL(unsigned fl){
        unsigned result = MaxFL();
        for (unsigned i = 0; i < dWindData.size(); i++){
            if ((dWindData.at(i).FL >= fl) && (result > dWindData.at(i).FL)){
                result = dWindData.at(i).FL;
            }
        }
        return result;
    };
    
    void AddWindDataPoint(unsigned fl, unsigned winddir, unsigned windspeed, signed oat){
    	TWindDataPoint tmp{fl, winddir, windspeed, oat};
    	dWindData.push_back(tmp);
    	};

    //Sucht das gr�sste vorhandene  FL und gibt es zur�ck
    unsigned MaxFL(){
        unsigned result = 0;
        for (unsigned i = 0; i < dWindData.size(); i++){
            if (dWindData.at(i).FL > result) result = dWindData.at(i).FL;
        }
        return result;
    };

    //Sucht das kleinste vorhandene  FL und gibt es zur�ck
    unsigned MinFL(){
        unsigned result = MaxFL();
        for (unsigned i = 0; i < dWindData.size(); i++){
            if (dWindData.at(i).FL < result) result = dWindData.at(i).FL;
        }
        return result;
    };
		
    // Erstellt einen Interpolierten DatenPunkt zum gewählten FL
    TWindDataPoint InterpolWindData(unsigned fl){
        if ((fl < MinFL()) || (fl > MaxFL())){
            TWindDataPoint tmp{999, INVAL_WINDDIR, INVAL_WINDSPEED, INVAL_OAT};
            return tmp;
        }
        TWindDataPoint lower = GetWindDataPoint(LowerFL(fl));
        TWindDataPoint upper = GetWindDataPoint(UpperFL(fl));
        double xcomp = InterpolateLinear1D(CalcXComp(lower.WindDir, lower.WindSpeed), CalcXComp(upper.WindDir, upper.WindSpeed), lower.FL, upper.FL, fl);
        double ycomp = InterpolateLinear1D(CalcYComp(lower.WindDir, lower.WindSpeed), CalcYComp(upper.WindDir, upper.WindSpeed), lower.FL, upper.FL, fl);
        unsigned winddir = CalcWindDir(xcomp, ycomp);
        unsigned windspeed = CalcWindSpeed(xcomp, ycomp);
        signed oat = InterpolateLinear1D(lower.OAT, upper.OAT,lower.FL, upper.FL,  fl);
        TWindDataPoint tmp{fl, winddir, windspeed, oat};
        return tmp;
    };
		
		unsigned GetWindDir(unsigned fl){
			return GetWindDataPoint(fl).WindDir;
		};
			
		unsigned GetWindSpeed(unsigned fl){
			return GetWindDataPoint(fl).WindSpeed;
		};
		
		signed GetOAT(unsigned fl){
			return GetWindDataPoint(fl).OAT;
		};
		
		signed GetHeadWind(unsigned fl, unsigned track){
			if ((GetWindDir(fl) == INVAL_WINDDIR) || (GetWindSpeed(fl) == INVAL_WINDSPEED)) return INVAL_HEADWIND;
			return CalcHeadWindComp(track, GetWindDir(fl), GetWindSpeed(fl));
		};
		
		//Durchschnittlicher Wind und OAT durch ein H�henprofil �ber NS und OW - Komponenten (sin und cos brauchen bogenmass). tofl wird nicht mehr betrachtet!
		TWindDataPoint GetAvgWindData(unsigned fromfl, unsigned tofl, unsigned stepwidth){
			double xcomp = 0;
			double ycomp = 0;
			signed oat = 0;
			for(unsigned ifl = fromfl; ifl < tofl; ifl += stepwidth){
				xcomp += CalcXComp(GetWindDir(ifl), GetWindSpeed(ifl));
				ycomp += CalcYComp(GetWindDir(ifl), GetWindSpeed(ifl));
				oat += GetOAT(ifl);
						}
			xcomp /= ((tofl - fromfl) / stepwidth);
			ycomp /= ((tofl - fromfl) / stepwidth);
			unsigned winddir = CalcWindDir(xcomp, ycomp);
			unsigned windspeed = CalcWindSpeed(xcomp, ycomp);
			oat = oat / (((double)tofl - fromfl) / stepwidth);
			TWindDataPoint tmp{(tofl-fromfl)/2, winddir, windspeed, oat};
			return tmp;
		};
		
		signed GetAvgWindDir(unsigned fromfl, unsigned tofl, unsigned stepwidth){
			return GetAvgWindData(fromfl, tofl, stepwidth).WindDir;
		};
		
		unsigned GetAvgWindSpeed(unsigned fromfl, unsigned tofl, unsigned stepwidth){
			return GetAvgWindData(fromfl, tofl, stepwidth).WindSpeed;
		};
		
		signed GetAvgOAT(unsigned fromfl, unsigned tofl, unsigned stepwidth){
			return GetAvgWindData(fromfl, tofl, stepwidth).OAT;
		};
		
	public:
		unsigned WindDir(unsigned fl){
			return GetWindDir(fl);
		};
		
		unsigned WindSpeed(unsigned fl){
			return GetWindSpeed(fl);
		};
		
		signed OAT(unsigned fl){
			return GetOAT(fl);
		};
		
		signed HeadWind(unsigned fl, unsigned track){
			return GetHeadWind(fl, track);
		}
		
		signed AvgWindDir(unsigned fromfl, unsigned tofl, unsigned stepwidth){
			return GetAvgWindDir(fromfl, tofl, stepwidth);
		};
		
		unsigned AvgWindSpeed(unsigned fromfl, unsigned tofl, unsigned stepwidth){
			return GetAvgWindSpeed(fromfl, tofl, stepwidth);
		};
		
		signed AvgOAT(unsigned fromfl, unsigned tofl, unsigned stepwidth){
			return GetAvgOAT(fromfl, tofl, stepwidth);
		};
		
		TWindDataPoint AvgWindData(unsigned fromfl, unsigned tofl, unsigned stepwidth){
			return GetAvgWindData(fromfl, tofl, stepwidth);
		};
		
		TWindDataPoint WindData(unsigned fl){
			return GetWindDataPoint(fl);
		};
			
		void WindData(unsigned fl, unsigned direction, unsigned speed, signed oat){
			AddWindDataPoint(fl, direction, speed, oat);
		};
};

#endif