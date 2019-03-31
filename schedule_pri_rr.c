/**
 * Implementation of various scheduling algorithms.
 *
 * Priority round-robin scheduling
 */

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "task.h"
#include "list.h"
#include "cpu.h"

struct node *head = NULL;

// pointer to the struct containing the next task
struct node *tmp;

Task *pickNextTask();

// add a new task to the list of tasks
void add(char *name, int priority, int burst) {

    // the linked list is kept in descending priority order

    // first create the new task
    Task *newTask = (Task *) malloc(sizeof(Task));

    newTask->name = name;
    newTask->priority = priority;
    newTask->burst = burst;
    
    // if head is empty
    if (head == NULL) {
        insert(&head, newTask);
        return;
    }

    // if new task is highest priority
    if (head->task->priority <= newTask->priority) {
        insert(&head, newTask);
        return;
    }

    // else iterate the list to find the insert position
    else {
        struct node *temp;
        temp = head;

        // using temp->next is required for keeping pointers straight
        while (temp->next != NULL) {
            if (temp->next->task->priority <= newTask->priority) {
                insert(&temp->next, newTask);
                return;
            }
            temp = temp->next;
        }
        // new task is lowest priority in list
        insert(&temp->next, newTask);
    }
}

/**
 * Run the priority scheduler
 */
void schedule() 
{
    traverse(head);

    Task *current;
    tmp = head;

    while (head != NULL) {
        current = pickNextTask();
        if (current->burst > QUANTUM) {
            run(current, QUANTUM);

            current->burst -= QUANTUM;
        }
        else {
            run(current, current->burst);
            
            current->burst = 0;

            printf("Task %s finished.\n",current->name);        

            delete(&head, current);
        }
    }
}

/**
 * Returns the next task selected to run
 */
Task *pickNextTask()
{
    Task *nextTask = tmp->task;
    // find next task to be called
    if (tmp->next == NULL) {
        // end of the list, start over
        tmp = head;
        return nextTask;
    }

    tmp = tmp->next;
    
    // if the next task is a lower priority, and    (a)
    // it is not the head (it isn't the only        (b)
    // ... task of that priority remianing) unless
    // the head will not be finished running        (c)
    // (a && (b || c))
    if (tmp->task->priority < nextTask->priority &&
        (strcmp(head->task->name, nextTask->name) ||
        head->task->burst > QUANTUM)) 
    {
        tmp = head;
    }
    return nextTask;
}

