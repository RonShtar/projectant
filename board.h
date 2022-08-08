#ifndef BOARD_H
#define BOARD_H
#define dataSize 100  //define size to input task
#define nameSize 20 //define size to input name
#define indexChoiceSize 10 
void todolist();  //manage system function
void manageuser(userPtr users); //manage user menu function
taskPtr managetasks(taskPtr list,char *username); //manage task menu function
int moveStdin(char*data,int len); //move stdin pointer to eof
#endif
