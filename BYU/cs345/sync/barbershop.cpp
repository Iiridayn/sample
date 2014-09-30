#include <semaphore.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

typedef sem_t* semaphore;

semaphore max_capacity;
semaphore freebarber; // which barber does the work doesn't matter

int numcustomer; // want to be global

// customers have 4 stages: waiting, cutting, paying, done
// can figure out stage, just wait for the same semaphore
semaphore *customers; // number of customers

// 3 queues of customers, 1 of chairs
// each queue has a number semaphore, and an enter semaphore

struct list_t {
	int value;
	struct list_t *next;
};
typedef list_t list;

typedef struct {
	list *head;
	list *tail;
	int maxsize;
	int length;
} queue;

queue *pay_queue;
semaphore cust_pay;
semaphore cust_pay_queue;

queue *sofa_queue;
semaphore cust_sofa;
semaphore cust_sofa_queue;

queue *free_chair_queue;
semaphore freechair;
semaphore freechair_queue;

queue *cust_chair_queue;
semaphore custchair;
semaphore custchair_queue;

int enqueue(int num, queue *which) {
	if(which->maxsize && which->length >= which->maxsize) {
		cout << "I should never see this 1" << endl;
 		return 0;
	}
	if(!which->head) {
		which->head = (list *) malloc(sizeof(list));
		which->tail = which->head;
	}
	else {
		which->tail->next = (list_t *) malloc(sizeof(list_t));
		which->tail = which->tail->next;
	}
	which->tail->value = num;
	which->length++;
	return 1;
}

int dequeue(queue *which) {
	if(which->length <= 0 || !which->head) {
		cout << "I should never see this 2" << endl;
		return -1;
	}
	int retval = which->head->value;
	list *temp = which->head;
	if(which->head == which->tail) {
		which->head = 0;
		which->tail = 0;
	}
	else
		which->head = which->head->next;
	free(temp);
	which->length--;
	return retval;	
}

void* cregister(void *ignored) {
	while(1) {
		// wait for a customer
		sem_wait(cust_pay);
		sem_wait(cust_pay_queue);
		int cust = dequeue(pay_queue);
		sem_post(cust_pay_queue);
		// wait for a barber
		sem_wait(freebarber);
		// "accept pay" = noop
		cout << "Cashier got paid by Customer " << cust << endl;
		// signal barber // instead, do all in line first? nah, follow code
		sem_post(freebarber);
		// signal customer
		sem_post(customers[cust]);
	}
}

void* barber(void *ignored) {
	while(1) {
		// wait for a customer on a chair
		sem_wait(custchair);
		sem_wait(custchair_queue);
		int cust = dequeue(cust_chair_queue);
		sem_post(custchair_queue);
		// wait for a barber
  		sem_wait(freebarber);
		// "cut hair" = noop
		cout << "Barber cut Customer " << cust << "'s hair." << endl;
		// signal barber
		sem_post(freebarber);
  		// signal cust[#] done
  		sem_post(customers[cust]);
		// customer indicates chair is ready when stands
	}
}

void* customer(void *arg) {
	int custnum = (int) arg;
	// wait max_capacity
	sem_wait(max_capacity);
	// "enter shop"	 = noop
	cout << "Customer " << custnum << " entered shop." << endl;
	// get custnum, handled my own way
	
	// sit on sofa block
	sem_wait(freechair_queue); // only one accessing
	if(free_chair_queue->length) { // if there is a free chair now
		sem_post(freechair_queue);
		sem_wait(freechair);
		sem_wait(freechair_queue);
		dequeue(free_chair_queue);
		sem_post(freechair_queue);
		
		sem_wait(custchair_queue);
		enqueue(custnum, cust_chair_queue);
		sem_post(custchair);
		sem_post(custchair_queue);
		
		cout << "Customer " << custnum << " sat in a chair." << endl;
	}
	else {
		sem_post(freechair_queue); // done looking at the chairs
		sem_wait(cust_sofa);
		sem_wait(cust_sofa_queue);
		enqueue(custnum, sofa_queue);
		sem_post(cust_sofa_queue);
		cout << "Customer " << custnum << " sat on the sofa." << endl;
		
		// Move from sofa to chair: remove a free chair, remove self from sofa,
		// sit on chair
		sem_wait(freechair);
		sem_wait(freechair_queue);
		dequeue(free_chair_queue);
		sem_post(freechair_queue);
		
		sem_wait(cust_sofa_queue);
		if(dequeue(sofa_queue) != custnum) cout << "NOT FCFS on the sofa." << endl;
		sem_post(cust_sofa); // someone else can sit down now
		sem_post(cust_sofa_queue);
		
		sem_wait(custchair_queue);
		enqueue(custnum, cust_chair_queue);
		sem_post(custchair);
		sem_post(custchair_queue);
		
		cout << "Customer " << custnum << " moved from the sofa to a chair." << endl;
	}
	sem_wait(customers[custnum]);
	
	// leaving barber chair, getting in line for the cash register
	sem_wait(freechair_queue);
	enqueue(1, free_chair_queue);
	sem_post(freechair);
	sem_post(freechair_queue);
	
	sem_wait(cust_pay_queue);
	enqueue(custnum, pay_queue);
	sem_post(cust_pay);
	sem_post(cust_pay_queue);
	
	cout << "Customer " << custnum << " got in line to pay." << endl;
	
	// Wait to pay
	sem_wait(customers[custnum]);
	
	sem_post(max_capacity); // leaving shop
	cout << "Customer " << custnum << " left the shop." << endl;
	if(custnum == numcustomer-1) exit(0);
	return 0;
}

