/*

	Program works by letting a user input an amount of dates and converting each into an integer value
	representing the number of days since 0/0/0000 in the same way as date2.c
	
	From here those integer values are copied onto another array and sorted into ascending order
	
	In theory to print the dates because the unsorted array matches up to the date array in position,
	going through the sorted array one value at a time and for each one finding the same value in the
	unsorted array will give the position of the date to be printed
	
	In practice this was done with a while loop nested in a for loop.
	
	The outside for loop goes through the array of sorted integers
	The inside while loop checks if the i element of the sorted integers is equal to the first element of the non sorted array
	
	If it is it will skip the loop and print the first date
	
	If not it will increment elementnum and check it against this integers element of the unsorted array
	This will continue through the unsorted array until the matching integer value is found
	
	At this point the elementnum is the element number of the date to be printed

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _date {
	int day;
	int month;
	int year;
};

typedef struct _date date;

date getDate();
int dayConvert(date date1);
void printDate(date date1);

main(){
	
	date *datearray; //array for inputted dates
	int *intarray; //array for inputted dates after conversion to days
	int *sorted_intarray; //sorted array of converted days
	
	int num_dates;
	scanf("%i", &num_dates);
	
	datearray = (date *)malloc(sizeof(date) * num_dates);
	intarray = (int *)malloc(sizeof(int) * num_dates);
	sorted_intarray = (int *)malloc(sizeof(int) * num_dates);
	
	
	int i, j, temp;
	for(i = 0; i < num_dates; i++)			//GET DATES
	{
		datearray[i] = getDate(); 
	}
	
	for(i = 0; i < num_dates; i++)			//CONVERT DATES TO INTEGERS
	{	
		intarray[i] = dayConvert(datearray[i]); 
	}
	
	for(i = 0; i < num_dates; i++)			//MAKE COPY OF INTEGERS
	{
		sorted_intarray[i] = intarray[i];
	}
	
	for(i = 0; i < num_dates; i++){			//SORT INTEGERS
		
		for(j = i + 1; j < num_dates; j++){
			
			if(sorted_intarray[i] > sorted_intarray[j]){
				
				temp = sorted_intarray[i];
				sorted_intarray[i] = sorted_intarray[j];
				sorted_intarray[j] = temp;
				
			}	
		}	
	}
	
	for(i = 0; i < num_dates; i++){			//FIND MATCHING INTEGER AND PRINT DATE
		
		int elementnum = 0;
		
		while(sorted_intarray[i] != intarray[elementnum])
		{
			elementnum++;	
		}
		
		printDate(datearray[elementnum]);
	}
	
	free(datearray);
	free(intarray);
	free(sorted_intarray);

}

/*
	getDate function to get user entry of a valid date and
	returns a date variable type
*/
date getDate(){
		date entrydate = {0,0,0};
		int days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
		int leapdays[12] = {31,29,31,30,31,30,31,31,30,31,30,31};
		char mark[2];

		/*
			loop to get entry of date that quits when a valid date is given
			should only run once if a valid date is entered, any errors result in continue statement
		*/
		while(1){

			char input[12];
			
			scanf("%s", input);
			
			sscanf(input,"%i%c%i%c%i", &entrydate.day, &mark[0], &entrydate.month, &mark[1], &entrydate.year);	
			//printf("%i\t%i\t%i\n", entrydate.day, entrydate.month, entrydate.year);
			
			if(mark[0] != mark[1])
			{
				fprintf(stderr, "ERROR: Use Consistent Delimeters\n");
				continue;
			}
			
			if(mark[0] != '/' && mark[0] != '-')
			{
				fprintf(stderr, "ERROR: Use either / or - as delimiters\n");
				continue;
			}
			
			if(entrydate.year > 10000 | entrydate.year < 1)
			{
				fprintf(stderr, "ERROR: Enter A Valid Year, Less than 10000 and Greater Than 0\n");
				continue;
			}
			
			if(entrydate.month > 12 | entrydate.year < 1) 
			{
				fprintf(stderr, "ERROR: Enter A Valid Month\n");
				continue;
			}
			
			//checks that day entered is not outside range for month
			
			if(entrydate.year % 4 == 0){
				
				if(entrydate.day > leapdays[(entrydate.month) - 1] | entrydate.day < 1){
					
					fprintf(stderr, "ERROR: Enter A Valid Day For The Month Entered\n");
					continue;
				}
			}
			else{
				
				if(entrydate.day > days[(entrydate.month) - 1] | entrydate.day < 1){
					
					fprintf(stderr, "ERROR: Enter A Valid Day For The Month Entered\n");
					continue;
				}
			}
			
			break;
		}
		
		return entrydate;
}

/*
	Function to convert date into number of days since 0/0/0000 used for sorting
*/
int dayConvert(date date1){
	
	 int date1day = 0;
	 int days[] = {0,31,59,90,120,151,181,212,243,273,304,334};	 

	 date1day += date1.day;
	 date1day += days[(date1.month) - 1];
	 date1day += (365*(date1.year));
	 
	 //Leap Year Handling
	 if(date1.year % 4 == 0){
		 date1day += ((date1.year / 4) - 1);
		 if(date1.month > 2){
			 date1day++;
		 }
	 }else{
		 date1day += (date1.year / 4);
	 }
	 
	return date1day;
}

/*
	Function to print dates
*/
void printDate(date date1){
	
	printf("%i/%i/%i\n", date1.day, date1.month, date1.year);
	
}