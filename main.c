#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#define MAX_CLASSROOMS 5
#define MAX_RESERVATIONS 100

struct Reservation
{
    char name[50];
    char date[20];
    char time[20];
    char classroom[50];
};

struct Classroom
{
    char name[50];
    int capacity;
    char schedule[5][20];
};

void reserveClassroom(FILE *file, struct Reservation reservations[], int *numReservations, const struct Classroom classrooms[], int numClassrooms);
void cancelReservationByName(struct Reservation reservations[], int *numReservations);
void displayAllReservations(const struct Reservation reservations[], int numReservations);
void displayReservationsForDate(const struct Reservation reservations[], int numReservations, const char *date);
void displayClassroomInfo(const struct Classroom classrooms[], int numClassrooms);

int main()
{
    printf("\n\n|^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^|                            |^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^|\n");
        printf("|________________________________________|                            |_______________________________________|\n");
        printf("|                                        |");
        system ("COLOR 0A");
        printf("Classroom Reservation System|                                       |\n");
        printf("|________________________________________|                            |_______________________________________|\n\n");
    FILE *file = fopen("reservations.txt", "a+"); // Open file for reading and appending

    if (file == NULL)
    {
        fprintf(stderr, "Error opening file.\n");
        return 1;
    }

    struct Reservation reservations[MAX_RESERVATIONS];
    int numReservations = 0;

    struct Classroom classrooms[MAX_CLASSROOMS] =
    {
        {"Room A", 30, {"08:00-10:00", "10:00-12:00", "13:00-15:00", "", ""}},
        {"Room B", 40, {"08:30-10:30", "10:30-12:30", "13:30-15:30", "", ""}},
        {"Room C", 25, {"09:00-11:00", "11:00-13:00", "14:00-16:00", "", ""}},
        {"Room D", 35, {"09:30-11:30", "11:30-13:30", "14:30-16:30", "", ""}},
        {"Room E", 50, {"10:00-12:00", "12:00-14:00", "15:00-17:00", "", ""}}
    };
    int numClassrooms = MAX_CLASSROOMS;

    int choice;

    do
    {

        printf("_______________________________________________________________________________________________________________\n\n");
        printf("1. Reserve a classroom\n");
        printf("2. Cancel a reservation\n");
        printf("3. Display all reservations\n");
        printf("4. Display reservations for a specific date\n");
        printf("5. Display classroom information\n");
        printf("6. Exit\n");
        printf("______________________\n\n");
        printf("Enter your choice: ");
        printf("\n______________________\n");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            reserveClassroom(file, reservations, &numReservations, classrooms, numClassrooms);
            break;
        case 2:
            cancelReservationByName(reservations, &numReservations);
            break;
        case 3:
            displayAllReservations(reservations, numReservations);
            break;
        case 4:
        {
            char date[20];
            printf("Enter date (MM/DD/YYYY): ");
            scanf("%s", date);
            displayReservationsForDate(reservations, numReservations, date);
            break;
        }
        case 5:
            displayClassroomInfo(classrooms, numClassrooms);
            break;
        case 6:
            printf("Exiting program.\n");
            break;
        default:
            printf("Invalid choice. Please enter a valid option.\n");
        }
    }
    while (choice != 6);

    fclose(file); // Close the file

    return 0;
}
void reserveClassroom(FILE *file, struct Reservation reservations[], int *numReservations, const struct Classroom classrooms[], int numClassrooms)
{
    struct Reservation reservation;

    if (*numReservations >= MAX_RESERVATIONS)
    {
        printf("Maximum number of reservations reached.\n");
        return;
    }

    printf("\nEnter your name: ");
    scanf("%s", reservation.name);

    printf("\nEnter date (MM/DD/YYYY): ");
    scanf("%s", reservation.date);

    printf("\nChoose a classroom:\n");
    for (int i = 0; i < numClassrooms; i++)
    {
        printf("%d. %s\n", i + 1, classrooms[i].name);
    }

    int choiceClassroom;
    printf("\nEnter the number of the classroom: ");
    scanf("%d", &choiceClassroom);

    if (choiceClassroom < 1 || choiceClassroom > numClassrooms)
    {
        printf("Invalid choice. Reservation failed.\n");
        return;
    }

    printf("Available times for %s: ", classrooms[choiceClassroom - 1].name);
    for (int j = 0; j < 5; j++)
    {
        if (strcmp(classrooms[choiceClassroom - 1].schedule[j], "") != 0)
        {
            printf("\n%d. %s", j + 1, classrooms[choiceClassroom - 1].schedule[j]);
        }
    }
    printf("\n");

    int choiceTime;
    printf("\nEnter the number of the time slot: ");
    scanf("%d", &choiceTime);

    if (choiceTime < 1 || choiceTime > 5 || strcmp(classrooms[choiceClassroom - 1].schedule[choiceTime - 1], "") == 0)
    {
        printf("Invalid time slot. Reservation failed.\n");
        return;
    }

    // Remove extra characters in the time slots for comparison
    char formattedTime[20];
    sscanf(classrooms[choiceClassroom - 1].schedule[choiceTime - 1], "%5s", formattedTime);

    strcpy(reservation.time, formattedTime);
    strcpy(reservation.classroom, classrooms[choiceClassroom - 1].name);

    fprintf(file, "%s %s %s %s\n", reservation.name, reservation.date, reservation.time, reservation.classroom);

    reservations[*numReservations] = reservation;
    (*numReservations)++;

    //printf("------------------------------------------------------------------\n");
    printf("\n----------------------*--------------------*----------------------\n");
    printf("~~~~~~~~~~~~~~~~~~~~  RESERVATION SUCCESSFUL  ~~~~~~~~~~~~~~~~~~~~\n");
    printf("----------------------*--------------------*----------------------\n");
    //printf("------------------------------------------------------------------");

    fclose(file); // Close the file after writing the reservation
}


