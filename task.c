#include "task.h" //implement header
#include <stdlib.h> //realloc and malloc
#include <stdio.h> //print and input 
#include <string.h> //string functions

taskPtr createTasksList(char *data) // create first task node which would be the list
{
    taskPtr taskList=(taskPtr)malloc(sizeof(struct taskNode)); //create task node
    memset(taskList,0,sizeof(struct taskNode));
    if(taskList==NULL)
    {
        printf("allocation error");
        return NULL;
    }
    else
    {
        taskList->data=(char*)malloc(strlen(data)); 
        memset(taskList->data,0,strlen(taskList->data));
        strncpy(taskList->data,data,strlen(data)); //put data in task node
        taskList->next=NULL; 
        taskList->task_num=1;
        return taskList;
    }
    free(data);    
}

void addTask(taskPtr* taskList,char *data) //add task to the end of the list
{
    taskPtr newTask=(taskPtr)malloc(sizeof(struct taskNode)); //create new task
    memset(newTask,0,sizeof(struct taskNode));
    if(newTask ==NULL)
    {
        printf("allocation error");
    }
    else
    {
        newTask->data=(char*)malloc(strlen(data));
        memset(newTask->data,0,strlen(newTask->data));
        strncpy(newTask->data,data,strlen(data)); //put data in the new task node
        newTask->next=NULL;
        if (*taskList !=NULL) //list exist
        {
            taskPtr p=*taskList; 
            while(p->next !=NULL) //get the last task node in list
            {
                p=p->next;
            }
            newTask->task_num=p->task_num+1; //number of task
            p->next=newTask;
        }
        else
            *taskList=createTasksList(data); //create new tasks list
    }
    data=NULL;
    free(data);
}
void numberTasks(taskPtr taskList,int num) //order numbers of tasks by getting the first task to fix and the number to change
{
    while (taskList != NULL) //go thourgh the list and number the tasks again
    {
        taskList->task_num=num;
        num++;
        taskList=taskList->next;
    }
    
}
void deleteTask(taskPtr *taskList,int taskNum) // delete task by getting index
{
    taskPtr p=*taskList;
    if (*taskList==NULL) //list does not exist
    {
        printf("list is empty\n");
    }
    else if(taskNum < 0) //illegal index
    {
        printf("index does not exist\n");
    }
    else if (taskNum==1) //first index
    {
       *taskList=p->next; 
       numberTasks(p->next,1); //order numbers of tasks 
       p->data=NULL;
       free(p);
    }
    else
    {
        for (int i = 2; i < taskNum && p->next != NULL; i++) //go through the list to get the requested task
        {
            p = p->next;
        }
        if (p->next != NULL) //get the requested task
        {
            taskPtr temp = p->next;
            p->next = temp->next;
            numberTasks(p->next,temp->task_num); //order numbers of tasks
            temp->data=NULL;
            free(temp);
        }
        else //index does not exist
        {
            printf("index is more than tasks\n");
        }
    }
}
void editTask(taskPtr taskList,int taskNum,char *data) //edit data of task by getting index
{
    if (taskList==NULL) //there is no list
    {
        printf("list is empty\n");
    }
    else if(taskNum < 0) //illegal index
    {
        printf("index does not exist\n");
    }
    else if (taskNum==1) //first index
    {
        taskList->data=NULL;
        taskList->data=(char*)malloc(strlen(data));
        memset(taskList->data,0,strlen(data));
        strncpy(taskList->data,data,strlen(data));
    }
    else
    {
        for (int i = 2; i <= taskNum && taskList != NULL; i++) //go through the list to get the requested task
        {
            taskList= taskList->next;
        }
        if (taskList != NULL) //get the requested task
        {
            taskList->data=NULL;
            taskList->data=(char*)malloc(strlen(data));
            memset(taskList->data,0,strlen(data)); 
            strncpy(taskList->data,data,strlen(data)); //copy the new data
        }
        else //index does not exist
        {
            printf("index is more than tasks\n");
        }
    }
    data=NULL;
    free(data);
}
void showTasksList(taskPtr taskList) //print all tasks for one user
{
    if(taskList == NULL)
    {
        printf("empty list\n");
    }
    while (taskList) //go through the list to print details
    {
        printf("%d-%*.*s \n",taskList->task_num,0,(int)strlen(taskList->data),taskList->data);
        taskList=taskList->next;
    }
}