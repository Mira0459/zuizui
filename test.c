// VLU - Introduction to Programming in C
// Thanh Thuy
// Date: Dec 24, 2025

#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct students
{
    char id [7];
    char name [40];
    int score;
    char grade;
    int remediation;
};

FILE *input_file, *output_file;

// Validate record
int read_record(struct students records[]); // return count
int valid_id(char id[]);
int valid_name(char name[]);
int valid_score( int score);

char convert_grade (int score);

float class_average ( struct students records[], int count);
void min_max ( struct students records[], int count, int *min, int *max);
void grade_distribution(struct students records[], int count,
                         int *A, int *B, int *C, int *D, int *F);
                        
void sort_remediation(struct students reme[], int rcount);
void print_remediation(struct students reme[], int rcount);

int main ()
{
    // initialize 25 element of the struct
    struct students records[25];

    input_file = fopen("students.txt", "r");
    output_file = fopen("report.txt", "w");

    
    int count = read_record(records);
    class_average(records, count);

    int min,max;
    min_max(records, count, &min, &max );

    int A,B,C,D,F;
    grade_distribution(records, count, &A, &B, &C, &D, &F);

    /* ---- Extract remediation students ---- */
    struct students remediation[25];
    int rcount = 0;

    for (int i = 0; i < count; i++)
    {
        if (records[i].remediation == 1)
        {
            remediation[rcount++] = records[i];
        }
    }

    /* ---- Sort remediation list ---- */
    sort_remediation(remediation, rcount);

    /* ---- Print remediation students ---- */
    print_remediation(remediation, rcount);


}

int read_record(struct students records[])
{
    char id [7];
    char name [40];
    int score;

    valid_id(id);
    valid_name(name);
    valid_score(score);

    int count = 0;

    while
    ( count < 25 &&
        fscanf( input_file, "%6s %39s %d", 
        id, 
        name, 
        &score ) == 3)
    {
        if ( valid_id(id) && valid_name(name) && valid_score(score)) {

            // store record
            strcpy ( records[count].id, id);
            strcpy ( records[count].name, name);
            records[count].score = score;

            //convert grade, check score
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

/* ---------- VALIDATION FUNCTIONS ---------- */

int valid_id(char id[])
{
    int i; // order of char

    // Check if 1st and 2nd letter is char
    if( strlen(id) != 6){
        return 0;
    }
    if ( !isalpha(id[0]) || !isalpha(id[1])) {
        return 0;
    }

    //Check if the rest is number
    for ( int i = 2; i < 6; i++) {
        if( !isdigit(id[i])) {
            return 0;
        }
    }

    return 1;
    
}

int valid_name(char name[])
{
    // Reject number in name

    int len = strlen(name);
    for ( int i = 0; i < len; i++) {
        if (!isalpha(name[i]) && name[i] != '_') {
            return 0;
        }
    }

    return 1;
}

int valid_score(int score)
{
    if( score < 0 || score > 100)
    {
        return 0;
    }

    return 1;
}

/* ---------- DERIVED FIELDS ---------- */

char convert_grade(int score)
{
    if (score >= 80) return 'A';
    if (score >= 70) return 'B';
    if (score >= 60) return 'C';
    if (score >= 50) return 'D';
    return 'F';
}



/* ---------- ANALYTICS ---------- */

float class_average( struct students records[], int count)
{
    int total = 0;
    for ( int i = 0; i < count, i++) 
    {
        total += records[i].score;
    }

    float total = total / count;
}

void min_max ( struct students records[], int count, int *min, int *max)
{
    *min = records[0].score;
    *max = records[0].score;

    for ( int i = 0; i < count; i++)
    {
        if (records[i].score < *min)
            *min = records[i].score;

        if (records[i].score > *max)
            *max = records[i].score;
    }
}

void grade_distribution(struct students records[], int count,
                         int *A, int *B, int *C, int *D, int *F)
{
    /* initialize counters */
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



