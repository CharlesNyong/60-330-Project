#include <stdint.h>
#include <stdio.h>
#include <string.h>

int isPageFault(unsigned int PageTable[], unsigned int PageNo);
int getPageFromBackingStoreToMainMemory(unsigned int PageNo, unsigned char PhyMemory[][256]);
void updatePageTable(int FrameIndex, unsigned int PageNo, unsigned int PageTable[]);
void printProjectOutput(unsigned int LogicalAdd, unsigned int Offset, int FrameIndex, unsigned char PhyMemory[][256]);
void printTable(unsigned int Table[]);
void displayPhysMemory(unsigned char PhyMemory[][256]);

void copyString(unsigned char dest[][256], char source[], int row);

int CurrentFrameIndex =0; // this is used to store each binary data in physical memory array 

int main(int argc, char *argv[]){

	int i;
	int NumOfPageFault = 0;
	unsigned int PageTable[256]; 
	unsigned char PhysicalMemory[256][256];
    unsigned int LogicalAdd;
    unsigned int PageNumber;
    unsigned int Offset;

	// sets all elements to -1 indicating an empty array
	memset(PageTable, -1, (sizeof(int) * 256)); 
	
    FILE *fp = fopen ("addresses.txt", "r");
    if (fp == NULL) {
        printf("Error: couldn't open file");
        return 1;
    }
	//printTable(PageTable);
    for(i = 1; i<=1000; i++){
		fscanf(fp, "%u", &LogicalAdd);
        PageNumber = (LogicalAdd >> 8);
		Offset = (LogicalAdd << 24);
		Offset = (Offset >> 24);
		
		if(isPageFault(PageTable, PageNumber)){
			NumOfPageFault++;
			getPageFromBackingStoreToMainMemory(PageNumber, PhysicalMemory);
			updatePageTable(CurrentFrameIndex, PageNumber, PageTable);
			//displayPhysMemory(PhysicalMemory);
			printProjectOutput(LogicalAdd, Offset, CurrentFrameIndex, PhysicalMemory);
			CurrentFrameIndex++;	
			// call the updatePageTable function here with the page table and number returned from the first function
		}
		//printf ("%u\n", LogicalAdd);	
    }
   	//printTable(PageTable);
	printf("\nPage Faults = %d\n", NumOfPageFault);
    fclose (fp);
    return 0;
}

// returns true if they is no element at the pageNo in the page table 
int isPageFault(unsigned int PageTable[], unsigned int PageNo){
	if(PageTable[PageNo] == -1) // means page fault
		return 1;
	else 
		return 0;	
}

/* 	returns the address(index) at which the binary data 
	gotten from backing store is stored in memory
*/
int getPageFromBackingStoreToMainMemory(unsigned int PageNo, unsigned char PhyMemory[][256]){
	//printf("frame index: %d \n", CurrentFrameIndex);
	FILE *fp;
	int k;
	unsigned char Buffer[256];
	fp = fopen("BACKING_STORE.bin", "rb");
	
	fseek(fp, PageNo, SEEK_SET);
	fread(Buffer, 1, 256, fp);	
	printf("length of buffer: %d\n", Buffer);
	strcpy(PhyMemory[CurrentFrameIndex], Buffer);
	//copyString(PhyMemory, Buffer, (CurrentFrameIndex + 1));
//	for(k=0; k<256; k++){
//		printf("%c", k, Buffer[k]);
//	}
//	printf("\n");
	
	/*
		- get the index in physical memory where the binary data will be stored 
		- store the data read from the binary file to physical memory at the index gotten above
		- return this index
	*/
}

void updatePageTable(int FrameIndex, unsigned int PageNo, unsigned int PageTable[]){
	PageTable[PageNo] = FrameIndex;
}


void printProjectOutput(unsigned int LogicalAdd, unsigned int Offset, int FrameIndex, unsigned char PhyMemory[][256]){

	char value = PhyMemory[FrameIndex][Offset];
	unsigned int PhysicalAdd = ((256 * FrameIndex) + Offset);
	
	printf("Virtual address: %u Physical address: %u Value: %d\n", LogicalAdd, PhysicalAdd, value);
}


void displayPhysMemory(unsigned char PhyMemory[][256]){
	
	int i, j;
	
	for(i=0; i<256; i++){
		for(j=0; j<256; j++){
			printf("%c", PhyMemory[i][j]);
		}
		printf("\n");
	}
}

void copyString(unsigned char dest[][256], char source[], int row){

	int i,j;
	
	for(i=0; i<row; i++){
		for(j=0; j<(strlen(source)); j++){
			dest[i][j] = source[j];
		}
	}
	

}


void printTable(unsigned int Table[]){
	
	int k;
	
	for(k=0; k<256; k++){
		printf("index = %d value = %d\n", k, Table[k]);
	}
	
}

