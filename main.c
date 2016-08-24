/*
Traveller/System Spawner

Spawns Gates/Planets around a single star


By Gareth "Sajuukthanatoskhar" Williams

Released under GPL v3

Make sure you share your code!

*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#define M_PI 3.141592653589
#define maxstring 100
#include <string.h>
#define minHZ  0.8F
#define maxHZ 1.3F

extern int time();

float phasespawn(float);
float angularspeedspawn(float);
float distancespawn(float);
void error();

void prompt();
void spawn();
void locater();
void mainspawner();
double getXcoord(float, float, float, float);
double getYcoord(float, float, float, float);
void locaterfromfile();
void spawn_random();
//Main Function, has a menu for the user to look at
int main(void)
{
	char chooser = 'd';
	int a = 0;
	char enter;
	printf("**************System Creator and Tracker*****************\n\n");

	while (a != 1)	//
	{


		printf("\na = System Body Spawner\nb = System Body Locator(Manual Input)\nc = System Body Locator(File Input) \nd = quit  \ne = random spawn (OH GOD DONT DO THIS) \n\n Otherwise Error");
		scanf("%c", &chooser);
		switch (chooser)	//Menu
		{
			case 'a':
				spawn();	//Goes to Planet Spawner
				break;
			case 'b':
				locater();	//Goes to Planet Locator
				break;
			case 'c':
				locaterfromfile();	//Goes to Planet Locator From File
				break;
			case 'd':
				a = 1;	//Exit
				break;
			case 'e':
				spawn_random();	//Exit
				break;
			default:
				error();
		}
		printf("\nPress enter to continue\n");
		scanf("%c", &enter);

	}
    return 0;
}
//This is the manual version of locaterfromfile() function where the user inputs their own set of values
//It outputs the same cartesian only file as well
void locater()
{
	int planets;
	int gates;
	int planetsnumber = 1;
	int gatesnumber = 1;
	float range;
	float angle;
	float phase;
	float timepassed;
	double Xcoord;
	double Ycoord;
//	float two = 2;


	FILE *ifp;  //output file pointer
	char entitydescription[100];  //Used to describe the system's name and whether a planet or gate's orbit,phase or range is being asked of the user in the prompts.
//	char suffix[25] = "_locater.ssref";


	printf("\nWhat is the name of this system?\n$");
	scanf("%s", entitydescription);  //Ask the User what the system's name is as well establishing the name of the file and making the data into a .ssref
	
	strcat(entitydescription, "_locater.ssref");
	//printf("%s",entitydescription);  //DEBUGGER
	ifp = fopen(entitydescription, "w");

	printf("How many planets are there in %s?\n", entitydescription);
	scanf("%d", &planets);
	printf("How many gates are there in %s?\n", entitydescription);
	scanf("%d", &gates);
	printf("How much Time has passed?\n");
	scanf("%f", &timepassed);
	fprintf(ifp, "System - %s at Day %f", entitydescription, timepassed);


	while (planets != 0)
	{

		printf("Planet %d\n\n", planetsnumber);
		printf("How far is planet %d?(AU)\n", planetsnumber);
		scanf("%f", &range);
		printf("What is the rate of orbit of planet %d\n", planetsnumber);
		scanf("%f", &angle);
		printf("Phase\n");
		scanf("%f", &phase);

		double Xcos = angle * timepassed * M_PI + phase * M_PI;  //Don't mind the variable declaration here.
		Xcoord = range * cos(Xcos);  //The Xcoord function is different but works like the Ycoord one does.  I dun goof'd on this one.
		Ycoord = getYcoord(timepassed, range, angle, phase);

		fprintf(ifp, "\nPlanet %d = (%f,%f)\n", planetsnumber, Xcoord, Ycoord);  //Record the planet's number and coordinates.

		planets -= 1;
		planetsnumber++;  //Moving to the next planet, planet n+1!
	}

	while (gates != 0)  //This is exactly the same as the previous section but Gate has no number with it, it has the system it points to attached to it
				 	  //ie, Serpentis Stargate = (0.000,1.000) is a stargate sitting at y= 1, x = 0 which leads to the Serpentis Starsystem
	{


		printf("Where does this gate lead to?");
		scanf("%s", entitydescription);  //Bug: Space here will cause super damage points for anyone's harddrive


		printf("How far is %s gate?(AU)\n", entitydescription);
		scanf("%f", &range);
		printf("What is the orbit rate of the %s gate?\n", entitydescription);
		scanf("%f", &angle);
		printf("What is the Phase offset of the %s gate?\n", entitydescription);
		scanf("%f", &phase);
		Xcoord = getXcoord(timepassed, range, angle, phase);
		Ycoord = getYcoord(timepassed, range, angle, phase);
		fprintf(ifp, "\n%s Gate = (%f,%f)\n", entitydescription, Xcoord, Ycoord);

		gates -= 1;
		gatesnumber++;
	}

	fclose(ifp);
	//close output file and go back to menu
}


/*
	   Reads the Following from a .ssdata file
	   And converts it into a .ssref file which has the format 'Gate/Planet X = (Xcoord, Ycoord) \n  --- Super useful yo.
	   Type(this is a char of P or g or q)

if P
	   PlanetNumber
	   Distance
	   Orbit
	   Phase
	   Timepassed
if g
	   GateNumber
	   Destination
	   Distance
	   Orbit
	   Phase
if q
	Finish
	*/
