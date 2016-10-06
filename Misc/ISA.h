#ifndef ISA
	#define ISA
	/*ISA Modell*/
	#include <iostream>
	#include "Interpol.h"
	
	#define NFLIGHTLEVELS 14

	typedef struct{
	 unsigned FL;
	 double ISATemp;
	} TISAFLTemp;
	
	const TISAFLTemp dISAFLTemp[]{
		{0, 15.0},
		{20, 11.0},
		{40, 7.1},
		{60, 3.1},
		{80, -0.8},
		{100, -4.8},
		{120, -8.8},
		{140, -12.7},
		{160, -16.7},
		{180, -20.7},
		{200, -24.6},
		{220, -28.6},
		{240, -32.5},
		{250, -34.5}
	}; 

	
		
	int ISAFLIndex(unsigned fl){
		int i;
		for (i = 0; i < NFLIGHTLEVELS; i++){
			if (dISAFLTemp[i].FL == fl) return i;
		}
		return i;
	};
		
	bool ISAFLCheck(unsigned fl){
		if (ISAFLIndex(fl) < NFLIGHTLEVELS) return true;
		else return false;
	};
				
	double ISAFLMin(){
		return dISAFLTemp[0].FL;
	};	
		
	double ISAFLMax(){
		return dISAFLTemp[NFLIGHTLEVELS - 1].FL;
		};
		
	double ISAFLLower(unsigned fl){
		double lower = ISAFLMin();
		for (int i = 0; i < NFLIGHTLEVELS; i++){
			if((dISAFLTemp[i].FL <= fl) && (lower < dISAFLTemp[i].FL)) lower = dISAFLTemp[i].FL;
		}
		return lower;
	};
		
	double ISAFLUpper(unsigned fl){
		double upper = ISAFLMax();
				for (int i = 0; i < NFLIGHTLEVELS; i++){
			if((dISAFLTemp[i].FL >= fl) && (upper > dISAFLTemp[i].FL)) upper = dISAFLTemp[i].FL;
		}
		return upper;
	};
			
	double ISATempInterpolate(unsigned fl){
		if ((fl < ISAFLMin()) || (fl > ISAFLMax())) return -999;
		TISAFLTemp upper = dISAFLTemp[ISAFLIndex(ISAFLUpper(fl))];
		TISAFLTemp lower = dISAFLTemp[ISAFLIndex(ISAFLLower(fl))];
		return InterpolateLinear1D(lower.ISATemp, upper.ISATemp, lower.FL, upper.FL, fl);
	};
		
	double ISAFLTemp(unsigned fl){
		if (ISAFLCheck(fl)) return dISAFLTemp[ISAFLIndex(fl)].ISATemp;
		else return ISATempInterpolate(fl);
	};
		
	double ISAFLOAT(unsigned fl, int isadev){
		return ISAFLTemp(fl) + isadev;
	};
	
	double ISADev(unsigned fl, signed oat){
		return oat - ISAFLTemp(fl);
	};
	
#endif