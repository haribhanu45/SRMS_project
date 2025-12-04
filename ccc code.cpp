#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define STUDENTFILE "students.txt"
#define CREDENTIALSFILE "credentials.txt"

struct student{
    int roll;
    char name[50];
    float marks;
};

char currentRole[20];
char currentUser[50];


int loginSystem(void);
void mainMenu(void);
void adminMenu(void);
void userMenu(void);
void staffMenu(void);
void getMenu(void);
void addStudent(void);
void displayStudents(void);
void searchStudent(void);
void updateStudent(void);
void deleteStudent(void);

int main (){
 if(   loginSystem()){
    mainMenu();
 }
 else{
    printf("Exiting the program.\n");
    return 0;
 }
     return 0;
}

int loginSystem(){
    char username[50];
    char password[50];
    int attempts = 3;
    
    printf("=== Login System ===\n");
    
    while(attempts > 0){
        printf("Enter username: ");
        scanf("%s", username);
        printf("Enter password: ");
        scanf("%s", password);

        FILE *fp = fopen(CREDENTIALSFILE, "r");
        if(!fp){
            printf("Error opening credentials file.\n");
            return 0;
        }

        char fileUser[50], filePass[50], filerole[20];
        int loginSuccess = 0;
        
        while(fscanf(fp, "%s %s %s", fileUser, filePass, filerole) == 3){
            if(strcmp(username, fileUser) == 0 && strcmp(password, filePass) == 0){
                strcpy(currentRole, filerole);
                strcpy(currentUser, fileUser);
                fclose(fp);
                printf("Login successful! Welcome %s (Role: %s).\n", currentUser, currentRole);
                return 1;
            }
        }
        fclose(fp);
        
        attempts--;
        if(attempts > 0){
            printf("Invalid username or password. Attempts remaining: %d\n\n", attempts);
        }
    }
    
    printf("Too many failed login attempts. Exiting...\n");
    return 0;
}

void mainMenu(){
    while(1){
        if(strcmp(currentRole, "ADMIN") == 0){
            adminMenu();
            break;
        }
        else if(strcmp(currentRole, "USER") == 0){
            userMenu();
            break;
        }
        else if(strcmp(currentRole, "STAFF") == 0){
            staffMenu();
            break;
        }
        else{
            getMenu();
            break;
        }
    }
}
   