void cancelReservationByName(struct Reservation reservations[], int *numReservations)
{
    if (*numReservations == 0)
    {
        printf("No reservations available to cancel.\n");
        return;
    }

    char cancelName[50];
    printf("Enter your name to cancel the reservation: ");
    scanf("%s", cancelName);

    int found = 0;
    for (int i = 0; i < *numReservations; i++)
    {
        if (strcmp(reservations[i].name, cancelName) == 0)
        {
            for (int j = i; j < *numReservations - 1; j++)
            {
                reservations[j] = reservations[j + 1];
            }
            (*numReservations)--;
            found = 1;
            break;
        }
    }

    if (found)
    {
        printf("\n------------------------------------------------------------\n");
        printf("Reservation for %s canceled successfully.", cancelName);
        printf("\n------------------------------------------------------------\n");
    }
    else
    {
        printf("\nReservation not found for %s.\n", cancelName);
    }
}

void displayAllReservations(const struct Reservation reservations[], int numReservations)
{
    if (numReservations == 0)
    {
        printf("\n--------------------------\n");
        printf(" No reservations available.\n");
        printf("--------------------------\n");
        return;
    }

    printf("\nAll Reservations:\n");
    for (int i = 0; i < numReservations; i++)
    {
        printf("\nName: %s,\nDate: %s,\nTime: %s,\nClassroom: %s\n",
               reservations[i].name, reservations[i].date,
               reservations[i].time, reservations[i].classroom);
    }
}


void displayReservationsForDate(const struct Reservation reservations[], int numReservations, const char *date)
{
    if (numReservations == 0)
    {
        printf("\n---------------------------\n");
        printf(" No reservations available\n");
        printf("---------------------------\n");
        return;
    }

    printf("\nReservations for Date %s:\n", date);
    for (int i = 0; i < numReservations; i++)
    {
        if (strcmp(reservations[i].date, date) == 0)
        {
            printf("\nName: %s,\nTime: %s,\nClassroom: %s\n",reservations[i].name, reservations[i].time, reservations[i].classroom);
        }
    }
}

void displayClassroomInfo(const struct Classroom classrooms[], int numClassrooms)
{
    printf("\n**-*-*-*-*-*-*-*-*-*-*-**");
    printf("\n  CLASSROOM INFORMATION\n");
    printf("**-*-*-*-*-*-*-*-*-*-*-**\n\n");
    for (int i = 0; i < numClassrooms; i++)
    {
        printf("\nClassroom: %s, Capacity: %d\n", classrooms[i].name, classrooms[i].capacity);
        printf("Schedule: ");
        for (int j = 0; j < 5; j++)
        {
            printf("%s%s", classrooms[i].schedule[j], (j == 4) ? "" : ", ");
        }
        printf("\n");
    }
}
