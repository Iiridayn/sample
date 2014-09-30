/* These are global variables declared in desert.c  They describe
 * the carrying capacity, number of significantlocations, and coordinates
 * of ech of the signficant locations.
 */
extern int capacity;
extern int locations;
extern int x[];
extern int y[];

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

// Max food is 1E6, so should work
#define TOO_FAR 1E7
#define FALSE 0
#define TRUE 1

typedef struct {
	double dist;
  int prev;
  bool settled;
} node;

double computeDistance(int i, int j);
void calcFood(int i, node* path, double* food);

/* This function computes the amount of food needed to cross the desert 
 * described by x, y, and locations.  The global variable capacity is the
 * total amount carrying capacity available.
 * 
 * Return values: amount of food necessary to cross the desert or -1 if
 * impossible.
 */

int compute_food_units() {
	int i,current;
  double test1,test2,test3,test4;
  double dist,food;
  node* path = (node*)malloc(locations * sizeof(node));
    
  // set up array
  for(i = 0; i < locations; i++) {
  	path[i].dist = TOO_FAR;
    path[i].prev = -1;
    path[i].settled = FALSE;
  }
  path[0].dist = 0; // we know this, and it provides a starting point

	// While the end node is unsettled
  while(!path[locations-1].settled) {
  	dist = -1;
    current = 0;
    
    //get shortest unsettled node
    for(i = 0;i < locations; i++) {
    	if(!path[i].settled) { // if unsettled
      	if(dist == -1) {
        	current = i;
         	dist = path[i].dist; 
        }
      	else if(path[i].dist < dist) {
        	current = i;
          dist = path[i].dist; 
        }
      }
    }
    path[current].settled = TRUE;  // we have chosen to settle it
    // should do this here so don't choose self below
    if(path[locations-1].settled) break;
    
    for(i = 0;i < locations; i++) { // compute new shortest dist
    	if(!path[i].settled) { //if settled, can't get better dist
      	dist = computeDistance(current,i);
        
        /*if node dist allowable*/
        if(i == (locations-1)) {
        	if(dist > (capacity/2)) // can go a bit farther for the last
          	dist = TOO_FAR;
        } // need ('{', '}') so else attaches properly
        else
        	if(dist > (capacity/3)) // need to be able to loop and store
          	dist = TOO_FAR;
        
        // if already TOO_FAR, shouldn't hurt anything to add a bit
        dist += path[current].dist; // total dist to node
        if(dist < path[i].dist) { // we have a better way to reach it
        	path[i].dist = dist;
          path[i].prev = current;
        } // fi
      } // fi (unsettled)
    } // for
  } // while
  
  // We know no path to it, so skip the compute dist code.
  if(path[locations-1].dist == TOO_FAR) {
  	free(path); // almost forgot ;P.  Memory leaks are bad.
  	return -1;
  }
  
  // now we know the path, now must calculate food reqs.
  //calcFood(locations-1,path,&food);
 	
  /*
  for(i = 0; i < locations; i++) {
  	printf("path[%d].dist = %f\npath[%d].prev = %d\n\n",i,path[i].dist,i,path[i].prev);
  }
	*/
  
  current = (locations-1);
  food = 0;
  while(path[current].prev != -1) { // while there is a previous node
  	// compute new total food cost
    //printf("food: %f\n",food);
    if(current == (locations-1)) {
    	test1 = computeDistance(current, path[current].prev);
      //printf("case 1: %f\n",test1);
      food+=test1;
    }
    else {
    	test1 = computeDistance(current,path[current].prev);
    	test2 = (capacity-3*test1);
      test4 = (capacity-2*test1);
      test3 = food/test4;
      //printf("case 2:\n dist: %f\n per:%f\n goal:%f\n trips:%f\n newFood:%f\n",
      				//test1,test2,food,test3,capacity*(test3));
      food += capacity*(test3);
    }
    current = path[current].prev;
  }
  
  // Kill the array at end
  free(path);
  
  if(food > 1E6) return -1; // max food as defined in other file
  return rint(food);
}

// Calculates the distance between 2 points
double computeDistance(int i, int j) {
	double result = sqrt( pow((x[j]-x[i]),2) + pow((y[j]-y[i]),2) );;
	//printf("Compute dist(%d,%d) is sqrt( (%d-%d)^2 + (%d-%d)^2 ) = %f\n",
  			//i,j,x[j],x[i],y[j],y[i],result);
	return result;
}

// Given a known path, calculates food requirements
void calcFood(int i, node* path, double* food) {
	int prev = path[i].prev;
  double storeAmount;
	if(prev == -1) // We at start, no food mods needed
  	return;
  else {
  	// we only care about final run distance, and we know is small enough
  	if(i == (locations-1)) {
    	(*food) = computeDistance(i, prev);
    	calcFood(prev,path,food);
    }
    else {
    	// cost of getting the required food to the location
      storeAmount = capacity-3*computeDistance(i,prev);
      (*food) = capacity*((*food)/storeAmount);
      calcFood(prev,path,food);
    }
  }
}

