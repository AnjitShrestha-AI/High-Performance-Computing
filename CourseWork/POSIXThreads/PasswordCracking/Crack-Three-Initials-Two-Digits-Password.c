#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <crypt.h>
#include <time.h>

/******************************************************************************
  This Demonstrates how to crack an encrypted password using a simple
  "brute force" algorithm which is called trial and error method. Works on passwords that consist only of 3 uppercase letters and a 2 digit integer.

  Compile with:
    cc -o Crack-Three-Initials-Two-Digits-Password Crack-Three-Initials-Two-Digits-Password.c -lcrypt

  Run with:
    ./Crack-Three-Initials-Two-Digits-Password > Crack-Three-Initials-Two-Digits-Password_results.txt


******************************************************************************/
int n_passwords = 4;

char *encrypted_passwords[] = {
  "$6$KB$KDusRrWxf0MqxfI6rmHIO/jJQh6n4jWHMpSzYBozk2UonZn9l63rRAPOaHK1O9oS1XQNpv18O9Q2tAuhUlUwJ0",
  "$6$KB$Ib6.NBt.BuNuPH26wL9b8Sc/XKRKeFPMw8XSiXIyy3G2nPFadPu3sE/a12cu3YVTOgwMVcx8iNFiCOahmaWCn0",
  "$6$KB$KGSlXLkVJR6H2Egc/SR7v.TXdfvDvqt5RkNCWf7Mr8FtxDAVo3JF4F0VD/XQqQc2WDcublLU7pe.coqh.vzhP1",
  "$6$KB$xLogzun/HaaIhmLhajA.KfobOkqLzLJ1rBxs/JlMGzmKsBADjaqbTxRN3K5eY6YgT2poih33bi7JfYkexJT9o0"
};

/**
 Required by lack of standard function in C.   
*/

void substr(char *dest, char *src, int start, int length){
  memcpy(dest, src + start, length);
  *(dest + length) = '\0';
}

/**
This function "crack" can crack the kind of password explained above. All combinations
 that are tried are showed and when the password is found, it is denoted by #, it is put at the 
 start of the line. 
*/

void crack(char *salt_and_encrypted){
  int a, b, c, d;     // Loop counters
  char salt[7];    // String used in hashing the password. Need space for \0
  char plain[7];   // The combination of letters currently being checked
  char *enc;       // Pointer to the encrypted password
  int count = 0;   // The number of combinations explored so far

  substr(salt, salt_and_encrypted, 0, 6);

  for(a='A'; a<='Z'; a++){
    for(b='A'; b<='Z'; b++){
	for(c='A'; c<='Z';c++){
	      for(d=0; d<=99; d++){
		sprintf(plain, "%c%c%c%02d", a, b, c, d); 
		enc = (char *) crypt(plain, salt);
		count++;
		if(strcmp(salt_and_encrypted, enc) == 0){
		  printf("#%-8d%s %s\n", count, plain, enc);
		} else {
		  printf(" %-8d%s %s\n", count, plain, enc);
		}
	      }
	}
    }
  }
  printf("%d solutions explored\n", count);
}

//Calculating time difference
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
int main() {
int k;
  struct timespec start, finish;   
  long long int time_elapsed;

  clock_gettime(CLOCK_MONOTONIC, &start);
 
//loops for encrypted passwords 
 for(k=0;k<n_passwords;k<k++) {
    crack(encrypted_passwords[k]);
  }

  clock_gettime(CLOCK_MONOTONIC, &finish);
  time_difference(&start, &finish, &time_elapsed);
  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed, 
         (time_elapsed/1.0e9)); 

  return 0;
}
