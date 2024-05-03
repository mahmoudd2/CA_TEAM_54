#include <stdio.h>
#include <stdlib.h>

// char *int_to_binary(int num, int num_bits, const char *type) {
//     // Allocate memory for the binary string
//     char *binary = (char *)malloc((num_bits + 1) * sizeof(char)); // +1 for the null terminator
//     if (binary == NULL) {
//         fprintf(stderr, "Memory allocation failed\n");
//         exit(1);
//     }

//     // Convert the integer to binary based on the type
//     if (strcmp(type, "IMM") == 0) { // 2's complement handling
//         if (num < 0) { // If negative, convert to 2's complement
//             num += (1 << num_bits);
//         }
//         for (int i = num_bits - 1; i >= 0; i--) {
//             if (num & (1 << i)) {
//                 binary[num_bits - 1 - i] = '1'; // Set the bit to '1' if it's set in num
//             } else {
//                 binary[num_bits - 1 - i] = '0'; // Set the bit to '0' if it's not set in num
//             }
//         }
//     } else { // Normal positive number handling
//         for (int i = num_bits - 1; i >= 0; i--) {
//             if (num & (1 << i)) {
//                 binary[num_bits - 1 - i] = '1'; // Set the bit to '1' if it's set in num
//             } else {
//                 binary[num_bits - 1 - i] = '0'; // Set the bit to '0' if it's not set in num
//             }
//         }
//     }

//     binary[num_bits] = '\0'; // Null-terminate the string

//     return binary;
// }

int main() {
    // Test the function with different types
    int num = 5; // Example 2's complement integer
    char * binary = (char*)(num & (1 << 3));
    printf("%s",binary);
    return 0;
}
