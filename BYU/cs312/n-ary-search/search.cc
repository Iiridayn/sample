// search.cc by Ged Crimsonclaw (Michael Clark) on 2-6-2004
// Implements both a binary and trinary search

#include <stdio.h>

/*
	int find: this is the number you are trying to find
	int count: this is the count of how many integers are in the list
	int *numbers: this is a pointer to the array of numbers you are searching through
	int *result: this is a pointer to the variable that will store the location or result of your search
*/

// Preforms a binary search on the data
void binary_search(int find, int count, int* numbers, int* result) {
	int high=(count-1),low=0,mid; // count-1 so no running off end
	(*result) = -1;
  
  while(high >= low) {  
  	mid = (high+low)/2;
        
    if(find > numbers[mid])
    	low = mid + 1;
    else if(find < numbers[mid])
    	high = mid - 1;
    else { // ==
    	(*result) = mid;
      return;
    }
  } //while
}

// Preforms a trinary search on the data
void trinary_search(int find, int count, int* numbers, int* result) {
	int high=(count-1),low=0,mid=((count-1)/2),mid1,mid2;
  (*result) = -1;
  
  while( (high >= mid) && (mid >= low) ) {
  	mid2 = (high+mid)/2;
    mid1 = (mid+low)/2;
    
    if(find < numbers[mid1])
    	high = mid1 - 1;
		else if(find > numbers[mid2])
    	low = mid2 + 1;
    else if( (find > numbers[mid1]) && (find < numbers[mid]) ) {
    	low = mid1 + 1;
      high = mid - 1;
    }
    else if( (find < numbers[mid2]) && (find > numbers[mid]) ) {
    	high = mid2 - 1;
      low = mid + 1;
    }
    else {
    	if(find == numbers[mid1])
		    (*result) = mid1;
      else
      	(*result) = mid2;
      return;
    }
		
    mid = (high+low)/2;
  } //while
}

