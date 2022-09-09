#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROSTERSIZE 50

struct student {
    char *first, *last, *major, *degree, *ta, *advisor;
    double gpa;
    int hours;
};
struct gpa_student {
    double gpa;
    char *name;
};
struct dept_adv {
    char major[100], names[500];
    int amt;
};

struct student createStudent(char *line) {
    struct student newstd;
    newstd.first = strdup(strtok(line, ","));
    newstd.last = strdup(strtok(NULL, ","));
    newstd.major = strdup(strtok(NULL, ","));
    newstd.degree = strdup(strtok(NULL, ","));
    newstd.gpa = atof(strtok(NULL, ","));
    newstd.hours = atoi(strtok(NULL, ","));
    newstd.ta = strdup(strtok(NULL, ","));
    newstd.advisor = strdup(strtok(NULL, "\r"));
    return newstd;
}

int main(void) {
    // Creates array of all students
    struct student roster[ROSTERSIZE];
    char *out = malloc(sizeof(char) * 100);
    int line = 0;
    FILE *csv = fopen("students_dataset.csv", "r");
    if (csv == NULL) {
        printf("No csv file!");
        exit(-1);
    }
    fgets(out, 100, csv);
    while (fgets(out, 100, csv) != NULL) {
        roster[line++] = createStudent(out);
    }
    fclose(csv);

    char *degrees = malloc(sizeof(char));
    int i, total_degrees = 0;
    int cs_majors = 0;
    int total_hours = 0;
    double highest_gpa, second_highest, total_cs_gpa = 0;
    struct gpa_student first, second, third;
    // Set up advisor array
    struct dept_adv advisors[20];
    for (i = 0; i < 20; i++) {
        strcpy(advisors[i].major, "N/A");
        strcpy(advisors[i].names, "N/A");
        advisors[i].amt = 0;
    }
    for (i = 0; i < ROSTERSIZE; i++) {
        // Find different degrees
        char *cdeg = strdup(roster[i].degree);
        if (strstr(degrees, cdeg) == NULL) {
            strcat(degrees, strcat(cdeg, ", "));
            total_degrees++;
        }
        //free(cdeg);
        // Find top three students
        double cgpa = roster[i].gpa;
        char *new_name = strdup(roster[i].first);
        if (cgpa > second_highest) {
            cgpa < highest_gpa ? (second_highest = cgpa) : (highest_gpa = cgpa);
            if (first.gpa < 1) {
                first.gpa = cgpa;
                first.name = strdup(strcat(new_name, roster[i].last));
                second.gpa = cgpa;
                second.name = strdup(strcat(new_name, roster[i].last));
                third.gpa = cgpa;
                third.name = strdup(strcat(new_name, roster[i].last));
            } else if (cgpa > first.gpa) {
                third.gpa = second.gpa;
                third.name = strdup(second.name);
                second.gpa = first.gpa;
                second.name = strdup(first.name);
                first.gpa = cgpa;
                first.name = strdup(strcat(new_name, roster[i].last));
            } else if (cgpa > second.gpa) {
                third.gpa = second.gpa;
                third.name = strdup(second.name);
                second.gpa = cgpa;
                second.name = strdup(strcat(new_name, roster[i].last));
            } else {
                third.gpa = cgpa;
                third.name = strdup(strcat(new_name, roster[i].last));
            }
        }
        free(new_name);
        // Find avg hours
        total_hours = total_hours + roster[i].hours;
        // Find ang GPA of CS students
        if (strcmp(roster[i].major, "Computer Science") == 0) {
            total_cs_gpa = total_cs_gpa + roster[i].gpa;
            cs_majors++;
        }
        // Find number of advisors per major/department
        int j;
        for (j = 0; j < 15; j++) {
            if (strcmp(advisors[j].major, "N/A") == 0) {
                memset(advisors[j].major, 0, strlen(advisors[j].major));
                memset(advisors[j].names, 0, strlen(advisors[j].names));
                strcpy(advisors[j].major, roster[i].major);
                strcpy(advisors[j].names, roster[i].advisor);
                advisors[j].amt = 1;
                break;
            } else if (strcmp(advisors[j].major, roster[i].major) == 0) {
                if (strstr(advisors[j].names, roster[i].advisor) == NULL) {
                    advisors[j].amt++;
                    strcat(advisors[j].names, roster[i].advisor);
                }
                break;
            }
        }
    }
    degrees[strlen(degrees) - 2] = ' ';
    printf("%d total degrees: %s\n", total_degrees, degrees);
    free(degrees);
    printf("GPAs: %s:%.2f\t %s:%.2f\t %s:%.2f\n", first.name, first.gpa,
           second.name, second.gpa, third.name, third.gpa);
    printf("Avg credit hours: %.2f\n", (total_hours / 50.0));
    printf("Avg CS GPA: %.2f\n", (total_cs_gpa / cs_majors));

    for (i = 0; i < 20; i++) {
        if (strcmp(advisors[i].major, "N/A") == 0) {
            break;
        } else {
            printf("%s: %d     ", advisors[i].major, advisors[i].amt);
        }
    }
    printf("\n");

    return 0;
}
