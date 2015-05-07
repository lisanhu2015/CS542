#include <stdio.h>
#include <stdbool.h>


int main(int argc, char * argv[]) {
	int x[10];
	int size;
	int i;
    int j;
	int k;
    int temp;
    bool swapped;
    bool binRes;
    int low;
    int high;
    int mid;

	printf("Enter an integer between 1 and 10: ");
	scanf("%d", &size);
	i = 0;
	while (i < size) {
		printf("Enter a positive integer: ");
		scanf("%d", &(x[i]));
		i = i + 1;
	}

    k = size-1;
    swapped = true;
    while ( swapped) {
        swapped = false;
        j = 0;
        while (j < k) {
            if (x[j] > x[j+1]) {
                temp = x[j];
                x[j] = x[j+1];
                x[j+1] = temp;
                swapped = true;
            }
            j = j+1;
        }
        k = k-1;
    }

	i = 0;
	printf("The array sorted: ");
	while (i < size) {
		printf("%d ", x[i]);
		i = i + 1;
	}
	printf("\n");

	printf("Enter a positive integer or 0 to quit: ");
	scanf("%d", &k);
	while (k != 0) {
        low = 0;
        high = size -1;
        while (low < high) {
            mid = (high+low)/2;
            if (x[mid] < k)
                low = mid+1;
            else
                high = mid;
        }
        printf("%d\n", low == high && x[low] == k);
		printf("Enter a positive integer or 0 to quit: ");
		scanf("%d", &k);
	}
}
		