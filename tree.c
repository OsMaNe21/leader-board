#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Team {
    char name[50];
    int points;
    int goalsScored;
    int goalsConceded;
    struct Team* left;
    struct Team* right;
} Team;

Team* createTeam(char* name);
Team* insertTeam(Team* root, char* name);
Team* findTeam(Team* root, char* name);
void updateMatch(Team* root);
void displayLeaderboard(Team* root);
void displayInOrder(Team* root);
void saveToFile(Team* root, FILE* file);
Team* loadFromFile(Team* root, FILE* file, int teamCount);
void freeTree(Team* root);

int main() {
    Team* root = NULL; 
    int choice;

    
    FILE* file = fopen("leaderboard.dat", "rb");
    if (file != NULL) {
        int teamCount;
        fread(&teamCount, sizeof(int), 1, file);
        root = loadFromFile(root, file, teamCount);
        fclose(file);
        printf("Data loaded successfully!\n");
    } else {
        printf("No previous data found. Starting fresh.\n");
    }

    do {
        printf("\nPremier League Leaderboard\n");
        printf("1. Add Team\n");
        printf("2. Update Match Result\n");
        printf("3. Display Leaderboard\n");
        printf("4. Save and Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1: {
                char name[50];
                printf("Enter team name: ");
                scanf("%s", name);
                root = insertTeam(root, name);
                break;
            }
            case 2:
                updateMatch(root);
                break;
            case 3:
                printf("\nLeaderboard:\n");
                displayLeaderboard(root);
                break;
            case 4: {
                FILE* file = fopen("leaderboard.dat", "wb");
                if (file == NULL) {
                    printf("Error saving file!\n");
                } else {
                    int teamCount = 0;
                    saveToFile(root, file);
                    fclose(file);
                    printf("Data saved. Exiting...\n");
                }
                freeTree(root);
                break;
            }
            default:
                printf("Invalid choice! Try again.\n");
        }
    } while (choice != 4);

    return 0;
}


Team* createTeam(char* name) {
    Team* newTeam = (Team*)malloc(sizeof(Team));
    strcpy(newTeam->name, name);
    newTeam->points = 0;
    newTeam->goalsScored = 0;
    newTeam->goalsConceded = 0;
    newTeam->left = NULL;
    newTeam->right = NULL;
    return newTeam;
}


Team* insertTeam(Team* root, char* name) {
    if (root == NULL) {
        return createTeam(name);
    }
    if (strcmp(name, root->name) < 0) {
        root->left = insertTeam(root->left, name);
    } else if (strcmp(name, root->name) > 0) {
        root->right = insertTeam(root->right, name);
    } else {
        printf("Team already exists!\n");
    }
    return root;
}


Team* findTeam(Team* root, char* name) {
    if (root == NULL || strcmp(root->name, name) == 0) {
        return root;
    }
    if (strcmp(name, root->name) < 0) {
        return findTeam(root->left, name);
    } else {
        return findTeam(root->right, name);
    }
}


void updateMatch(Team* root) {
    char team1[50], team2[50];
    int goals1, goals2;

    printf("Enter Team 1 name: ");
    scanf("%s", team1);
    printf("Enter Team 2 name: ");
    scanf("%s", team2);
    printf("Enter goals scored by %s: ", team1);
    scanf("%d", &goals1);
    printf("Enter goals scored by %s: ", team2);
    scanf("%d", &goals2);

    Team* t1 = findTeam(root, team1);
    Team* t2 = findTeam(root, team2);

    if (t1 == NULL || t2 == NULL) {
        printf("One or both teams not found!\n");
        return;
    }

    t1->goalsScored += goals1;
    t1->goalsConceded += goals2;
    t2->goalsScored += goals2;
    t2->goalsConceded += goals1;

    if (goals1 > goals2) {
        t1->points += 3;
    } else if (goals1 < goals2) {
        t2->points += 3;
    } else {
        t1->points += 1;
        t2->points += 1;
    }

    printf("Match result updated successfully!\n");
}


void displayLeaderboard(Team* root) {
    printf("\n%-20s %-10s %-10s %-10s\n", "Team", "Points", "Scored", "Conceded");
    displayInOrder(root);
}


void displayInOrder(Team* root) {
    if (root == NULL) {
        return;
    }
    displayInOrder(root->left);
    printf("%-20s %-10d %-10d %-10d\n",
           root->name, root->points, root->goalsScored, root->goalsConceded);
    displayInOrder(root->right);
}


void saveToFile(Team* root, FILE* file) {
    if (root == NULL) {
        return;
    }
    fwrite(root, sizeof(Team), 1, file);
    saveToFile(root->left, file);
    saveToFile(root->right, file);
}


Team* loadFromFile(Team* root, FILE* file, int teamCount) {
    for (int i = 0; i < teamCount; i++) {
        Team temp;
        fread(&temp, sizeof(Team), 1, file);
        root = insertTeam(root, temp.name);
        Team* team = findTeam(root, temp.name);
        team->points = temp.points;
        team->goalsScored = temp.goalsScored;
        team->goalsConceded = temp.goalsConceded;
    }
    return root;
}


void freeTree(Team* root) {
    if (root == NULL) {
        return;
    }
    freeTree(root->left);
    freeTree(root->right);
    free(root);
}
