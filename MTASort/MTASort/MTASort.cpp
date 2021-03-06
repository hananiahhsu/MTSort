#include "pch.h"
#include <iostream>
#include <windows.h>
#include <tchar.h>

using namespace std;

#define THREADS_NUMBER 2
#define ELEMENTS_NUMBER 20
#define BLOCK_SIZE ELEMENTS_NUMBER/THREADS_NUMBER
#define MAX_VALUE 100

typedef struct _tagARRAYOBJECT
{
	int *iArray;
	int iSize;
	int iThreadId;
}ARRAYOBJECT,*PARRAYOBJECT;


DWORD WINAPI ThreadStart(LPVOID lpParameter);
void PrintArray(int* iArray,int iSize);
void MergeArray(int* leftArrauy,int leftArrayLength,int* rightArray,int rightArrayLength,int* mergeArray);







int main(int argc,char* argv[])
{
   // std::cout << "Hello World!\n"; 
	int iArray_1[BLOCK_SIZE], iArray_2[BLOCK_SIZE], iArray[ELEMENTS_NUMBER];
	for (int iIndex = 0;iIndex < BLOCK_SIZE;iIndex++)
	{
		iArray_1[iIndex] = rand() % MAX_VALUE;
		iArray_2[iIndex] = rand() % MAX_VALUE;
	}

	//Create thread 
	HANDLE hThreads[THREADS_NUMBER];
	ARRAYOBJECT p_obj_1 = {&iArray_1[0],BLOCK_SIZE,0};
	hThreads[0] = CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)ThreadStart,(LPVOID)&p_obj_1,0,NULL);


	ARRAYOBJECT p_obj_2 = { &iArray_2[0],BLOCK_SIZE,1 };
	hThreads[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadStart, (LPVOID)&p_obj_2, 0, NULL);


	cout << "waiting execution... ..." << endl;
	//waiting for mutiple onjects
	WaitForMultipleObjects(THREADS_NUMBER,hThreads,TRUE,INFINITE);

	//Print array of 1 --2018.11.13
	PrintArray(iArray_1, BLOCK_SIZE);

	//Print array of 2 --2018.11.13
	PrintArray(iArray_2, BLOCK_SIZE);


	//Merge array of threads
	MergeArray(&iArray_1[0],BLOCK_SIZE,&iArray_2[0],BLOCK_SIZE,iArray);

	//Print array
	PrintArray(iArray,ELEMENTS_NUMBER);

	CloseHandle(hThreads[0]);
	CloseHandle(hThreads[1]);

	cout << "array sorted ... ..." << endl;
	Sleep(15000);
	return 0;
}


///////////////////////////////////////////////////////////////////
//Increasing the iArray
DWORD WINAPI ThreadStart(LPVOID lpParameter)
{
	PARRAYOBJECT p_obj = (PARRAYOBJECT)lpParameter;
	int iTmp = 0;
	for (int m = 0;m < p_obj->iSize;m++)
	{
		for (int n = p_obj->iSize - 1; n > m; n--)
		{
			if (p_obj->iArray[m] < p_obj->iArray[n])
			{
				iTmp = p_obj->iArray[n];
				p_obj->iArray[n] = p_obj->iArray[m];
				p_obj->iArray[m] = iTmp;
			}
		}
	}
	return 0;
}


//print array
void PrintArray(int *iArray,int iSize)
{
	for (int m = 0;m < iSize;m++)
	{
		cout << " " << iArray[m];
	}
	cout << endl;
}


//Merge 2 arrays to one array by comparing the elements 
void MergeArray(int* leftArrauy, int leftArrayLength, int* rightArray, int rightArrayLength, int* mergeArray)
{
	int i = 0, j = 0, k = 0;
	while (i < leftArrayLength && j < rightArrayLength)
	{
		if (leftArrauy[i] < rightArray[j])
		{
			mergeArray[k] = leftArrauy[i];
			i++;
		}
		else
		{
			mergeArray[k] = rightArray[j];
			j++;
		}
		k++;
	}

	if (i >= leftArrayLength)
	{
		while (j < rightArrayLength)
		{
			mergeArray[k] = rightArray[j];
			j++;
			k++;
		}
	}

	if (j >= rightArrayLength)
	{
		while (i < leftArrayLength)
		{
			mergeArray[k] = leftArrauy[i];
			i++;
			k++;
		}
	}
}