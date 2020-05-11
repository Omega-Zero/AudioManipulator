#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "wave.h"

/***************
* Jeffrey Marple 
* Project 4: Wave
****************/



//--Prototypes--//
void reverse(short *array, int arraySize);
void flipChannels(short *array1, short *array2, int arraySize);

short* changeSpeed(short *array, int length, double factor);
void scaleVolume(short *array, int arraySize, double scale);
	
void fadeOut(short *array, int arraySize, double seconds);
void fadeIn(short *array, int arraySize, double seconds);
short* echo(short *array, int arraySize, double delay, double scale);

short readShort();
void writeBytes(short num);




//command line main
int main(int argc, char* argv[]){
		
		WaveHeader header;
		readHeader(&header);

		int arraySize = (header.dataChunk.size)/4 ;
		

		 //-----------testing-----------------------------------------------//
	    // this is where I'll recieve the 
	   //initial wave array/allocate memory and find its arraySize in the future. 

		short *shortArray1;
		short *shortArray2;

		shortArray1 = malloc(arraySize * sizeof(short)); //create test shortArray
		shortArray2 = malloc(arraySize * sizeof(short));

		if(shortArray1 == NULL || shortArray2 ==NULL)                     {
     	  fprintf(stderr, "Program out of memory");
      	  exit(0);
 	   }		
		

		//read in data
		for(int i=0; i<arraySize; i++){
			shortArray1[i] = readShort();
			shortArray2[i] = readShort();
			
		}

		
	
				

		//-------end testing----------------------------------------------------------------------//




		//starts checking every argument
		for(int i=1; i<argc-1; i++){



				//reverse
				if ( strcmp(argv[i], "-r") == 0){
				

					reverse(shortArray1, arraySize);
					reverse(shortArray2, arraySize);


				//speed(double factor) 
				}else if ( strcmp(argv[i], "-s") == 0){
					


					++i;
					//check for number for next argument
					if ( atof(argv[i]) > 0 ) {//argument is number, converted into double && not negative
						changeSpeed(shortArray1, arraySize, atof(argv[i]) );
						changeSpeed(shortArray2, arraySize, atof(argv[i]) );
					}else{//argument following arg is negative or not a number				
							fprintf(stderr, "A positive number must be supplied for the speed change\n"); 
					}		
					
		




			
				//flips
				}else if (( strcmp(argv[i], "-f") == 0)){
					flipChannels(shortArray1, shortArray2, arraySize);
		


				//fadeOut(double time) 
				}else if ( strcmp(argv[i], "-o") == 0){
		
					++i;

					if (  atof(argv[i]) > 0   ) {//arguments are numbers, converted into double && not negative
						fadeOut(shortArray1, arraySize, atof(argv[i]) );
						fadeOut(shortArray2, arraySize, atof(argv[i]) );

					}else{//argument following arg is negative or not a number					
							fprintf(stderr, "A positive number must be supplied for the fade in and fade out time\n"); 
					}


				
				//fadeIn(double time) 
				}else if ( strcmp(argv[i], "-i") == 0){
					
					++i;
					if (  atof(argv[i]) > 0  ) {//arguments are numbers, converted into double && not negative
						fadeIn(shortArray1, arraySize, atof(argv[i]) );
						fadeIn(shortArray2, arraySize, atof(argv[i]) );

					}else{//argument following arg is negative or not a number					
						fprintf(stderr, "A positive number must be supplied for the fade in and fade out time\n"); 
					}



				//Volume(double scale)
				}else if ( strcmp(argv[i], "-v") == 0) {

					++i;
					if ( atof(argv[i]) > 0 ) {//argument is number, converted into double && not negative
						scaleVolume(shortArray1, arraySize, atof(argv[i]));
						scaleVolume(shortArray2, arraySize, atof(argv[i]));

					}else{//argument following arg is negative or not a number					
							fprintf(stderr, "A positive number must be supplied for the volume scale\n"); 
					}
		
		


				//echo(double delay, double scale)
				}else if ( strcmp(argv[i], "-e") == 0){
					
					
					++i;
					if (  (atof(argv[i]) > 0) && (atof(argv[i+1]) > 0)  ) {//arguments are numbers, converted into double && not negative
						
						echo(shortArray1, arraySize, atof(argv[i]), atof(argv[i+1]));
						echo(shortArray2, arraySize, atof(argv[i]), atof(argv[i+1]));
						++i;
					}else{//argument following arg is negative or not a number	
							fprintf(stderr, "Positive numbers must be supplied for the echo delay and scale parameters\n"); 
					}

				



				//Command other than the seven recognized commands was given:		
				}else{
					

						fprintf(stderr, "Usage: wave [[-r][-s factor][-f][-o delay][-i delay][-v scale][-e delay scale] < input > output\n");

					

				}//end arguement if check 
			



		}//end argument count for loop




	writeHeader(&header);	
	//output 
	for(int i=0; i<arraySize; i++){
			writeBytes(shortArray1[i]);
			writeBytes(shortArray2[i]);
			
	}




	//FREEDOMMMMMM
	free(shortArray1);
	free(shortArray2);

}//end main()





