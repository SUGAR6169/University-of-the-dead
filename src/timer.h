#ifndef TIMER_H
#define TIMER_H

typedef struct {
    float timeLeft;
    int frozen;
} Timer;

void InitTimer(Timer *t, float startingTime);
void UpdateTimer(Timer *t, float delta);

#endif