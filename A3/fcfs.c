/*
 -------------------------------------
 Assignment 3
 File: fcfs.c
 -------------------------------------
 Author:  Jidaan Dahiya / Andrew Graco
 ID:      211738270 / 210422740
 Email:   dahi8270@mylaurier.ca / grec2740@mylaurier.ca
 Version  2023-06-25
 -------------------------------------
 */

// JD

#include <stdio.h>
#include <stdlib.h>

typedef struct threadInfo {
    int thread_id;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
    int completion_time;
} threadInfo;

void calculateTimes(threadInfo *threads, int n) {
    threads[0].waiting_time = 0;
    threads[0].turnaround_time = threads[0].burst_time;

    for (int i = 0; i < n; i++) {
        threads[i].waiting_time = threads[i - 1].completion_time - threads[i].arrival_time;
        if (threads[i].waiting_time < 0)
            threads[i].waiting_time = 0;
        threads[i].completion_time = threads[i].waiting_time + threads[i].burst_time + threads[i].arrival_time;
        threads[i].turnaround_time = threads[i].completion_time - threads[i].arrival_time;
    }
}

void printTable(threadInfo *threads, int n) {

    printf("ThreadID      Arrival Time      Burst Time      Completion Time Turnaround Time    Waiting Time\n");	
    for (int i = 0; i < n; i++) {
        printf("%2d %20d %17d %15d %13d %12d\n",
            threads[i].thread_id, threads[i].arrival_time, threads[i].burst_time, threads[i].completion_time,	
            threads[i].turnaround_time, threads[i].waiting_time);
    }
}

float calculateAverageWaitingTime(threadInfo *threads, int n) {
    float total_waiting_time = 0;

    for (int i = 0; i < n; i++) {
        total_waiting_time += threads[i].waiting_time;
    }

    return total_waiting_time / n;
}

float calculateAverageTurnaroundTime(threadInfo *threads, int n) {
    float total_turnaround_time = 0;

    for (int i = 0; i < n; i++) {
        total_turnaround_time += threads[i].turnaround_time;
    }

    return total_turnaround_time / n;
}

int main() {
    threadInfo threads[20];
    int count = 0;

    FILE *file = fopen("sample_in_schedule.txt", "r");
    if (file == NULL) {
        printf("Error opening the file.\n");
        return 1;
    }

    while (fscanf(file, "%d, %d, %d", &threads[count].thread_id, &threads[count].arrival_time, &threads[count].burst_time) != EOF) {
        count++;
    }
    fclose(file);

    calculateTimes(threads, count);
    printTable(threads, count);

    float avg_waiting_time = calculateAverageWaitingTime(threads, count);
    float avg_turnaround_time = calculateAverageTurnaroundTime(threads, count);

    printf("\nAverage Waiting Time: %.2f\n", avg_waiting_time);
    printf("Average Turnaround Time: %.2f\n", avg_turnaround_time);

    return 0;
}
