
// name: jessica stueber
// Class: Operating Systems
// Lab: Virtual Memory Lab


#include <stdio.h>
#include "oslabs.h"
#include <stdbool.h>


int process_page_access_fifo(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp);
int process_page_access_fifo(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){

    if(page_table[page_number].is_valid == true){
        page_table[page_number].reference_count += 1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        return page_table[page_number].frame_number;
    }

    else if((page_table[page_number].is_valid == false) && (*frame_cnt > 0)){

        page_table[page_number].frame_number = frame_pool[*frame_cnt-1];
        *frame_cnt -= 1;

        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        page_table[page_number].is_valid = true;

        return page_table[page_number].frame_number;
    }

    else{
        bool first_flag = false;
        int intial_frame = 0;
        int intial_index = 0;
        int arrival_stamp = 0;
        for(int p = 0; p < *table_cnt ; p++){
            if((page_table[p].is_valid == true) && (first_flag== false)){
                intial_index = p;
                intial_frame = page_table[p].frame_number;
                arrival_stamp = page_table[p].arrival_timestamp;
                first_flag = true;
            }
            else if((page_table[p].is_valid == true) && (arrival_stamp > page_table[p].arrival_timestamp) && (first_flag== true)){
                arrival_stamp = page_table[p].arrival_timestamp;
                intial_index = p;
                intial_frame = page_table[p].frame_number;
            }
        }
        if(first_flag == true){
            page_table[intial_index].arrival_timestamp = -1;
            page_table[intial_index].reference_count = -1;
            page_table[intial_index].last_access_timestamp = -1;
            page_table[intial_index].frame_number = -1;
            page_table[intial_index].is_valid = false;
            page_table[page_number].frame_number = intial_frame;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].is_valid = true;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
        }
        return intial_frame;
    }
};


int count_page_faults_fifo(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt);
int count_page_faults_fifo(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt){
    int timestamp = 1;
    int page_fault = 0;
    for(int i = 0; i < reference_cnt; i++){
        if(page_table[refrence_string[i]].is_valid == true){
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count += 1;
        }
        else if((page_table[refrence_string[i]].is_valid == false) && (frame_cnt > 0)){
            page_table[refrence_string[i]].frame_number = frame_pool[frame_cnt-1];
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count += 1;
            page_table[refrence_string[i]].is_valid = true;
            frame_cnt -= 1;
            page_fault += 1;
        }
        else if((page_table[refrence_string[i]].is_valid == false) && (frame_cnt == 0)){
            page_fault += 1;
            bool first_flag = false;
            int intial_index = 0;
            int intial_frame = 0;
            int arrival_stamp = 0;
            for(int x = 0; x < table_cnt ; x++){
                if((page_table[x].is_valid == true) && (first_flag==false)){
                    intial_index = x;
                    intial_frame = page_table[x].frame_number;
                    arrival_stamp = page_table[x].arrival_timestamp;
                    first_flag = true;
                }
                else if((page_table[x].is_valid == true) && (arrival_stamp > page_table[i].arrival_timestamp) && (first_flag == true)){
                    arrival_stamp = page_table[x].arrival_timestamp;
                    intial_index = x;
                    intial_frame = page_table[x].frame_number;
                }
            }
            if(first_flag == true){
                page_table[intial_index].arrival_timestamp = 0;
                page_table[intial_index].reference_count = 0;
                page_table[intial_index].last_access_timestamp = 0;
                page_table[intial_index].frame_number = 0;
                page_table[intial_index].is_valid = false;
                page_table[refrence_string[i]].frame_number = intial_frame;
                page_table[refrence_string[i]].arrival_timestamp = arrival_stamp;
                page_table[refrence_string[i]].is_valid = true;
                page_table[refrence_string[i]].last_access_timestamp = timestamp;
                page_table[refrence_string[i]].reference_count = 0;
            }
        }
        timestamp += 1;
    }
    return page_fault;
};

