#include <stdio.h>

int temp;
int temp1;
int factorial;
int k;
int fib1;
int fib2;
int fib3;

int fact() {
    
	if (temp == k) return factorial*k;
    factorial = factorial * temp;
	temp = temp+1;
	return fact();
}

int fib() {
	if (temp1 >= k) return fib2;
    fib3 = fib1+fib2;
    fib1 = fib2;
    fib2 = fib3;
    temp1 = temp1 + 1;
	return fib();
}

int main(int argc, char * argv[]) {
	printf("Enter a positive integer or 0 to quit: ");
	scanf("%d", &k);
	while (k > 0) {
        factorial = 1;
        temp = 1;
        temp1 = 1;
        fib1 = 1;
        fib2 = 1;
		printf("%d  %d\n", fact(), fib());
		printf("Enter a positive integer or 0 to quit: ");
		scanf("%d", &k);
	}
}