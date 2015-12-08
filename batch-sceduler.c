/* Tests cetegorical mutual exclusion with different numbers of threads.
 * Automatic checks only catch severe problems like crashes.
 */
#include <stdio.h>
#include "tests/threads/tests.h"
#include "threads/malloc.h"
#include "threads/synch.h"
#include "threads/thread.h"
#include "lib/random.h" //generate random numbers

#define BUS_CAPACITY 3
#define SENDER 0
#define RECEIVER 1
#define NORMAL 0
#define HIGH 1

struct semaphore sem;
/*
 *	initialize task with direction and priority
 *	call o
 * */
typedef struct {
	int direction;
	int priority;
} task_t;

int currentDirection,Spriority,Rpriority;

void batchScheduler(unsigned int num_tasks_send, unsigned int num_task_receive,
        unsigned int num_priority_send, unsigned int num_priority_receive);

void senderTask(void *);
void receiverTask(void *);
void senderPriorityTask(void *);
void receiverPriorityTask(void *);


void oneTask(task_t task);/*Task requires to use the bus and executes methods below*/
	void getSlot(task_t task); /* task tries to use slot on the bus */
	void transferData(task_t task); /* task processes data on the bus either sending or receiving based on the direction*/
	void leaveSlot(task_t task); /* task release the slot */



/* initializes semaphores */ 
void init_bus(void){ 
 
    random_init((unsigned int)123456789); 
    sema_init(&sem,3);//initialize semaphore
 //   msg("NOT IMPLEMENTED");
    /* FIXME implement */

}

/*
 *  Creates a memory bus sub-system  with num_tasks_send + num_priority_send
 *  sending data to the accelerator and num_task_receive + num_priority_receive tasks
 *  reading data/results from the accelerator.
 *
 *  Every task is represented by its own thread. 
 *  Task requires and gets slot on bus system (1)
 *  process data and the bus (2)
 *  Leave the bus (3).
 */

void batchScheduler(unsigned int num_tasks_send, unsigned int num_task_receive,
        unsigned int num_priority_send, unsigned int num_priority_receive)
{ const char name;
  Spriority=num_priority_send;
  Rpriority=num_priority_receive;
  while( ( num_tasks_send > 0 ) || (number_task_receive > 0) ) {
    if (num_priority_send > 0){
      num_priority_send--; 
      num_tasks_send--;// assuming that the number of tasks_send is equal to the number of tasks with low and high priority
      thread_create(&name,1,senderPriorityTask,0); 
    }    
    else if ( num_priority_receive > 0 ){
      num_priority_receive--;
      num_task_receive--;
      thread_create(&name,1,receiverPriorityTask,0)
    }
    else if ( num_tasks_send > 0 ){
      num_tasks_send--;
      thread_create(&name,0,senderTask,0); 
    }
    else if ( num_task_receive >0 ) {
      num_task_receive--;
      thread_create(&name,0,receiverTask,0);
    }
  }
}

/* Normal task,  sending data to the accelerator */
void senderTask(void *aux UNUSED){
        task_t task = {SENDER, NORMAL};
        oneTask(task);
}

/* High priority task, sending data to the accelerator */
void senderPriorityTask(void *aux UNUSED){
        task_t task = {SENDER, HIGH};
        oneTask(task);
}

/* Normal task, reading data from the accelerator */
void receiverTask(void *aux UNUSED){
        task_t task = {RECEIVER, NORMAL};
        oneTask(task);
}

/* High priority task, reading data from the accelerator */
void receiverPriorityTask(void *aux UNUSED){
        task_t task = {RECEIVER, HIGH};
        oneTask(task);
}

/* abstract task execution*/
void oneTask(task_t task) {
  getSlot(task);
  transferData(task);
  leaveSlot(task);
}


/* task tries to get slot on the bus subsystem */
void getSlot(task_t task) 
{
  int taskDirection=task->direction;

  while (task->priority==0 )  { 
    if ( ( Spriority == 0 ) && ( Rpriority == 0 ) ) {
     break;
    }
  }
  while !( ( taskDirection == currentDirection ) || ( sem == 3 ) ){

  }
  currentDirection=taskDirection;
  sema_down(&sem); //waits for the semaphore to become positive and then decrements sem by one
  if ( task->priority == HIGH ) {
    if ( task->direction == 0 ) {
       Spriority--; 
    } 
    else{ 
       Rpriority--;
    }
  } 
   
}

/* task processes data on the bus send/receive */
void transferData(task_t task) 
{
  printf("In the process of transfering data..........");
  sleep(random_ulong());//the thread sleeps or a random time
  printf("Exiting the transfering data ........"); 
 
}

/* task releases the slot */
void leaveSlot(task_t task) 
{
  sema_up(&sem); //increments the value of semaphore by one and wakes up the next
}
