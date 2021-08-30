#include<bits/stdc++.h>
using namespace std;

#define PRIORITY_NORMAL 5
#define PRIORITY_HIGH 1
#define PRIORITY_LOW 10
#define QUANTUM 5

int PID = 1, TotalWaitingTime = 0, TotalProcessExecuted = 0, CLOCK = 0;

typedef struct a
{
    int processID, priority;
    int burstTime, arrivalTime, waitingTime, turnAroundTime;
    int lastSeen;
    struct a *next, *previous;
} PCB; //Process Control Block

typedef struct
{
    PCB *head, *tail;
} Queue;

void printQueue(Queue *q)
{
    PCB *pointer = q->head;
    printf("\n\n PID\t CPUBurstTime\t ArrivalTime\t WaitingTime\t NodeLocation\n");

    while(pointer!= 0)
    {
        printf("%3d %8d %15d %15d %15d\n",
               pointer->processID, pointer->burstTime, pointer->arrivalTime, pointer->waitingTime, pointer);
        pointer = pointer->next;
    }
}
void insertProcess(Queue *q, int burst, int priority)
{
    PCB *newProcess = (PCB *) malloc(sizeof(PCB));
    newProcess->next = newProcess->previous = 0; // Reset Link to avoid garbage value

    //set values for new process
    newProcess->processID = PID++;
    newProcess->arrivalTime = CLOCK;

    newProcess->burstTime = burst;
    newProcess->priority = priority;

    newProcess->waitingTime = -1;
    newProcess->turnAroundTime = -1;
    newProcess->lastSeen = -1;

    //link setup
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

PCB deQueue(Queue *list)
{
    PCB t;
    t.processID = 0;

    if(list->head == 0)
    {
        printf("\nQueue is empty\n");
        return t;
    }
    else
    {
        t = *list->head;
        list->head = list->head->next;
        return t;
    }
}

void reAddPCB(Queue *q, PCB r)
{
    PCB *p = (PCB *) malloc(sizeof(PCB));

    //Copy values of r
    p->processID = r.processID;
    p->arrivalTime = r.arrivalTime;
    p->burstTime = r.burstTime;
    p->waitingTime = r.waitingTime;
    p->priority = r.priority;
    p->lastSeen = r.lastSeen;

    p->next = p->previous = 0; //Link Reset
    //Link set
    if(q->head == 0)
    {
        q->head = q->tail = p;
    }
    else
    {
        q->tail->next = p;
        p->previous = q->tail;
        q->tail = p;
    }
}

void executeProcessRR(Queue *rq)
{
    while(!rq->head == 0) //CPU Execution until the rq is not empty
    {
        int waiting;
        PCB r = deQueue(rq); // fetch process from rq
        printf("Process %d in being processed...\n", r.processID);

        //waiting time calculation
        if(r.lastSeen == -1) //1st time to the CPU execution
        {
            waiting = CLOCK - r.arrivalTime;
        }
        else //2nd or following times of entering the CPU
        {
            waiting = CLOCK - r.lastSeen;
        }
        r.waitingTime = waiting;
        TotalWaitingTime += waiting;

        //update CLOCK
        if(r.burstTime <= QUANTUM) //Process completed in this Quantum
        {
            CLOCK += r.burstTime;
            //Upon completion of r
            TotalProcessExecuted++;
            printf("  <<<<<<<<<<<<<<Process %d Completed>>>>>>>>>>>>>>\n", r.processID);
        }
        else //could not finish within this quantum, need more quantum; so re-added to the rear of the Ready Queue
        {
            CLOCK += QUANTUM;
            r.burstTime -= QUANTUM;
            r.lastSeen = CLOCK;
            reAddPCB(rq,r); //the slice expires; so, reAddPCB()

        }
        printf("Waiting time for %d is %d\n", r.processID, r.waitingTime);
        printf("After time slice/QUANTUM completion CLOCK = %d\n\n", CLOCK);
    }//CPU execution loop
}

int main()
{
    Queue *readyQ = (Queue *) malloc(sizeof(Queue));
    readyQ->head = readyQ->tail = 0;

    insertProcess(readyQ, 10, PRIORITY_NORMAL);
    insertProcess(readyQ, 15, PRIORITY_NORMAL);
    insertProcess(readyQ, 2, PRIORITY_NORMAL);
    insertProcess(readyQ, 100, PRIORITY_HIGH);
    printQueue(readyQ);
    executeProcessRR(readyQ);
    printf("\n\nAverage waiting time = %f", (float) (TotalWaitingTime) / (TotalProcessExecuted));
}

