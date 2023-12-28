#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>
#include <pthread.h>

#define SIZE 10

int N, k;  // Global variables
pthread_mutex_t mutex;
int ortak[5];
int i=0;

void swap(int *p, int *q){  
    int temp = *p;  
    *p = *q;  
    *q = temp;  
}  
  
void max_heapify(int arr[], int n, int i)  
{  
    int largest = i;               
    int left_child = 2 * i + 1;   
    int right_child = 2 * i + 2;  
  
    
    if (left_child < n && arr[left_child] > arr[largest])  
        largest = left_child; 
  
   
    if (right_child < n && arr[right_child] > arr[largest])  
        largest = right_child; 
  
    
    if (largest != i)  
    {  
        swap(&arr[i], &arr[largest]);  
        max_heapify(arr, n, largest);  
    }  
}  

void heapSort(int arr[], int N)
{
 
    for (int i = N / 2 - 1; i >= 0; i--)
        max_heapify(arr, N, i);

    for (int i = N - 1; i > 0; i--) {

        swap(&arr[0], &arr[i]);

        max_heapify(arr, i, 0);
    }
}

  
void build_max_heap(int arr[], int n)  
{  
    int start = (n / 2) - 1;  
  
    for (int i = start; i >= 0; i--)  
    {  
        max_heapify(arr, n, i);  
    }  
}  
  

int find_kth(int arr[], int n, int k)  
{  
    
    build_max_heap(arr, n);  
        
        for (int i = 0; i != k - 1; i++)  
        {  
            swap(&arr[0], &arr[n - i - 1]);  
            n = n - 1;  
            max_heapify(arr, n, 0);  
        }  
    return arr[0];  
}


int readFile(FILE *fp){
	int num = 0, c, count=0;
	int numbers[SIZE];
	
	
	while((c=fgetc(fp))!=EOF) {
		if(c!='\n') {
			num = num * 10 + (c-'0');
			//printf("%d\n", num);
		}
		else {
			numbers[count++] = num;
			num = 0;
		}
 	}
 	
 	int kth = find_kth(numbers,SIZE,k);
	return kth;
	
}


void* routine(void *arg) {

	char* fileName = (char*)arg;
	FILE* fp;
	fp = fopen(fileName, "r");
	int kth = readFile(fp);
	pthread_mutex_lock(&mutex);
	// Critical section
	ortak[i] = kth;
	i++;
	pthread_mutex_unlock(&mutex);
}



int main(int argc, char *argv[]){
	
    
    FILE* fp;
    
    if (argc < 5) {
        printf("Usage: %s <k> <N> <infile1> ... <infileN> <outfile>\n", argv[0]);
        return 1;
    }
  
    k = atoi(argv[1]);  
    N = atoi(argv[2]);  
    char *outfile = argv[argc - 1]; 

    //int* pidArr = createChildProcess(argv);
    
    pthread_t a[N];
    
    pthread_mutex_init(&mutex, NULL);
    for(int i=1;i<=N;i++) {
    	
    	pthread_create(&a[i], NULL, &routine, (void*)argv[i+3]);
    }
    
    for(int i=1;i<=N;i++) {
    	pthread_join(a[i], NULL);
    }
    
    
    heapSort(ortak, N);
    fp = fopen(outfile,"w");
    for(int i=0;i<N;i++) {
    	fprintf(fp,"%d\n",ortak[N-1-i]);
    }
    fclose(fp);
	
	return 0;

}
