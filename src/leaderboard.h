#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#define MAX_LEADERBOARD_ENTRIES 10
#define PLAYER_NAME_LENGTH 32

typedef struct {
    char name[PLAYER_NAME_LENGTH];
    float timeRemaining;
} LeaderboardEntry;

/* Initialize the leaderboard */
void InitLeaderboard(void);

/* Add a new score to the leaderboard */
void AddLeaderboardScore(const char *name, float timeRemaining);

/* Save leaderboard to file */
void SaveLeaderboard(void);

/* Load leaderboard from file */
void LoadLeaderboard(void);

/* Draw the leaderboard on screen */
void DrawLeaderboard(void);

#endif