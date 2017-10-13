/*library two functions
one function for function generator
class FunctionGenerator
{
Visession DG1022;
}
second class
resource manager
class ResourceManager
{
	all to initialize it 
	use this as reference for the function generator
}
*/

/*
class ResourceManager
{
public:
	ResourceManager();


};

*/

//note to self
//problems running functions singenerate and sweep
	
#include <stdio.h>
#include <stdlib.h>  
#include <visa.h>
#include <unistd.h>
unsigned int sleep(unsigned int seconds);




class FunctionGenerator
{
	ViSession viDG1022;

	ViStatus status;
	ViSession defaultRM;
	ViFindList fList;
	ViUInt32 numInst;
	ViChar desc[VI_FIND_BUFLEN];
	ViPBuf resultBuffer;
	ViUInt32 resultCount;
public:
	FunctionGenerator();
	void idn();
	void funon();	
	void funoff();
	void startsin();
	void singenerate(int freq,int amp,int offset);
	void sweep( int start,int stop,int time);
	void trigger();
};

FunctionGenerator::FunctionGenerator()
{
	resultBuffer = new ViByte[256];
	status=viOpenDefaultRM(&defaultRM);
	status=viFindRsrc(defaultRM,"USB[0-9]::0x0400?*INSTR",&fList,&numInst,desc);
	status=viOpen(defaultRM,desc,VI_NULL,VI_NULL,&viDG1022);

}

void FunctionGenerator::idn()
{
	printf("\nOpened: %s", desc);
	status= viWrite(viDG1022,(unsigned char*)"*IDN?\n",6,VI_NULL);
	sleep(2);
	status= viRead(viDG1022,resultBuffer,256,&resultCount );
	sleep(2);
	printf("\nFound instrument: %s", resultBuffer);
}

void FunctionGenerator::funon()
{
status= viWrite(viDG1022,(unsigned char*)"OUTP ON\n",8,VI_NULL);
}

void FunctionGenerator::funoff()
{
status= viWrite(viDG1022,(unsigned char*)"OUTP OFF\n",8,VI_NULL);
}



void FunctionGenerator::startsin()
{
status= viWrite(viDG1022,(unsigned char*)"FUNC SIN",30,VI_NULL);	
}

//problems
void FunctionGenerator::singenerate(int freq,int amp,int offset)
{
	/*
ViPBuf commandbuffer;
commandbuffer = new ViByte[55];
*/
ViChar commandbuffer[55];
sprintf(commandbuffer,"FREQ  %i",freq); 									//set frequency
	sleep(2);
status= viWrite(viDG1022,(unsigned char*)commandbuffer,sizeof commandbuffer, VI_NULL);
sleep(2);
sprintf(commandbuffer,"VOLT %i",amp); 										//amplitude voltage
	sleep(2);
status= viWrite(viDG1022,(unsigned char*)commandbuffer,sizeof commandbuffer, VI_NULL);
sleep(2);
sprintf(commandbuffer,"VOLT:OFFS %i",offset); 		
	sleep(2);						//offset voltage
status= viWrite(viDG1022,(unsigned char*)commandbuffer,sizeof commandbuffer, VI_NULL);
	sleep(2);
printf("freq=%i amp=%i offset=%i", freq,amp,offset); 						//printing variables
}


void FunctionGenerator::sweep(int start,int stop,int time)
{/*
ViPBuf commandbuffer;
commandbuffer = new ViByte[55];
*/
ViChar commandbuffer[55];
status= viWrite(viDG1022,(unsigned char*)"SWE:STAT ON",30,VI_NULL);  
sleep(2);
status= viWrite(viDG1022,(unsigned char*)"SWE:SPAC LIN",30,VI_NULL);
sleep(2);



	sprintf(commandbuffer,"FREQ:STAR %i,",start); 									//start sweep
	sleep(2);
	status= viWrite(viDG1022,(unsigned char*)commandbuffer,sizeof commandbuffer, VI_NULL);
	sleep(2);

 	sprintf(commandbuffer,"FREQ:STOP %i,",stop); 	
 		sleep(2);							//stop sweep
 	status= viWrite(viDG1022,(unsigned char*)commandbuffer,sizeof commandbuffer, VI_NULL);
 	sleep(2);
 	sprintf(commandbuffer,"SWE:TIME %i,",time); 
 		sleep(2);									//sweep time
 	status= viWrite(viDG1022,(unsigned char*)commandbuffer,sizeof commandbuffer, VI_NULL);
	sleep(2);
	printf("start=%i stop=%i time=%i",start,stop,time); 

}



void FunctionGenerator::trigger()
{
status= viWrite(viDG1022,(unsigned char*)"TRIG:SOUR IMM",30,VI_NULL);	

}


int main(void)
{
//ResourceManager B;
int start=23;  //start sweep
int stop=4444; //stop sweep
int time=1; //time sweep 
int freq=53442; //set frequency
int amp=2;  //amplitude voltage
int offset=2; //offset voltage

FunctionGenerator A;


A.idn(); //outputing instrument data
sleep(2);
A.startsin();
sleep(2);
A.sweep(start,stop,time);
sleep(2);
A.singenerate(freq,amp,offset);
sleep(2);
A.trigger();
A.funon();
sleep(2);
//A.funoff();
return 0;
}