#include<bits/stdc++.h>
using namespace std;

#define Priority_Normal 5
#define Priority_High 1
#define Priority_Low 10

int PID = 1, CLOCK = 0, TotalWaitingTime = 0, TotalProcessExecuted = 0;

typedef struct a
{
    int pid, priority;
    int burstTime, arrivalTime, waitingTime, turnAroundTime;
    struct a *next, *previous;
} PCB; //Process Control Blog

typedef struct
{
    PCB *head, *tail;
} Queue;

void printQueue(Queue *q)
{
    PCB *pn = q->head;
    printf("\nPID\t CPUBurst\t Arrival\t Waiting\t NodeLocation\n");
    while(pn != 0)
    {
        printf("%3d %8d %15d %15d %15d\n",
               pn->pid,pn->burstTime,pn->arrivalTime,pn->waitingTime,pn);
        pn=pn->next;
    }
}

void insertProcess(Queue *q, int burst, int priority)
{
    PCB *newProcess = (PCB *) malloc(sizeof(PCB));
    newProcess->next = newProcess->previous = 0; //Link reset

    //set values for new process
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

//Lab 4
PCB deQueue(Queue *list)
{
    PCB t;
    t.pid = 0;
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
    while(!rq->head == 0)//CPU Execution
    {
        PCB r = deQueue(rq);
        printf("\n\nProcess %d is being Processed", r.pid);
        r.waitingTime = CLOCK - r.arrivalTime;
        TotalWaitingTime += r.waitingTime;
        CLOCK += r.burstTime;
        printf("\n Waiting time for Process-%d is %d",r.pid, r.waitingTime);
        printf("\nAfter Completion, CLOCK-%d", CLOCK);
        TotalProcessExecuted++;
    }
}
//lab 4 end

main()
{
    Queue *readyQ = (Queue *) malloc(sizeof(Queue));
    readyQ->head = readyQ->tail = 0;

    insertProcess(readyQ, 10, Priority_Normal);
    insertProcess(readyQ, 5, Priority_Normal);
    insertProcess(readyQ, 20, Priority_Normal);

    printQueue(readyQ);
    //deQueue(readyQ);
    //printQueue(readyQ);
    executeProcess(readyQ);
    printf("\n\nAverage Waiting time = %f",(float)(TotalWaitingTime/TotalProcessExecuted));
}
