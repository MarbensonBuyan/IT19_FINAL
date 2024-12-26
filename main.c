#include <stdio.h>
 
// Function prototypes
int romanToDecimal(char roman[]);
void performOperation(char operation[], char result[]);
void numberToWords(int num, char word[]);
int stringLength(const char str[]);
void stringCopy(char dest[], const char src[]);
void stringConcat(char dest[], const char src[]);
 
// Main function
int main() {
    FILE *inputFile, *outputFile;
    char operation[100], result[500];
 
    // Open files
    inputFile = fopen("input.txt", "r");
    outputFile = fopen("output.txt", "w");
 
    if (inputFile == NULL || outputFile == NULL) {
        printf("Error opening file!\n");
        return 1;
    }
 
    // Process each line in the input file
    while (fgets(operation, sizeof(operation), inputFile)) {
        performOperation(operation, result);
        fprintf(outputFile, "%s\n", result);
    }
 
    fclose(inputFile);
    fclose(outputFile);
 
    printf("Processing completed. Check Output.txt for results.\n");
    return 0;
}
 
// Function to calculate string length
int stringLength(const char str[]) {
    int len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}
 
// Function to copy one string to another
void stringCopy(char dest[], const char src[]) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}
 
// Function to concatenate two strings
void stringConcat(char dest[], const char src[]) {
    int destLen = stringLength(dest);
    int i = 0;
    while (src[i] != '\0') {
        dest[destLen + i] = src[i];
        i++;
    }
    dest[destLen + i] = '\0';
}
 
// Function to convert Roman numeral to decimal
int romanToDecimal(char roman[]) {
    int value = 0;
    int romanValues[26] = {0};
 
    // Assign values to Roman characters
    romanValues['I' - 'A'] = 1;
    romanValues['V' - 'A'] = 5;
    romanValues['X' - 'A'] = 10;
    romanValues['L' - 'A'] = 50;
    romanValues['C' - 'A'] = 100;
    romanValues['D' - 'A'] = 500;
    romanValues['M' - 'A'] = 1000;
 
    for (int i = 0; i < stringLength(roman); i++) {
        if (i + 1 < stringLength(roman) && romanValues[roman[i] - 'A'] < romanValues[roman[i + 1] - 'A']) {
            value -= romanValues[roman[i] - 'A'];
        } else {
            value += romanValues[roman[i] - 'A'];
        }
    }
 
    return value;
}
 
void performOperation(char operation[], char result[]) {
    char roman1[50], roman2[50], op;
    int i = 0, j = 0;
    
    // Parse input manually
    while (operation[i] != ' ') {
        roman1[j++] = operation[i++];
    }
    roman1[j] = '\0';
    
    i++; // Skip space
    op = operation[i++];
    i++; // Skip space
    
    j = 0;
    while (operation[i] != '\0' && operation[i] != '\n') {
        roman2[j++] = operation[i++];
    }
    roman2[j] = '\0';
 
    int decimal1 = romanToDecimal(roman1);
    int decimal2 = romanToDecimal(roman2);
    int operationResult;
 
    switch (op) {
        case '+': 
            operationResult = decimal1 + decimal2;
            break;
        case '-': 
            operationResult = decimal2 - decimal1;  // Fixed order for subtraction
            break;
        case '*': 
            operationResult = (decimal1 * decimal2);  // Scale down multiplication
            break;
        case '/': 
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
 
    numberToWords(operationResult < 0 ? -operationResult : operationResult, result);
}
 
// Helper function to remove trailing spaces from a string
void removeTrailingSpaces(char str[]) {
    int len = stringLength(str) - 1;
    while (len >= 0 && (str[len] == ' ' || str[len] == '\t')) {
        str[len] = '\0';
        len--;
    }
}
 
// Function to convert a number to its word representation
// Function to convert a number to its word representation
void numberToWords(int num, char word[]) {
    char *units[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    char *thousands[] = {"", "Thousand", "Million"};
 
    char temp[500] = "";
    int part, place = 0;
 
    // Handle zero
    if (num == 0) {
        stringCopy(word, "Zero");
        return;
    }
 
    // Handle negative
    if (num < 0) {
        stringConcat(temp, "Negative ");
        num = -num;
    }
 
    // Build the word representation in chunks of three digits
    while (num > 0) {
        part = num % 1000;
        if (part != 0) {
            char partWord[100] = "";
 
            // Handle hundreds
            if (part / 100 > 0) {
                stringConcat(partWord, units[part / 100]);
                stringConcat(partWord, " Hundred ");
            }
 
            // Handle tens and units
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
 
            // Add thousands place if needed
            if (place > 0) {
                stringConcat(partWord, " ");
                stringConcat(partWord, thousands[place]);
                stringConcat(partWord, " ");
            }
 
            // Insert current part at the front
            stringConcat(partWord, temp);
            stringCopy(temp, partWord);
        }
 
        num /= 1000;
        place++;
    }
 
    // Remove any trailing spaces
    removeTrailingSpaces(temp);
 
    // Copy final result
    stringCopy(word, temp);
}