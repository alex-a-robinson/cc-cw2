#include "dinning_philosophers.h"

/* TODO
* - Implement sleep
* - Implement lock
* - Implement unlock
* - Implement shared memory
*
* NOTE: Access to pointer is security violation? Pass with it size so can only access some amount
*/

typedef struct {
    int forks[PHILOSOPHERS];
    int ids[PHILOSOPHERS];
    int state; // game state 0 init 1 going
} arbitrator_t;

arbitrator_t* arbitrator;

int get_id() {
    int id = -1;
    lockm(arbitrator);
    for (int i=0; i<PHILOSOPHERS; i++) {
        if (arbitrator->ids[i] != -1) {
            id = arbitrator->ids[i];
            arbitrator->ids[i] = -1;
            if (i == PHILOSOPHERS-1) { // Start once the last id has been selected
                arbitrator->state = 1;
            }
            break;
        }
    }
    unlockm(arbitrator);
    return id;
}

int get_left_fork(int id, arbitrator_t* arbitrator) {
    int left_fork = arbitrator->forks[id];
    if (left_fork) {
        arbitrator->forks[id] = 0;
    }
    return left_fork;
}

int get_right_fork(int id, arbitrator_t* arbitrator) {
    int right_fork = arbitrator->forks[(id+1)%PHILOSOPHERS];
    if (right_fork) {
        arbitrator->forks[(id+1)%PHILOSOPHERS] = 0;
    }
    return right_fork;
}

void put_forks_back(int id, arbitrator_t* arbitrator, int left_fork, int right_fork) {
    if (left_fork) {
        arbitrator->forks[id] = 1;
    }
    if (right_fork) {
        arbitrator->forks[(id+1)%PHILOSOPHERS] = 1;
    }
}

void philosopher() {
    int eat_count, think_count = 0;
    char b[1024];
    int id = get_id();

    // Once all the IDs are given out, start
    while (1) {
        sleep(2);
        lockm(arbitrator);
        if (arbitrator->state) {
            break;
        }
        unlockm(arbitrator);
    }

    puts("Hello from Philosopher #"); puts(ss(b,id));puts("!\n");

    for (int j=0; j<10; j++) { // 10 iterations
        think_count++;
        sleep(2); // Thinking

        if (lockm(arbitrator) == -1) { // Failed to lock
            exit(EXIT_FAILURE);
        }

        int left_fork = get_left_fork(id, arbitrator);
        int right_fork = get_left_fork(id, arbitrator);
        if (left_fork && right_fork) {
            eat_count++;
            puts("Philosopher #"); puts(ss(b,id));puts(" thought "); puts(ss(b,think_count));puts(" times and eaten "); puts(ss(b,eat_count));puts(" times\n");
            sleep(1);
        }
        put_forks_back(id, arbitrator, left_fork, right_fork);
        unlockm(arbitrator);
    }

    exit(eat_count);
}

void main_dp() {
    char b[1024];

    err("Iniated forks\n");
    // Init forks and ids
    arbitrator->state = 0;
    for (int i=0; i<PHILOSOPHERS; i++) {
        arbitrator->forks[i] = 1;
        arbitrator->ids[i] = i;
    }

    // Share memeory
    sharem(arbitrator);

    err("Spawning philisophers\n");

    // Spawn philosphers
    int philosopher_pids[PHILOSOPHERS] = {0};
    for (int i=0; i<PHILOSOPHERS; i++) {
        pid_t pid = fork_wait();
        if (0 == pid) {
            exec(&philosopher);
        } else {
            philosopher_pids[i] = pid;
        }
    }

    // Wait for results
    err("Waiting for responses\n");
    for (int i=0; i<PHILOSOPHERS; i++) {
        //err("Waiting for: "); err(ss(b, philosopher_pids[i])); err("\n");
        int r = waitp(philosopher_pids[i]);
        err("-Philosopher #"); err(ss(b,i));err(" ate "); err(ss(b,r)); err("\n");

    }

    // Unshare memory
    unsharem(arbitrator);

    exit(EXIT_SUCCESS);
}
