#include "structs.h"
#include <stdio.h>
#include <stdlib.h>

struct Student* createStudent(int sid){
    struct Student* newNode=malloc(sizeof(struct Student));
    newNode->arrivalTime=5+rand()%(11);
    newNode->sid=sid;
    return newNode;
}


struct Queue* createQueue(){
    struct Queue* newQueue=malloc(sizeof(struct Queue));
    newQueue->head=NULL;
    return newQueue;
}

void enqueue(struct Queue* queue, struct Student* student){
    struct Node* studentNode=malloc(sizeof(struct Node));
    studentNode->data=student;
    studentNode->next=NULL;
    if(queue->head==NULL){
       queue->head=studentNode;
       queue->tail=queue->head;

    }else{
       queue->tail->next=studentNode;
       queue->tail=studentNode;
    
    }
}



void dequeueStudy(struct Queue* queue, struct Student* student){
    if(queue->head==NULL) return;
    struct Node* temp=queue->head;
    struct Node* prev=queue->head;
    while(temp!=NULL){
        if(student==temp->data){
            if(temp==queue->head){
                queue->head=queue->head->next;
            }
            prev->next=temp->next;
            free(temp);
            return;

        }
        prev=temp;
        temp=temp->next;
    }

}


 struct Student* dequeueWait(struct Queue* queue){
    struct Node* temp=queue->head;
    struct Student* firstStudent=queue->head->data;   
    queue->head=queue->head->next;
    free(temp);
    return firstStudent;
}



void displayQueue(struct Queue* queue){
    struct Node* temp=queue->head;
    while(temp!=NULL){
        printf("|%d",(temp->data)->sid);
        temp=temp->next;
    }
    printf("\n");   
}

int getQueueLen(struct Queue* queue){
    int count=0;
    struct Node* temp=queue->head;
    while(temp!=NULL){
        ++count;
        temp=temp->next;
    }
    return count;
}

// int main() {
//     struct Queue *myqueue = createQueue();

//     struct Student *st1 = createStudent(1);
//     struct Student *st2 = createStudent(2);
//     struct Student *st3 = createStudent(3);
//     struct Student *st4 = createStudent(4);

//     enqueue(myqueue, st1);
//     enqueue(myqueue, st2);
//     enqueue(myqueue, st3);
//     enqueue(myqueue, st4);

//     displayQueue(myqueue);

//     dequeueStudy(myqueue, st4);
//     dequeueStudy(myqueue, st3);
//     dequeueStudy(myqueue, st2);
//     dequeueStudy(myqueue, st1);

//     displayQueue(myqueue);
    
//     return 0;
// }