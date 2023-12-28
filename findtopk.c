#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <sys/wait.h>

#define SIZE 1000

int N, k;  // Global variables

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
 	
 	int kth = find_kth(numbers,SIZE,k); // k. en büyük sayıyı bulur. 
	//printf("%d\n", numbers[1]);
	return kth;
	
}


void childProcess(char fileName[]) {
	
	FILE *fp, *fp2;
	char interFileName[20];
	fp = fopen(fileName,"r");
	int kth = readFile(fp);
	fclose(fp);
	
	sprintf(interFileName, "inter%d.txt", getpid()); 
	fp2 = fopen(interFileName, "w");
	fprintf(fp2, "%d", kth);
	fclose(fp2);
	
	
	/*
	FILE *fp;
	int i;
	char fileName[20];
	
	
	sprintf(fileName, "Dosya%d.txt", getpid());
	
	srand(time(NULL) ^ getpid());
		
	fp = fopen(fileName, "w");
	for(i=0;i<SIZE;i++){
		fprintf(fp,"%d\n",rand()%1000000+1);
	}
	
	fclose(fp);
	*/
				
		
}


int* createChildProcess(char *argv[]){
	int i, pid;
	char fileName[20];
	int* pidArr = (int*)malloc(N * sizeof(int));
	
	for(i=0;i<N;i++){
		pid = fork();
		if(pid==0){
			// Child process
			childProcess(argv[i+3]);
			exit(0);
		}
		else{
			// Parent process
			pidArr[i] = pid;
		}
		
	}
	return pidArr;
}


int main(int argc, char *argv[]){
	
    int numArr[N];
    FILE* fp;
    char interFileName[20];
    int num=0, c;	
		
	
    if (argc < 5) {
        printf("Usage: %s <k> <N> <infile1> ... <infileN> <outfile>\n", argv[0]);
        return 1;
    }
    
    k = atoi(argv[1]);  
    N = atoi(argv[2]);  
    char *outfile = argv[argc - 1]; 

    int* pidArr = createChildProcess(argv);
    
    wait(NULL); // parent waiting for children
    
    for(int i=0;i<N;i++) {
    	sprintf(interFileName,"inter%d.txt",pidArr[i]);
    	fp = fopen(interFileName,"r");
    	while((c=fgetc(fp))!=EOF) {
		num = num * 10 + (c-'0');
	}
	numArr[i] = num;
	num = 0;
	fclose(fp);
	remove(interFileName);
    }
    heapSort(numArr, N);
    fp = fopen(outfile,"w");
    for(int i=0;i<N;i++) {
    	fprintf(fp,"%d\n",numArr[N-1-i]);
    }
    fclose(fp);
	
	return 0;

}