void locaterfromfile()
{
	FILE *dfp;	//Input Datafile
	char dataname[100];	//Datafile filename
	char systemname[100];
	char sysfilename[100];
	char gatedest[100];
	int gatecount;
	float orbitdistance, orbitrate, phase;
	float timepassed;
	float Xcoord;
	float Ycoord;
//	float angle = 0;
//	float range = 0;
	int planetsnumber = 0;
	double Xcos;
	char buf[255];
	FILE *ifp;
	printf("Input .ssdata file: \n$");
	scanf("%s", dataname);
	strcpy(systemname, dataname);
	strcat(dataname, ".ssdata");
	dfp = fopen(dataname, "r");
		if (dfp == NULL)
	{
		printf("Cannot Open File, Aborting like a mofo at a disco...\n");
		prompt();
	}
	if(dfp!=NULL)
	{
	strcpy(sysfilename, systemname);
	strcat(sysfilename, ".ssref");
	ifp = fopen(sysfilename, "w");



	printf("How much has time has passed?");
	scanf("%f",&timepassed);

	fprintf(ifp, "System - %s\n", dataname);	//Outputs System Name to file

	int chooser;
//	long strstatus;
	while (1)
	{

		chooser = fgetc(dfp);
		//strstatus = ftell(dfp);
		//printf("%ld",strstatus);

		if (chooser == 'P')
		{

			fgets(buf,255,dfp);
			//puts(buf);
			//printf("\nBuffer String :%s\n",buf);
			sscanf(buf,"%d %8f %8f %8f",&planetsnumber,&orbitdistance,&orbitrate,&phase);

			Xcos = orbitrate * timepassed * M_PI + phase * M_PI;
			Xcoord = orbitdistance * cos(Xcos);
			Ycoord = getYcoord(timepassed, orbitdistance, orbitrate, phase);
			printf("Planet %d = (%f,%f)\n", planetsnumber, Xcoord, Ycoord);
			fprintf(ifp,"Planet %d = (%f,%f)\n",planetsnumber, Xcoord, Ycoord);


		}
		if (chooser == 'g')
		{
			//This needs to fixed for the string
			//fscanf(ifp, "%d\n%s\n%8f\n%8f\n%8f\n", &gatecount, gatedest, &orbitdistance, &orbitrate, &phase);
				fgets(buf,255,dfp);
				//puts(buf);
			//	printf("\nBuffer String :%s\n",buf);
				sscanf(buf,"%d %f %f %f %s",&gatecount,&orbitdistance,&orbitrate,&phase,gatedest);

			Xcos = orbitrate * timepassed * M_PI + phase * M_PI;
			Xcoord = orbitdistance * cos(Xcos);
			Ycoord = getYcoord(timepassed, orbitdistance, orbitrate, phase);

			printf("\n%s Gate(%d) = (%f,%f)\n", gatedest, gatecount, Xcoord, Ycoord);
			fprintf(ifp, "\n%s Gate(%d) = (%f,%f)\n", gatedest, gatecount, Xcoord, Ycoord);
		}
		if (chooser =='q')
			break;



	}
	fclose(ifp);
	}
	fclose(dfp);
}





//Gets the X coordinates from the polar equation using timepassed.  I Think this isn't used but is identical to getYcoord
double getXcoord(float timepassed, float range, float orbitmod, float phase)
{
	float X;
	double Xcos = timepassed * orbitmod * 3.141592653589 + phase * 3.141592653589;
	X = range * cos(Xcos);


	return X;
}
//getYcoord
double getYcoord(float timepassed, float range, float orbitmod, float phase)
{
	double Ysin = timepassed * orbitmod * 3.141592653589 + phase * 3.141592653589;
	double Y = range * sin(Ysin);

	return Y;
}




void error()	//Done, outputs error message
{
	printf("\nError\n");

}

