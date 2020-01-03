#include <stdio.h>
#include <cuda_runtime_api.h>
#include <time.h>
/*********************************************************************************************
//To Compile:

// nvcc -o PasswordCrackingByCuda PasswordCrackingByCuda.cu 

// To Run: ./PasswordCrackingByCuda

*****************************************************************************************/
__device__ int is_a_match(char *check) { // compares each password check

//Initilizing passwords
	char password1[]="AN9810";
	char password2[]="JI2205";
	char password3[]="TM5298";
	char password4[]="UN6085";

 
	char *c1 = check;
	char *c2 = check;
	char *c3 = check;
	char *c4 = check;

	char *pw1 = password1;
	char *pw2 = password2;
	char *pw3 = password3;
	char *pw4 = password4;

	
	while(*c1 == *pw1){

		if(*c1 == '\0'){
		
			return 1;		
		}
		c1++;
		pw1++;
	}


	while(*c2 == *pw2){
		
		if(*c2 == '\0'){
			return 1;
		}
		c2++;
		pw2++;

	}


	while(*c3 == *pw3){
		
		if(*c3 == '\0'){
			return 1;
		}
		c3++;
		pw3++;

	}


	while(*c4 == *pw4){
		
		if(*c4 == '\0'){
			return 1;
		}
		c4++;
		pw4++;

	}

   return 0; // returns 0

}


__global__ void kernel() {

	char check[7];
	check[6] = '\0';
	char i, j, k, l;
	int m = blockIdx.x+65;
	int n = threadIdx.x+65;
	char firstValue = m;
	char secondValue = n;

	check[0] = firstValue;
	check[1] = secondValue;
	for(i='0';i<='9';i++){
		for(j='0'; j<='9'; j++){
			for(k='0'; k<='9'; k++){
				for(l='0'; l<='9'; l++){
				  check[2] = i;
				  check[3] = j;
				  check[4] = k;
				  check[5] = l;

				  if(is_a_match(check)){
					printf("Password successfully cracked: %s\n", check);
				  }
				  //else {
				       //printf("tried: %s\n", check);
				  //}
				}
			}
		}
	 }
}


int time_difference(struct timespec *start, struct timespec *finish, long long int *difference)
 {
	  long long int ds =  finish->tv_sec - start->tv_sec; 
	  long long int dn =  finish->tv_nsec - start->tv_nsec; 

	  if(dn < 0 ) 
	  {
	    ds--;
	    dn += 1000000000; 
          } 

	  *difference = ds * 1000000000 + dn;
	  return !(*difference > 0);
}

//main
int main(int argc, char *argv[])
{
	struct timespec start, finish;   
  	long long int time_elapsed;

  	clock_gettime(CLOCK_MONOTONIC, &start);

 	kernel <<<26, 26>>>();

  	cudaThreadSynchronize();

	clock_gettime(CLOCK_MONOTONIC, &finish);
        time_difference(&start, &finish, &time_elapsed);
	  printf("Time elapsed was %lldns or %0.9lfs\n", time_elapsed,(time_elapsed/1.0e9)); 
  return 0;
}





