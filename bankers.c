/*****************************************************************************************************************************************
 * File: bankers.c
 *
 * Purpose:
 * This program implements **Banker's Algorithm** for **deadlock avoidance** in an operating system.
 * The input is read from a 'input.txt'.
 *
 *****************************************************************************************************************************************/

// Add more header files if required...
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

/* =======================================================================================================================================*/
// Defining maximum limits for threads and resources
#define MAX_THREADS 10      // Maximum number of threads allowed
#define MAX_RESOURCES 10    // Maximum number of resource types allowed
#define MAX_LINE_LENGTH 256 // Maximum length of a line in input file

/* =======================================================================================================================================*/
// Add more Global variables if required...
int allocation[MAX_THREADS][MAX_RESOURCES]; // Stores the allocated resources for each thread
int max[MAX_THREADS][MAX_RESOURCES];        // Maximum demand of each thread
int available[MAX_RESOURCES];               // Available resources in the system
int need[MAX_THREADS][MAX_RESOURCES];       // Remaining need for each thread (max - allocation)
int n, m;                                   // n = number of threads, m = number of resource types
int total_resources[MAX_RESOURCES];         // Stores total resources in the system

/* =======================================================================================================================================*/
// Function declarations. Add more functions if needed...
void calculate_need();                               // Calculates the need matrix
void calculate_available();                          // Computes the available resources
void print_available();                              // Prints the available resources
void print_matrices();                               // Prints all relevant matrices (allocation, max, need, available)
int extract_numbers(const char *, int *, int);       // Extracts numbers from a line
int extract_matrix_values(const char *, int *, int); // Extracts matrix values from input
int extract_thread_id(const char *);                 // Extracts thread ID from input
int read_input_file(const char *);                   // Reads input file and initializes matrices
bool is_safe_state(int[]);                           // Determines if the system is in a safe state
bool thread_resource_request(int, int[]);            // Handles resource requests from threads

/* =======================================================================================================================================*/
// main function (PLEASE DO NOT MODIFY THIS FUNCTION CODE unless you want to alter the logic!!!)
int main(int argc, char *argv[])
{
    const char *filename = "input.txt";

    printf("\nReading Banker's Algorithm data from file: %s\n", filename);
    printf("\nThe MAX_THREADS allowed is: %d\n", MAX_THREADS);
    printf("The MAX_RESOURCES allowed is: %d\n", MAX_RESOURCES);

    // Initialize all matrices to zero to prevent garbage values
    memset(allocation, 0, sizeof(allocation));
    memset(max, 0, sizeof(max));
    memset(total_resources, 0, sizeof(total_resources));

    if (!read_input_file(filename))
    {
        printf("\nFailed to read input file. Exiting.\n");
        return 1;
    }

    // Compute the need matrix and available resources after reading input
    calculate_need();
    calculate_available();

    // Display initial state
    print_matrices();

    // Check if initial state is safe
    int safe_sequence[MAX_THREADS];
    if (is_safe_state(safe_sequence))
    {
        printf("[GRANT]: Initial state is SAFE. The Safe Sequence is: <");
        for (int i = 0; i < n; i++)
        {
            printf("T%d%s", safe_sequence[i], (i < n - 1) ? ", " : "");
        }
        printf(">\n\n");
    }
    else
    {
        printf("[DANGER]: Initial state is UNSAFE!\n\n");
    }

    // Interactive loop for handling thread resource requests
    while (1)
    {
        char input[10];
        printf("\nEnter thread number (0-%d) and request (", n - 1);
        for (int j = 0; j < m; j++)
        {
            printf("R%d%s", j, (j < m - 1) ? " " : "");
        }
        printf(") (or 'q' to quit): \n");

        scanf("%s", input);

        // If user enters 'q', terminate the program
        if (tolower(input[0]) == 'q')
            break;

        // Convert input to thread ID
        int thread_id = atoi(input);

        // Read requested resources
        int request[MAX_RESOURCES];
        for (int j = 0; j < m; j++)
        {
            if (scanf("%d", &request[j]) != 1)
            {
                printf("Error reading request values!\n");
                // Clear input buffer
                int c;
                while ((c = getchar()) != '\n' && c != EOF)
                    ;
                break;
            }
        }

        // Process the thread request
        thread_resource_request(thread_id, request);
    }

    return 0;
}

/* =======================================================================================================================================*/
// Function to calculate need matrix ((IMPLEMENT THIS FUNCTION))
void calculate_need()
{
    /*** your code... ***/
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            need[i][j] = max[i][j] - allocation[i][j];
        }
    }
}

