#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
const int MAX_LENGTH_COMMAND = 300;

const int MAX_NO_TASKS = 100;

const int MAX_LENGTH_TITLE = 100;
const int MAX_LENGTH_DESCRIPTION = 200;
const int MAX_LENGTH_TIME = 33;

const int WEEK_CELL_FIRST_COL_WIDTH = 10;
const int WEEK_CELL_OTHER_COL_WIDTH = 20;
*/
#define MAX_LENGTH_COMMAND 300
#define MAX_NO_TASKS 100
#define MAX_LENGTH_TITLE 100
#define MAX_LENGTH_DESCRIPTION 200
#define MAX_LENGTH_TIME 33
#define WEEK_CELL_FIRST_COL_WIDTH 10
#define WEEK_CELL_OTHER_COL_WIDTH 20

enum Status
{
    IN_PROGRESS,
    DONE,
    ARCHIVED
};
char *status_name[] = {"In Progress", "Done", "Archived"};
enum CommandType
{
    ADD,
    EDIT,
    SHOW,
    DELETE,
    QUIT,
    INVALID
};
char *command_name[] = {"ADD", "EDIT", "SHOW", "DELETE", "QUIT", "INVALID"};

struct Task
{
    int num;
    char title[MAX_LENGTH_TITLE + 1];
    char description[MAX_LENGTH_DESCRIPTION + 1];
    char time[MAX_LENGTH_TIME + 1];
    enum Status status;
};

void printTask(struct Task *task)
{
    printf("--------------------------------------------\n");
    printf("Num: #%d. Title: %s\n", task->num, task->title);
    printf("Description: %s\n", task->description);
    printf("Status: %s\n", status_name[task->status]);
    printf("--------------------------------------------\n");
}

void printUnsupportedTime(struct Task *task)
{
    printf("----- Show week view -----\n");
    printf("Error: Unsupported time with non-zero minutes: %s\n", task->time);
    printf("In Task:\n");
    printTask(task);
}

// ------ Begin: Student Answer ------
enum CommandType getCommandType(char *command)
{
    // Make backup of command since strtok modifies it
    char cmd[MAX_LENGTH_COMMAND + 1];
    strcpy(cmd, command);
    char *token = strtok(cmd, " ");
    if (token == NULL)
    {
        return INVALID;
    }
    if (stricmp(token, command_name[0]) == 0)
    {
        return ADD;
    }
    else if (stricmp(token, command_name[1]) == 0)
    {
        return EDIT;
    }
    else if (stricmp(token, command_name[2]) == 0)
    {
        return SHOW;
    }
    else if (stricmp(token, command_name[3]) == 0)
    {
        return DELETE;
    }
    else if (stricmp(token, command_name[4]) == 0)
    {
        return QUIT;
    }
    else
        return INVALID;
}

// Other functions
void getTitleFromAdd(char *command, char *out_title)
{
    // Make backup
    char cmd[MAX_LENGTH_COMMAND + 1];
    strcpy(cmd, command);
    // Parse the first square block
    char *token = strtok(cmd, "[");
    // Keep parsing until met a closing bracket
    token = strtok(NULL, "]");
    strcpy(out_title, token);
}

void getDescriptionFromAdd(char *command, char *out_description)
{
    // Make backup
    char cmd[MAX_LENGTH_COMMAND + 1];
    strcpy(cmd, command);
    // Parse the second square block
    char *token = strtok(cmd, "[");
    token = strtok(NULL, "[");
    token = strtok(NULL, "]");
    strcpy(out_description, token);
}

void getTimeFromAdd(char *command, char *out_time)
{
    // Make backup
    char cmd[MAX_LENGTH_COMMAND + 1];
    strcpy(cmd, command);
    // Parse third square block
    char *token = strtok(cmd, "[");
    token = strtok(NULL, "[");
    token = strtok(NULL, "[");
    token = strtok(NULL, "]");
    strcpy(out_time, token);
}

int checkTitle(char *raw_title)
{
    char title[200];
    strcpy(title, raw_title);
    // If exceed max title length
    if (strlen(title) > 100)
        return strlen(title);
    // First and last chars cannot be a space
    if (title[0] == ' ')
        return 0;
    if (title[strlen(title) - 1] == ' ')
        return strlen(title) - 1;
    char allowed[] = " ,.-:|/";
    int allowed_size = strlen(allowed) - 1;
    // Loop through title, if char is not lowercase, uppercase, digit or allowed, return current position
    for (int i = 0; i < strlen(title); i++)
    {
        if (title[i] >= 'A' && title[i] <= 'Z')
            continue;
        if (title[i] >= 'a' && title[i] <= 'z')
            continue;
        if (title[i] >= '0' && title[i] <= '9')
            continue;
        int miss = 0;
        for (int j = 0; j < allowed_size; j++)
            if (strchr(allowed, title[i]) == NULL)
                miss++;
        if (miss >= allowed_size)
            return i;
    }
    return -1;
}

