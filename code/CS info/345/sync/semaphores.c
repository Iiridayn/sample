/* Quinn Snell                         */

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>

#if defined(__GNU_LIBRARY__) && !defined(_SEM_SEMUN_UNDEFINED)
     /* union semun is defined by including <sys/sem.h> */
#else
     /* according to X/OPEN we have to define it ourselves */
union semun {
    int val;                    /* value for SETVAL */
    struct semid_ds *buf;       /* buffer for IPC_STAT, IPC_SET */
    unsigned short int *array;  /* array for GETALL, SETALL */
    struct seminfo *__buf;      /* buffer for IPC_INFO */
    };
#endif



typedef struct
{
    int x;
    int y;
    int shared;
}shared_mem;

main(int argc, char *argv[])
{
    int     i,j, myid;

    shared_mem *shared;
    key_t shared_mem_key=252;
    int shared_mem_id;
    struct shmid_ds shm_ds;

    union semun argument;
    int semid;
    key_t semkey=253;

    struct sembuf operations[1];

    int ignored_int = 1;
    union semun ignored_argument;

    if (argc < 2)
    {
        printf("Usage: semaphores procnum   (Where procnum is 1 or 2)\n");
        exit(1);    
    }
    myid = atoi(argv[1]);

    /* We need to set up the shared memory that will be needed */
    shared_mem_id = shmget(shared_mem_key, sizeof(shared_mem), 0777 | IPC_CREAT);

    /* Now we must attach to the shared memory */
    shared = (shared_mem *)shmat(shared_mem_id, NULL, SHM_RND);

    /* Now we get the id of the semaphore */
    semid = semget (semkey, 1, 0700 | IPC_CREAT);
    printf("Created or attached to semaphore with id %d and key %d\n", semid, semkey);


    /* If I am process 1, I initialize x, otherwise y */
    switch(myid)
    {
        case 1: shared->x = 1;
                /* initialize the semaphore */
                argument.val = 0;
                semctl(semid, 0, SETVAL, argument);

                operations[0].sem_num = 0;            /* operate on the first and only semaphore */
                operations[0].sem_op = -1;            /* decrement by one, and wait */
                operations[0].sem_flg = 0;            /* do not permit undo */
                semop(semid, operations, 1);          /* perform the operation on one semaphore */

                /* Now change the shared memory location to 1 */
                shared->shared = 1;
                printf("The values in the shared memory are now: \n");
                printf("         x = %d\n", shared->x);
                printf("         y = %d\n", shared->y);
                printf("    shared = %d\n", shared->shared);

                break;

        case 2: shared->y = 2;
                /* initialize the semaphore */
                argument.val = 0;
                semctl(semid, 0, SETVAL, argument);


                /* Set the shared memory location to 2 */
                shared->shared = 2;
                printf("The values in the shared memory are now: \n");
                printf("         x = %d\n", shared->x);
                printf("         y = %d\n", shared->y);
                printf("    shared = %d\n", shared->shared);

                operations[0].sem_num = 0;            /* operate on the first and only semaphore */
                operations[0].sem_op = +1;            /* increment by one, and signal */
                operations[0].sem_flg = 0;            /* do not permit undo */
                semop(semid, operations, 1);          /* perform the operation on one semaphore */

                break;

        default:
                printf("Usage: semaphores procnum   (Where procnum is 1 or 2)\n");
                exit(1);    
    }

    /* Now release the semaphore */
    semctl (semid, ignored_int, IPC_RMID, ignored_argument);

    /* Now detach from the memory and remove it */
    shmdt(shared);
    shmctl(shared_mem_id, IPC_RMID, &shm_ds);

}

