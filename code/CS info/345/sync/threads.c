#include <pthread.h>
#include <stdio.h>

int print_message_function( void *ptr );

int x = 1;

main()
{
        pthread_t thread1, thread2;
        int thread1result, thread2result;
        char *message1 = "Hello";
        char *message2 = "World";
        pthread_attr_t *pthread_attr_default = NULL;

        printf("Begin\n");
        pthread_create( &thread1, pthread_attr_default,
                        (void*)&print_message_function, (void*) message1);
        pthread_create(&thread2, pthread_attr_default,
                        (void*)&print_message_function, (void*) message2);

        pthread_join(thread1, (void *)&thread1result);
        printf("End thread1 with %d\n", thread1result);

        pthread_join(thread2, (void *)&thread2result);
        printf("End thread2 with %d\n", thread2result);

        exit(0);
}

int print_message_function( void *ptr )
{
        char *message;
        message = (char *) ptr;
        printf("%s ", message);
        fflush(stdout);
        return x++;
}