/* =======================================================================================================================================*/
// Function to print the available resources at the moment (PLEASE DO NOT MODIFY THIS FUNCTION CODE unless you want to alter the logic!!!)
void print_available()
{
    printf("\nAvailable Resources at this moment:\n");
    printf("===================================\n");
    for (int j = 0; j < m; j++)
    {
        printf("R%d = %d\n", j, available[j]);
    }
}

/* =======================================================================================================================================*/
// Function to print the matrices and vectors (PLEASE DO NOT MODIFY THIS FUNCTION CODE unless you want to alter the logic!!!)
void print_matrices()
{
    printf("\nNumber of Threads: %d\n", n);
    printf("Number of Resources: %d\n\n", m);

    printf("Total Resources:\n");
    printf("================\n");
    for (int j = 0; j < m; j++)
    {
        printf("R%d = %d\n", j, total_resources[j]);
    }

    printf("\nAllocation Matrix:\n");
    printf("==================\n");
    printf("    ");
    for (int j = 0; j < m; j++)
    {
        printf("R%d  ", j);
    }
    printf("\n");

    for (int i = 0; i < n; i++)
    {
        printf("T%d  ", i);
        for (int j = 0; j < m; j++)
        {
            printf("%d   ", allocation[i][j]);
        }
        printf("\n");
    }

    printf("\nMax Matrix:\n");
    printf("===========\n");
    printf("    ");
    for (int j = 0; j < m; j++)
    {
        printf("R%d  ", j);
    }
    printf("\n");

    for (int i = 0; i < n; i++)
    {
        printf("T%d  ", i);
        for (int j = 0; j < m; j++)
        {
            printf("%d   ", max[i][j]);
        }
        printf("\n");
    }

    printf("\nNeed Matrix:\n");
    printf("============\n");
    printf("    ");
    for (int j = 0; j < m; j++)
    {
        printf("R%d  ", j);
    }
    printf("\n");

    for (int i = 0; i < n; i++)
    {
        printf("T%d  ", i);
        for (int j = 0; j < m; j++)
        {
            printf("%d   ", need[i][j]);
        }
        printf("\n");
    }

    print_available();
    printf("\n");
}

/* =======================================================================================================================================*/
// Function to extract numbers from a line (PLEASE DO NOT MODIFY THIS FUNCTION CODE unless you want to alter the logic!!!)
int extract_numbers(const char *line, int *numbers, int max_numbers)
{
    int count = 0;
    const char *ptr = line;
    char *endptr;

    while (*ptr && count < max_numbers)
    {
        // Skip non-digit characters
        while (*ptr && !isdigit(*ptr) && *ptr != '-')
            ptr++;

        if (!*ptr)
            break;

        // Convert to integer
        long val = strtol(ptr, &endptr, 10);

        if (ptr != endptr) // Conversion successful
        {
            numbers[count++] = (int)val;
            ptr = endptr;
        }
        else // No valid number found
            break;
    }

    return count;
}

/* =======================================================================================================================================*/
// Function to extract matrix values following a thread ID (PLEASE DO NOT MODIFY THIS FUNCTION CODE unless you want to alter the logic!!!)
int extract_matrix_values(const char *line, int *numbers, int max_numbers)
{
    int count = 0;
    const char *ptr = line;

    // First, find the thread ID part
    while (*ptr)
    {
        if (*ptr == 'T' && isdigit(*(ptr + 1)))
        {
            // Skip past the thread ID (T0, T1, etc.)
            while (*ptr && !isspace(*ptr))
                ptr++;

            // Now extract the numbers that follow
            while (*ptr && count < max_numbers)
            {
                // Skip whitespace
                while (*ptr && isspace(*ptr))
                    ptr++;

                if (!*ptr || !isdigit(*ptr))
                    break;

                // Convert to integer
                char *endptr;
                long val = strtol(ptr, &endptr, 10);

                if (ptr != endptr) // Conversion successful
                {
                    numbers[count++] = (int)val;
                    ptr = endptr;
                }
                else // No valid number found
                    break;
            }

            // We've processed the line after the thread ID
            break;
        }
        ptr++;
    }

    return count;
}

/* =======================================================================================================================================*/
// Function to extract thread ID from lines like "T0 0 1 0" (PLEASE DO NOT MODIFY THIS FUNCTION CODE unless you want to alter the logic!!!)
int extract_thread_id(const char *line)
{
    const char *ptr = line;

    // Look for "T" followed by digits
    while (*ptr)
    {
        if (*ptr == 'T' && isdigit(*(ptr + 1)))
        {
            return atoi(ptr + 1);
        }
        ptr++;
    }

    return -1; // Thread ID not found
}

