///****PhaseDetector.c****
//Gives a look up table for the linearized Phase
//Gives the Degree look up table for Phases
//Calculates the Phase from a given input


unsigned int const PhaseRange[14] = {1892,1869,1814,1661,1450,1239,1020,843,682,518,367,267,182,62};
unsigned int const PhaseRange3[14] = {2167,2116,1996,1798,1580,1372,1122,904,750,556,382,262,191,92};
unsigned int const PhaseRangeNeg[14] = {1898,1863,1776,1644,1448,1222,986,797,604,508,497,466,248,124};
unsigned int const PhaseRangeNeg3[14] = {2177,2082,1959,1795,1556,1256,1064,764,467,395,256,170,157,136};
unsigned int const PhaseDeg[16] = {0,5,10,15,20,25,30,35,40,45,50,55,60,65};
unsigned int const PhaseShifterINew[27] = {190,151,116,85,59,37,20,8,2,0,4,12,26,45,69,97,129,166,207,251,299,349,402,457,514,572,631};
unsigned int const PhaseShifterI[27] = {0,3,12,29,59,106,175,267,384,522,678,841,1001,1144,1257,1331,1356,1331,1257,1144,1001,841,678,522,384,267,175};
unsigned int const PhaseShifterQNew[27] = {1149,1105,1057,1007,954,899,842,784,725,666,607,549,491,435,381,329,279,233,190,151,116,85,59,37,20,8,2};
unsigned int const PhaseShifterQ[27] = {710,750,805,874,955,1043,1132,1217,1289,1338,1356,1336,1274,1170,1030,861,678,494,326,185,81,19,0,18,67,138,223};
int NUMTABLE =14;

//***Phase_Conversion(v1,v2)***
//**Converts 2 voltages from ADC into Phase
//**Inputs: V1,V2 from the ADC for Detector 1 and Detector 2 respectively
//**Outpus: Signed value of the Phase in Degrees

signed int Incidence_Conversion(unsigned long v1, unsigned long v2){
	int i =0;
	int V = (v1+v2)/2;
	if(v2>v1){
		//give positive angle
		if(V >= PhaseRange[0]){
			return PhaseDeg[0];
		}
		if(V <= PhaseRange[13]){
			return PhaseDeg[13];
		}
		for(i=1;i < NUMTABLE; i+= 1){
			if((V>=PhaseRange[i]) && (V<= PhaseRange[i-1]) ){
				return PhaseDeg[i];
			}
		}
	}
	
	else{ //give negative angle
		if(V >= PhaseRangeNeg[0]){
			return PhaseDeg[0];
		}
		if(V<= PhaseRangeNeg[13]){
			return -PhaseDeg[13];
		}
		for(i = 1; i < NUMTABLE; i +=1){
			if((V>=PhaseRangeNeg[i]) && (V<= PhaseRangeNeg[i-1])){
				return -PhaseDeg[i];
			}
		}
		
	}
	
	//will also need to give a condition when they are equal
	
	return -1; //some shit got messed up
}

unsigned int Channel_I(signed int phase){
	unsigned int index = (phase + 65)/5;
	//return PhaseShifterI[index]; //original code Senior Design
	return PhaseShifterINew[index];
}

unsigned int Channel_Q(signed int phase){
	unsigned int index = (phase +65)/5;
	//return PhaseShifterQ[index]; //orignal code Senior Design
	return PhaseShifterQNew[index];
}

