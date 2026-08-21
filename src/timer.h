#ifndef TIMER_H
#define TIMER_H
//works as header guard. It prevents the same definions from being repeated.
//if multiple .c files include timer.h file, then the compiler won't get confused.
//#indef_timer_H makes sure that the file is read only if the file hasn't been read before 

typedef struct {
    float timeLeft;
    int frozen;
} Timer;
//timeleft holds how many seconds remain
//frozen holds either 0 or 1
//0 means counting is ongoing
//1 means timer's paused inside the mosque
//float for timerleft cause it needs precision
// int for frozen cause its just 0 or 1 

void InitTimer(Timer *t, float startingTime);
void UpdateTimer(Timer *t, float delta);
//the actual codes for these functions are in timer.c
//this header file is just announcing their existence so other files can use them

#endif
//this closes the header guard that opened with #indef