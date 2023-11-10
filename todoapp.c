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
    if (strcasecmp(token, command_name[0]) == 0)
    {
        return ADD;
    }
    else if (strcasecmp(token, command_name[1]) == 0)
    {
        return EDIT;
    }
    else if (strcasecmp(token, command_name[2]) == 0)
    {
        return SHOW;
    }
    else if (strcasecmp(token, command_name[3]) == 0)
    {
        return DELETE;
    }
    else if (strcasecmp(token, command_name[4]) == 0)
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

int getHourOfDate(char *date)
{
    // Make backup
    char cmd[MAX_LENGTH_COMMAND + 1];
    strcpy(cmd, date);
    // Get first 2 characters before ':'
    char *token = strtok(cmd, ":");
    int hour = atoi(token);
    // A valid hour must be between 0 and 23
    // if (hour < 0 || hour > 23)
    //    return -1;
    return hour;
}

int getMinuteOfDate(char *date)
{
    // Make backup
    char cmd[MAX_LENGTH_COMMAND + 1];
    strcpy(cmd, date);
    // Get 2 characters after ':' before '|'
    char *token = strtok(cmd, ":");
    token = strtok(NULL, "|");
    int minute = atoi(token);
    // A valid minute must be between 0 and 59
    // if (minute < 0 || minute > 59)
    //    return -1;
    return minute;
}

int getYearOfDate(char *date)
{
    // Make backup
    char cmd[MAX_LENGTH_COMMAND + 1];
    strcpy(cmd, date);
    // Get last 4 characters after the second '/'
    char *token = strtok(cmd, "/");
    token = strtok(NULL, "/");
    token = strtok(NULL, " ");
    int year = atoi(token);
    // A valid year must be positive integer
    // if (year < 0)
    //    return -1;
    return year;
}

// Returns additional number represents how many days in month
int getMonthOfDate(char *date, int *days)
{
    // Make backup
    char cmd[MAX_LENGTH_COMMAND + 1];
    strcpy(cmd, date);
    // Get 2 characters after first '/' before second '/'
    char *token = strtok(cmd, "/");
    token = strtok(NULL, "/");
    int month = atoi(token);
    // A valid month must be between 1 and 12
    if (month < 1 || month > 12)
    {
        *days = -1;
        // return -1;
    }
    else
    {
        *days = 31;
        if (month == 4 || month == 6 || month == 9 || month == 11)
            *days = 30;
        if (month == 2)
            // Check for leap year
            if (getYearOfDate(date) % 4 == 0)
                *days = 29;
            else
                *days = 28;
    }
    return month;
}

int getDayOfDate(char *date, int max_day)
{
    // Make backup
    char cmd[MAX_LENGTH_COMMAND + 1];
    strcpy(cmd, date);
    // Get 2 characters after '|' before '/'
    char *token = strtok(cmd, "|");
    token = strtok(NULL, "/");
    int day = atoi(token);
    // A valid day must be between 1 and max_day
    // if (day < 1 || day > max_day)
    //    return -1;
    return day;
}

