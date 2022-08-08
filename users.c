#include "task.h" //tasks functions
#include "users.h" //users declareations and implement header
#include <stdlib.h> //malloc and realloc
#include <stdio.h> //print and input
#include <string.h> //for string functions

int usersLen = 0;  //length of users dynamic array
userPtr createUserList(char *name, char *data) //create first user 
{
    userPtr user = (userPtr)malloc(sizeof(struct user)); //pointer to user struct
    memset(user,0,sizeof(struct user));
    if (user == NULL)
    {
        printf("allocation error");
        return NULL;
    }
    else
    {
        user->name = (char *)malloc(strlen(name)); //pointer to name 
        memset(user->name,0,strlen(name));
        if(user->name == NULL)
        {
            printf("allocation error");
            return NULL;
        }
        strncpy(user->name, name, strlen(name));
        user->list = createTasksList(data); //create tasks list for user
        usersLen++; 
    }
    name=NULL;
    free(name);
    return user;
}
userPtr addUser(userPtr users, char *name, char *data) //add user to users dynamic array
{
    if (usersLen == 0) //first we need to call createUserList before adding user to null array
    {
        printf("you should create user list\n");
        return NULL;
    }
    else
    {
        users = (userPtr)realloc(users, (usersLen + 1) * sizeof(struct user)); //resize the users array with new cell
        if(users == NULL)
        {
            printf("allocation error");
            return NULL;
        }
        users[usersLen].name = (char *)malloc(strlen(name)); //create a pointer to name
        memset(users[usersLen].name,0,strlen(name));
        if(users[usersLen].name == NULL)
        {
            printf("allocation error");
            return NULL;
        }
        strncpy(users[usersLen].name, name, strlen(name));
        users[usersLen].list = createTasksList(data); //create tasks list to user
        usersLen++;
    }
    name=NULL;
    free(name);
    return users;
}
userPtr removeUser(userPtr users, int num) //remove user by index
{
    userPtr p;
    if (usersLen==1 && num==0) //there are no users
    {
        printf("all users are removed the system is closing now");
        free(users);
        return NULL;
    }
    if (num > usersLen - 1 || num < 0) //index does not exist
    {
        printf("number of user does not exist\n");
    }
    else
    {
        users[num].list = users[usersLen - 1].list; //put last user in the requested index
        users[num].name=NULL;
        users[num].name = (char *)malloc(strlen(users[usersLen - 1].name)); //copy fields
        memset(users[num].name,0,strlen(users[usersLen - 1].name));
        if(users[num].name == NULL)
        {
            printf("allocation error");
            return NULL;
        }
        strncpy(users[num].name, users[usersLen - 1].name, strlen(users[usersLen - 1].name));
        users = (userPtr)realloc(users, (usersLen - 1) * sizeof(struct user)); //remove last cell because we moved it to the requested cell that we should to remove
        if(users == NULL)
        {
            printf("allocation error");
            return NULL;
        }
        usersLen--; 
    }
    return users;
}
void printUser(userPtr users, int index) //print user by index
{
    printf("name: %*.*s\n",0,(int)strlen(users[index].name),users[index].name);
    showTasksList(users[index].list); //prints user's task list
}
void printUserList(userPtr users) //print users
{
    for (int i = 0; i < usersLen; i++) //go through user array
    {
        printf("name: %*.*s\n",0,(int)strlen(users[i].name),users[i].name);
        showTasksList(users[i].list); //print user task list
        printf("--------------------------\n");
    }
}
userPtr findUser(userPtr users, int num) //return user by index
{
    userPtr foundUser;
    if (num > usersLen - 1 || num < 0) //index does no exist 
    {
        printf("number of user does not exist\n");
    }
    else
    {
        return &users[num]; 
    }
    return NULL;
}