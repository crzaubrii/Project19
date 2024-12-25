#include <stdio.h>   // For input/output functions
#include <string.h>  // For string manipulation functions
#include <stdlib.h>  // For general-purpose functions like memory handling

// Function prototypes
typedef char** charptr; // Example usage of pointer type
int romanToDecimal(const char *roman); // Converts Roman numeral to decimal
long long performOperation(long long num1, long long num2, char operator); // Performs arithmetic operations
char* decimalToWords(long long num); // Converts decimal numbers to English words
void processFile(const char* inputFileName, const char* outputFileName); // Processes the input and output file

// Arrays for converting numbers into words
char* ones[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
char* teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
char* tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
char* thousands[] = {"", "Thousand", "Million", "Billion"};

// Main function
int main() {
    // Input and output file names
    const char* inputFileName = "input.txt";
    const char* outputFileName = "output.txt";

    // Process the file with the defined function
    processFile(inputFileName, outputFileName);

    // Inform the user that processing is complete
    printf("Processing completed. Check %s for results.\n", outputFileName);
    return 0;
}

// Converts Roman numerals to decimal numbers
int romanToDecimal(const char *roman) {
    struct Roman {
        char symbol; // Roman numeral character
        int value;   // Corresponding decimal value
    } romanMap[] = {
        {'I', 1}, {'V', 5}, {'X', 10}, {'L', 50}, {'C', 100}, {'D', 500}, {'M', 1000}
    };

    int total = 0, i = 0; // Initialize total to 0 and index to 0

    // Loop through each character in the Roman numeral string
    while (roman[i]) {
        int current = 0, next = 0; // Values of the current and next Roman numeral

        // Find the value of the current and next Roman numeral
        for (int j = 0; j < 7; j++) {
            if (roman[i] == romanMap[j].symbol) current = romanMap[j].value;
            if (roman[i + 1] == romanMap[j].symbol) next = romanMap[j].value;
        }

        // Subtract if the current numeral is smaller than the next, else add
        if (current < next) total -= current;
        else total += current;

        i++; // Move to the next character
    }

    return total; // Return the decimal equivalent
}

// Perform arithmetic operations on two numbers
long long performOperation(long long num1, long long num2, char operator) {
    // Switch statement to determine the operation based on the operator
    switch (operator) {
        case '+': return num1 + num2; // Addition
        case '-': return num1 - num2; // Subtraction
        case '*': return num1 * num2; // Multiplication
        case '/': return (num2 != 0) ? num1 / num2 : 0; // Division (avoiding divide by zero)
        default: return 0; // Default case for unsupported operators
    }
}

// Converts a decimal number to English words
char* decimalToWords(long long num) {
    static char buffer[1000]; // Buffer to hold the result
    buffer[0] = '\0'; // Initialize buffer

    // Handle negative numbers
    if (num < 0) {
        strcpy(buffer, "Negative ");
        num = -num; // Make the number positive for further processing
    }

    // Handle zero
    if (num == 0) {
        strcpy(buffer, "Zero");
    } else {
        char temp[1000] = ""; // Temporary buffer for processing
        int unitIndex = 0;     // Index for thousands place

        // Process each group of three digits
        while (num > 0) {
            int chunk = num % 1000; // Extract the last three digits

            if (chunk > 0) {
                char chunkWords[100] = ""; // Buffer for the current chunk

                // Handle hundreds place
                if (chunk >= 100) {
                    strcat(chunkWords, ones[chunk / 100]);
                    strcat(chunkWords, " Hundred ");
                    chunk %= 100; // Remove the hundreds place
                }

                // Handle tens and ones place
                if (chunk >= 20) {
                    strcat(chunkWords, tens[chunk / 10]);
                    strcat(chunkWords, " ");
                    chunk %= 10; // Remove the tens place
                } else if (chunk >= 10) {
                    strcat(chunkWords, teens[chunk - 10]);
                    chunk = 0; // Teens are complete numbers
                }

                // Handle ones place
                if (chunk > 0) {
                    strcat(chunkWords, ones[chunk]);
                    strcat(chunkWords, " ");
                }

                // Append the thousands place if needed
                if (unitIndex > 0) {
                    strcat(chunkWords, thousands[unitIndex]);
                    strcat(chunkWords, " ");
                }

                strcat(chunkWords, temp); // Prepend chunk to the result
                strcpy(temp, chunkWords); // Update temporary buffer
            }

            num /= 1000; // Remove the processed chunk
            unitIndex++; // Move to the next thousands place
        }

        strcat(buffer, temp); // Copy the result to the final buffer
    }

    return buffer; // Return the result
}

// Processes the input file and writes results to the output file
void processFile(const char* inputFileName, const char* outputFileName) {
    FILE *inputFile = fopen(inputFileName, "r"); // Open input file for reading
    FILE *outputFile = fopen(outputFileName, "w"); // Open output file for writing

    // Check if files were opened successfully
    if (inputFile == NULL || outputFile == NULL) {
        printf("Error opening file.\n");
        return;
    }

    char line[100]; // Buffer to hold each line from the file

    // Read each line from the input file
    while (fgets(line, sizeof(line), inputFile)) {
        if (strlen(line) < 3) continue; // Skip empty or invalid lines

        char roman1[20], roman2[20], operator; // Buffers for operands and operator

        // Parse the line into two Roman numerals and an operator
        sscanf(line, "%s %c %s", roman1, &operator, roman2);

        // Convert Roman numerals to decimal numbers
        int num1 = romanToDecimal(roman1);
        int num2 = romanToDecimal(roman2);

        // Perform the specified operation
        long long result = performOperation(num1, num2, operator);

        // Convert the result to English words
        char* resultInWords = decimalToWords(result);

        // Write the result to the output file
        fprintf(outputFile, "%s\n", resultInWords);
    }

    fclose(inputFile); // Close the input file
    fclose(outputFile); // Close the output file
}