// Check if date1 is sooner than date2
bool isSooner(char *date1, char *date2)
{
    // First check years
    int year1 = getYearOfDate(date1);
    int year2 = getYearOfDate(date2);
    if (year2 > year1)
        return true;
    if (year2 < year1)
        return false;
    // Then check months
    int max_days = 0; // Just to avoid passing NULL to function
    int month1 = getMonthOfDate(date1, &max_days);
    int month2 = getMonthOfDate(date2, &max_days);
    if (month2 > month1)
        return true;
    if (month2 < month1)
        return false;
    // Then check days
    int day1 = getDayOfDate(date1, 99);
    int day2 = getDayOfDate(date2, 99);
    if (day2 > day1)
        return true;
    if (day2 < day1)
        return false;
    // Check hours
    int hour1 = getHourOfDate(date1);
    int hour2 = getHourOfDate(date2);
    if (hour2 > hour1)
        return true;
    if (hour2 < hour1)
        return false;
    // Check minutes
    int minute1 = getMinuteOfDate(date1);
    int minute2 = getMinuteOfDate(date2);
    if (minute2 > minute1)
        return true;
    if (minute2 < minute1)
        return false;
    return false;
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
    // Date format: <hh>:<mm>|<dd>/<mo>/<yyyy>
    int max_day1, max_day2;
    // Get max_day for both dates
    getMonthOfDate(date1, &max_day1);
    getMonthOfDate(date2, &max_day2);
    // Check if both date are valid
    int h1 = getHourOfDate(date1);
    if (h1 < 0 || h1 > 23)
        return 1100 + h1;
    int h2 = getHourOfDate(date2);
    if (h2 < 0 || h2 > 23)
        return 1200 + h2;
    int m1 = getMinuteOfDate(date1);
    if (m1 < 0 || m1 > 59)
        return 2100 + m1;
    int m2 = getMinuteOfDate(date2);
    if (m2 < 0 || m2 > 59)
        return 2200 + m2;
    int d1 = getDayOfDate(date1, max_day1);
    if (d1 < 1 || d1 > max_day1)
        return 3100 + d1;
    int d2 = getDayOfDate(date2, max_day2);
    if (d2 < 1 || d2 > max_day2)
        return 3200 + d2;
    int mo1 = getMonthOfDate(date1, &max_day1);
    if (mo1 < 1 || mo1 > 12)
        return 4100 + mo1;
    int mo2 = getMonthOfDate(date2, &max_day2);
    if (mo2 < 1 || mo2 > 12)
        return 4200 + mo2;
    int y1 = getYearOfDate(date1);
    if (y1 < 0)
        return 510000 + y1;
    int y2 = getYearOfDate(date2);
    if (y2 < 0)
        return 520000 + y2;
    // date2 must not be sooner than date1
    if (!isSooner(date1, date2))
    {
        // printf("Error: Date2 must not be sooner than date1\n");
        return 0;
    }
    // Final case, if everything else passes
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

int getFieldFromEdit(char *edit_cmd)
{
    // Make backup
    char cmd[MAX_LENGTH_COMMAND + 1];
    strcpy(cmd, edit_cmd);
    // Token from second whitespace to before colon
    char *token = strtok(cmd, " ");
    token = strtok(NULL, " ");
    token = strtok(NULL, ":");
    // Compare token with title, description, time or status string
    if (strcmp(token, "title") == 0)
        return 1;
    if (strcmp(token, "description") == 0)
        return 2;
    if (strcmp(token, "time") == 0)
        return 3;
    if (strcmp(token, "status") == 0)
        return 4;
    return 0;
}

enum Status getStatusFromEdit(char *edit_cmd)
{
    // Make backup
    char cmd[MAX_LENGTH_COMMAND + 1];
    strcpy(cmd, edit_cmd);
    // Token from after "[" to the end of string
    char *token = strtok(cmd, "[");
    token = strtok(NULL, "]");
    // Compare token to 'I', 'D' or 'A' case-insensitive
    if (strcasecmp(token, "I") == 0)
        return IN_PROGRESS;
    if (strcasecmp(token, "D") == 0)
        return DONE;
    return ARCHIVED;
}

void printAllTasks(struct Task *array_tasks, int no_tasks)
{
    for (int i = 0; i < no_tasks; i++)
    {
        printTask(&array_tasks[i]);
    }
}

void printTaskByNum(struct Task *array_tasks, int no_tasks, int num)
{
    for (int i = 0; i < no_tasks; i++)
    {
        if (array_tasks[i].num == num)
        {
            printTask(&array_tasks[i]);
        }
    }
}

void printHeadTasks(struct Task *array_tasks, int no_tasks, int quan)
{
    // Print first <quan> tasks
    // First check if <quan> is smaller than no_tasks
    if (quan < no_tasks)
        for (int i = 0; i < quan; i++)
            printTask(&array_tasks[i]);
    // Else print all tasks
    else
        printAllTasks(array_tasks, no_tasks);
}

void printTailTasks(struct Task *array_tasks, int no_tasks, int quan)
{
    // Print last <quan> tasks but keeps the order
    if (quan < no_tasks)
        for (int i = no_tasks - quan; i < no_tasks; i++)
            printTask(&array_tasks[i]);
    else
        printAllTasks(array_tasks, no_tasks);
}

void printFilteredTasksByTitle(struct Task *array_tasks, int no_tasks, char *filter_title)
{
    // filter_title can be a substring of title
    for (int i = 0; i < no_tasks; i++)
    {
        if (strstr(array_tasks[i].title, filter_title) != NULL)
            printTask(&array_tasks[i]);
    }
}

void printFilteredTasksByDescription(struct Task *array_tasks, int no_tasks, char *filter_description)
{
    for (int i = 0; i < no_tasks; i++)
    {
        if (strstr(array_tasks[i].description, filter_description) != NULL)
            printTask(&array_tasks[i]);
    }
}

void printFilteredTasksByStatus(struct Task *array_tasks, int no_tasks, enum Status filter_status)
{
    for (int i = 0; i < no_tasks; i++)
    {
        if (array_tasks[i].status == filter_status)
            printTask(&array_tasks[i]);
    }
}

bool addTask(struct Task *array_tasks, int no_tasks, char *new_title, char *new_description, char *new_time)
{
    // Check if there is enough space
    if (no_tasks >= MAX_NO_TASKS)
        return false;
    // Append new task
    array_tasks[no_tasks].num = no_tasks + 1;
    // Must perform each check before copy string value
    if (checkTitle(new_title) == -1)
        strcpy(array_tasks[no_tasks].title, new_title);
    else
        return false;
    if (checkDescription(new_description) == -1)
        strcpy(array_tasks[no_tasks].description, new_description);
    else
        return false;
    if (checkTime(new_time) == -1)
        strcpy(array_tasks[no_tasks].time, new_time);
    else
        return false;
    array_tasks[no_tasks].status = IN_PROGRESS;
    // no_tasks is not passed by reference, no need to increase
    // no_tasks++;
    return true;
}

bool deleteTask(struct Task *array_tasks, int no_tasks, int num)
{
    // Check if there is enough space
    if (no_tasks <= 0)
        return false;
    // Search for num
    for (int i = 0; i < no_tasks; i++)
    {
        if (array_tasks[i].num == num)
        {
            // Shift all tasks back 1 unit
            for (int j = i; j < no_tasks - 1; j++)
            {
                array_tasks[j] = array_tasks[j + 1];
                // Also shift num
                array_tasks[j].num = j + 1;
            }
            return true;
        }
    }
    return false;
}

int flooor(float x)
{
    // If x is already an integer, return x
    return x == (int)x ? x :
                       // If x is positive, cast x to an int and return it
               x > 0 ? (int)x
                     :
                     // If x is negative, cast x to an int and subtract 1 from it
               (int)x - 1;
}

int p(int ws, int wc)
{
    return flooor((wc - ws) / 2);
}

// 0 = SUN, 1 = MON ... 6 = SAT
int getWeekday(int d, int m, int y)
{
    if (m < 3)
    {
        y = y - 1;
        m = m + 12;
    }
    return (d + m * 13 / 5 + y + y / 4 - y / 100 + y / 400 + 5) % 7;
}

char *weekdays[] = {"MON", "TUE", "WED", "THU", "FRI", "SAT", "SUN"};

int printWeekTime(struct Task *array_tasks, int no_tasks, char *date)
{
    // Make backup
    char cmd[MAX_LENGTH_COMMAND + 1];
    strcpy(cmd, date);
    int y = getYearOfDate(cmd);
    int max_day;
    int m = getMonthOfDate(cmd, &max_day);
    int d = getDayOfDate(cmd, max_day);
    int weekday = getWeekday(d, m, y);
    // Get monday's day
    int mon = d - weekday + 1;
    // Print the year
    for (int i = 0; i < p(4, WEEK_CELL_FIRST_COL_WIDTH); i++)
        printf(" ");
    printf("%d", y);
    for (int j = 0; j < 7; j++)
    {
        // Print the horizontal header contains MON to SUN
        for (int i = 0; i < p(3, WEEK_CELL_OTHER_COL_WIDTH); i++)
            printf(" ");
        printf("%s", weekdays[j]);
        // Print header subtitle day of each weekdays
        printf("\t%d/%d", mon + j + 1, m);
    }
    printf("\n");
    // Print time and task associated with each day
    printf("%d:00\t", 0);
    printf("#%d|%d:%d-%d:%d", 1, 7, 0, 12, 0);
}

// ------ End: Student Answer ------

void runTodoApp()
{
    struct Task array_tasks[MAX_NO_TASKS];
    // Append some simple cases for array tasks
    array_tasks[0].num = 1;
    strcpy(array_tasks[0].title, "Course Intro to Programming");
    strcpy(array_tasks[0].description, "Room 701-H6");
    strcpy(array_tasks[0].time, "07:00|01/10/2023-12:00|01/10/2023");
    array_tasks[0].status = IN_PROGRESS;
    array_tasks[1].num = 2;
    strcpy(array_tasks[1].title, "Data Structure and Algorithm");
    strcpy(array_tasks[1].description, "Room 702-H6");
    strcpy(array_tasks[1].time, "08:00|02/10/2023-12:00|02/10/2023");
    array_tasks[1].status = DONE;
    array_tasks[2].num = 3;
    strcpy(array_tasks[2].title, "Principle of Programming Language");
    strcpy(array_tasks[2].description, "Room 703-H6");
    strcpy(array_tasks[2].time, "09:00|03/10/2023-12:00|03/10/2023");
    array_tasks[2].status = ARCHIVED;
    array_tasks[3].num = 4;
    strcpy(array_tasks[3].title, "Fun Task :)");
    strcpy(array_tasks[3].description, "Fun Room :)");
    strcpy(array_tasks[3].time, "Fun time :)");
    array_tasks[3].status = IN_PROGRESS;
    array_tasks[4].num = 5;
    strcpy(array_tasks[4].title, "Course Intro to Developing");
    strcpy(array_tasks[4].description, "Room 704-H6");
    strcpy(array_tasks[4].time, "10:00|04/10/2023-12:00|04/10/2023");
    array_tasks[4].status = DONE;
    int no_tasks = 5;

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
            bool success = addTask(array_tasks, no_tasks, raw_title, raw_description, raw_time);
            if (success)
            {
                no_tasks++;
                printf("Task added!\n");
            }
            else
                printf("Task not added!\n");
        }
        else if (commandType == EDIT)
        {
            char raw_title[200];
            char raw_description[200];
            char raw_time[200];
            int num;
            int field;
            enum Status status;
            getTitleFromEdit(command, raw_title);
            getDescriptionFromEdit(command, raw_description);
            getTimeFromEdit(command, raw_time);
            num = getNumFromCommand(command);
            field = getFieldFromEdit(command);
            status = getStatusFromEdit(command);
            printf("Raw title: %s\n", raw_title);
            printf("Raw description: %s\n", raw_description);
            printf("Raw time: %s\n", raw_time);
            printf("Raw num: %d\n", num);
            printf("Raw field: %d\n", field);
            printf("Raw status: %d\n", status);
        }
        else if (commandType == SHOW)
        {
            /*
            printf("Printing all tasks:\n");
            printAllTasks(array_tasks, no_tasks);
            printf("Printing task by num:\n");
            int num = 2;
            printTaskByNum(array_tasks, no_tasks, num);
            int quan = 2;
            printf("Printing head of %d tasks:\n", quan);
            printHeadTasks(array_tasks, no_tasks, quan);
            printf("Printing tail of %d tasks:\n", quan);
            printTailTasks(array_tasks, no_tasks, quan);
            char filter_title[] = "Intro";
            printf("Printing filtered tasks by title of %s:\n", filter_title);
            printFilteredTasksByTitle(array_tasks, no_tasks, filter_title);
            char filter_description[] = "H6";
            printf("Printing filtered tasks by description of %s:\n", filter_description);
            printFilteredTasksByDescription(array_tasks, no_tasks, filter_description);
            enum Status stt = DONE;
            printf("Printing filtered tasks by status of %d:\n", stt);
            printFilteredTasksByStatus(array_tasks, no_tasks, stt);
            */
            printWeekTime(array_tasks, no_tasks, "07:00|10/10/2023");
        }
        else if (commandType == DELETE)
        {
            int num_to_delete = 3;
            bool success = deleteTask(array_tasks, no_tasks, num_to_delete);
            if (success)
            {
                no_tasks--;
                printf("Task deleted!\n");
            }
        }
        else if (commandType == QUIT)
            break;
    }
}

int main()
{
    runTodoApp();
    return 0;
}