/* =======================================================================================================================================*/
// Function to read input file with the specified format (PLEASE DO NOT MODIFY THIS FUNCTION CODE unless you want to alter the logic!!!)
int read_input_file(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        perror("Error opening input file");
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    int line_count = 0;
    int in_allocation_matrix = 0;
    int in_max_matrix = 0;
    int allocation_rows_found = 0;
    int max_rows_found = 0;
    int resource_instances_found = 0;

    // Process each line
    while (fgets(line, sizeof(line), file))
    {
        line_count++;

        // Remove trailing newline
        size_t len = strlen(line);
        if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
            line[--len] = '\0';
        if (len > 0 && (line[len - 1] == '\n' || line[len - 1] == '\r'))
            line[--len] = '\0';

        // Skip empty lines
        if (len == 0)
            continue;

        // Check for number of threads
        if (strstr(line, "threads") != NULL)
        {
            int numbers[2];
            if (extract_numbers(line, numbers, 2) > 0)
            {
                n = numbers[0];

                if (n > MAX_THREADS)
                {
                    printf("[ERROR]: Number of Threads (%d) exceeds maximum allowed (%d).\n", n, MAX_THREADS);
                    fclose(file);
                    return 0;
                }
            }
            continue;
        }

        // Check for number of resources
        if (strstr(line, "resources") != NULL)
        {
            int numbers[2];
            if (extract_numbers(line, numbers, 2) > 0)
            {
                m = numbers[0];

                if (m > MAX_RESOURCES)
                {
                    printf("[ERROR]: Number of resources (%d) exceeds maximum allowed (%d).\n", m, MAX_RESOURCES);
                    fclose(file);
                    return 0;
                }
            }
            continue;
        }

        // Check for resource instances
        if (strstr(line, "instances of resource") != NULL)
        {
            int numbers[2];
            int count = extract_numbers(line, numbers, 2);
            if (count >= 2)
            {
                int resource_id = numbers[0];
                total_resources[resource_id] = numbers[1];
                resource_instances_found++;
            }
            continue;
        }

        // Check for allocation matrix start
        if (strstr(line, "Allocation matrix") != NULL)
        {
            in_allocation_matrix = 1;
            in_max_matrix = 0;
            continue;
        }

        // Check for max matrix start
        if (strstr(line, "Max matrix") != NULL)
        {
            in_allocation_matrix = 0;
            in_max_matrix = 1;
            continue;
        }

        // Skip header lines
        if (strstr(line, "R0") != NULL)
        {
            continue;
        }

        // Skip divider lines (contains only = or - characters)
        bool is_divider = true;
        for (int i = 0; line[i]; i++)
        {
            if (line[i] != '=' && line[i] != '-' && !isspace(line[i]))
            {
                is_divider = false;
                break;
            }
        }
        if (is_divider)
        {
            continue;
        }

        // Thread matrix rows
        int thread_id = extract_thread_id(line);
        if (thread_id >= 0)
        {
            int numbers[MAX_RESOURCES];
            int num_count = extract_numbers(line, numbers, MAX_RESOURCES);

            // Get the matrix values using the improved extractor function
            int matrix_values[MAX_RESOURCES];
            int value_count = extract_matrix_values(line, matrix_values, MAX_RESOURCES);

            if (in_allocation_matrix && value_count > 0)
            {
                // Make sure we store all m values in the correct positions
                if (value_count < m)
                {
                    printf("[WARNING]: Thread T%d has fewer resources (%d) than expected (%d) in allocation matrix\n",
                           thread_id, value_count, m);
                    return 0;
                }

                // Store the values directly in the right positions
                for (int j = 0; j < value_count && j < m; j++)
                {
                    allocation[thread_id][j] = matrix_values[j];
                }
                allocation_rows_found++;
            }
            else if (in_max_matrix && value_count > 0)
            {
                // Make sure we store all m values in the correct positions
                if (value_count < m)
                {
                    printf("[WARNING]: Thread T%d has fewer resources (%d) than expected (%d) in max matrix\n",
                           thread_id, value_count, m);
                    return 0;
                }

                // Store the values directly in the right positions
                for (int j = 0; j < value_count && j < m; j++)
                {
                    max[thread_id][j] = matrix_values[j];
                }
                max_rows_found++;
            }
        }
    }

    fclose(file);

    // Perform basic validation
    if (n <= 0)
    {
        printf("[ERROR]: Invalid Number of Threads!...\n");
        return 0;
    }

    if (m <= 0)
    {
        printf("[ERROR]: Invalid number Resources!...\n");
        return 0;
    }

    if (resource_instances_found < m)
    {
        printf("[ERROR]: Not all Resource Instances were specified!...\n");
    }

    if (allocation_rows_found < n)
    {
        printf("[ERROR]: Not all Allocation matrix rows were found!...\n");
        return 0;
    }

    if (max_rows_found < n)
    {
        printf("[ERROR]: Not all Max matrix rows were found!...\n");
        return 0;
    }

    return 1;
}

