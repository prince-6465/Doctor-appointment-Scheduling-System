#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_PATIENTS 100
#define FILE_NAME "appointments.txt"

struct Appointment {
    int id;
    char patientName[50];
    char doctorName[50];
    char date[20];
    char time[20];
};

struct Appointment appointments[MAX_PATIENTS];
int count = 0;

// Function declarations
void loadAppointments();
void saveAppointments();
void addAppointment();
void viewAppointments();
void cancelAppointment();

int main() {
    int choice;

    loadAppointments(); // Load existing data from file

    while (1) {
        printf("\n===== DOCTOR APPOINTMENT SCHEDULING SYSTEM =====\n");
        printf("1. Schedule an Appointment\n");
        printf("2. View All Appointments\n");
        printf("3. Cancel an Appointment\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                addAppointment();
                break;
            case 2:
                viewAppointments();
                break;
            case 3:
                cancelAppointment();
                break;
            case 4:
                saveAppointments(); // Save before exiting
                printf("All data saved successfully. Exiting system.\n");
                exit(0);
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}

// Load appointments from file
void loadAppointments() {
    FILE *fp = fopen(FILE_NAME, "r");
    if (fp == NULL) {
        // No file yet, start fresh
        return;
    }

    while (fscanf(fp, "%d,%49[^,],%49[^,],%19[^,],%19[^\n]\n",
                  &appointments[count].id,
                  appointments[count].patientName,
                  appointments[count].doctorName,
                  appointments[count].date,
                  appointments[count].time) == 5) {
        count++;
    }

    fclose(fp);
}

// Save appointments to file
void saveAppointments() {
    FILE *fp = fopen(FILE_NAME, "w");
    if (fp == NULL) {
        printf("Error saving data!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d,%s,%s,%s,%s\n",
                appointments[i].id,
                appointments[i].patientName,
                appointments[i].doctorName,
                appointments[i].date,
                appointments[i].time);
    }

    fclose(fp);
}

// Add new appointment
void addAppointment() {
    if (count >= MAX_PATIENTS) {
        printf("Appointment limit reached!\n");
        return;
    }

    struct Appointment newApp;
    newApp.id = (count == 0) ? 1 : appointments[count - 1].id + 1;

    printf("Enter patient name: ");
    fgets(newApp.patientName, sizeof(newApp.patientName), stdin);
    newApp.patientName[strcspn(newApp.patientName, "\n")] = 0;

    printf("Enter doctor name: ");
    fgets(newApp.doctorName, sizeof(newApp.doctorName), stdin);
    newApp.doctorName[strcspn(newApp.doctorName, "\n")] = 0;

    printf("Enter appointment date (DD/MM/YYYY): ");
    fgets(newApp.date, sizeof(newApp.date), stdin);
    newApp.date[strcspn(newApp.date, "\n")] = 0;

    printf("Enter appointment time (HH:MM): ");
    fgets(newApp.time, sizeof(newApp.time), stdin);
    newApp.time[strcspn(newApp.time, "\n")] = 0;

    appointments[count++] = newApp;
    saveAppointments();

    printf("✅ Appointment scheduled successfully! ID: %d\n", newApp.id);
}

// View all appointments
void viewAppointments() {
    if (count == 0) {
        printf("No appointments found.\n");
        return;
    }

    printf("\n----- All Appointments -----\n");
    for (int i = 0; i < count; i++) {
        printf("ID: %d\n", appointments[i].id);
        printf("Patient: %s\n", appointments[i].patientName);
        printf("Doctor: %s\n", appointments[i].doctorName);
        printf("Date: %s\n", appointments[i].date);
        printf("Time: %s\n", appointments[i].time);
        printf("-----------------------------\n");
    }
}

// Cancel an appointment
void cancelAppointment() {
    if (count == 0) {
        printf("No appointments to cancel.\n");
        return;
    }

    int id, found = 0;
    printf("Enter Appointment ID to cancel: ");
    scanf("%d", &id);
    getchar();

    for (int i = 0; i < count; i++) {
        if (appointments[i].id == id) {
            found = 1;
            for (int j = i; j < count - 1; j++) {
                appointments[j] = appointments[j + 1];
            }
            count--;
            saveAppointments();
            printf("❌ Appointment ID %d cancelled successfully.\n", id);
            break;
        }
    }

    if (!found)
        printf("Appointment with ID %d not found.\n", id);
}