void prompt()  //Outputs a $ like most prompts... sometimes I skip this but it is needed on all scanf prompts
{
	printf("$ ");
}
//&breathability,breathcondition
void fbreathcondition(float *breathval,char breathvalue[])
{

if( *breathval <= 56)
    strcpy(breathvalue,"Lethal");
if(*breathval > 56 && *breathval <= 68)
    strcpy(breathvalue,"Breathing Aid required");
if(*breathval > 68 && *breathval <= 100)
    strcpy(breathvalue,"Terrans will be able to breath");
}
void planetinfogen(float *tempval, float *sizeval, float *densityval, float *breathval, float *acidval, float *gravityval,
                   int *TLval, char name[], char *nameval,float *presval)
{
    char has_atmosphere;

    //Planet name
    printf("Does the planet have a name?\n");
    prompt();
    scanf("%c",nameval);
    scanf("%c",nameval);
    //printf("%c",*nameval);

if(*nameval == 'y' || *nameval == 'Y')
{
    printf("What is the name?\n");
    prompt();
    scanf("%s",name);
}
/*printf("What is the tech level of the planet?");*/
/*prompt();*/
//scanf("%d",TLval);

//Planet Gravity And Size

printf("What is the size of the planet in terms of Earth?");
prompt();
scanf("%f",sizeval);
printf("What is the density of the planet in terms of Earth?");
prompt();
scanf("%f",densityval);
*gravityval = *densityval * (*sizeval) * 9.8;
*sizeval *= 6360;//in km from center

//Air Stuff
printf("Does the planet have an atmosphere?\n");
prompt();
scanf("%c",&has_atmosphere);
scanf("%c",&has_atmosphere);

if(has_atmosphere == 'y' || has_atmosphere == 'Y')
{
	printf("How much acid is there in the air(%%tage)?\n");
	prompt();

	scanf("%f",acidval);

	printf("How Breathable is the air(%%tage)?\n");
	prompt();

	scanf("%f",breathval);

	printf("What is the pressure like compared to Earth(Multiplier)?");
	prompt();
	scanf("%f",presval);
}

if(has_atmosphere == 'n' || has_atmosphere == 'N' || *acidval >= 30)
    *breathval = 0;
if(has_atmosphere == 'n' || has_atmosphere == 'N')
    {
    *presval = 0;
    *acidval = 0;
    }
    //printf("%f Breathability", *breathval);
}


void planetinfogen_random(float *tempval, float *sizeval, float *densityval, float *breathval, float *acidval, float *gravityval,
                   int *TLval, char name[], char *nameval,float *presval)
{

	
    //Planet name
   // printf("Does the planet have a name?\n");
   // prompt();
   // scanf("%c",nameval);
   // scanf("%c",nameval);
    //printf("%c",*nameval);
	
	srand(time(NULL));


/*
if(*nameval == 'y' || *nameval == 'Y')
{
    printf("What is the name?\n");
    prompt();
    scanf("%s",name);
}
*/
/*printf("What is the tech level of the planet?");*/
/*prompt();*/
//scanf("%d",TLval);

//Planet Gravity And Size

printf("What is the size of the planet in terms of Earth?");
prompt();
scanf("%f",sizeval);
printf("What is the density of the planet in terms of Earth?");
prompt();
scanf("%f",densityval);
*gravityval = *densityval * (*sizeval) * 9.8;
*sizeval *= 6360;//in km from center


int no_atm = rand() % 3;
//Air Stuff
	if( no_atm  == 0)
	{
		acidval = (float *)(rand() % 101);
		
		srand(time(NULL)+(int)acidval);

		breathval = (float *)(rand() % 101);
		
		srand(time(NULL)+(int)breathval);

	printf("What is the pressure like compared to Earth(Multiplier)?");
	prompt();
	scanf("%f",presval);
	}

	else if( no_atm  == 1 || no_atm == 2 || *acidval >= 30)
    *breathval = 0;
	if( no_atm  == 1 || no_atm == 2)
    {
    *presval = 0;
    *acidval = 0;
    }
    //printf("%f Breathability", *breathval);
}

