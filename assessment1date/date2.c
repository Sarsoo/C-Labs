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
int dateDiff(date date1, date date2);

main(){

	date date1 = getDate();
	date date2 = getDate();
	
	int diff = dateDiff(date1, date2);
	
	printf("%i\n", diff);

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
	Function to find difference between two dates
	
	Calculated based on turning dates into number of days since 0/0/0000
	and finding difference between these two numbers
*/
int dateDiff(date date1, date date2){
	
	 int date1day = 0, date2day = 0;
	 int days[] = {0,31,59,90,120,151,181,212,243,273,304,334};	 //Cumulative Day Array
	 
	 //DATE 1 CONVERSION
	 date1day += date1.day;
	 date1day += days[(date1.month) - 1];
	 date1day += (365*(date1.year));
	 
	 if(date1.year % 4 == 0){	//Leap Year Handling
		 date1day += ((date1.year / 4) - 1);
		 if(date1.month > 2){
			 date1day++;
		 }
	 }else{
		 date1day += (date1.year / 4);
	 }
	 
	 //DATE 2 CONVERSION
	 date2day += date2.day;
	 date2day += days[(date2.month) - 1];
	 date2day += (365*(date2.year));

	 if(date2.year % 4 == 0){	//Leap Year Handling
		 date2day += ((date2.year / 4) - 1);
		 if(date2.month > 2){
			 date2day++;
		 }
	 }else{
		 date2day += (date2.year / 4);
	 }
	 
	return (date2day - date1day);
}
