#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* maximum number of employees that can be stored at once (relevant only
   to storage using an array) */
#define MAX_EMPLOYEES 200

#define MAX_NAME_LENGTH 100
#define MAX_JOB_LENGTH  100

/* Employee structure
 */
struct Employee
{
   /* Employee details */
   char name[MAX_NAME_LENGTH+1]; /* name string */
   char sex;                     /* sex identifier, either 'M' or 'F' */
   int  age;                     /* age */
   char job[MAX_JOB_LENGTH+1];   /* job string */
   
   /* pointers to previous and next employee structures in the linked list
      (for if you use a linked list instead of an array) */
   struct Employee *prev, *next;
};

typedef struct Employee emp;

emp *list_root = NULL;

/* read_line():
 *
 * Read line of characters from file pointer "fp", copying the characters
 * into the "line" string, up to a maximum of "max_length" characters, plus
 * one for the string termination character '\0'. Reading stops upon
 * encountering the end-of-line character '\n', for which '\0' is substituted
 * in the string. If the end of file character EOF is reached before the end
 * of the line, the failure condition (-1) is returned. If the line is longer
 * than the maximum length "max_length" of the string, the extra characters
 * are read but ignored. Success is returned (0) on successfully reading
 * a line.
 */
static int read_line ( FILE *fp, char *line, int max_length )
{
   int i;
   char ch;

   /* initialize index to string character */
   i = 0;

   /* read to end of line, filling in characters in string up to its
      maximum length, and ignoring the rest, if any */
   for(;;)
   {
      /* read next character */
      ch = fgetc(fp);

      /* check for end of file error */
      if ( ch == EOF )
	 return -1;

      /* check for end of line */
      if ( ch == '\n' )
      {
	 /* terminate string and return */
	 line[i] = '\0';
	 return 0;
      }

      /* fill character in string if it is not already full*/
      if ( i < max_length )
	 line[i++] = ch;
   }

   /* the program should never reach here */
   return -1;
}

/* read_string():
 *
 * Reads a line from the input file pointer "fp", starting with the "prefix"
 * string, and filling the string "string" with the remainder of the contents
 * of the line. If the start of the line does not match the "prefix" string,
 * the error condition (-1) is returned. Having read the prefix string,
 * read_string() calls read_line() to read the remainder of the line into
 * "string", up to a maximum length "max_length", and returns the result.
 */
static int read_string ( FILE *fp,
			 char *prefix, char *string, int max_length )
{
   int i;

   /* read prefix string */
   for ( i = 0; i < strlen(prefix); i++ )
      if ( fgetc(fp) != prefix[i] )
	 /* file input doesn't match prefix */
	 return -1;

   /* read remaining part of line of input into string */
   return ( read_line ( fp, string, max_length ) );
}


/*	Moved sorting into separate function so it could be called by either add employee
	or read database function	*/

static void insert_employee(emp* newemp){
	
	if(list_root == NULL){
		newemp->prev = NULL;
		newemp->next = list_root;
		list_root = newemp;
	}else{
	   
		if(strcmp(newemp->name, list_root->name) < 1){
			newemp->prev = NULL;
			newemp->next = list_root;
			list_root = newemp;			
		}else{
			
			emp* marker = list_root;
			while(marker->next != NULL){
				marker = marker->next;
			}
			if(strcmp(newemp->name, marker->name) > 0){
				marker->next = newemp;
				newemp->prev = marker;
				newemp->next = NULL;
			}else{			
				marker = list_root;
				while(marker->next != NULL){
					if(strcmp(newemp->name, marker->name) > 0 && strcmp(newemp->name, marker->next->name) <= 0){
						newemp->next = marker->next;
						newemp->prev = marker;
						marker->next->prev = newemp;
						marker->next = newemp;
					}
				
					marker = marker->next;
				}
			}
		}		
	}
	
}

/* menu_add_employee():
 *
 * Add new employee to database
 */
static void menu_add_employee(void)
{
   /* fill in the code here in part 1, and add any extra functions you need */
   
   emp *newemp;
   newemp = (emp *)malloc(sizeof(emp));
   
   
   fprintf(stderr, "Enter Name: ");
   read_line(stdin, newemp->name, MAX_NAME_LENGTH + 1);
   
   char gender;
   for(;;){
	   
	   fprintf(stderr, "Enter Gender: ");
	   
	   gender = getchar();
	   
	   if(gender == '\n') gender = getchar();
	   if(gender == 'F' || gender == 'M') break;
	   fprintf(stderr, "Enter either M or F\n");
   }
   newemp->sex = gender;
   
   int age;
   for(;;){
	   
	   fprintf(stderr, "Enter Age: ");
	   
	   scanf("%i", &age);
	   
	   if(age > 0) break;
	   fprintf(stderr, "Enter a Positive Integer\n");
   }
   newemp->age = age;
	getchar();
   
   fprintf(stderr, "Enter Job: ");
   read_line(stdin, newemp->job, MAX_JOB_LENGTH + 1);
   
   insert_employee(newemp);
}

/* menu_print_database():
 *
 * Print database of employees to standard output.
 */
static void menu_print_database(void)
{
   /* fill in the code here in part 1, and add any extra functions you need */
   
	if(list_root != NULL){

	emp* marker = list_root;	
	do{
		printf("Name: %s\n", marker->name);
		printf("Sex: %c\n", marker->sex);
		printf("Age: %i\n", marker->age);
		printf("Job: %s\n\n", marker->job);
		marker = marker->next;
	}while(marker->name != NULL);
	
	}else{ fprintf(stderr, "Nothing To Print"); }
}	    