int process_page_access_lru(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp);
int process_page_access_lru(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){
    if(page_table[page_number].is_valid == true){
        page_table[page_number].reference_count += 1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        return page_table[page_number].frame_number;
    }
    else if((page_table[page_number].is_valid == false) && (*frame_cnt > 0)){
        page_table[page_number].frame_number = frame_pool[*frame_cnt-1];
        *frame_cnt -= 1;

        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        page_table[page_number].is_valid = true;
        return page_table[page_number].frame_number;
    }
    else{
        bool first_flag = false;
        int intial_index = 0;
        int intial_frame = 0;
        int last_timestamp = 0;
        for(int i = 0; i < *table_cnt ; i++){
            if((page_table[i].is_valid == true) && (first_flag== false)){
                intial_index = i;
                intial_frame = page_table[i].frame_number;
                last_timestamp = page_table[i].last_access_timestamp;
                first_flag = true;
            }
            else if((page_table[i].is_valid == true) && (last_timestamp > page_table[i].last_access_timestamp) && (first_flag== true)){
                last_timestamp = page_table[i].last_access_timestamp;
                intial_index = i;
                intial_frame = page_table[i].frame_number;
            }
        }
        if(first_flag == true){
            page_table[intial_index].arrival_timestamp = -1;
            page_table[intial_index].reference_count = -1;
            page_table[intial_index].last_access_timestamp = -1;
            page_table[intial_index].frame_number = -1;
            page_table[intial_index].is_valid = false;
            page_table[page_number].frame_number = intial_frame;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].is_valid = true;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
        }
    return intial_frame;
    }
};

int count_page_faults_lru(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt);
int count_page_faults_lru(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt){
    int timestamp = 1;
    int estimated_page_fault = 0;
    for(int i = 0; i < reference_cnt; i++){
        if(page_table[refrence_string[i]].is_valid == true){
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            //Remove a frame from the process pool and inserted into the page table entry.
            page_table[refrence_string[i]].reference_count += 1;
        }
        else if((page_table[refrence_string[i]].is_valid == false) && (frame_cnt > 0)){
            page_table[refrence_string[i]].frame_number = frame_pool[frame_cnt-1];
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count += 1;
            page_table[refrence_string[i]].is_valid = true;
            frame_cnt -= 1;
            estimated_page_fault += 1;
        }
        else if((page_table[refrence_string[i]].is_valid == false) && (frame_cnt == 0)){
            bool first_flag = false;
            int intial_index = 0;
            int initial_frame = 0;
            int last_arrival_timestamp = 0;
            estimated_page_fault += 1;
            for(int p = 0; p < table_cnt ; p++){
                if((page_table[p].is_valid == true) && (first_flag == false)){
                    intial_index = p;
                    initial_frame = page_table[p].frame_number;
                    last_arrival_timestamp = page_table[p].arrival_timestamp;
                    first_flag = true;
                }
                else if((page_table[p].is_valid == true) && (last_arrival_timestamp > page_table[p].last_access_timestamp) && (first_flag== true)){
                    last_arrival_timestamp = page_table[p].last_access_timestamp;
                    intial_index = p;
                    initial_frame = page_table[p].frame_number;
                }
            }
            if(first_flag == true){
                page_table[intial_index].arrival_timestamp = 0;
                page_table[intial_index].reference_count = 0;
                page_table[intial_index].last_access_timestamp = 0;
                page_table[intial_index].frame_number = 0;
                page_table[intial_index].is_valid = false;
                page_table[refrence_string[i]].frame_number = initial_frame;
                page_table[refrence_string[i]].arrival_timestamp = timestamp;
                page_table[refrence_string[i]].is_valid = true;
                page_table[refrence_string[i]].last_access_timestamp = timestamp;
                page_table[refrence_string[i]].reference_count = 0;
            }
        }
        timestamp += 1;
    }
    return estimated_page_fault;
};

