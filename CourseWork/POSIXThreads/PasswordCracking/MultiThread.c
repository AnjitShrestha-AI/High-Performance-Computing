#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>
#include <pthread.h>
#include <ctype.h>


/******************************************************************************

  Compile with:
    cc -o MultiThread MultiThread.c -lcrypt -pthread

  Run with:
    ./MultiThread

    ./MultiThread > MultiThreadResult.txt

  
******************************************************************************/
int n_passwords = 4;

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
This functions includes two pthread and two kernel function 
which are used to encrypted the password explained above. 
all combinations that re tried are displayed and when the 
password is found, #, is put at the start of the line.


**/

void crack(){
 int k;
 pthread_t thread_1, thread_2;

 void *kernel_function_1();
 void *kernel_function_2();


 for(k=0; k<n_passwords; k<k++) {
   pthread_create(&thread_1, NULL, kernel_function_1, encrypted_passwords[k]);
   pthread_create(&thread_2, NULL, kernel_function_2, encrypted_passwords[k]);

   pthread_join(thread_1, NULL);
   pthread_join(thread_2, NULL);

 }

}

//kernel_function_1
void *kernel_function_1(char *salt_and_encrypted){
 int a,b,c;     			// Loop counters
  char salt[7];    			// String used in hashing the password. Need space
  char plain[7];   			// The combination of letters currently being checked
  char *enc;       			// Pointer to the encrypted password
  int count = 0;   			// The number of combinations explored so far
  
  substr(salt, salt_and_encrypted, 0, 6);

  for(a='A'; a<='M'; a++){ 
    for(b='A'; b<='Z'; b++){
       for(c=0; c<=99; c++){
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

//kernel_function_2
void *kernel_function_2(char *salt_and_encrypted){
  int a, b, c;    //variables decleration for loop counters     
  char salt[7];   //String used in hasing the password. Need space 
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // the number of comnination explored so far
  substr(salt, salt_and_encrypted, 0, 6);
  
  for(a='N'; a<='Z'; a++){
    for(b='A'; b<='Z'; b++){
       for(c=0; c<=99; c++){
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


//calculating time difference

int time_difference(struct timespec *start, struct timespec *finish, 
                              long long int *difference) {
  long long int ds =  finish->tv_sec - start->tv_sec; 
  long long int dn =  finish->tv_nsec - start->tv_nsec; 

  if(dn < 0 ) {
    ds--;
    dn += 1000000000; 
  } 
  *difference = ds * 1000000000 + dn;
  return !(*difference > 0);
}

//main
int main(int argc, char *argv[]) {
  struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);

 
  crack();

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9)); 

  return 0;
}
