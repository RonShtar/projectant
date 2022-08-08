#ifndef USER_H 
#define USER_H

struct user  //struct of user that contains user's name and taks list
{
    char *name;
    taskPtr list;
};
typedef struct user *userPtr;

userPtr createUserList(char *name,char *data); //create first user
userPtr addUser(userPtr users,char *name,char *data); //add user to dynamic array
userPtr removeUser(userPtr users,int num); //remove user from dynamic array
void printUser(userPtr users,int index); //print specific user
void printUserList(userPtr users); //print all users
userPtr findUser(userPtr users,int num); //get specific user

#endif