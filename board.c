#include "task.h" //for tasks functions 
#include "users.h" //for users functions
#include "board.h" //implement header
#include <stdlib.h>  //realloc and malloc
#include <stdio.h> //print and scanf
#include <string.h> //string functions
#include <ctype.h> //char functions
#include <unistd.h> //sleep

int moveStdin(char *data,int len) //move stdin pointer in file to eof to prevent overflow
{
  int extra=0;
  if (data[len - 1] == '\0' && data[len - 2] != '\n') {   //check if enter was clicked it means that there is no overflow
    int ch;
    while ((ch = fgetc(stdin)) != '\n' && ch != EOF) {   //move pointer to end of stdin file
      extra = 1;
    }
  }
  return extra;
}

taskPtr managetasks(taskPtr taskList,char* username) //task menu
{
    char data[dataSize];
    char name[nameSize];
    char indexC[indexChoiceSize];
    int choiceNum,userNum;
    int flag=1;
    while(flag){  
        printf("%*.*s task's list",0,(int)strlen(username),username);
        printf("\n1-To see your tasks list");
        printf("\n2-To add task");
        printf("\n3-To delete task");
        printf("\n4-To edit task");
        printf("\n5-To exit to switch user");
        printf("\n\nEnter your choice..");
        fgets(indexC,indexChoiceSize,stdin); //input one of the choices above
        moveStdin(indexC,indexChoiceSize);
        choiceNum=atoi(indexC);
        switch(choiceNum){
      case 1:
        system("clear");
        showTasksList(taskList); //prints all tasks list
        sleep(4);
        break;
      case 2:
         system("clear");
         printf("Enter your task\n"); 
         fgets(data,dataSize,stdin);
         moveStdin(data,dataSize);
         addTask(&taskList,data); //add task to task list
         memset(data,0,strlen(data)); 
        break;
      case 3:
         system("clear");
         printf("Enter nummber of task to delete\n");
         fgets(indexC,indexChoiceSize,stdin); //input index to remove
         moveStdin(indexC,indexChoiceSize);
         userNum=atoi(indexC);
         deleteTask(&taskList,userNum); //delete task from task list by giving index
         sleep(1);
        break;
      case 4:
         system("clear");
         printf("Enter nummber of task to edit\n");
         //scanf("%d",&index); //input index of task to edit
         fgets(indexC,indexChoiceSize,stdin);
         moveStdin(indexC,indexChoiceSize);
         userNum=atoi(indexC);
         printf("Enter your task\n");
         fgets(data,dataSize,stdin);
         moveStdin(data,dataSize);
         //scanf("%s",data); //input task
         editTask(taskList,userNum,data); //edit task from task list by giving index
         memset(data,0,strlen(data));
         break;
      case 5:
         flag=0;  //change flag to exit and go back to user menu
         system("clear");
         break;
      default:
          printf("unknown command");
          break;
        }
    }
    return taskList;
}

void manageuser(userPtr users) //user menu
{
  char data[dataSize];
  char name[nameSize];
  char indexC[indexChoiceSize];
  int choiceNum,userNum;
  int flag=1;
  while(flag && users!=NULL)  //users list exist and exit choice has no requeted
  {
        system("clear"); //clear output
        puts("\n1-To add user");
        puts("\n2-To remove user");
        puts("\n3-To choose user");
        puts("\n4-To show all users");
        puts("\n5-To exit");
        puts("\n\nEnter your choice..");
        fgets(indexC,indexChoiceSize,stdin); //input one of the choices above
        moveStdin(indexC,indexChoiceSize);
        choiceNum=atoi(indexC);
          switch(choiceNum){
      case 1:
        system("clear");
        printf("enter your name to create your user\n");
        fgets(name,nameSize,stdin); //input name for new user
        moveStdin(name,nameSize);
        printf("enter your first task\n");
        fgets(data,dataSize,stdin);//input first task
        moveStdin(data,dataSize);
        users=addUser(users,name,data); //adding user to users dynamic array
        memset(data,0,strlen(data));
        memset(name,0,strlen(name));
        break;
      case 2:
         system("clear");
         printf("Enter number of user to remove\n"); //input index to remove user from array
         fgets(indexC,indexChoiceSize,stdin);
         moveStdin(indexC,indexChoiceSize);
         userNum=atoi(indexC);
         users=removeUser(users,userNum-1); //remove user by giving index-1 (1=arr[0])
         sleep(1);
        break;
      case 3:
         system("clear");
         printf("Enter nummber of user to choose\n");
         fgets(indexC,indexChoiceSize,stdin);
         moveStdin(indexC,indexChoiceSize);
         userNum=atoi(indexC);
         userPtr user=findUser(users,userNum-1); //get user by giving index-1 (1=arr[0])
         if(user != NULL)
         {
           user->list=managetasks(user->list,user->name); //go to list menu for this user
         }
         sleep(1);
        break;
      case 4:
        system("clear");
        printUserList(users); //print all users 
        sleep(4);
        break;
      case 5:
         flag=0; //exit the system
         break;
      default:
        printf("unknown command");
        break;
      }
  }
}
void todolist()  //start system to work
{
  userPtr users;
  char data[dataSize];
  char name[nameSize];
  printf("Welcome to do list \n");
  printf("enter your name to create your first user\n"); //input name
  fgets(name,nameSize,stdin);
  moveStdin(name,nameSize);
  //read(0,name,nameSize);
  printf("enter your first task\n");//input task
  fgets(data,dataSize,stdin);
  moveStdin(data,dataSize);
  //read(0,data,dataSize);
  users=createUserList(name,data); //create the new user
  memset(data,0,strlen(data)); 
  memset(name,0,strlen(name));
  manageuser(users); //call function that takes care of user menu
}