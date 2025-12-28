#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>


struct student
{
    char name[50];
    char id[9];
    int grade;
    char grade_letter;
    int remediation;
};



int name_valid(char *name)
{
    int len = strlen(name);
    if ( len < 1 || len > 50)
    {
        return 0;
    }
    
    for (int i = 0; i < len; i++)
    {
        if ( !isalpha(name[i]) && name[i] != ' ')
        {
            return 0;
        }
    }
    return 1;
}



int student_id(char *id)
{
    int len = strlen(id);
    if ( len < 1 || len > 8)
    {
        return 0;
    }
    
    for (int i = 0; i < len; i++)
    {
        if ( id [i] < '0' || id [i] > '9')
        {
            return 0;
        }
    }
    return 1;
}



int score_grade(int grade)
{
    if(grade >= 0 && grade <= 100)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}



int remediation_flag(int grade)
{
    if (grade < 50)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}



char grade_to_letter(int grade) 
{
    if (grade >= 90)
    {
        return 'A';
    }
    else if (grade >= 80)
    { 
        return 'B';
    }
    else if (grade >= 70)
    {
        return 'C';
    }
    else if (grade >= 60)
    {
        return 'D';
    }
    else
    {
        return 'F';
    }
}



int main()
{
    FILE *fp = fopen("students.txt", "r");

    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }   
    
    struct student record[100];
    int count = 0;

    while (fscanf(fp, "%s %s %d", record[count].id, record[count].name, &record[count].grade) == 3)
    {
        if (!name_valid(record[count].name))
        {
            printf("Invalid name for student %d\n", count + 1);
            continue;
        }

        if (!student_id(record[count].id))
        {
            printf("Invalid ID for student %d\n", count + 1);
            continue;
        }

        if (!score_grade(record[count].grade))
        {
            printf("Invalid grade for student %d\n", count + 1);
            continue;
        }

        record[count].grade_letter = grade_to_letter(record[count].grade);
        record[count].remediation = remediation_flag(record[count].grade);

        count++;
    }
    fclose(fp);

    return 0;
}