//Reverse the audio
void reverse(short *array, int arraySize){
	fprintf(stderr,"r: reverse audio called\n");
	
	int end = arraySize-1;
	int start = 0;
	int temp = 0;

	//flips the first and last indexes until n/2
	while (start < end){ 
        temp = array[start];    
        array[start] = array[end]; 
        array[end] = temp;
	 
        start++; 
        end--; 
    }
	

}//end reverse()




//Change audio speed by a specific factor
short* changeSpeed(short *array, int length, double factor){

	double newSize	= length/factor;
	short *newArray = malloc(newSize * sizeof(short));

	if(newArray == NULL){
     	  fprintf(stderr, "Program out of memory");
      	  exit(0);
 	}	

	for(int i=0; i<length; i++){

		// fill newarrays with values from the original arrays
		newArray[i*(int)(factor)] = array[i];
		

	}



		//Both the size and the dataChunk.size values should be updated.
	

	//free old array
	free(array);

	//fprintf(stderr, "s: changeSpeed called\n");
	return *newArray;
}//end changeSpeed()




//Flip the left and the right stereo channels
void flipChannels(short *array1, short *array2, int arraySize){

	short *temp = malloc(arraySize*sizeof(short));
	if(temp == NULL){
     	  fprintf(stderr, "Program out of memory");
      	  exit(0);
 	}
	
	for(int i=0; i<arraySize; i++){
		temp[i] = array1[i];
		array1[i] = array2[i];
		array2[i] = temp[i];
		
	}
	free(temp);
	
	//fprintf(stderr,"f: flipChannels called\n");
}//end flipChannels





//Fade out from full volume to zero at the end of the file over a specified time
void fadeOut(short *array, int arraySize, double seconds){
	
	double fadeSample = floor(seconds * 44100);
	double fadeStart = arraySize-fadeSample-1;

	for(int i=fadeStart; i<arraySize; i++){	
		array[i] =  array[i]*( pow( 1-((i-fadeStart)/fadeSample) , 2)  );
	}





	//fprintf(stderr,"o: fadeOut called\n");
}//end fadeOut()







//Fade in from zero to full volume at the beginning of the file over a specified time
void fadeIn(short *array, int arraySize, double seconds){

	double fadeSample = floor(seconds * 44100);
	
	for(int i=0; i<fadeSample-1; i++){
		array[i] = array[i] * pow( i/fadeSample , 2);



	}



	//fprintf(stderr, "i: fadeIn called\n");
}//end fadeIn()




//Scale the volume of the file by a specified amount
void scaleVolume(short *array, int arraySize, double scale){


	//loop through each array and multiply the array value by the input scale
	for(int i=0; i<arraySize; i++){


		//check to see if scaled samples are within short domain
		if( (array[i] * scale) <= SHRT_MIN){
			array[i] = SHRT_MIN;
		}else if((array[i] * scale) >= SHRT_MAX){
			array[i] = SHRT_MAX;
		}else{
			array[i] = array[i] * scale;
		}

	}


	//fprintf(stderr,"v: scaleVolume called\n");
}//end scaleVolume()





//Add an echo to the sound with a specified delay and a specific scale factor
short* echo(short *array, int arraySize, double delay, double scale){

	//adjust array size to fit delay values
	int delaySize = 44100 * delay;
	int newSize = arraySize + delaySize;

	short *newArray = malloc(newSize * sizeof(short));
	if(newArray == NULL ){
     	  fprintf(stderr, "Program out of memory");
      	  exit(0);
 	 }	


	//copy the original sound into the new arrays
	//once it reaches the point where the delay starts, add the values from the delay
	for(int i=0; i<arraySize; i++){
		newArray[i] = array[i]; 


	}
	for(int i=arraySize; i<newSize; i++){
		for(int j=0; j<scale; j++){		
			newArray[i+j] = array[i]; 
		
		}
	}



	//-----------update the header so that it contains the correct size information


	free(array);
	//make the heads pointers of the old arrays point to new arrays 
	
	//fprintf(stderr,"e: echo called\n");
	return *newArray;
}//end echo()





//-----helper funcs
short readShort(){//combines 2 bytes into a short

	int value1 = getchar();
	int value2 = getchar();
	
	value2 = value2 << 8;
	
	return value1 | value2;


}


void writeBytes(short num){

	
	int value2 = num >> 8;

	int value1 = num & 255;  
	
	putchar(value1);
	putchar(value2);
}



