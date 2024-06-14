/**
***************************
* Name: Christina Doka*
* AM:   4577            *
* Date: 5/11/2023        *
***************************
**/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include "structs.h"

#define MAX_STUDY_SIZE 8

pthread_mutex_t mutexWaiting;
pthread_mutex_t mutexStudying;
pthread_mutex_t mutexPass;
pthread_mutex_t mutexCheckFunctionCall;
pthread_cond_t condStud;

int pass=1;
int checkFunctionCall=1;

struct Queue* queueWait;
struct Queue* queueStudy;

void printRooms();
void tryingToEnterStudyRoom(struct Student* newStud);
void dequeueStudyRoom(struct Student* newStud);
void updateStudyPass();
void checkPass(struct Student* newStud);

void *routine(void* arg){
    struct  Student* newStud;
    int sid=*(int*)arg;
    newStud=createStudent(sid);

    updateStudyPass();
    checkPass(newStud);
    pthread_mutex_lock(&mutexCheckFunctionCall);
    if(checkFunctionCall==1){
        tryingToEnterStudyRoom(newStud);
    }   
    else{
        checkFunctionCall=1;
    }
    pthread_mutex_unlock(&mutexCheckFunctionCall);
  
    sleep(newStud->arrivalTime);
    dequeueStudyRoom(newStud);
    free(arg);
}

int main(int argc, char* argv[]) {  
    
    int N=atoi(argv[1]);
    if(N<20 || N>40){
        printf("it can only enter 20 till 40!! Sorry\n");
        exit(1);
    }
    pthread_t* studentThreads=(pthread_t*)malloc(sizeof(pthread_t)*N);
    queueWait=createQueue();
    queueStudy=createQueue();
    pthread_mutex_init(&mutexWaiting, NULL);
    pthread_mutex_init(&mutexPass, NULL);
    pthread_mutex_init(&mutexStudying, NULL);
    pthread_mutex_init(&mutexCheckFunctionCall, NULL);
    pthread_cond_init(&condStud, NULL);
    for (int i = 0; i < N; i++) {
            int *sid=(int*)malloc(sizeof(int));
            *sid=i; 
            if (pthread_create(&studentThreads[i], NULL, &routine, sid) != 0) {
                perror("Failed to create thread");
            }  
    }
    for (int i = 0; i < N; i++) {
        if (pthread_join(studentThreads[i], NULL) != 0) {
            perror("Failed to join thread");
        }
    }
    pthread_mutex_destroy(&mutexWaiting);
    pthread_mutex_destroy(&mutexPass);
    pthread_mutex_destroy(&mutexStudying);
    pthread_mutex_destroy(&mutexCheckFunctionCall);
    pthread_cond_destroy(&condStud);
    return 0;
}

void printRooms(){
    printf("Study Room:");
    displayQueue(queueStudy);
    printf("waiting Room:");
    displayQueue(queueWait);
    printf("\n");
}

void updateStudyPass(){
    pthread_mutex_lock(&mutexStudying);
    if(getQueueLen(queueStudy)==MAX_STUDY_SIZE){
        pthread_mutex_lock(&mutexPass);
        pass=0;
        pthread_mutex_unlock(&mutexPass);
    }
    pthread_mutex_unlock(&mutexStudying);
}

void checkPass(struct Student* newStud){
    pthread_mutex_lock(&mutexPass);
    struct Student* firstStud;
    if(pass==0){
        printf("Student %d cannot enter the study hall. It is full.\n",newStud->sid);
        pthread_mutex_lock(&mutexWaiting);
        enqueue(queueWait,newStud);
        printRooms();
        pthread_mutex_unlock(&mutexWaiting);
        pthread_cond_wait(&condStud, &mutexPass);
        firstStud=dequeueWait(queueWait);
        tryingToEnterStudyRoom(firstStud);
        pthread_mutex_lock(&mutexCheckFunctionCall);
        checkFunctionCall = 0;
        pthread_mutex_unlock(&mutexCheckFunctionCall);

    }
    pthread_mutex_unlock(&mutexPass);
}

void tryingToEnterStudyRoom(struct Student* newStud){
    pthread_mutex_lock(&mutexStudying);
    enqueue(queueStudy,newStud);
    printRooms();
    pthread_mutex_unlock(&mutexStudying);  
}

void dequeueStudyRoom(struct Student* newStud){
    pthread_mutex_lock(&mutexStudying);
    printf("Student %d exited the study hall after studying for %d secs\n",newStud->sid ,newStud->arrivalTime);
    dequeueStudy(queueStudy,newStud);
    printRooms();
   
    if(getQueueLen(queueStudy)==0){
         pthread_mutex_lock(&mutexPass);
        pass = 1;
         pthread_mutex_unlock(&mutexPass);
        for(int i=0;i<MAX_STUDY_SIZE;i++){
            pthread_cond_signal(&condStud);
        }
       
    }
    pthread_mutex_unlock(&mutexStudying);  
}