int process_page_access_lfu(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp);
int process_page_access_lfu(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){

    if((page_table[page_number].is_valid == false) && (*frame_cnt > 0)){
        page_table[page_number].frame_number = frame_pool[*frame_cnt-1];
        *frame_cnt -= 1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        page_table[page_number].is_valid = true;
        return page_table[page_number].frame_number;
    }
    else if(page_table[page_number].is_valid == true){
        page_table[page_number].reference_count += 1;
        page_table[page_number].last_access_timestamp = current_timestamp;

        return page_table[page_number].frame_number;
    }
    else{
        int intial_index = 0;
        int intial_frame = 0;
        int arrival_time = 0;
        int frames_used = 0;
        bool first_flag = false;
        for(int i = 0; i < *table_cnt ; i++){
            if((page_table[i].is_valid == true) && (frames_used >= page_table[i].reference_count) && (first_flag == true)){
                if(((frames_used == page_table[i].reference_count) && (arrival_time > page_table[i].arrival_timestamp)) || (frames_used > page_table[i].reference_count)){
                    frames_used = page_table[i].reference_count;
                    intial_index = i;
                    intial_frame = page_table[i].frame_number;
                    arrival_time = page_table[i].arrival_timestamp;
                }
            }
            else if((page_table[i].is_valid == true) && (first_flag == false)){
                intial_index = i;
                intial_frame = page_table[i].frame_number;
                frames_used = page_table[i].reference_count;
                arrival_time = page_table[i].arrival_timestamp;
                first_flag = true;
            }
        }
        if(first_flag == true){
            page_table[intial_index].arrival_timestamp = -1;
            page_table[intial_index].reference_count = -1;
            page_table[intial_index].last_access_timestamp = -1;
            page_table[intial_index].frame_number = -1;
            page_table[intial_index].is_valid = false;
            page_table[page_number].frame_number = intial_frame;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].is_valid = true;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
        }
        return intial_frame;
    }
};
int count_page_faults_lfu(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt);
int count_page_faults_lfu(struct PTE page_table[TABLEMAX],int table_cnt, int refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int frame_cnt){
    int timestamp = 1;
    int page_fault = 0;
    for(int i = 0; i < reference_cnt; i++){
        if ((page_table[refrence_string[i]].is_valid == false) && (frame_cnt > 0)){
            page_table[refrence_string[i]].frame_number = frame_pool[frame_cnt-1];
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].arrival_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count += 1;
            page_table[refrence_string[i]].is_valid = true;
            frame_cnt -= 1;
            page_fault += 1;
        }
        else if (page_table[refrence_string[i]].is_valid == true){
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count += 1;
        }
        else if((page_table[refrence_string[i]].is_valid == false) && (frame_cnt == 0)){
            page_fault += 1;
            bool first_flag = false;
            bool flag = false;
            int intial_index = 0;
            int frame = 0;
            int used_frame = 0;
            int arrival_time = 0;
            for (int p = 0; p < table_cnt ; p++){
                if((page_table[p].is_valid == true) && (first_flag == false)){
                    intial_index = p;
                    frame = page_table[p].frame_number;
                    used_frame = page_table[p].reference_count;
                    arrival_time = page_table[p].arrival_timestamp;
                    first_flag = true;
                }
                else if ((page_table[p].is_valid == true) && (used_frame >= page_table[p].reference_count) && (first_flag== true)){
                    if(((used_frame == page_table[p].reference_count) && (arrival_time > page_table[p].arrival_timestamp))){
                        used_frame = page_table[p].reference_count;
                        intial_index = p;
                        frame = page_table[p].frame_number;
                        arrival_time = page_table[p].arrival_timestamp;
                        flag = true;
                    }
                    else if((used_frame > page_table[p].reference_count) && (flag == false)){
                        used_frame = page_table[p].reference_count;
                        intial_index = p;
                        frame = page_table[p].frame_number;
                        arrival_time =page_table[p].arrival_timestamp;
                    }
                }
            }
            if (first_flag == true){
                page_table[intial_index].arrival_timestamp = 0;
                page_table[intial_index].reference_count = 0;
                page_table[intial_index].last_access_timestamp = 0;
                page_table[intial_index].frame_number = 0;
                page_table[intial_index].is_valid = false;
                page_table[refrence_string[i]].frame_number = frame;
                page_table[refrence_string[i]].arrival_timestamp = timestamp;
                page_table[refrence_string[i]].is_valid = true;
                page_table[refrence_string[i]].last_access_timestamp = timestamp;
                page_table[refrence_string[i]].reference_count = 0;
            }
        }
        timestamp += 1;
    }
    return page_fault;
};
