#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include "time_diff.h"
#include <stdio.h>
#include <mpi.h>
#include <unistd.h>


/******************************************************************************

  Compile with:
    mpicc -o PasswordCrackingByMpi PasswordCrackingByMpi.c time_diff.c -lcrypt -lm

  To Run:
   
    mpirun -n 3 ./PasswordCrackingByMpi

******************************************************************************/
int n_passwords = 4;
pthread_t t1, t2;
char *encrypted_passwords[] = {
  "$6$KB$yPvAP5BWyF7oqOITVloN9mCAVdel.65miUZrEel72LcJy2KQDuYE6xccHS2ycoxqXDzW.lvbtDU5HuZ733K0X0",
  "$6$KB$H8RIsbCyr2r7L1lklCPKY0tLK9k5WudNWpxkNbx2bCBRHCsI3qyVRY.0nrovdkDLDJRsogQE9mA3OqcIafVsV0",
  "$6$KB$zd0hywe3NHS5T209L69g1LWlJjEXoT7OCwzWs.tL5mjK6.DlCY8azuxxy/ucDmebOMWKgWvS/A2..Ht1MyfI1/",
  "$6$KB$yRaZFVbx3SBqrI7KCFC572n/v5hWxyQXR3Y8WVbh05yX6GNeBGWN1tTCPT51Etagi4EQ8Vdd91ldNm6tIrIV40"
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
 This function can crack the kind of password explained above. All combinations
 that are tried are displayed and when the password is found, #, is put at the 
 start of the line. Note that one of the most time consuming operations that 
 it performs is the output of intermediate results, so performance experiments 
 for this kind of program should not include this. i.e. comment out the printfs.
*/

void kernel_function_1(char *salt_and_encrypted){
 int a,b,c;     			// Loop counters
  char salt[7];    			// String used in hashing the password. Need space
  char plain[7];   			// The combination of letters currently being checked
  char *enc;       			// Pointer to the encrypted password
  int count = 0;   			// The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(a='A'; a<='M'; a++){
    for(b='A'; b<='Z'; b++){
       for(c=0; c<=99; c++){
	printf("This is Instance 1");
        sprintf(plain, "%c%c%02d", a,b,c);
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } 
	else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }

       }
    }
  }
printf("%d solutions explored\n", count);
}

//kernel functio 2
void kernel_function_2(char *salt_and_encrypted){
int a, b, c;    //loop counters 
  char salt[7];    //String used in hashing the password. Need space \0
  char plain[7];   //The combination of letters currently being cracked
  char *enc;       //Pointer to the encrypted password
  int count = 0;  //The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(a='N'; a<='Z'; a++){
    for(b='A'; b<='Z'; b++){
       for(c=0; c<=99; c++){
	printf("This is Instance 2");
        sprintf(plain, "%c%c%02d", a, b, c);
        enc = (char *) crypt(plain, salt);
        count++;
        if(strcmp(salt_and_encrypted, enc) == 0){
          printf("#%-8d%s %s\n", count, plain, enc);
        } 
	else {
          printf(" %-8d%s %s\n", count, plain, enc);
        }
       }
    }
  }
   printf("%d solutions explored\n", count);
}


int main(int argc, char *argv[]) {
  struct timespec start, finish;
  long long int difference;   
  int account = 0;
  clock_gettime(CLOCK_MONOTONIC, &start);
  int size, rank;
//Initialize the MPI library
  MPI_Init(NULL, NULL);
//Get the size of a communicator
  MPI_Comm_size(MPI_COMM_WORLD, &size);
//Get the rank of the calling process in the communicator
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//condition to check the size of processes
  if(size != 3) {
    if(rank == 0) {
      printf("This program needs to run on exactly 3 processes\n");
      return 0;
    }
  } else {
    if(rank ==0){  //node 0
      int a;
      int b;
      //Send message to the another process
	MPI_Send(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);  
        MPI_Send(&b, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
	
    } else {
      if(rank == 1){ //node 1
	int k;
        int num = rank + 10;
	//Receive a message from another process
      	MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	for ( k = 0; k<n_passwords;k<k++){
		kernel_function_1(encrypted_passwords[k]);
	}
      }
	else if(rank == 2){  //node 2
	int k;
      	int num = rank + 10;

	//Receive a message from another process
      	MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	for ( k = 0; k<n_passwords;k<k++){
		kernel_function_2(encrypted_passwords[k]);
	}
	}
    }
  }
//clean up all MPI state
  MPI_Finalize(); 
//Get the system time after we have run the Password cracking by mpi
  clock_gettime(CLOCK_MONOTONIC, &finish);
//Calculate the time spent between the start time and end time
  time_difference(&start, &finish, &difference);
//Output the time spent running the program
    printf("Elapsed Time: %9.5lfs\n", difference/1000000000.0);
  return 0;
}
