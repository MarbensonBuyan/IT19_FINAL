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
    inputFile = fopen("Input.txt", "r");
    outputFile = fopen("Output.txt", "w");

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

// Function to perform arithmetic operations
void performOperation(char operation[], char result[]) {
    char roman1[50], roman2[50], op;
    int decimal1, decimal2, operationResult;

    // Parse input line (e.g., "MCMXCIV + MMXXIV")
    sscanf(operation, "%s %c %s", roman1, &op, roman2);

    decimal1 = romanToDecimal(roman1);
    decimal2 = romanToDecimal(roman2);

    // Perform the operation
    switch (op) {
        case '+':
            operationResult = decimal1 + decimal2;
            break;
        case '-':
            operationResult = decimal1 - decimal2;
            break;
        case '*':
            operationResult = decimal1 * decimal2;
            break;
        case '/':
            if (decimal2 != 0) {
                operationResult = decimal1 / decimal2;
            } else {
                stringCopy(result, "Division by zero error.");
                return;
            }
            break;
        default:
            stringCopy(result, "Invalid operator.");
            return;
    }

    // Convert the result to words
    numberToWords(operationResult, result);
}

// Function to convert a number to its word representation
void numberToWords(int num, char word[]) {
    // Define arrays for word mapping
    char *units[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    char *teens[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    char *tens[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    char *thousands[] = {"", "Thousand", "Million"};

    char temp[500] = "";
    int part, place = 0;

    if (num == 0) {
        stringCopy(word, "Zero");
        return;
    }

    if (num < 0) {
        stringConcat(temp, "Negative ");
        num = -num;
    }

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

            // Add thousands place
            if (place > 0) {
                stringConcat(partWord, " ");
                stringConcat(partWord, thousands[place]);
                stringConcat(partWord, " ");
            }

            stringConcat(partWord, temp);
            stringCopy(temp, partWord);
        }

        num /= 1000;
        place++;
    }

    stringCopy(word, temp);
}
