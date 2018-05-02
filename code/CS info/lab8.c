#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

// Assists users in computing taxes.  Takes their
// earnings, their Filing status, and their dependants
// (if applicable).  Then informs them what they 
// owe the government.

int getIncome();
int getStatus();
int getDependants();
int makeTax(int,int);
int stringComp(char*,char*);

int main() { // No arguments needed/accepted
	int income = getIncome();
	int status = getStatus();
	int dependants = (status)?getDependants():-1;
	int tax = makeTax(income, dependants);
	printf("Total tax that you owe the government is: $%d\n", tax);
	return 0;
}

int getIncome() {
	printf("Please enter your annual income: ");
	int income;
	while((!scanf("%d",&income)) || (income < 0)) { //scanf return 0 if not done
		if(!(income<0)) {
			char* buf[80];
			scanf("%s",buf); //clean buffer -- needed? yes. very.
		}
		printf("Invalid income!  Please enter a valid annual income: ");
	}
	return income;
}

int getStatus() {
	printf("Please indicate your filing status (married or single): ");
	char* inbuf[80];
	while(1) {
		scanf("%s", inbuf);
		if(stringComp(inbuf, "married")) return 1;
		else if(stringComp(inbuf, "single")) return 0;
		else printf("Invalid filing status!  Please enter a valid filing status (married or single): ");
	}
}

int stringComp(char* one, char* two) {
	int size = strlen(one);
	int i;
	for(i = 0; i < size; i++)
		one[i] = tolower(one[i]);
	return (!strcmp(one, two))?1:0;
}

int getDependants() {
	printf("Please enter the number of dependants: ");
	int dependants;
	while((!scanf("%d", &dependants)) || (dependants < 0) || (dependants > 10)) {
		if(!((dependants<0)||(dependants>10))) {
			char* buf[80];
			scanf("%s", buf); // needed? yes. very.
		}
		printf("Invalid number of dependants!  Please enter a value from 0 to 10: ");
	}
	return dependants;
}

int makeTax(int money, int dependants) {
	if(dependants < 0) {
		if(money > 44999)
			return rint(money*.32);
		else if(money >= 12000)
			return rint(money*.26);
		else return 0;
	}
	else {
		money -= 3000*dependants;
		if(money > 64999)
			return (rint(money*.32));
		else if(money >= 20000)
			return (rint(money*.32));
		else return 0;
	}
}
