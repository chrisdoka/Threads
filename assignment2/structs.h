#ifndef QUEUE_H
#define QUEUE_H

struct Student {    /* struct for student details */
    int arrivalTime;   /* time at waiting room arrival*/
    int sid;         /* AM*/
} ;

struct Node {                /* node info*/
    struct Student* data;
    struct Node* next;
};

struct Queue {
    struct Node* head;
    struct Node* tail;
};

struct Student* createStudent(int sid);
struct Queue* createQueue();
void enqueue(struct Queue* queue, struct Student* student);
void dequeueStudy(struct Queue* queue,struct Student* student);
struct Student* dequeueWait(struct Queue* queue);
void displayQueue(struct Queue* queue);
int getQueueLen(struct Queue* queue);

#endif 