/* =======================================================================================================================================*/
// Function to calculate available resources ((IMPLEMENT THIS FUNCTION))
void calculate_available()
{
    /*** your code... ***/
    for (int j = 0; j < m; j++) {
        available[j] = total_resources[j];
        for (int i = 0; i < n; i++) {
            available[j] -= allocation[i][j];
        }
    }
}

/* =======================================================================================================================================*/
// Function to check if the system is in a safe state (IMPLEMENT THIS FUNCTION)
bool is_safe_state(int safe_sequence[])
{
    /*** Implement the remaining code based on the provided HINT. ***/

    // HINT: Initialize work[] to available[]
    // HINT: Try to find a safe sequence
    // HINT: If a thread can finish, add its resources to work
    // HINT: If no thread can be found to finish, the system is in an unsafe state
    int work[MAX_RESOURCES];
    bool finish[MAX_THREADS] = {false};

    // HINT: Initialize work[] to available[]
    for (int j = 0; j < m; j++) {
        work[j] = available[j];
    }

    int count = 0;

    while (count < n) {
        bool found = false;
        for (int i = 0; i < n; i++) {
            if (!finish[i]) {
                bool can_finish = true;
                for (int j = 0; j < m; j++) {
                    if (need[i][j] > work[j]) {
                        can_finish = false;
                        break;
                    }
                }
                // HINT: If a thread can finish, add its resources to work
                if (can_finish) {
                    for (int j = 0; j < m; j++) {
                        work[j] += allocation[i][j];
                    }
                    safe_sequence[count++] = i;
                    finish[i] = true;
                    found = true;
                }
            }
        }

        if (!found) {
            return false; // HINT: If no thread can be found to finish, the system is in an unsafe state
        }
    }

    return true;
}

/* ========================================================================================================================*/
// Function to process a thread resource request (IMPLEMENT THIS FUNCTION)
bool thread_resource_request(int thread_id, int request[])
{
    /*** Implement the remaining code based on the provided HINT. ***/

    // Check if thread_id is valid
    if (thread_id < 0 || thread_id >= n)
    {
        printf("[ERROR]: Invalid thread ID. Must be between 0 and %d!...\n", n - 1);
        return false;
    }

    // HINT: Check if request exceeds need
    for (int j = 0; j < m; j++) {
        if (request[j] > need[thread_id][j]) {
            printf("\n[AVOID]: Thread T%d requested more than its need!...\n", thread_id);
            return false;
        }
    }
    // HINT: Check if request exceeds available
    for (int j = 0; j < m; j++) {
        if (request[j] > available[j]) {
            printf("\n[AVOID]: Thread T%d must wait, resources not available!...\n", thread_id);
            return false;
        }
    }
    // HINT: Try to allocate resources (temporarily)
    for (int j = 0; j < m; j++) {
        available[j] -= request[j];
        allocation[thread_id][j] += request[j];
        need[thread_id][j] -= request[j];
    }

    int temp_safe_sequence[MAX_THREADS];
    bool safe = is_safe_state(temp_safe_sequence);

    // HINT: Check if resulting state is safe
    if (safe) {
        printf("\n[GRANT]: Request granted. System is in Safe State...\n");
        printf("Safe Sequence: <");
        for (int i = 0; i < n; i++) {
            printf("T%d%s", temp_safe_sequence[i], (i < n - 1) ? ", " : "");
        }
        printf(">\n");
        return true;
    } else {

    // HINT: Roll back the changes
    for (int j = 0; j < m; j++) {
        available[j] += request[j];
        allocation[thread_id][j] -= request[j];
        need[thread_id][j] += request[j];
    }

    printf("\n[AVOID]: Request denied. System will not be in a safe state if this request is granted!...\n");
    printf("Resources have been rolled-back to their previous state...\n");
    return false;
}
}
