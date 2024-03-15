#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

void *Alloc(size_t sz)
{
	extraMemoryAllocated += sz;
	size_t* ret = malloc(sizeof(size_t) + sz);
	*ret = sz;
	printf("Extra memory allocated, size: %ld\n", sz);
	return &ret[1];
}

void DeAlloc(void* ptr)
{
	size_t* pSz = (size_t*)ptr - 1;
	extraMemoryAllocated -= *pSz;
	printf("Extra memory deallocated, size: %ld\n", *pSz);
	free((size_t*)ptr - 1);
}

size_t Size(void* ptr)
{
	return ((size_t*)ptr)[-1];
}

// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void merge(int pData[], int l, int m, int r){
    //The Mystery Mouseketools that will help us later
    int a, b, c;

    //First you need a length for the left and right array
    int lengthLeft = m - l + 1;
    int lengthRight = r - m;

    int* leftArr = Alloc(lengthLeft);
    int* rightArr = Alloc(lengthRight);


    for(int i = 0; i < lengthLeft; i++){
        leftArr[i] = pData[i + 1];
    }

    for(int j = 0; j < lengthRight; j++){
        rightArr[j] = pData[m + 1 + j];
    }

    //Miska (the index for the left side of the array)
    a = 0;
    //Mouska (the index for the right side of the array)
    b = 0;
    //Mickey Mouse (the length/index of pData)
    c = l;

    while (a < lengthLeft && b < lengthRight){
        if(leftArr[a] <= rightArr[b]){
            pData[c] = leftArr[a];
            a++;
        }
        else{
            pData[c] = rightArr[b];
            b++;
        }
        c++;
    }

    for(a = 0, b = 0, c = l; c <= r; c++){
		if((a < lengthLeft) && (b >= lengthRight) || leftArr[a] <= rightArr[b]){
			pData[c] = leftArr[a];
			a++;
		}
		else{
			pData[c] = rightArr[b];
			b++;
		}
	}

    DeAlloc(leftArr);
    DeAlloc(rightArr);
}

void mergeSort(int pData[], int l, int r)
{
    if(pData == NULL){
        return;
    }

    //Find the middle
    int m = l + (r - l)/2;

    if(l < r){
        //split up the left and right
        mergeSort(pData, l, m);
        mergeSort(pData, m+1, r);

        merge(pData, l, m, r);
    }

}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
	FILE* inFile = fopen(inputFileName,"r");
	int dataSz = 0;
	int i, n, *data;
	*ppData = NULL;
	
	if (inFile)
	{
		fscanf(inFile,"%d\n",&dataSz);
		*ppData = (int *)malloc(sizeof(int) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i=0;i<dataSz;++i)
		{
			fscanf(inFile, "%d ",&n);
			data = *ppData + i;
			*data = n;
		}

		fclose(inFile);
	}
	
	return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
	int i, sz = dataSz - 100;
	printf("\tData:\n\t");
	for (i=0;i<100;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\t");
	
	for (i=sz;i<dataSz;++i)
	{
		printf("%d ",pData[i]);
	}
	printf("\n\n");
}

int main(void)
{
	clock_t start, end;
	int i;
    double cpu_time_used;
	char* fileNames[] = { "input1.txt", "input2.txt", "input3.txt", "input4.txt" };
	
	for (i=0;i<4;++i)
	{
		int *pDataSrc, *pDataCopy;
		int dataSz = parseData(fileNames[i], &pDataSrc);
		
		if (dataSz <= 0)
			continue;
		
		pDataCopy = (int *)malloc(sizeof(int)*dataSz);
	
		printf("---------------------------\n");
		printf("Dataset Size : %d\n",dataSz);
		printf("---------------------------\n");
		
		printf("Merge Sort:\n");
		memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
		extraMemoryAllocated = 0;
		start = clock();
		mergeSort(pDataCopy, 0, dataSz - 1);
		end = clock();
		cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
		printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
		printArray(pDataCopy, dataSz);
		
		free(pDataCopy);
		free(pDataSrc);
	}
	
}