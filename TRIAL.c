#include <stdio.h>
#include <stdlib.h>

char *int_to_binary(int num, int num_bits, const char *type) {
    // Allocate memory for the binary string
    char *binary = (char *)malloc((num_bits + 1) * sizeof(char)); // +1 for the null terminator
    if (binary == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    // Convert the integer to binary based on the type
    if (strcmp(type, "IMM") == 0) { // 2's complement handling
        if (num < 0) { // If negative, convert to 2's complement
            num += (1 << num_bits);
        }
        for (int i = num_bits - 1; i >= 0; i--) {
            if (num & (1 << i)) {
                binary[num_bits - 1 - i] = '1'; // Set the bit to '1' if it's set in num
            } else {
                binary[num_bits - 1 - i] = '0'; // Set the bit to '0' if it's not set in num
            }
        }
    } else { // Normal positive number handling
        for (int i = num_bits - 1; i >= 0; i--) {
            if (num & (1 << i)) {
                binary[num_bits - 1 - i] = '1'; // Set the bit to '1' if it's set in num
            } else {
                binary[num_bits - 1 - i] = '0'; // Set the bit to '0' if it's not set in num
            }
        }
    }

    binary[num_bits] = '\0'; // Null-terminate the string

    return binary;
}

int main() {
    // Test the function with different types
    int num = -5; // Example 2's complement integer
    int num_bits = 4; // Example number of bits

    char *binary_str = int_to_binary(num, num_bits, "IMM");
    printf("Binary representation of %d (type IMM): %s\n", num, binary_str);
    free(binary_str);

    num = 5; // Example positive integer
    binary_str = int_to_binary(num, num_bits, "POS");
    printf("Binary representation of %d (type POS): %s\n", num, binary_str);
    free(binary_str);

    return 0;
}