void fpressure(float *presval,char readout[])
{
	if(*presval >= 0.1 && *presval <= 0.56)
		strcpy(readout,"Low Pressure");
	if(*presval > 0.56 && *presval <= 1.32)
		strcpy(readout,"Medium Pressure");
	if(*presval > 1.32 && *presval <= 2)
		strcpy(readout,"Heavy Pressure");
	if(*presval > 2)
		strcpy(readout,"Inhospitable Pressure");
	if(*presval < 0.1)
		strcpy(readout,"No Pressure");
}
void ftemperature(float *tempval,float *sizeval,float *orbitval, char heatreadout[],float *ptempval)
{
    float heat;
    *orbitval /= 149597870;
//    printf("%f,%f,%f,%f",*tempval,*sizeval,*orbitval,*ptempval);
    heat = sqrt((double)*tempval)*sqrt((double)*sizeval)*288*pow(*orbitval,-0.835) - 273; //Is in K but converts to C
    //printf("%f",heat);
    if(heat < -50)
        strcpy(heatreadout,"Far too cold for Terrans (less than -50C)");
    if(heat >=-50 && heat < -25)
        strcpy(heatreadout,"Extremely Cold, Need protective gear");
    if(heat >=-25 && heat < 0)
        strcpy(heatreadout,"Chilly");
    if(heat >=0 && heat < 10)
       strcpy(heatreadout,"Cool");
    if(heat >=10 && heat < 25)
        strcpy(heatreadout,"Temperate");
    if(heat >=25 && heat < 45)
        strcpy(heatreadout,"Uncomfortable heat");
    if(heat >=45 && heat < 65)
        strcpy(heatreadout,"Need Protective Gear from the extreme heat");
    if(heat >=65 && heat < 180)
        strcpy(heatreadout,"Too hot for Terrans, okay for vehicles");
    if(heat>180)
        strcpy(heatreadout,"Too hot for Terran vehicles");
    *ptempval = heat;
    //printf("%f",*ptempval);

}
void spawn()	//Spawns Gates and Planets, puts them in a user defined file with extensions .ss and .ssdata, .ss is readable for humans and .ssdata is to be machine-read
{
	int planetcount;
	int gatecount;
//  float min;
//  float max;
	char name[maxstring];
	FILE *ifp;
	FILE *data;
	int entitycount = 0;
	char gatedest[100];	//Needs to be addressed

	float distance = 0;
	float angularspeed = 0;	//Angular speed factor for the polar function
	float phase = 0;
	float speed;	//Orbital Speed
	float acceleration;	//orbital acceleration
	float timetaken;
	char startype[100];
	char size[100];
	char dataname[100];
	char sname[100];

	float Temperature;
	float RelSize;
	float HZM; //Habitable Zone Multiplier
	float min;
	float max;
//Planetvariables follow
float pressure;
float planet_temp;
float planet_size;
float planet_density;
float breathability;
float planet_acidity;
float planet_gravity;
int planet_techlevel;
char planet_name[100];
char has_name;
char breathcondition[100];
char pressurecondition[100];
char temperaturecondition[100];

float radius;


	printf("What is the name of the system (No Spaces) \n");	//Needs String
	prompt();
	scanf("%s", name);
	strcpy(dataname, name);
	strcpy(sname, name);
	strcat(dataname, ".ssdata");

	data = fopen(dataname, "w");




	strcat(name, ".ss");
	ifp = fopen(name, "w");



while(1)
{

	printf("What is the Temperature multiplier for the star in terms of Sol?\nSol=1\nWhiteStar=1.375\nBlue=4\nRed=0.5\n");
	scanf("%f",&Temperature);


	if(Temperature <= 0.5)
			strcpy(startype,"Red");
	if(Temperature > 0.5 && Temperature <= 0.66)
			strcpy(startype,"Orange");
	if(Temperature > 0.66 && Temperature <= 1.375)
			strcpy(startype,"Yellow");
	if(Temperature > 1.375 && Temperature <= 4)
			strcpy(startype,"White");
	if(Temperature > 4 && Temperature <= 8)
			strcpy(startype,"Blue");
	if(Temperature > 8)
			strcpy(startype,"Violet");
	break;


}
while(1)
{
	printf("\nWhat is the star size in terms of Sol?  \n");
	prompt();
	scanf("%f", &RelSize);

	if(RelSize <= 0.5)
			{
			strcpy(size,"Small");
			}

	if(RelSize > 0.5 && RelSize < 1.5)
			strcpy(size,"Medium");


	if(RelSize >= 1.5 && RelSize < 3)
			strcpy(size,"Large");

	if(RelSize >= 3 && RelSize < 6)
			strcpy(size,"Big");

	if(RelSize >= 6 && RelSize < 12)
			strcpy(size,"Red Giant");

	if(RelSize >= 12)
			strcpy(size,"Supergiant");

	break;
}
	HZM = sqrt(RelSize)*sqrt(Temperature);
//This is dealt with.  Do NOT fuck this code up.
	min = (65+273)/(HZM*288);
	min = pow(min,1/-0.835);
float mintemp = HZM*288*pow(min,-0.835)-273;
	max = (-15+273)/(HZM*288);
	max = pow(max,1/-0.835);
float maxtemp = HZM*288*pow(max,-0.835)-273;

	radius = RelSize*69550/(2*149597870);
	//printf("DEBUG=%f",min);
	//printf("DEBUG=%f",max);
	//printf("DEBUG=%f",radius);
	printf("\nHabitable zone (%f (Temp %f C) - %f (Temp %f C))\n\n", min,mintemp, max,maxtemp);
	fprintf(ifp, "\nSystem Name : %s -- (%s %s)  \nHabitable Zone Min = %f  AU (Temp %f C) \nHabitable Zone Max = %f AU (Temp %f C)\nStar Radius = %f AU", name, startype, size,min,mintemp,max,maxtemp,radius);
	//fprintf(data,"%s\n",sname);




	fclose(data);
	fclose(ifp);






	/*
	   //This sets the range of how planets are far from
	   printf("How many AU's does the first planet sit at\n");
	   prompt();
	   scanf("%f",&min);


	   printf("How many AU's is the upper bound\n");
	   prompt();
	   scanf("%f",&max);
	 */
	printf("How many planets are there in this system?\n");	//Planet Spawner
	prompt();
	scanf("%d", &planetcount);


	printf("How many stargates are here? \n");
	prompt();
	scanf("%d", &gatecount);




	while (gatecount > 0 || planetcount > 0)
	{
		data = fopen(dataname, "a");
		while (planetcount > 0)
		{

			ifp = fopen(name, "a");
			entitycount += 1;
			printf("Planet %d", entitycount);
			fprintf(ifp, "\n\nPlanet %d \n", entitycount);

			while (1)
			{
			printf("\nHow far is the planet?(AU)\n $ ");
			distance = distancespawn(distance);
			if(distance > radius)
				break;
			else
				printf("Too close to star, radius is %f", radius);
			}
			if(distance >= min && distance <= max)
				fprintf(ifp, "\nDistance from Primary Star : %f AU  (HABITABLE PLANET)", distance);
			else
				fprintf(ifp, "\nDistance from Primary Star : %f AU", distance);
			printf("\nHow many days does it take to make a complete revolution?\n $ ");
			angularspeed = 2 / (angularspeedspawn(angularspeed));
			fprintf(ifp, "\nPolar Orbital rate per day passed (in terms of pi) : %f", angularspeed);	//In the equation position(t)=Rcis(mt), this will be the m, t will be time passed)

			printf("\nWhere is it at in its orbit in days?\n $ ");

			phase = phasespawn(phase)*(angularspeed);

			fprintf(ifp, "\nPolar Phase offset is %f pi", phase);


			fprintf(data, "P %d %f %f %f\n", entitycount, distance, angularspeed, phase);
			distance = 149597870 * distance;
			timetaken = 1 / (angularspeed * 0.5);
			speed = (2000 * 3.141592653589 * distance / (timetaken * (24 * 3600)));	//m/s
			planetinfogen(&planet_temp,&planet_size,&planet_density,&breathability,&planet_acidity,&planet_gravity,&planet_techlevel,planet_name,&has_name,&pressure);
            fbreathcondition(&breathability,breathcondition);
			acceleration = (speed * speed) / (distance * 1000);
			fprintf(ifp, "\nVelocity is %f m/s \nAcceleration is %f m/s2", speed, acceleration);
			if(has_name == 'y' || has_name == 'Y')
                fprintf(ifp, "\n\nMisc Details of Currentplanet:\nName : %s \n",planet_name);
            fpressure(&pressure,pressurecondition);
            ftemperature(&Temperature, &RelSize,&distance, temperaturecondition, &planet_temp);

            fprintf(ifp,"Planet Radius: %f km\nGravity %f m/s^2\nBreathability: %s\nPressure: %s \nPlanet Temperature: %.1f \nTemperature Status %s \nAcidity %.1f%% \n",
                    planet_size,planet_gravity, breathcondition,pressurecondition,planet_temp,temperaturecondition,planet_acidity);
            printf("%f\n%s",planet_temp,temperaturecondition);
			//Planet misc values are posted here, oh boy this is gonna be good


			planetcount -= 1;
			fclose(ifp);
		}

		entitycount = 0;
		printf(" \n\n\n\n");
		while (gatecount > 0)
		{
			ifp = fopen(name, "a");
			entitycount += 1;

			printf("Gate %d", entitycount);
			printf("\nWhere does this gate go to?\n $");
			scanf("%s", gatedest);
			fprintf(ifp, "\n\nGate %d - (%s) \n", entitycount, gatedest);
while(1)
{
			printf("\nHow far is the gate?(AU)\n $ ");
			distance = distancespawn(distance);
			if(distance > radius)
				break;
			else
				printf("Too close to star, radius is %f", radius);
}

			fprintf(ifp, "\nDistance from Primary Star : %f AU", distance);

			printf("\nHow many days does it take to make a complete revolution?\n $ ");
			angularspeed = 2 / angularspeedspawn(angularspeed);
			fprintf(ifp, "\nRotation Rate per day passed (in terms of pi) : %f", angularspeed);

			printf("\nWhat is its starting point?\n $ ");
			phase = phasespawn(phase)*(angularspeed);
			fprintf(ifp, "\nPhase offset is %f pi", phase);
			fprintf(data, "g %d %f %f %f %s\n", entitycount, distance, angularspeed, phase, gatedest);

			gatecount -= 1;

			distance = 149597870 * distance;
			timetaken = 1 / (angularspeed * 0.5);
			speed = (2000 * 3.141592653589 * distance / (timetaken * (24 * 3600)));	//m/s


			acceleration = (speed * speed) / (distance * 1000);
			fprintf(ifp, "\nVelocity is %f m/s \nAcceleration is %f m/s2", speed, acceleration);

			fclose(ifp);
		}
		fprintf(data,"\nq");
		fclose(data);
	}





}



