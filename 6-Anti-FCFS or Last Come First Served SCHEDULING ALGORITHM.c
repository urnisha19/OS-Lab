#include<stdio.h>
#include<stdlib.h>

int PID = 1, CLOCK = 0, TotalWaitingTime = 0, TotalProcessExecuted = 0;

typedef struct a
{
    int pid, priority;
    int burstTime, arrivalTime, waitingTime, turnAroundTime;
    struct a *next, *previous;
} PCB;

typedef struct
{
    PCB *head, *tail;
} Queue;

void printQueue(Queue *q)
{
    PCB *pointer = q->head;
    printf("\nPID\t CPUBurst\t Arrival\t Waiting\t NodeLocation\n");
    while(pointer != 0)
    {
        printf("%3d %8d %15d %15d %15d\n",
               pointer->pid,pointer->burstTime,pointer->arrivalTime,pointer->waitingTime,pointer);
        pointer=pointer->next;
    }
}

void insertProcess(Queue *q, int burst, int priority)
{
    PCB *newProcess = (PCB *) malloc(sizeof(PCB));
    newProcess->next = newProcess->previous = 0;

    newProcess->pid = PID++;
    newProcess->arrivalTime = CLOCK;

    newProcess->burstTime = burst;
    newProcess->priority = priority;

    newProcess->waitingTime = -1;
    newProcess->turnAroundTime = -1;

    if(q->head == 0)
    {
        q->head = q->tail = newProcess;
    }
    else
    {
        q->tail->next = newProcess;
        newProcess->previous = q->tail;
        q->tail = newProcess;
    }
}

//Change in this function
PCB deQueue(Queue *list)
{
    PCB t;
    t.pid = 0;

    if(list->head == 0)//If the list is empty it will show empty
    {
        printf("\n Queue is Empty \n");
        return t;
    }
    //If the list is not empty we will take the PCB variable from the tail for the Last Come First Serve Process

    t = *list->tail; //taking pointer value(tail) of the PCB
    list->tail = list->tail->previous; //updating the tail value, setting the tail to the previous
    return t;
}

//Change in this function
void executeProcess(Queue *rq)
{
    while(!rq->tail == 0) //CPU will execute the process until the ready queue tail is emtpy
    {
        PCB r = deQueue(rq);
        printf("\n\nProcess %d is being Processed", r.pid);

        r.waitingTime = CLOCK - r.arrivalTime;
        TotalWaitingTime += r.waitingTime;

        CLOCK += r.burstTime;

        printf("\nWaiting time for Process-%d is %d",r.pid, r.waitingTime);
        printf("\nAfter Completion, CLOCK-%d", CLOCK);

        TotalProcessExecuted++;
    }
}

main()
{
    Queue *readyQ = (Queue *) malloc(sizeof(Queue));
    readyQ->head = readyQ->tail = NULL;

    insertProcess(readyQ, 5,10);
    insertProcess(readyQ, 15,5);

    printQueue(readyQ);
    executeProcess(readyQ);

    printf("\n\nAverage Waiting time = %f",(float) (TotalWaitingTime/TotalProcessExecuted));
    return 0;
}

