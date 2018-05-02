/* Nathan Messick                         */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <semaphore.h>

typedef struct
{
    int x;
    int y;
    int shared;
}shared_mem;

shared_mem shared;
sem_t mutex_on_shared_mem;

void *write_to_shared_mem (void * thread_no){
    int myid;
    myid  = *((int *) thread_no) ;
    /* If I am process 1, I initialize x, otherwise y */
    switch(myid)
    {   /* Block this process and wait until another process signals*/
        case 1: sem_wait (&mutex_on_shared_mem);  
                shared.x = 1;
                shared.y = 1;

                /* Now change the shared memory location to 1 */
                shared.shared = 1;
                printf("The semaphore blocking this process was just\n");
                printf("signaled.  That allows this thread to continue execution.\n");
                printf("Note that this happens inbetween instruction to be\n");
                printf("executed by thread number 2\n");
                printf("The values in the shared memory are now: \n");
                printf("         x = %d\n", shared.x);
                printf("         y = %d\n", shared.y);
                printf("    shared = %d\n\n", shared.shared);

                sem_post (&mutex_on_shared_mem);

                break;

        case 2: shared.x = 2;
                shared.y = 2;
                /* Set the shared memory location to 2 */
                shared.shared = 2;
                printf("Note that even though thread 1 was started\n");
                printf("first, thread 2's information printed first.\n");
                printf("The values in the shared memory are now:\n");
                printf("         x = %d\n", shared.x);
                printf("         y = %d\n", shared.y);
                printf("    shared = %d\n\n", shared.shared);

                sem_post (&mutex_on_shared_mem); 

                sem_wait(&mutex_on_shared_mem);
                printf("The semaphore blocking this process was just\n");
                printf("signaled.  That allows this thread to continue execution.\n");
                printf("Thread 2 now completes the rest of its instructions\n");
                printf("The values in the shared memory are now: \n");
                printf("         x = %d\n", shared.x);
                printf("         y = %d\n", shared.y);
                printf("    shared = %d\n\n", shared.shared);

                break;

        default:
                printf("Usage: semaphores procnum   (Where procnum is 1 or 2)\n");
                exit(1);    
    }
}

main(int argc, char *argv[])
{
    /* This semaphore will be used by only one process and its intial value is
     * zero*/
    sem_init (&mutex_on_shared_mem, 0, 0); 
    printf("Created semaphore\n" );

    /*Creating two thread structures.  Each will execute the
     * write_to_shared_mem function*/
    pthread_t thread1;
    pthread_t thread2;
    int thread_no1 = 1;
    int thread_no2 = 2;

    /* Make two calls to the write_to_shared_mem function.  Send an id number
     * so we can distinguish which thread is which */
    pthread_create (&thread1, NULL, &write_to_shared_mem, &thread_no1);
    pthread_create (&thread2, NULL, &write_to_shared_mem, &thread_no2);

    /* With out calling pthread_join, the main process is allowed to execute
     * to completion.  If this happens before a thread has its share of the
     * CPU time then no thread instructions will be executed as the threads
     * die with the parent process.  These functions tell the main process not
     * to exit until the threads complete their execution */
    pthread_join (thread1, NULL);
    pthread_join (thread2, NULL);


    /* Now release the semaphore */
    sem_destroy (&mutex_on_shared_mem);
}

