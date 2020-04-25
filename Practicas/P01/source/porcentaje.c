#include<stdio.h>
#include<stdlib.h>

int main(int argc, char* argv[]){

	unsigned int p= atoi(argv[1]);
	unsigned int n= atoi(argv[2]);
	unsigned int x = n*p/100;
	printf("El %d%% de %d es %d\n",p,n,x);
	return 0;
}

/*


2.1E9

50 * 50'000'000 = 5E1 * 5E7 = 25E8 => 2.5E9

*/
