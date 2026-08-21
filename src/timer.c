#include "timer.h"
//quation marks instead of <> mean look into our own project folder
void InitTimer(Timer *t, float startingTime) {
    t->timeLeft = startingTime;
    t->frozen = 0;
}
//startingtime is the arguement passed to the function
//whatever value is passed become the time to complete the game

void UpdateTimer(Timer *t, float delta) {
    if (!t->frozen) t->timeLeft -= delta;
}
//delta= time passed since last frame
//delta makes time accurate regardless of the timeframe
//subtraction only happens when timer is not frozen 