void adminMenu(){
    int choice ;
    do{
        printf ("=== Admin Menu ===\n");
        printf("1.Add Student\n");
        printf("2.Dispaly Students\n");
        printf("3.Search Student\n");
        printf("4.Update Student\n");
        printf("5.Delete Student\n");
        printf("6.Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        switch(choice){
            case 1:
                addStudent();
                break;
            case 2:
                displayStudents();
                break;
            case 3:
                searchStudent();
                break;
            case 4:
                updateStudent();
                break;
            case 5:
                deleteStudent();
                break;
            case 6:
                printf("Logging out...\n");
                return ;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }while(1);
}


void userMenu(void){
    int choice;
    do{
        printf("\n=== User Menu ===\n");
        printf("1. View All Students\n");
        printf("2. Search Student\n");
        printf("3. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice){
            case 1:
                displayStudents();
                break;
            case 2:
                searchStudent();
                break;
            case 3:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }while(1);
}

void staffMenu(void){
    int choice;
    do{
        printf("\n=== Staff Menu ===\n");
        printf("1. View All Students\n");
        printf("2. Search Student\n");
        printf("3. Update Student Marks\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        
        switch(choice){
            case 1:
                displayStudents();
                break;
            case 2:
                searchStudent();
                break;
            case 3:
                updateStudent();
                break;
            case 4:
                printf("Logging out...\n");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }while(1);
}

void getMenu(void){
    printf("Unknown role '%s'. No menu available.\n", currentRole);
}

void addStudent(void){
    struct student s;
    FILE *fp = fopen(STUDENTFILE, "a");
    
    if(!fp){
        printf("Error opening students file.\n");
        return;
    }
    
    printf("\n--- Add Student ---\n");
    printf("Enter Roll Number: ");
    scanf("%d", &s.roll);
    printf("Enter Student Name: ");
    scanf("%s", s.name);
    printf("Enter Marks: ");
    scanf("%f", &s.marks);
    
    fprintf(fp, "%d %s %.1f\n", s.roll, s.name, s.marks);
    fclose(fp);
    printf("Student added successfully!\n");
}

void displayStudents(void){
    struct student s;
    FILE *fp = fopen(STUDENTFILE, "r");
    
    if(!fp){
        printf("Error opening students file.\n");
        return;
    }
    
    printf("\n--- Student Records ---\n");
    printf("%-10s %-20s %-10s\n", "Roll", "Name", "Marks");
    printf("============================================\n");
    
    while(fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3){
        printf("%-10d %-20s %-10.1f\n", s.roll, s.name, s.marks);
    }
    
    fclose(fp);
    printf("============================================\n");
}

void searchStudent(void){
    struct student s;
    int searchRoll;
    int found = 0;
    
    FILE *fp = fopen(STUDENTFILE, "r");
    if(!fp){
        printf("Error opening students file.\n");
        return;
    }
    
    printf("\n--- Search Student ---\n");
    printf("Enter Roll Number to search: ");
    scanf("%d", &searchRoll);
    
    while(fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3){
        if(s.roll == searchRoll){
            printf("\nStudent Found:\n");
            printf("Roll Number: %d\n", s.roll);
            printf("Name: %s\n", s.name);
            printf("Marks: %.1f\n", s.marks);
            found = 1;
            break;
        }
    }
    
    if(!found){
        printf("Student with roll number %d not found.\n", searchRoll);
    }
    
    fclose(fp);
}

void updateStudent(void){
    struct student s, temp;
    int searchRoll;
    int found = 0;
    FILE *fp, *tempfp;
    
    fp = fopen(STUDENTFILE, "r");
    if(!fp){
        printf("Error opening students file.\n");
        return;
    }
    
    tempfp = fopen("temp.txt", "w");
    if(!tempfp){
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }
    
    printf("\n--- Update Student ---\n");
    printf("Enter Roll Number to update: ");
    scanf("%d", &searchRoll);
    
    while(fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3){
        if(s.roll == searchRoll){
            printf("Current Details:\n");
            printf("Roll: %d, Name: %s, Marks: %.1f\n", s.roll, s.name, s.marks);
            
            printf("Enter new marks: ");
            scanf("%f", &s.marks);
            
            printf("Student updated successfully!\n");
            found = 1;
        }
        fprintf(tempfp, "%d %s %.1f\n", s.roll, s.name, s.marks);
    }
    
    fclose(fp);
    fclose(tempfp);
    
    if(found){
        remove(STUDENTFILE);
        rename("temp.txt", STUDENTFILE);
    } else {
        printf("Student with roll number %d not found.\n", searchRoll);
        remove("temp.txt");
    }
}

void deleteStudent(void){
    struct student s;
    int searchRoll;
    int found = 0;
    FILE *fp, *tempfp;
    
    fp = fopen(STUDENTFILE, "r");
    if(!fp){
        printf("Error opening students file.\n");
        return;
    }
    
    tempfp = fopen("temp.txt", "w");
    if(!tempfp){
        printf("Error creating temporary file.\n");
        fclose(fp);
        return;
    }
    
    printf("\n--- Delete Student ---\n");
    printf("Enter Roll Number to delete: ");
    scanf("%d", &searchRoll);
    
    while(fscanf(fp, "%d %s %f", &s.roll, s.name, &s.marks) == 3){
        if(s.roll == searchRoll){
            printf("Student with roll number %d deleted successfully!\n", searchRoll);
            found = 1;
        } else {
            fprintf(tempfp, "%d %s %.1f\n", s.roll, s.name, s.marks);
        }
    }
    
    fclose(fp);
    fclose(tempfp);
    
    if(found){
        remove(STUDENTFILE);
        rename("temp.txt", STUDENTFILE);
    } else {
        printf("Student with roll number %d not found.\n", searchRoll);
        remove("temp.txt");
    }
}
