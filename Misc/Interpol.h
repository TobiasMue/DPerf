#ifndef INTERPOL
	#define INTERPOL
	/*Funktionen zum interpolieren*/
//	#include <iostream>
	
	double GetFraction(int valueA, int valueB, int value){
		if ((valueB - valueA) == 0) return 0; else return (value - valueA) / (valueB - valueA);
	}

	double GetFraction(int valueA, int valueB, double value){
		if ((valueB - valueA) == 0) return 0; else return (value - valueA) / (valueB - valueA);
	}

	double GetFraction(int valueA, double valueB, int value){
		if ((valueB - valueA) == 0) return 0; else return (value - valueA) / (valueB - valueA);
	}

	double GetFraction(double valueA, int valueB, int value){
		if ((valueB - valueA) == 0) return 0; else return (value - valueA) / (valueB - valueA);
	}

	double GetFraction(int valueA, double valueB, double value){
		if ((valueB - valueA) == 0) return 0; else return (value - valueA) / (valueB - valueA);
	}

	double GetFraction(double valueA, int valueB, double value){
		if ((valueB - valueA) == 0) return 0; else return (value - valueA) / (valueB - valueA);
	}

	double GetFraction(double valueA, double valueB, int value){
		if ((valueB - valueA) == 0) return 0; else return (value - valueA) / (valueB - valueA);
	}


	double GetFraction(double valueA, double valueB, double value){
		if ((valueB - valueA) == 0) return 0; else return (value - valueA) / (valueB - valueA);
	}

	double InterpolateLinear1D(double valueA, double valueB, double fractionAB){
		return (valueA + fractionAB * (valueB - valueA));
	}

	double InterpolateLinear1D(double valueA, double valueB, double keyA, double keyB, double queryAB){
		return InterpolateLinear1D(valueA, valueB, GetFraction(keyA, keyB, queryAB));
	}

#endif