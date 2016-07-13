#include <stdint.h>
#include <stdio.h>
#include <string.h>

int isPageFault(unsigned int PageTable[], unsigned int PageNo);
void getPageFromBackingStoreToMainMemory(unsigned int PageNo, char PhyMemory[][256]);
void updatePageTable(int FrameIndex, unsigned int PageNo, unsigned int PageTable[]);
void printProjectOutput(unsigned int LogicalAdd, unsigned int Offset, int FrameIndex, char PhyMemory[][256]);

int CurrentFrameIndex =0; // this is used to store each binary data in physical memory array 

int main(int argc, char *argv[]){

	int i;
	double NumOfPageFault = 0;
	unsigned int PageTable[256]; 
	char PhysicalMemory[256][256];
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

    for(i = 1; i<=1000; i++){
		fscanf(fp, "%u", &LogicalAdd);
        PageNumber = (LogicalAdd >> 8);
		Offset = (LogicalAdd << 24);
		Offset = (Offset >> 24);
		
		if(isPageFault(PageTable, PageNumber)){
			NumOfPageFault++;
			getPageFromBackingStoreToMainMemory(PageNumber, PhysicalMemory);
			updatePageTable(CurrentFrameIndex, PageNumber, PageTable);
			printProjectOutput(LogicalAdd, Offset, CurrentFrameIndex, PhysicalMemory);
			CurrentFrameIndex++;	
		}
		else{ // if its not a page fault get the frame index from the page table and send that to the output function
			int FrameIndex = PageTable[PageNumber];
			printProjectOutput(LogicalAdd, Offset, FrameIndex, PhysicalMemory);
		}
		
		
    }
	printf("\nPage Fault Rate = %.3f\n", (NumOfPageFault/1000));
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

// gets the binary data at the page number location
void getPageFromBackingStoreToMainMemory(unsigned int PageNo, char PhyMemory[][256]){
	FILE *fp;
	int k;
	char Buffer[256];
	fp = fopen("BACKING_STORE.bin", "rb");
	
	fseek(fp, (PageNo*256), SEEK_SET);
	fread(Buffer, sizeof(char), 256, fp);	
	memcpy(PhyMemory[CurrentFrameIndex], Buffer, 256); // use memcpy to copy binary data 
	fclose(fp);
}

void updatePageTable(int FrameIndex, unsigned int PageNo, unsigned int PageTable[]){
	PageTable[PageNo] = FrameIndex;
}


void printProjectOutput(unsigned int LogicalAdd, unsigned int Offset, int FrameIndex, char PhyMemory[][256]){
	char value = PhyMemory[FrameIndex][Offset];
	unsigned int PhysicalAdd = ((256 * FrameIndex) + Offset);
	
	printf("Virtual address: %u Physical address: %u Value: %d\n", LogicalAdd, PhysicalAdd, (int)value);
}
