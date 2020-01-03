#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include "time_diff.h"
#include <stdio.h>
#include <mpi.h>
#include <unistd.h>


/******************************************************************************
 Works on passwords that consist only of 2 uppercase letters and a 4 digit integer. 

  Compile with:
    mpicc -o PasswordCrackingByMpiAA9999 PasswordCrackingByMpiAA9999.c time_diff.c -lcrypt -lm

  To Run:
   
    mpirun -n 3 ./PasswordCrackingByMpiAA9999

  


******************************************************************************/
int n_passwords = 4;
pthread_t t1, t2;
char *encrypted_passwords[] = {
  "$6$KB$gdJTwoA2T3awMU8u1KP9YkNyO5BD0/OA176k31WXJHFWaEfI2FkJAbrcEvXLcFef3YvmcFN4ICcgjRVYP61OW/",
  "$6$KB$TjschdyojU8EXuemzfKLcu3LWAHx1CsG04Y2RK8yEyhmYUcyPe3MH.Q1stcQV4MfQ611n7fs3uJeOA8w7bqmy/",
  "$6$KB$YP6cGW2dVjDmuU2cPiaFBfMwBhPvdAupniRBb8I0ydsZJQRY2de8JY9O2gQX01gx4FKFNspFwrco7MSVCgePk/",
  "$6$KB$bqlbhvPNH4mMxvNxq5utFjwmLMgw43PCBEULkRjB3y/3byGt7iev1MNmd82RwYrb9zT1GVDo.MmvevIk0vlr3."
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
 start of the line. 
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
       for(c=0; c<=9999; c++){
	printf("This is Instance 1");
        sprintf(plain, "%c%c%04d", a,b,c);
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


void kernel_function_2(char *salt_and_encrypted){
int a, b, c;     
  char salt[7];    
  char plain[7];   
  char *enc;       
  int count = 0;  
  substr(salt, salt_and_encrypted, 0, 6);

  for(a='N'; a<='Z'; a++){
    for(b='A'; b<='Z'; b++){
       for(c=0; c<=9999; c++){
	printf("This is Instance 2");
        sprintf(plain, "%c%c%04d", a, b, c);
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

  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if(size != 3) {
    if(rank == 0) {
      printf("This program needs to run on exactly 3 processes\n");
      return 0;
    }
  } else {
    if(rank ==0){
      int a;
      int b;
      
	MPI_Send(&a, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);  
        MPI_Send(&b, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
	
    } else {
      if(rank == 1){
	int k;
        int num = rank + 10;
      	MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	for ( k = 0; k<n_passwords;k<k++){
		kernel_function_1(encrypted_passwords[k]);
	}
      }
	else if(rank == 2){
	int k;
      	int num = rank + 10;
      	MPI_Recv(&num, 1, MPI_INT, 0, 0, MPI_COMM_WORLD,MPI_STATUS_IGNORE);
	for ( k = 0; k<n_passwords;k<k++){
		kernel_function_2(encrypted_passwords[k]);
	}
	}
    }
  }
  MPI_Finalize(); 
  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &difference);
    printf("Elapsed Time: %9.5lfs\n", difference/1000000000.0);
  return 0;
}
