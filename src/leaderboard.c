#include "leaderboard.h"
#include "raylib.h"

#include <stdio.h>
#include <string.h>

static LeaderboardEntry leaderboard[MAX_LEADERBOARD_ENTRIES];
static int leaderboardCount = 0;

static const char *LEADERBOARD_FILE = "leaderboard.dat";


void InitLeaderboard(void)
{
    leaderboardCount = 0;

    LoadLeaderboard();
}


void AddLeaderboardScore(const char *name, float timeRemaining)
{
    if (leaderboardCount < MAX_LEADERBOARD_ENTRIES)
    {
        strncpy(leaderboard[leaderboardCount].name,
                name,
                PLAYER_NAME_LENGTH - 1);

        leaderboard[leaderboardCount].name[PLAYER_NAME_LENGTH - 1] = '\0';

        leaderboard[leaderboardCount].timeRemaining = timeRemaining;

        leaderboardCount++;
    }
    else
    {
        /* If the leaderboard is full,
           check whether the new score is better
           than the lowest score. */

        if (timeRemaining > leaderboard[leaderboardCount - 1].timeRemaining)
        {
            strncpy(leaderboard[leaderboardCount - 1].name,
                    name,
                    PLAYER_NAME_LENGTH - 1);

            leaderboard[leaderboardCount - 1].name[PLAYER_NAME_LENGTH - 1] = '\0';

            leaderboard[leaderboardCount - 1].timeRemaining = timeRemaining;
        }
        else
        {
            return;
        }
    }

    /* Sort the leaderboard from highest time
       to lowest time. */

    for (int i = 0; i < leaderboardCount - 1; i++)
    {
        for (int j = 0; j < leaderboardCount - i - 1; j++)
        {
            if (leaderboard[j].timeRemaining <
                leaderboard[j + 1].timeRemaining)
            {
                LeaderboardEntry temp = leaderboard[j];

                leaderboard[j] = leaderboard[j + 1];

                leaderboard[j + 1] = temp;
            }
        }
    }

    SaveLeaderboard();
}


void SaveLeaderboard(void)
{
    FILE *file = fopen(LEADERBOARD_FILE, "wb");

    if (file == NULL)
    {
        return;
    }

    fwrite(&leaderboardCount,
           sizeof(int),
           1,
           file);

    fwrite(leaderboard,
           sizeof(LeaderboardEntry),
           leaderboardCount,
           file);

    fclose(file);
}


void LoadLeaderboard(void)
{
    FILE *file = fopen(LEADERBOARD_FILE, "rb");

    if (file == NULL)
    {
        return;
    }

    fread(&leaderboardCount,
          sizeof(int),
          1,
          file);

    if (leaderboardCount < 0 ||
        leaderboardCount > MAX_LEADERBOARD_ENTRIES)
    {
        leaderboardCount = 0;

        fclose(file);

        return;
    }

    fread(leaderboard,
          sizeof(LeaderboardEntry),
          leaderboardCount,
          file);

    fclose(file);
}


void DrawLeaderboard(void)
{
    DrawText("LEADERBOARD", 700, 100, 40, GOLD);

    if (leaderboardCount == 0)
    {
        DrawText("No scores yet.", 750, 180, 25, WHITE);
        return;
    }

    for (int i = 0; i < leaderboardCount; i++)
    {
        char scoreText[100];

        sprintf(scoreText,
                "%d. %s    %.2f seconds",
                i + 1,
                leaderboard[i].name,
                leaderboard[i].timeRemaining);

        DrawText(scoreText,
                 600,
                 180 + i * 45,
                 25,
                 WHITE);
    }
}