#include<bits/stdc++.h>
using namespace std;

#define PRIORITY_Normal 5
#define Priority_High
#define PRIORITY_Low 10
#define QUANTUM 5

int PID = 1, CLOCK = 0, TotalWaitingTime = 0, TotalProcessExecuted = 0;
typedef struct a
{
    int pid, priority;
    int burstTime, arrivalTime, waitingTime, turnAroundTime;
    int lastSeen;
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

void insertProcessAsSJ(Queue *q, int burst, int priority)
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
    //appropriate location search
    PCB *loc = q->head;
    while(loc != 0 && burst > loc->burstTime){
        loc = loc->next;
    }
    //loc pv finds out thepost position for the new PCB

    //link setup
    if(loc == 0)//found nothing so it has to be at the end(tail)
    {
        q->tail->next = newProcess;
        newProcess->previous = q->tail;
        q->tail = newProcess;
    }
    else if(loc == q->head)
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

void re_addPCB(Queue *q, PCB r){
    //Copy values of r
    PCB *p = (PCB *) malloc(sizeof(PCB));
    p->pid = r.pid;
    p->priority = r.priority;
    p->burstTime = r.burstTime;
    p->arrivalTime = r.arrivalTime;
    p->waitingTime = r.waitingTime;
    p->turnAroundTime = r.turnAroundTime;
    p->lastSeen = r.lastSeen;

    p->next = p->previous = 0;
    if(q->head == 0){
        q->head = q->tail = p;
    }
    else{
        q->tail->next = p;
        p->previous = q->tail;
        q->tail = p;
    }
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
     printf("\n\nAverage Waiting time = %f",(float)(TotalWaitingTime/TotalProcessExecuted));
}

void executeProcessRR(Queue *rq, int  waiting)
{
    while(!rq->head == 0)//CPU Execution untill the RQ is not empty
    {
        PCB r = deQueue(rq);
        printf("\n\nProcess %d is being Processed", r.pid);
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
        if(r.burstTime <= QUANTUM)//r is finished with THIS QUANTUM/RR time slice
        {
            CLOCK += r.burstTime;
            //conditioned upon completion of r
            TotalProcessExecuted++;
            printf("\n Process Completed %d\n", r.pid);
        }
        else //could not finish within this quantum, need more quantum; so re-added to the rear of the Ready Queue
        {
            //the slice expires; so, add to rear of the RQ
            CLOCK += QUANTUM;
            r.burstTime = r.burstTime - QUANTUM;
            r.lastSeen = CLOCK;
            re_addPCB(rq, r);
        }
        printf("\n Waiting time for Process-%d is %d",r.pid, r.waitingTime);
        printf("\nAfter Completion, CLOCK-%d", CLOCK);
    } //CPU execution loop
     printf("\n\nAverage Waiting time = %f",(float)(TotalWaitingTime/TotalProcessExecuted));
}

main(){
 Queue *readyQ = (Queue *) malloc(sizeof(Queue));
    readyQ->head = readyQ->tail = 0;

    insertProcess(readyQ, 10, PRIORITY_Normal);
    insertProcess(readyQ, 5, PRIORITY_Normal);
    insertProcess(readyQ, 20, PRIORITY_Normal);
    printQueue(readyQ);
    executeProcess(readyQ);
}

