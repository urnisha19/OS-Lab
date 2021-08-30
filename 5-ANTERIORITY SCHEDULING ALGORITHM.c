#include<stdio.h>
#include<stdlib.h>

int PID = 1, CLOCK = 0, TotalWaitingTime = 0, TotalProcessExecuted = 0;

typedef struct a
{
    int processID, priority;
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

    printf("\n\nPID\t CPUBurstTime\t Priority\t ArrivalTime\t WaitingTime\t NodeLocation\n");
    while(pointer != 0)
    {
        printf("%3d %8d %15d %15d @%15d\n",
               pointer->processID,pointer->burstTime,pointer->priority,pointer->arrivalTime,pointer->waitingTime,pointer);
        pointer=pointer->next;
    }
}

void insertProcess(Queue *q, int CPUBurstTime, int priority)
{
    PCB *newProcess = (PCB *) malloc(sizeof(PCB));
    newProcess->next = newProcess->previous = 0;

    newProcess->processID = PID++;
    newProcess->arrivalTime = CLOCK;

    newProcess->burstTime = CPUBurstTime;
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

        //find the suitable location for PCB based on Low priority First process
        PCB *loc = q->head;
        while (loc != 0 && priority > loc->priority)
        {
            loc = loc->next;
        }

        if (loc == 0)
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

void executeProcess(Queue *rq)
{
    while(!rq->head == 0)
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

    //PCB#1 (Burst Time, Priority) : 5 10
    insertProcessAsShortestJob(readyQ, 5, 10);
    //PCB#2 (Burst Time, Priority) : 15 5
    insertProcessAsShortestJob(readyQ, 15, 5);

    printQueue(readyQ);
    executeProcess(readyQ);
}

