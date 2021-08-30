#include<bits/stdc++.h>
using namespace std;

#define PRIORITY_NORMAL 5
#define PRIORITY_HIGH 1
#define PRIORITY_LOW 10

int PID = 1, CLOCK = 0, TotalWaitingTime = 0, TotalProcessExecuted = 0;

//For an item of linked list
typedef struct a
{
    int processID, priority;
    int burstTime, arrivalTime, waitingTime, turnAroundTime;
    struct a *next, *previous;
} PCB; //PCB:Process Control Block

//For a Linked list
typedef struct
{
    PCB *head, *tail;
} Queue;

//visit and print all nodes
void printQueue(Queue *q)
{
    PCB *pointer = q->head;


    printf("\n\nPID\t CPUBurstTime\t ArrivalTime\t WaitingTime\t NodeLocation\n");
    while(pointer != 0)
    {
        printf("%3d %8d %15d %15d @%15d\n",
               pointer->processID,pointer->burstTime,pointer->arrivalTime,pointer->waitingTime,pointer);
        pointer=pointer->next;
    }
}

void insertProcess(Queue *q, int CPUBurstTime, int priority)
{
    PCB *newProcess = (PCB *) malloc(sizeof(PCB));
    newProcess->next = newProcess->previous = 0; //Link reset to avoid garbage value

    //set values for new process
    newProcess->processID = PID++;
    newProcess->arrivalTime = CLOCK;

    newProcess->burstTime = CPUBurstTime;
    newProcess->priority = priority;

    newProcess->waitingTime = -1;
    newProcess->turnAroundTime = -1;

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

//SJF process scheduling
void insertProcessAsShortestJob(Queue *q, int CPUBurstTime, int priority)
{
    if (q->head == 0)
    {
        insertProcess(q, CPUBurstTime, priority);
    }
    else
    {
        PCB *newProcess = (PCB *)malloc(sizeof(PCB));
        newProcess->next = newProcess->previous = 0;

        newProcess->processID = PID++;
        newProcess->arrivalTime = CLOCK;

        newProcess->burstTime = CPUBurstTime;
        newProcess->priority = priority;

        newProcess->waitingTime = -1;
        newProcess->turnAroundTime = -1;

        //find the suitable location for PCB
        PCB *loc = q->head;
        while (loc != 0 && CPUBurstTime > loc->burstTime)
        {
            loc = loc->next;
        }

        // Link Setup
        if (loc == 0) //found nothing so it has to be at the end(tail)
        {
            q->tail->next = newProcess;
            newProcess->previous = q->tail;
            q->tail = newProcess;
        }
        else if (loc == q->head)
        {
            q->head->previous = newProcess;
            newProcess->next = q->head;
            q->head = newProcess;
        }
        else
        {
            newProcess->previous = loc->previous;
            loc->previous->next = newProcess;
            loc->previous = newProcess;
            newProcess->next = loc;
        }
    }
}

PCB deQueue(Queue *list)
{
    PCB t;
    t.processID = 0;
    if(list->head == 0)
    {
        printf("\n Queue is empty \n");
        return t;
    }
    t = *list->head;
    list->head =  list->head->next;
    return t;
}

//CPU Execution
void executeProcess(Queue *rq)
{
    while(!rq->head == 0)//CPU Execution
    {
        PCB r = deQueue(rq);
        printf("\n\nProcess %d is being Processed", r.processID);

        r.waitingTime = CLOCK - r.arrivalTime;
        TotalWaitingTime += r.waitingTime;
        CLOCK += r.burstTime;

        printf("\nWaiting time for Process->%d is %d",r.processID, r.waitingTime);
        printf("\nAfter Completion, CLOCK->%d", CLOCK);

        TotalProcessExecuted++;
    }
    printf("\n\nAverage Waiting time(FCFS) = %f\n",(float)(TotalWaitingTime/TotalProcessExecuted));
}

main()
{
    Queue *readyQ = (Queue *) malloc(sizeof(Queue));
    readyQ->head = readyQ->tail = 0;
    //SJF
    insertProcessAsShortestJob(readyQ, 10, PRIORITY_NORMAL);
    insertProcessAsShortestJob(readyQ, 5, PRIORITY_NORMAL);
    insertProcessAsShortestJob(readyQ, 20, PRIORITY_NORMAL);

    printQueue(readyQ);
    executeProcess(readyQ);
}

