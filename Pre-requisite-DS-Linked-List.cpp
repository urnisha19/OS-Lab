#include<bits/stdc++.h>
using namespace std;

//For an item of linked list
typedef struct a
{
    int id;
    struct a *next, *previous; 
    //location rakar jonno struct a type er pointer variable *next *previous
} node; 
//typedef diye struct a ke node nam e define kore deway just node likhlei sheti struct a mean korbe

//For a Linked list
typedef struct
{
    node *head, *tail; 
    //1 ta linked list start(*head) and end( *tail) position
} List;

//1 ta array index diye(e.g a[2]) jevabe amra 1ta variable access korte pari,
//ekane shevabe 1ta variable access korar jonno kunu 1ta list er kunu 1ta location theke khuje ber korte hobe
//e kaje amara stack use kore korbo. (Stack-Last in First Out)

void push(List *list, int id)//here, List=kun list e *list=location and id=value
{
    //1ta node toiri korbo, jar modde id,*next,*previous 3ta value ache
    node *t=(node *) malloc(sizeof(node));
    //*t(1ta temporary node pointer) toiri kore item add korar jonno memory allocate
    //function malloc() is used to allocate the requested size of bytes and it returns a pointer to the first byte of allocated memory.

    t->id = id;//je value push korbo sheta assign
    //sturcture er pointer teke member access korar jonno -> use kora hoy. eta shudhu structure hole t.id liktam
    
    t->next= t->previous=0; //initially assign 0 to avoid garbage value
    //item jodi 1st e take tobe previous takbe na r jodi last e take tobe next takbe na
    //list empty takle head tail same hobe, and next previous empty;

    if(list->head==0)//if the list is empty
    {
        list->head = list->tail = t;
    }
    else
    {
        list->tail->next = t; //setting pointer t to pointer next
        t->previous= list->tail; //now setting prestent tail pointer as previous
        list->tail = t; //again tail set kore deowa
    }
    printf("%d has been added %d\n", t->id,t); //t holo node er pointer location ja address dibe
}

//Lab 2
int pop(List *list)
{
    if(list->tail == 0)//if the list is empty
    {
        printf("\nStack underflow \n");
        return -1;
    }
    int t = list->tail->id; //Last theke orthat tail teke pop korbe
    list->tail = list->tail->previous;
    return t;
}
//Lab 02 finish

main()
{
    //Linked list
    List *stack = (List *) malloc(sizeof(List));
    stack->head = stack->tail = 0;

    push(stack, 1);
    push(stack, 2);
    push(stack, 3);

    printf("\nPopped %d", pop(stack));
    printf("\nPopped %d", pop(stack));
    printf("\nPopped %d", pop(stack));
    printf("\nPopped %d", pop(stack));
}