int checkDescription(char *raw_description)
{
    char description[200];
    strcpy(description, raw_description);
    // If exceed max description length
    if (strlen(description) > 200)
        return strlen(description);
    if (description[0] == ' ')
        return 0;
    if (description[strlen(description) - 1] == ' ')
        return strlen(description) - 1;
    char allowed[] = " ,.-:|/";
    int allowed_size = strlen(allowed) - 1;
    for (int i = 0; i < strlen(description); i++)
    {
        if (description[i] >= 'A' && description[i] <= 'Z')
            continue;
        if (description[i] >= 'a' && description[i] <= 'z')
            continue;
        if (description[i] >= '0' && description[i] <= '9')
            continue;
        int miss = 0;
        for (int j = 0; j < allowed_size; j++)
            if (strchr(allowed, description[i]) == NULL)
                miss++;
        if (miss >= allowed_size)
            return i;
    }
    return -1;
}

int checkTime(char *raw_time)
{
    char time[MAX_LENGTH_TIME + 1];
    strcpy(time, raw_time);
    // Since date1 and date2 seperated by -
    char date1[MAX_LENGTH_TIME + 1];
    char date2[MAX_LENGTH_TIME + 1];
    char *token = strtok(time, "-");
    strcpy(date1, token);
    token = strtok(NULL, " ");
    strcpy(date2, token);
    // WIP
    return -1;
}

void getTitleFromEdit(char *command, char *out_title)
{
    // Make backup
    char cmd[MAX_LENGTH_COMMAND + 1];
    strcpy(cmd, command);
    // Parse the first square block
    char *token = strtok(cmd, "[");
    // Keep parsing until met a closing bracket
    token = strtok(NULL, "]");
    strcpy(out_title, token);
}

void getDescriptionFromEdit(char *command, char *out_description)
{
    // Make backup
    char cmd[MAX_LENGTH_COMMAND + 1];
    strcpy(cmd, command);
    // Parse the second square block
    char *token = strtok(cmd, "[");
    token = strtok(NULL, "]");
    strcpy(out_description, token);
}

void getTimeFromEdit(char *command, char *out_time)
{
    // Make backup
    char cmd[MAX_LENGTH_COMMAND + 1];
    strcpy(cmd, command);
    // Parse third square block
    char *token = strtok(cmd, "[");
    token = strtok(NULL, "]");
    strcpy(out_time, token);
}

int getNumFromCommand(char *command)
{
    // Make backup
    char cmd[MAX_LENGTH_COMMAND + 1];
    strcpy(cmd, command);
    // Check if there is a #
    if (strchr(cmd, '#') == NULL)
        return -1;
    // Get the character after # token
    char *token = strtok(cmd, "#");
    token = strtok(NULL, " ");
    int num = atoi(token);
    printf("Num converted: %d\n", num);
    // Workaround if there is no num after #
    // This method fails if num itself is 0
    // WIP
    if (num == 0)
        return -1;
    return num > 0 ? num : 0;
}

// ------ End: Student Answer ------

void runTodoApp()
{
    // Example of command Add
    char command[MAX_LENGTH_COMMAND + 1];

    while (true)
    {
        // Sample input:
        // Add [Course Intro to Programming] [Room 701-H6] [07:00|01/10/2023-12:00|01/10/2023]
        // Edit #1 title:[Course Intro to Developing]
        fgets(command, MAX_LENGTH_COMMAND + 1, stdin);
        command[strlen(command) - 1] = '\0';

        enum CommandType commandType = getCommandType(command);
        printf("Command     : %s\n", command);
        printf("Command type: %s\n", command_name[commandType]);

        if (commandType == ADD)
        {
            char raw_title[200];
            char raw_description[200];
            char raw_time[200];
            getTitleFromAdd(command, raw_title);
            getDescriptionFromAdd(command, raw_description);
            getTimeFromAdd(command, raw_time);
            printf("Raw title: %s\n", raw_title);
            printf("Raw description: %s\n", raw_description);
            printf("Raw time: %s\n", raw_time);
            printf("CheckTitle: %d\n", checkTitle(raw_title));
            printf("CheckDescription: %d\n", checkDescription(raw_description));
            printf("CheckTime: %d\n", checkTime(raw_time));
        }
        else if (commandType == EDIT)
        {
            char raw_title[200];
            char raw_description[200];
            char raw_time[200];
            int num;
            getTitleFromEdit(command, raw_title);
            getDescriptionFromEdit(command, raw_description);
            getTimeFromEdit(command, raw_time);
            num = getNumFromCommand(command);
            printf("Raw title: %s\n", raw_title);
            printf("Raw description: %s\n", raw_description);
            printf("Raw time: %s\n", raw_time);
            printf("Raw num: %d\n", num);
        }

        break; // only one loop for simple test
               // actual app will break when encounter QUIT command
    }
}

int main()
{
    runTodoApp();
    return 0;
}