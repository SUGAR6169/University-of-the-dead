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
    if (leaderboardCount < MAX_LEADERBOARD_ENTRIES) //handles max entries
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
    int screenWidth = GetScreenWidth();

    // Leaderboard title
    const char *title = "LEADERBOARD";
    int titleSize = 40;
    int titleWidth = MeasureText(title, titleSize);

    DrawText(
        title,
        (screenWidth - titleWidth) / 2,
        100,
        titleSize,
        GOLD
    );

    // No scores
    if (leaderboardCount == 0)
    {
        const char *message = "No scores yet.";
        int messageSize = 25;
        int messageWidth = MeasureText(message, messageSize);

        DrawText(
            message,
            (screenWidth - messageWidth) / 2,
            180,
            messageSize,
            WHITE
        );

        return;
    }

    // Column centers
   int rankCenter = screenWidth / 2 - 200;
    int nameCenter = screenWidth / 2;
    int timeCenter = screenWidth / 2 + 200;

    int fontSize = 25;

    // Column headings
    const char *rankHeading = "RANK";
    const char *nameHeading = "NAME";
    const char *timeHeading = "TIME";

    DrawText(
        rankHeading,
        rankCenter - MeasureText(rankHeading, fontSize) / 2,
        150,
        fontSize,
        GOLD
    );

    DrawText(
        nameHeading,
        nameCenter - MeasureText(nameHeading, fontSize) / 2,
        150,
        fontSize,
        GOLD
    );

    DrawText(
        timeHeading,
        timeCenter - MeasureText(timeHeading, fontSize) / 2,
        150,
        fontSize,
        GOLD
    );

    // Leaderboard entries
    for (int i = 0; i < leaderboardCount; i++)
    {
        char rankText[20];
        char timeText[50];

        sprintf(rankText, "%d.", i + 1);

        sprintf(
            timeText,
            "%.2f seconds",
            leaderboard[i].timeRemaining
        );

        int y = 200 + i * 45; //shift ranking downwards

        // Rank
        DrawText(
            rankText,
            rankCenter - MeasureText(rankText, fontSize) / 2,
            y,
            fontSize,
            WHITE
        );

        // Name
        DrawText(
            leaderboard[i].name,
            nameCenter - MeasureText(leaderboard[i].name, fontSize) / 2,
            y,
            fontSize,
            WHITE
        );

        // Time
        DrawText(
            timeText,
            timeCenter - MeasureText(timeText, fontSize) / 2,
            y,
            fontSize,
            WHITE
        );
    }
}