float distancespawn(float distance)
{
	scanf("%f", &distance);

	return distance;
}

float angularspeedspawn(float angularspeed)
{
	float daysspent;

	scanf("%f", &daysspent);



	if (daysspent == 0)
		angularspeed = 0;
	else
		angularspeed = daysspent;
	return angularspeed;
}

float phasespawn(float phase)
{
	float phasesp = phase;

	float daysspent;

	scanf("%f", &daysspent);



	if (daysspent == 0)
		phasesp = 0;
	else
		phasesp = daysspent;



	return phasesp;
}


int quit(int a)
{
	a = 1;
	return a;
}


void spawn_random()	//Spawns Gates and Planets, puts them in a user defined file with extensions .ss and .ssdata, .ss is readable for humans and .ssdata is to be machine-read
{
	int planetcount;
	int gatecount;
//  float min;
//  float max;
	char name[maxstring];
	FILE *ifp;
	FILE *data;
	int entitycount = 0;
	char gatedest[100];	//Needs to be addressed

	float distance = 0;
	float angularspeed = 0;	//Angular speed factor for the polar function
	float phase = 0;
	float speed;	//Orbital Speed
	float acceleration;	//orbital acceleration
	float timetaken;
	char startype[100];
	char size[100];
	char dataname[100];
	char sname[100];

	float Temperature;
	float RelSize;
	float HZM; //Habitable Zone Multiplier
	float min;
	float max;
//Planetvariables follow
float pressure;
float planet_temp;
float planet_size;
float planet_density;
float breathability;
float planet_acidity;
float planet_gravity;
int planet_techlevel;
char planet_name[100];
char has_name;
char breathcondition[100];
char pressurecondition[100];
char temperaturecondition[100];

float radius;


	printf("What is the name of the system (No Spaces) \n");	//Needs String
	prompt();
	scanf("%s", name);
	strcpy(dataname, name);
	strcpy(sname, name);
	strcat(dataname, ".ssdata");

	data = fopen(dataname, "w");




	strcat(name, ".ss");
	ifp = fopen(name, "w");



while(1) //Temp Multiplier
{

	printf("What is the Temperature multiplier for the star in terms of Sol?\nSol=1\nWhiteStar=1.375\nBlue=4\nRed=0.5\n");
	scanf("%f",&Temperature);


	if(Temperature <= 0.5)
			strcpy(startype,"Red");
	if(Temperature > 0.5 && Temperature <= 0.66)
			strcpy(startype,"Orange");
	if(Temperature > 0.66 && Temperature <= 1.375)
			strcpy(startype,"Yellow");
	if(Temperature > 1.375 && Temperature <= 4)
			strcpy(startype,"White");
	if(Temperature > 4 && Temperature <= 8)
			strcpy(startype,"Blue");
	if(Temperature > 8)
			strcpy(startype,"Violet");
	break;


}
while(1) //Star Size
{
	printf("\nWhat is the star size in terms of Sol?  \n");
	prompt();
	scanf("%f", &RelSize);

	if(RelSize <= 0.5)
			{
			strcpy(size,"Small");
			}

	if(RelSize > 0.5 && RelSize < 1.5)
			strcpy(size,"Medium");


	if(RelSize >= 1.5 && RelSize < 3)
			strcpy(size,"Large");

	if(RelSize >= 3 && RelSize < 6)
			strcpy(size,"Big");

	if(RelSize >= 6 && RelSize < 12)
			strcpy(size,"Red Giant");

	if(RelSize >= 12)
			strcpy(size,"Supergiant");

	break;
}
	HZM = sqrt(RelSize)*sqrt(Temperature);
//This is dealt with.  Do NOT fuck this code up.
	min = (65+273)/(HZM*288);
	min = pow(min,1/-0.835);
float mintemp = HZM*288*pow(min,-0.835)-273;
	max = (-15+273)/(HZM*288);
	max = pow(max,1/-0.835);
float maxtemp = HZM*288*pow(max,-0.835)-273;

	radius = RelSize*69550/(2*149597870);
	//printf("DEBUG=%f",min);
	//printf("DEBUG=%f",max);
	//printf("DEBUG=%f",radius);
	printf("\nHabitable zone (%f (Temp %f C) - %f (Temp %f C))\n\n", min,mintemp, max,maxtemp);
	fprintf(ifp, "\nSystem Name : %s -- (%s %s)  \nHabitable Zone Min = %f  AU (Temp %f C) \nHabitable Zone Max = %f AU (Temp %f C)\nStar Radius = %f AU", name, startype, size,min,mintemp,max,maxtemp,radius);
	//fprintf(data,"%s\n",sname);




	fclose(data);
	fclose(ifp);






	/*
	   //This sets the range of how planets are far from
	   printf("How many AU's does the first planet sit at\n");
	   prompt();
	   scanf("%f",&min);


	   printf("How many AU's is the upper bound\n");
	   prompt();
	   scanf("%f",&max);
	 */
	printf("How many planets are there in this system?\n");	//Planet Spawner
	prompt();
	scanf("%d", &planetcount);


	printf("How many stargates are here? \n");
	prompt();
	scanf("%d", &gatecount);

	float min_dist = 0;
	float max_dist = 0; 
	// rand() % max-min + min;  //for general equation of increasing distance


	while (gatecount > 0 || planetcount > 0)
	{
		data = fopen(dataname, "a");
		while (planetcount > 0)
		{ 
			//MIN & MAX DISTANCE SETTING BLOCK
				if(max_dist == 0 && max_dist == 0)  //Part of Random Part
				{
					printf("Max Distance for Planets? \n $ ");
					scanf("%f",&max_dist);
					
					
					
					printf("Min Distance for Planets? \n $ ");
					scanf("%f",&min_dist);
					
					if(min_dist <= radius)
						min_dist = 1.01*radius;
					
				
				//Debug block
				
				{
					printf("\n%f\n%f\n",min_dist,max_dist);
				}
				
				min_dist *= 100000;
				max_dist *= 100000;
				
					//Debug block
					{
						printf("\n%f\n%f\n",min_dist,max_dist);
					}
						//if the min is less than the star's radius, the min radius will be placed 101% star radius out of it
				}
			
			
			/*
			For the purposes of the rand(), need to multiply min_dist and max_dist by 100000 
			
			Once this is done, reduce it down to AU again
			
			This process might suck on a 32 bit machine and might need to be compiled for x64
			*/
			srand(time(NULL)+entitycount);
			ifp = fopen(name, "a");
			entitycount += 1;
			printf("\nPlanet %d\n", entitycount);
			fprintf(ifp, "\n\nPlanet %d \n", entitycount);

			
			distance = (float)(rand() % ((int)max_dist - (int)min_dist) + (int)min_dist)/100000;  //Looks good
			
			
			//min_dist = distance*100000; //matching minimum distance
		
			
			if(distance >= min && distance <= max)
				fprintf(ifp, "\nDistance from Primary Star : %f AU  (HABITABLE PLANET)", distance);
			else
				fprintf(ifp, "\nDistance from Primary Star : %f AU", distance);
			//printf("\nHow many days does it take to make a complete revolution?\n $ ");
			
			

			
			angularspeed = 2 / (365*pow(distance,1.504)); //Input is days
				fprintf(ifp, "Days of Orbit : %f days \n", (365*pow(distance,1.504)));
			if(((rand() % 3) -1) == 0) //either 1 or 2 or 0 or 1, 1 is positive/anticlockwise rotation
				{
				angularspeed *= -1;
				}
				
				
			fprintf(ifp, "\nPolar Orbital rate per day passed (in terms of pi) : %f", angularspeed);	//In the equation position(t)=Rcis(mt), this will be the m, t will be time passed)

			//printf("\nWhere is it at in its orbit in days?\n $ ");

			phase = ((365*pow(distance,1.504))*(rand() % 201 - 100) / 100)*(angularspeed);  //Double check this

			fprintf(ifp, "\nPolar Phase offset is %f pi", phase);


			fprintf(data, "P %d %f %f %f\n", entitycount, distance, angularspeed, phase);
			distance = 149597870 * distance;
			timetaken = 1 / (angularspeed * 0.5);
			speed = (2000 * 3.141592653589 * distance / (timetaken * (24 * 3600)));	//m/s
			
			
			planetinfogen_random(&planet_temp,&planet_size,&planet_density,&breathability,&planet_acidity,&planet_gravity,&planet_techlevel,planet_name,&has_name,&pressure);
            fbreathcondition(&breathability,breathcondition);
			
			acceleration = (speed * speed) / (distance * 1000);
			
			fprintf(ifp, "\nVelocity is %f m/s \nAcceleration is %f m/s2", speed, acceleration);
			
			if(has_name == 'y' || has_name == 'Y')
                fprintf(ifp, "\n\nMisc Details of Currentplanet:\nName : %s \n",planet_name);
				
            fpressure(&pressure,pressurecondition);
			
            ftemperature(&Temperature, &RelSize,&distance, temperaturecondition, &planet_temp);

            fprintf(ifp,"Planet Radius: %f km\nGravity %f m/s^2\nBreathability: %s\nPressure: %s \nPlanet Temperature: %.1f \nTemperature Status %s \nAcidity %.1f%% \n",
                    planet_size,planet_gravity, breathcondition,pressurecondition,planet_temp,temperaturecondition,planet_acidity);
            printf("%f\n%s",planet_temp,temperaturecondition);
			//Planet misc values are posted here, oh boy this is gonna be good


			planetcount -= 1;
			fclose(ifp);
		}
		min_dist = 0;
		max_dist = 0;
		entitycount = 0;
		printf(" \n\n\n\n");
		while (gatecount > 0)
		{
		
			{//MIN & MAX DISTANCE SETTING BLOCK
				if(max_dist == 0 && max_dist == 0)  //Part of Random Part
				{
					printf("Max Distance for Gates? \n $ ");
					scanf("%f",&max_dist);
					
					
					
					printf("Min Distance for Gates? \n $ ");
					scanf("%f",&min_dist);
					
					if(min_dist <= radius)
						min_dist = 1.01*radius;
					
				
				//Debug block
				
				{
					printf("\n%f\n%f\n",min_dist,max_dist);
				}
				
				min_dist *= 100000;
				max_dist *= 100000;
				
					//Debug block
					{
						printf("\n%f\n%f\n",min_dist,max_dist);
					}
						//if the min is less than the star's radius, the min radius will be placed 101% star radius out of it
				}
			}
			srand(time(NULL)+entitycount);
			ifp = fopen(name, "a");
			entitycount += 1;
			
			printf("Gate %d", entitycount);
			printf("\nWhere does this gate go to?\n $");
			scanf("%s", gatedest);
			fprintf(ifp, "\n\nGate %d - (%s) \n", entitycount, gatedest);

			//printf("\nHow far is the gate?(AU)\n $ ");
			
			
			distance = (float)(rand() % ((int)max_dist - (int)min_dist) + min_dist)/100000;  //Looks good
			
			min_dist = distance*100000; //matching minimum distance

			fprintf(ifp, "\nDistance from Primary Star : %f AU", distance);

	

			
			angularspeed = 2 / (365*pow(distance,1.504)); //Input is days
			
			if(((rand() % 3) -1) == 0) //either 1 or 2 or 0 or 1, 1 is positive/anticlockwise rotation
				{
				angularspeed *= -1;
				}
				
				
			fprintf(ifp, "\nRotation Rate per day passed (in terms of pi) : %f", angularspeed);

			//printf("\nWhat is its starting point?\n $ ");
			phase = ((365*pow(distance,1.504))*(rand() % 201 - 100) / 100)*(angularspeed);  //Double check this
			fprintf(ifp, "\nPhase offset is %f pi", phase);
			fprintf(data, "g %d %f %f %f %s\n", entitycount, distance, angularspeed, phase, gatedest);

			gatecount -= 1;

			distance = 149597870 * distance;
			timetaken = 1 / (angularspeed * 0.5);
			speed = (2000 * 3.141592653589 * distance / (timetaken * (24 * 3600)));	//m/s


			acceleration = (speed * speed) / (distance * 1000);
			fprintf(ifp, "\nVelocity is %f m/s \nAcceleration is %f m/s2", speed, acceleration);

			fclose(ifp);
		}
		fprintf(data,"\nq");
		fclose(data);
	}





}





