#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
    int rollNo;
    char name[50];
    int age;
    float marks;
};

void addStudent();
void listStudents();
void searchStudent();
void updateStudent();
void deleteStudent();

int main() {
    int choice;
    while(1) {
        printf("\n--- Student Record Management System ---\n");
        printf("1. Add Student\n");
        printf("2. List Students\n");
        printf("3. Search Student\n");
        printf("4. Update Student\n");
        printf("5. Delete Student\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: addStudent(); break;
            case 2: listStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: exit(0);
            default: printf("Invalid choice! Please try again.\n");
        }
    }
    return 0;
}

void addStudent() {
    FILE *fp = fopen("students.dat", "ab");
    struct Student st;
    
    printf("Enter Roll Number: ");
    scanf("%d", &st.rollNo);
    printf("Enter Name: ");
    scanf(" %[^\n]s", st.name);
    printf("Enter Age: ");
    scanf("%d", &st.age);
    printf("Enter Marks: ");
    scanf("%f", &st.marks);
    
    fwrite(&st, sizeof(st), 1, fp);
    fclose(fp);
    printf("Student added successfully!\n");
}

void listStudents() {
    FILE *fp = fopen("students.dat", "rb");
    struct Student st;
    
    if(fp == NULL) {
        printf("No student records found.\n");
        return;
    }
    
    printf("\nList of Students:\n");
    printf("RollNo\tName\t\tAge\tMarks\n");
    printf("---------------------------------------------\n");
    
    while(fread(&st, sizeof(st), 1, fp)) {
        printf("%d\t%s\t\t%d\t%.2f\n", st.rollNo, st.name, st.age, st.marks);
    }
    
    fclose(fp);
}

void searchStudent() {
    int rollNo;
    FILE *fp = fopen("students.dat", "rb");
    struct Student st;
    int found = 0;
    
    if(fp == NULL) {
        printf("No student records found.\n");
        return;
    }
    
    printf("Enter Roll Number to search: ");
    scanf("%d", &rollNo);
    
    while(fread(&st, sizeof(st), 1, fp)) {
        if(st.rollNo == rollNo) {
            printf("Student Found:\n");
            printf("Roll Number: %d\nName: %s\nAge: %d\nMarks: %.2f\n", st.rollNo, st.name, st.age, st.marks);
            found = 1;
            break;
        }
    }
    
    if(!found)
        printf("Student with Roll Number %d not found.\n", rollNo);
    
    fclose(fp);
}

void updateStudent() {
    int rollNo;
    FILE *fp = fopen("students.dat", "rb+");
    struct Student st;
    int found = 0;
    
    if(fp == NULL) {
        printf("No student records found.\n");
        return;
    }
    
    printf("Enter Roll Number to update: ");
    scanf("%d", &rollNo);
    
    while(fread(&st, sizeof(st), 1, fp)) {
        if(st.rollNo == rollNo) {
            printf("Current Details:\n");
            printf("Name: %s\nAge: %d\nMarks: %.2f\n", st.name, st.age, st.marks);
            
            printf("Enter new Name: ");
            scanf(" %[^\n]s", st.name);
            printf("Enter new Age: ");
            scanf("%d", &st.age);
            printf("Enter new Marks: ");
            scanf("%f", &st.marks);
            
            fseek(fp, -sizeof(st), SEEK_CUR);
            fwrite(&st, sizeof(st), 1, fp);
            found = 1;
            printf("Student updated successfully.\n");
            break;
        }
    }
    
    if(!found)
        printf("Student with Roll Number %d not found.\n", rollNo);
    
    fclose(fp);
}

void deleteStudent() {
    int rollNo;
    FILE *fp = fopen("students.dat", "rb");
    FILE *temp = fopen("temp.dat", "wb");
    struct Student st;
    int found = 0;
    
    if(fp == NULL) {
        printf("No student records found.\n");
        return;
    }
    
    printf("Enter Roll Number to delete: ");
    scanf("%d", &rollNo);
    
    while(fread(&st, sizeof(st), 1, fp)) {
        if(st.rollNo == rollNo) {
            found = 1;
            continue; // skip writing this student to temp file
        }
        fwrite(&st, sizeof(st), 1, temp);
    }
    
    fclose(fp);
    fclose(temp);
    
    if(found) {
        remove("students.dat");
        rename("temp.dat", "students.dat");
        printf("Student deleted successfully.\n");
    } else {
        remove("temp.dat");
        printf("Student with Roll Number %d not found.\n", rollNo);
    }
}
