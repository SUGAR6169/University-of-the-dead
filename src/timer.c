#include "timer.h"

void InitTimer(Timer *t, float startingTime) {
    t->timeLeft = startingTime;
    t->frozen = 0;
}

void UpdateTimer(Timer *t, float delta) {
    if (!t->frozen) t->timeLeft -= delta;
}