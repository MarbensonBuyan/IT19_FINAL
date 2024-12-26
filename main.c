#include <stdio.h>
 
// Function prototypes for Roman numeral conversion, operation handling, and string manipulation
int romanToDecimal(char roman[]); // Converts Roman numerals to decimal numbers
void performOperation(char operation[], char result[]); // Handles arithmetic operations
void numberToWords(int num, char word[]); // Converts numbers to English word representation
int stringLength(const char str[]); // Gets length of string
void stringCopy(char dest[], const char src[]); // Copies source string to destination
void stringConcat(char dest[], const char src[]); // Concatenates source string to destination
 
// Calculates length of string by counting characters until null terminator
int stringLength(const char str[]) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}
 
// Copies characters from source to destination string until null terminator
void stringCopy(char dest[], const char src[]) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}
 
// Appends source string to end of destination string
void stringConcat(char dest[], const char src[]) {
    int destLen = stringLength(dest);
    int i = 0;
    while (src[i] != '\0') {
        dest[destLen + i] = src[i];
        i++;
    }
    dest[destLen + i] = '\0';
}
 
// Converts Roman numeral string to decimal value using standard Roman numeral rules
int romanToDecimal(char roman[]) {
    int value = 0;
    // Array to store decimal values for Roman numerals (indexed by letter - 'A')
    int romanValues[26] = {0};
 
    // Initialize values for each Roman numeral
    romanValues['I' - 'A'] = 1;
    romanValues['V' - 'A'] = 5;
    romanValues['X' - 'A'] = 10;
    romanValues['L' - 'A'] = 50;
    romanValues['C' - 'A'] = 100;
    romanValues['D' - 'A'] = 500;
    romanValues['M' - 'A'] = 1000;
 
    // Process each character, handling subtractive notation (e.g., IV = 4)
    for (int i = 0; i < stringLength(roman); i++) {
        if (i + 1 < stringLength(roman) && romanValues[roman[i] - 'A'] < romanValues[roman[i + 1] - 'A']) {
            value -= romanValues[roman[i] - 'A'];
        } else {
            value += romanValues[roman[i] - 'A'];
        }
    }
 
    return value;
}
 
// Processes arithmetic operation on Roman numerals and converts result to words
void performOperation(char operation[], char result[]) {
    // Arrays to store parsed Roman numerals and operator
    char roman1[50], roman2[50], op;
    int i = 0, j = 0;
    
    // Parse first Roman numeral
    while (operation[i] != ' ') {
        roman1[j++] = operation[i++];
    }
    roman1[j] = '\0';
    
    // Get operator and skip spaces
    i++;
    op = operation[i++];
    i++;
    
    // Parse second Roman numeral
    j = 0;
    while (operation[i] != '\0' && operation[i] != '\n') {
        roman2[j++] = operation[i++];
    }
    roman2[j] = '\0';
 
    // Convert Roman numerals to decimal
    int decimal1 = romanToDecimal(roman1);
    int decimal2 = romanToDecimal(roman2);
    int operationResult;
 
    // Perform arithmetic based on operator
    switch (op) {
        case '+': 
            operationResult = decimal1 + decimal2;
            break;
        case '-': 
            operationResult = decimal2 - decimal1;
            break;
        case '*': 
            operationResult = (decimal1 * decimal2);
            break;
        case '/': 
            // Handle division by zero error
            if (decimal2 != 0) {
                operationResult = decimal1 / decimal2;
            } else {
                int i = 0;
                char error[] = "Division by zero error.";
                while (error[i] != '\0') {
                    result[i] = error[i];
                    i++;
                }
                result[i] = '\0';
                return;
            }
            break;
    }
 
    // Convert result to word representation
    numberToWords(operationResult < 0 ? -operationResult : operationResult, result);
}
 
// Removes trailing spaces from string
void removeTrailingSpaces(char str[]) {
    int len = stringLength(str) - 1;
    while (len >= 0 && (str[len] == ' ' || str[len] == '\t')) {
        str[len] = '\0';
        len--;
    }
}
 
// Converts numerical value to English word representation
void numberToWords(int num, char word[]) {
    // Arrays for word representations of numbers
    char *units[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    char *thousands[] = {"", "Thousand", "Million"};
 
    // Temporary storage for building result
    char temp[500] = "";
    int part, place = 0;
 
    // Handle zero case
    if (num == 0) {
        stringCopy(word, "Zero");
        return;
    }
 
    // Handle negative numbers
    if (num < 0) {
        stringConcat(temp, "Negative ");
        num = -num;
    }
 
    // Process number in groups of three digits
    while (num > 0) {
        part = num % 1000;
        if (part != 0) {
            char partWord[100] = "";
 
            // Process hundreds place
            if (part / 100 > 0) {
                stringConcat(partWord, units[part / 100]);
                stringConcat(partWord, " Hundred ");
            }
 
            // Process tens and ones places
            part %= 100;
            if (part >= 10 && part <= 19) {
                stringConcat(partWord, teens[part - 10]);
            } else {
                if (part / 10 > 0) {
                    stringConcat(partWord, tens[part / 10]);
                    stringConcat(partWord, " ");
                }
                if (part % 10 > 0) {
                    stringConcat(partWord, units[part % 10]);
                }
            }
 
            // Add place value words
            if (place > 0) {
                stringConcat(partWord, " ");
                stringConcat(partWord, thousands[place]);
                stringConcat(partWord, " ");
            }
 
            // Combine parts
            stringConcat(partWord, temp);
            stringCopy(temp, partWord);
        }
 
        num /= 1000;
        place++;
    }
 
    // Clean up result and store final string
    removeTrailingSpaces(temp);
    stringCopy(word, temp);
}

// Main function - reads operations from input.txt and writes results to output.txt
int main() {
    FILE *inputFile, *outputFile;
    char operation[100], result[500];
 
    // Open input and output files
    inputFile = fopen("input.txt", "r");
    outputFile = fopen("output.txt", "w");
 
    // Check for file opening errors
    if (inputFile == NULL || outputFile == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
 
    // Process each line from input file
    while (fgets(operation, sizeof(operation), inputFile)) {
        performOperation(operation, result);
        fprintf(outputFile, "%s\n", result);
    }
 
    // Close files and display completion message
    fclose(inputFile);
    fclose(outputFile);
 
    printf("Processing completed. Check Output.txt for results.\n");
    return 0;
}