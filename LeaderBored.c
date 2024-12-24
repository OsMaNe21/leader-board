#include <stdio.h>
#include <string.h>

#define MAX_TEAMS 20


typedef struct {
    char name[50];
    int points;
    int goalsScored;
    int goalsConceded;
} Team;


void addTeam(Team teams[], int *teamCount);
void updateMatch(Team teams[], int teamCount);
void displayLeaderboard(Team teams[], int teamCount);
void saveToFile(Team teams[], int teamCount);
void loadFromFile(Team teams[], int *teamCount);
void sortLeaderboard(Team teams[], int teamCount);

int main() {
    Team teams[MAX_TEAMS];
    int teamCount = 0;
    int choice;

    loadFromFile(teams, &teamCount);

    do {
        printf("\nPremier League Leaderboard\n");
        printf("1. Add Team\n");
        printf("2. Update Match Result\n");
        printf("3. Display Leaderboard\n");
        printf("4. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addTeam(teams, &teamCount);
                break;
            case 2:
                updateMatch(teams, teamCount);
                break;
            case 3:
                displayLeaderboard(teams, teamCount);
                break;
            case 4:
                saveToFile(teams, teamCount);
                printf("Data saved. Exiting...\n");
                break;
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (choice != 4);

    return 0;
}

// Function to add a new team
void addTeam(Team teams[], int *teamCount) {
    if (*teamCount >= MAX_TEAMS) {
        printf("Maximum team limit reached!\n");
        return;
    }
    printf("Enter team name: ");
    scanf("%s", teams[*teamCount].name);
    teams[*teamCount].points = 0;
    teams[*teamCount].goalsScored = 0;
    teams[*teamCount].goalsConceded = 0;
    (*teamCount)++;
    printf("Team added successfully!\n");
}

// Function to update match results
void updateMatch(Team teams[], int teamCount) {
    char team1[50], team2[50];
    int goals1, goals2, i, j;
    printf("Enter Team 1 name: ");
    scanf("%s", team1);
    printf("Enter Team 2 name: ");
    scanf("%s", team2);
    printf("Enter goals scored by %s: ", team1);
    scanf("%d", &goals1);
    printf("Enter goals scored by %s: ", team2);
    scanf("%d", &goals2);

    // Find teams
    int index1 = -1, index2 = -1;
    for (i = 0; i < teamCount; i++) {
        if (strcmp(teams[i].name, team1) == 0) index1 = i;
        if (strcmp(teams[i].name, team2) == 0) index2 = i;
    }

    if (index1 == -1 || index2 == -1) {
        printf("One or both teams not found!\n");
        return;
    }

    // Update goals
    teams[index1].goalsScored += goals1;
    teams[index1].goalsConceded += goals2;
    teams[index2].goalsScored += goals2;
    teams[index2].goalsConceded += goals1;

    // Update points
    if (goals1 > goals2) {
        teams[index1].points += 3;
    } else if (goals1 < goals2) {
        teams[index2].points += 3;
    } else {
        teams[index1].points += 1;
        teams[index2].points += 1;
    }

    printf("Match result updated successfully!\n");
}

// Function to display the leaderboard
void displayLeaderboard(Team teams[], int teamCount) {
    sortLeaderboard(teams, teamCount);
    printf("\n%-20s %-10s %-10s %-10s\n", "Team", "Points", "Scored", "Conceded");
    for (int i = 0; i < teamCount; i++) {
        printf("%-20s %-10d %-10d %-10d\n",
               teams[i].name, teams[i].points,
               teams[i].goalsScored, teams[i].goalsConceded);
    }
}

// Function to save data to a file
void saveToFile(Team teams[], int teamCount) {
    FILE *file = fopen("leaderboard.dat", "wb");
    if (file == NULL) {
        printf("Error saving file!\n");
        return;
    }
    fwrite(&teamCount, sizeof(int), 1, file);
    fwrite(teams, sizeof(Team), teamCount, file);
    fclose(file);
}

// Function to load data from a file
void loadFromFile(Team teams[], int *teamCount) {
    FILE *file = fopen("leaderboard.dat", "rb");
    if (file == NULL) {
        printf("No previous data found. Starting fresh.\n");
        return;
    }
    fread(teamCount, sizeof(int), 1, file);
    fread(teams, sizeof(Team), *teamCount, file);
    fclose(file);
    printf("Data loaded successfully!\n");
}

// Function to sort teams by points (descending)
void sortLeaderboard(Team teams[], int teamCount) {
    for (int i = 0; i < teamCount - 1; i++) {
        for (int j = i + 1; j < teamCount; j++) {
            if (teams[j].points > teams[i].points) {
                Team temp = teams[i];
                teams[i] = teams[j];
                teams[j] = temp;
            }
        }
    }
}
