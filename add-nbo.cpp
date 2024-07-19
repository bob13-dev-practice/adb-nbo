#include <stdint.h>
#include <iostream>

uint32_t ntohl(uint32_t n) {
    uint32_t n1 = n & 0xFF000000;
    uint32_t n2 = n & 0x00FF0000;
    uint32_t n3 = n & 0x0000FF00;
    uint32_t n4 = n & 0x000000FF;

    n1 >>= 24;
    n2 >>= 8;
    n3 <<= 8;
    n4 <<= 24;

    return n1 | n2 | n3 | n4;
}

uint32_t readBinaryFile(const char* filename) {
    FILE* file = fopen(filename, "rb");
    if (!file) {
        printf("Can't open this file: %s\n", filename);
        exit(EXIT_FAILURE);
    }

    uint32_t p;
    uint32_t operand;

    size_t ret = fread(&p, sizeof(uint32_t), 1, file);
    if (ret == 1) {
        operand = ntohl(p);
    } else {
        if (feof(file)) {
            printf("The file doesn't contain enough data.\n");
        } else if (ferror(file)) {
            printf("Failed to read data.\n");
        }
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    return operand;
}

int main(int argc, char** argv) {
    if (argc < 3) {
        printf("Usage: %s <filename1> <filename2>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    uint32_t operand1, operand2;
    uint32_t result;

    operand1 = readBinaryFile(argv[1]);
    operand2 = readBinaryFile(argv[2]);

    result = operand1 + operand2;
    printf("%d(0x%x) + %d(0x%x) = %d(0x%x)\n", operand1, operand1, operand2, operand2, result, result);

    return 0;
}