/* menu_delete_employee():
 *
 * Delete new employee from database.
 */
static void menu_delete_employee(void)
{
   /* fill in the code here in part 2, and add any extra functions you need */

	fprintf(stderr, "Enter Name To Be Deleted: ");
	char name[MAX_NAME_LENGTH + 1];	
	read_line(stdin, name, MAX_NAME_LENGTH + 1);
	
	int found = 0;
		
	emp* marker = list_root;
	for(;;){
		if(strcmp(marker->name, name) == 0){

			if(list_root == marker){	//FIRST EMPLOYEE
				if(marker->next == NULL){
					free(marker);
					list_root = NULL;
				}else{
					list_root = marker->next;
					marker->next->prev = NULL;
					free(marker);
				}
			}else{
				if(marker->next == NULL){//LAST EMPLOYEE
					marker->prev->next = NULL;
					free(marker);
				}else{
					marker->prev->next = marker->next;
					marker->next->prev = marker->prev;
					free(marker);
				}
			}
			found = 1;
			break;
		}else{
			if(marker->next != NULL){
				marker = marker->next;
			}else{
				break;
			}		
		}
	}
	if(found = 0) fprintf(stderr, "Name not Found\n");

getchar();
}

/* read file containing database of employees */
static void read_employee_database ( char *file_name )
{
   /* fill in the code here in part 3, and add any extra functions you need */
	
	FILE *empfile;
	empfile = fopen(file_name, "r");
	
	if(empfile == NULL){	/*	Checks file has opened correctly	*/
		fprintf(stderr, "Error Opening %s File to Read", file_name);
		exit(EXIT_FAILURE);
	}
	
	for(;;){	/*Infinite loop that reads in employees until can't read any more*/
		emp* newemp; 
		newemp = (emp *)malloc(sizeof(emp));
	
		int name_check = read_string(empfile, "Name: ", newemp->name, MAX_NAME_LENGTH+1);
		if(name_check == -1) break;	/*	This line exits loop when no new name can be found	*/
	
		char gender[2];	/*	sends string to be read to	*/
		read_string(empfile, "Sex: ", gender, 2);
		if(gender[0] != 'M' && gender[0] != 'F'){	/*	Gender Check	*/
			fprintf(stderr, "Incorrect Gender Format Found For Employee %s\n", newemp->name);
			exit(EXIT_FAILURE);
		}
		newemp->sex = gender[0];	/*	Takes first character of array for gender char	*/
		
		
		int age;
		char char_age[10];
		read_string(empfile, "Age: ", char_age, 10);
		age = atoi(char_age);	/*	Uses function to cast string as int, read_string only reads to strings	*/
		if(age < 1){
			fprintf(stderr, "Negative Age Found For Employee %s\n", newemp->name);
			exit(EXIT_FAILURE);
		}
		newemp->age = age;
	
		read_string(empfile, "Job: ", newemp->job, MAX_JOB_LENGTH+1);
	
		insert_employee(newemp);	/*	Correctly sorts and inserts employee	*/
		
		char carriage[2];
		read_line(empfile, carriage, 1);	/*	Important line to skip blank line inbetween employees	*/
	}

	fclose(empfile);	/*	Close File	*/
	
	
}

/* codes for menu */
#define ADD_CODE    0
#define DELETE_CODE 1
#define PRINT_CODE  2
#define EXIT_CODE   3

int main ( int argc, char *argv[] )
{
   /* check arguments */
   if ( argc != 1 && argc != 2 )
   {
      fprintf ( stderr, "Usage: %s [<database-file>]\n", argv[0] );
      exit(-1);
   }

   /* read database file if provided, or start with empty database */
   if ( argc == 2 )
      read_employee_database ( argv[1] );

   for(;;)
   {
      int choice, result;
      char line[301];

      /* print menu to standard error */
      fprintf ( stderr, "\nOptions:\n" );
      fprintf ( stderr, "%d: Add new employee to database\n", ADD_CODE );
      fprintf ( stderr, "%d: Delete employee from database\n", DELETE_CODE );
      fprintf ( stderr, "%d: Print database to screen\n", PRINT_CODE );
      fprintf ( stderr, "%d: Exit database program\n", EXIT_CODE );
      fprintf ( stderr, "\nEnter option: " );

      if ( read_line ( stdin, line, 300 ) != 0 ) continue;

      result = sscanf ( line, "%d", &choice );
      if ( result != 1 )
      {
	 fprintf ( stderr, "corrupted menu choice\n" );
	 continue;
      }

      switch ( choice )
      {
         case ADD_CODE: /* add employee to database */
	 menu_add_employee();
	 break;

         case DELETE_CODE: /* delete employee from database */
	 menu_delete_employee();
	 break;

         case PRINT_CODE: /* print database contents to screen
			     (standard output) */
	 menu_print_database();
	 break;

	 /* exit */
         case EXIT_CODE:
	 break;

         default:
	 fprintf ( stderr, "illegal choice %d\n", choice );
	 break;
      }

      /* check for exit menu choice */
      if ( choice == EXIT_CODE )
	 break;
   }

   return 0;   
}
