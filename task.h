#ifndef TASK_H_ 
#define TASK_H

struct taskNode //struct that contains task number, task data and next tass(pointer to next struct) in list
{
    int task_num;
    char *data;
    struct taskNode *next;
};
typedef struct taskNode *taskPtr;

taskPtr createTasksList(char *data); //create first task and make it list
void addTask(taskPtr* taskList, char *data); //add task to list
void numberTasks(taskPtr taskList, int taskNum); //order task by giving them correct number
void deleteTask(taskPtr *taskList, int task_num); //delete task from list
void editTask(taskPtr taskList, int task_num, char *data); //edit specific task from list
void showTasksList(taskPtr taskList);
#endif