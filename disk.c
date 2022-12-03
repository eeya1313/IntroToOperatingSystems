// name: jessica stueber
// Class: Operating Systems
// Lab: CPU Scheduling


#include "oslabs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


bool nullRCB(struct RCB requests){
    if((requests.request_id == 0) && (requests.arrival_timestamp == 0) && (requests.cylinder == 0) && (requests.address == 0) && (requests.process_id == 0)){
        return true;
    }
    else{
        return false;
    }
}


struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp);
struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){

    if(nullRCB(current_request)){
        return new_request;
    }
    else{
        if(*queue_cnt <= QUEUEMAX){
            request_queue[*queue_cnt] = new_request;
            *queue_cnt += 1;
            return current_request;
        }
        else{
            return current_request;
        }
    }
}; 


struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX],int *queue_cnt);
struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX],int *queue_cnt){
    //If request_queue is empty indicating there is no request next.
    struct RCB nullRCB = {0, 0, 0, 0, 0};
    if(*queue_cnt == 0){
        return nullRCB;
    }
    else{
        int firstInitial = 0;
        struct RCB nextRCB;
        int arrival = request_queue[0].arrival_timestamp;
        for(int i = 1; i < *queue_cnt; i++){
            if(arrival > request_queue[i].arrival_timestamp){
                arrival = request_queue[i].arrival_timestamp;
                firstInitial= i;
            }
        }
        nextRCB = request_queue[firstInitial];
        for(int i = firstInitial; i < *queue_cnt; i++){
            request_queue[i] = request_queue[i+1];
        }

        *queue_cnt -= 1;
        return nextRCB;
    }
};


struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp);
struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){

    if(nullRCB(current_request)){
        return new_request;
    }
    else{
 
        if(*queue_cnt <= QUEUEMAX){

            request_queue[*queue_cnt] = new_request;
            *queue_cnt += 1;
            return current_request;
        }
        else{
            return current_request;
        }
    }
};


struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt,int current_cylinder);
struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX],int *queue_cnt,int current_cylinder){
   
    struct RCB nullRCB = {0, 0, 0, 0, 0};
    if(*queue_cnt == 0){
        return nullRCB;
    }
    else{
        struct RCB nextRCB;
        int firstInitial = 0, firstCylinder = 0, firstArrival = 0;
        firstCylinder = abs(current_cylinder - request_queue[0].cylinder);
        firstArrival = request_queue[0].arrival_timestamp;
        //Looking for the smallest gap between the cylinder(s).
        for(int i = 1; i < *queue_cnt; i++){
            if(firstCylinder == abs(current_cylinder - request_queue[i].cylinder)){
                if(firstArrival > request_queue[i].arrival_timestamp){
                    firstCylinder = abs(current_cylinder - request_queue[i].cylinder);
                    firstArrival = request_queue[i].arrival_timestamp;
                    firstInitial = i;
                }
            }
            else if(firstCylinder > abs(current_cylinder - request_queue[i].cylinder)){
                firstCylinder = abs(current_cylinder - request_queue[i].cylinder);
                firstArrival = request_queue[i].arrival_timestamp;
                firstInitial = i;
            }
        }
        nextRCB = request_queue[firstInitial];
        for(int i = firstInitial; i < *queue_cnt; i++){
            request_queue[i] = request_queue[i+1];
        }
        *queue_cnt -= 1;
        return nextRCB;
    }
};


struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp);
struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX],int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp){
    //If disk space is free return new request.
    if(nullRCB(new_request)){
        return new_request;
    }

    else{
        if(*queue_cnt <= QUEUEMAX){
            request_queue[*queue_cnt] = new_request;
            *queue_cnt += 1;
            return current_request;
        }
        else{
            return current_request;
        }
    }
};

\
struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],int  *queue_cnt, int current_cylinder, int scan_direction);
struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX],int  *queue_cnt, int current_cylinder, int scan_direction){
    //Handle empty queue.
    struct RCB nullRCB = {0, 0, 0, 0, 0};
    if(*queue_cnt <= 0){
        return nullRCB;
    }

    else{
        struct RCB nextRCB;
        int pointer = 0, initialCylinder = 0, intialArrival = 0;
        bool flag = false, posCylinder = false, firstCylinder = false, firstDirection = false, oppositeDirection = false;
        for (int i = 0; i < *queue_cnt; i++){

            if (current_cylinder == request_queue[i].cylinder){
                if (firstCylinder == false){
                    intialArrival = request_queue[i].arrival_timestamp;
                    pointer = i;
                    firstCylinder = true;
                    flag = true;
                }
                else if (intialArrival > request_queue[i].arrival_timestamp){
                    intialArrival = request_queue[i].arrival_timestamp;
                    pointer = i;
                }
            }

            else if ((scan_direction == 1) && (flag == false)){
                if(request_queue[i].cylinder - current_cylinder > 0){

                    if (firstDirection == false){
                        initialCylinder = request_queue[i].cylinder - current_cylinder;
                        pointer = i;
                        posCylinder = true;
                        firstDirection = true;
                    }
                    else if (initialCylinder > request_queue[i].cylinder - current_cylinder){
                        initialCylinder = request_queue[i].cylinder - current_cylinder;
                        pointer = i;
                    }
                }
                else if ((posCylinder == false) && (current_cylinder - request_queue[i].cylinder > 0)){
                    if (oppositeDirection == false){
                        initialCylinder = abs(request_queue[i].cylinder - current_cylinder);
                        pointer = i;
                        oppositeDirection = true;
                    }
                    else if(initialCylinder > abs(request_queue[i].cylinder - current_cylinder)){
                        initialCylinder = abs(request_queue[i].cylinder - current_cylinder);
                        pointer = i;
                    }
                }
            }
 
            else if ((scan_direction == 0) && (flag == false)){
                if(current_cylinder - request_queue[i].cylinder > 0){

                    if (firstDirection == false){
                        initialCylinder = current_cylinder - request_queue[i].cylinder;
                        pointer = i;
                        posCylinder = true;
                        firstDirection = true;
                    }
                    else if (initialCylinder > current_cylinder - request_queue[i].cylinder){
                        initialCylinder = current_cylinder - request_queue[i].cylinder;
                        pointer = i;
                    }
                }
                else if ((posCylinder == false) && (current_cylinder - request_queue[i].cylinder < 0)){
                    if (oppositeDirection == false){
                        initialCylinder = abs(current_cylinder - request_queue[i].cylinder);
                        pointer = i;
                        oppositeDirection = true;
                    }
                    else if(initialCylinder > abs(request_queue[i].cylinder - current_cylinder)){
                        initialCylinder = abs(current_cylinder - request_queue[i].cylinder);
                        pointer = i;
                    }
                }
            }
        }
        nextRCB = request_queue[pointer];
        for (int i = pointer; i < *queue_cnt ; i++){
            request_queue[i] = request_queue[i+1];
        }

        *queue_cnt -= 1;
        return nextRCB;
    }
};
