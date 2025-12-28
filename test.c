// VLU - Introduction to Programming in C
// Thanh Thuy
// Date: Dec 24, 2025

#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct students
{
    char id[7];
    char name[40];
    int score;
    char grade;
    int remediation;
};

FILE *input_file, *output_file;

/* ---------- FUNCTION PROTOTYPES ---------- */
int read_record(struct students records[]);
int valid_id(char id[]);
int valid_name(char name[]);
int valid_score(int score);

char convert_grade(int score);

float class_average(struct students records[], int count);
void min_max(struct students records[], int count, int *min, int *max);
void grade_distribution(struct students records[], int count,
                        int *A, int *B, int *C, int *D, int *F);

void sort_remediation(struct students a[], int n);
void sort_top_students(struct students a[], int n);

void print_remediation(struct students a[], int n);
void print_top5(struct students a[], int n);

void write_remediation(FILE *out, struct students a[], int n);
void write_top5(FILE *out, struct students a[], int n);

/* ---------- MAIN ---------- */
int main(void)
{
    struct students records[25];

    input_file = fopen("students.txt", "r");
    output_file = fopen("report.txt", "w");

    if (input_file == NULL || output_file == NULL)
    {
        printf("File error.\n");
        return 1;
    }

    int count = read_record(records);

    float avg = class_average(records, count);

    int min, max;
    min_max(records, count, &min, &max);

    int A, B, C, D, F;
    grade_distribution(records, count, &A, &B, &C, &D, &F);

    /* ---------- REMEDIATION ---------- */
    struct students remediation[25];
    int rcount = 0;

    for (int i = 0; i < count; i++)
        if (records[i].remediation)
            remediation[rcount++] = records[i];

    sort_remediation(remediation, rcount);

    /* ---------- TOP STUDENTS ---------- */
    struct students top[25];
    for (int i = 0; i < count; i++)
        top[i] = records[i];

    sort_top_students(top, count);

    /* ---------- WRITE REPORT ---------- */
    fprintf(output_file, "===== STUDENT REPORT =====\n\n");
    fprintf(output_file, "Total students: %d\n", count);
    fprintf(output_file, "Class average: %.2f\n", avg);
    fprintf(output_file, "Minimum score: %d\n", min);
    fprintf(output_file, "Maximum score: %d\n\n", max);

    fprintf(output_file, "Grade distribution:\n");
    fprintf(output_file, "A: %d\nB: %d\nC: %d\nD: %d\nF: %d\n\n",
            A, B, C, D, F);

    write_remediation(output_file, remediation, rcount);
    write_top5(output_file, top, count);

    /* ---------- CONSOLE OUTPUT ---------- */
    print_remediation(remediation, rcount);
    print_top5(top, count);

    fclose(input_file);
    fclose(output_file);

    return 0;
}

/* ---------- READ & VALIDATE ---------- */
int read_record(struct students records[])
{
    char id[7], name[40];
    int score;
    int count = 0;

    while (count < 25 &&
           fscanf(input_file, "%6s %39s %d", id, name, &score) == 3)
    {
        if (valid_id(id) && valid_name(name) && valid_score(score))
        {
            strcpy(records[count].id, id);
            strcpy(records[count].name, name);
            records[count].score = score;
            records[count].grade = convert_grade(score);
            records[count].remediation = (score < 50);
            count++;
        }
        else
        {
            printf("Invalid record skipped: %s %s %d\n", id, name, score);
        }
    }
    return count;
}

/* ---------- VALIDATION ---------- */
int valid_id(char id[])
{
    // Format: S10001 (1 letter + 5 digits)
    if (strlen(id) != 6)
        return 0;

    if (!isalpha(id[0]))
        return 0;

    for (int i = 1; i < 6; i++)
        if (!isdigit(id[i]))
            return 0;

    return 1;
}


int valid_name(char name[])
{
    for (int i = 0; name[i]; i++)
        if (!isalpha(name[i]) && name[i] != '_')
            return 0;
    return 1;
}

int valid_score(int score)
{
    return (score >= 0 && score <= 100);
}

/* ---------- DERIVED ---------- */
char convert_grade(int score)
{
    if (score >= 80) return 'A';
    if (score >= 70) return 'B';
    if (score >= 60) return 'C';
    if (score >= 50) return 'D';
    return 'F';
}

/* ---------- ANALYTICS ---------- */
float class_average(struct students records[], int count)
{
    int sum = 0;
    for (int i = 0; i < count; i++)
        sum += records[i].score;

    return (float)sum / count;
}

void min_max(struct students records[], int count, int *min, int *max)
{
    *min = *max = records[0].score;

    for (int i = 1; i < count; i++)
    {
        if (records[i].score < *min) *min = records[i].score;
        if (records[i].score > *max) *max = records[i].score;
    }
}

void grade_distribution(struct students records[], int count,
                        int *A, int *B, int *C, int *D, int *F)
{
    *A = *B = *C = *D = *F = 0;

    for (int i = 0; i < count; i++)
    {
        switch (records[i].grade)
        {
            case 'A': (*A)++; break;
            case 'B': (*B)++; break;
            case 'C': (*C)++; break;
            case 'D': (*D)++; break;
            case 'F': (*F)++; break;
        }
    }
}

/* ---------- SORTING ---------- */
void sort_remediation(struct students a[], int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (a[j].score > a[j + 1].score ||
               (a[j].score == a[j + 1].score &&
                strcmp(a[j].name, a[j + 1].name) > 0))
            {
                struct students temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
}

void sort_top_students(struct students a[], int n)
{
    for (int i = 0; i < n - 1; i++)
        for (int j = 0; j < n - i - 1; j++)
            if (a[j].score < a[j + 1].score ||
               (a[j].score == a[j + 1].score &&
                strcmp(a[j].name, a[j + 1].name) > 0))
            {
                struct students temp = a[j];
                a[j] = a[j + 1];
                a[j + 1] = temp;
            }
}

/* ---------- OUTPUT ---------- */
void print_remediation(struct students a[], int n)
{
    printf("\nStudents needing remediation:\n");
    printf("ID      Name               Score Grade\n");
    for (int i = 0; i < n; i++)
        printf("%-6s  %-18s  %3d    %c\n",
               a[i].id, a[i].name, a[i].score, a[i].grade);
}

void print_top5(struct students a[], int n)
{
    int limit = (n < 5) ? n : 5;
    printf("\nTop %d students by grade:\n", limit);
    printf("ID      Name               Score Grade\n");
    for (int i = 0; i < limit; i++)
        printf("%-6s  %-18s  %3d    %c\n",
               a[i].id, a[i].name, a[i].score, a[i].grade);
}

void write_remediation(FILE *out, struct students a[], int n)
{
    fprintf(out, "Students needing remediation:\n");
    fprintf(out, "ID      Name               Score Grade\n");
    for (int i = 0; i < n; i++)
        fprintf(out, "%-6s  %-18s  %3d    %c\n",
                a[i].id, a[i].name, a[i].score, a[i].grade);
    fprintf(out, "\n");
}

void write_top5(FILE *out, struct students a[], int n)
{
    int limit = (n < 5) ? n : 5;
    fprintf(out, "Top %d students by grade:\n", limit);
    fprintf(out, "ID      Name               Score Grade\n");
    for (int i = 0; i < limit; i++)
        fprintf(out, "%-6s  %-18s  %3d    %c\n",
                a[i].id, a[i].name, a[i].score, a[i].grade);
}