void usage() {
	cout << "Usage: barbershop [OPTION]..." << endl;
	cout << "Simulates the barbershop problem, supports changing most parameters." << endl << endl;
	
 	printf("  %-25s%s\n", "-b", "specifies the number of barbers");
	printf("  %-25s%s\n", "-c", "specifies the number of customers");
	printf("  %-25s%s\n", "-C", "specifies the number of barber chairs");
	printf("  %-25s%s\n", "-h, --help", "displays this help and exits");
	printf("  %-25s%s\n", "-m", "specifies the maximum occupancy of the shop");
	printf("  %-25s%s\n", "-r", "specifies the number of cash registers");
	printf("  %-25s%s\n", "-s", "specifies the maximum sofa occupancy");
	
	cout << endl << "By default, there are 3 barbers, 50 customers, 1 register, " 
		<< "3 chairs," << endl << "4 people on the sofa, and 20 in the building." << endl;
	exit(1);
}

int main(int argc, char *argv[]) {
	int barbers, cregisters, chairs, sofasize, maxcapacity;
 
 	// using some defaults
	barbers = 3;
	numcustomer = 50; // global, so can terminate on its own
	cregisters = 1;
	chairs = 3;
	sofasize = 4;
	maxcapacity = 20;
 
 	//parse some args
	// can overwrite any of the defaults
	int previous = 0;
	for(int i = 1; i < argc; i++) {
		if(argv[i][0] != '-') {
  			if(!previous) usage();
  			
  			int value;
  			int test = sscanf(argv[i], "%d", &value);
  			if(!test) usage();
  			
  			switch(previous) {
  				case 1:
  					barbers = value;
  					break;
  				case 2:
  					numcustomer = value;
  					break;
  				case 3:
  					chairs = value;
  					break;
  				case 4:
  					maxcapacity = value;
  					break;
  				case 5:
  					cregisters = value;
  					break;
  				case 6:
  					sofasize = value;
  					break;
  			}
  			previous = 0;
		} // if ! '-'
		else {
			if(previous) usage();
			switch(argv[i][1]) {
				case 'b':
					previous = 1;
					break;
				case 'c':
			 		previous = 2;
			 		break;
				case 'C':
					previous = 3;
					break;
				case 'h':
				case '-': // if ISN'T help, would still do usage anyway
					usage();
				case 'm':
					previous = 4;
					break;
				case 'r':
					previous = 5;
					break;
				case 's':
					previous = 6;
					break;
				default:
					usage();
			}
		}
	}
	if(previous) usage();
	
	// initializes registers, barbers, and customers
	freebarber = (semaphore) malloc(sizeof(sem_t));
	sem_init(freebarber, 0, barbers);
	max_capacity = (semaphore) malloc(sizeof(sem_t));
	sem_init(max_capacity, 0, maxcapacity);
	customers = (semaphore *) calloc(numcustomer, sizeof(semaphore));
	for(int i = 0; i < numcustomer; i++) {
		customers[i] = (semaphore) malloc(sizeof(sem_t));
		sem_init(customers[i], 0, 0);
	}
	
	// setup queues
	pay_queue = (queue *) malloc(sizeof(queue));
	memset(pay_queue, 0, sizeof(queue));
	pay_queue->maxsize = 0;
	cust_pay = (semaphore) malloc(sizeof(sem_t));
	sem_init(cust_pay, 0, 0);
	cust_pay_queue = (semaphore) malloc(sizeof(sem_t));
	sem_init(cust_pay_queue, 0, 1);
	
	sofa_queue = (queue *) malloc(sizeof(queue));
	memset(sofa_queue, 0, sizeof(queue));
	sofa_queue->maxsize = sofasize;
	cust_sofa = (semaphore) malloc(sizeof(sem_t));
	sem_init(cust_sofa, 0, sofasize);
	cust_sofa_queue = (semaphore) malloc(sizeof(sem_t));
	sem_init(cust_sofa_queue, 0, 1);

	free_chair_queue = (queue *) malloc(sizeof(queue));
	memset(free_chair_queue, 0, sizeof(queue));
	free_chair_queue->maxsize = chairs;
	freechair = (semaphore) malloc(sizeof(sem_t));
	sem_init(freechair, 0, chairs);
	freechair_queue = (semaphore) malloc(sizeof(sem_t));
	sem_init(freechair_queue, 0, 1);
	for(int i = 0; i < chairs; i++)
		enqueue(1, free_chair_queue);
	
	cust_chair_queue = (queue *) malloc(sizeof(queue));
	memset(cust_chair_queue, 0, sizeof(queue));
	cust_chair_queue->maxsize = chairs;
	custchair = (semaphore) malloc(sizeof(sem_t));
	sem_init(custchair, 0, 0);
	custchair_queue = (semaphore) malloc(sizeof(sem_t));
	sem_init(custchair_queue, 0, 1);
	
	// initialize people (and registers)
	pthread_t temp;
	for(int i = 0; i < cregisters; i++)
		pthread_create(&temp, 0, cregister, 0);
	
	for(int i = 0; i < barbers; i++) 
		pthread_create(&temp, 0, barber, 0);
		
	for(int i = 0; i < numcustomer; i++)
		pthread_create(&temp, 0, customer, (void *) i);
	
	while(1); // use ctrl-c to terminate
}
