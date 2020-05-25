// ==============================
// So I just learned about gcc optimization flags...
// This entire file is useless. Optimization is slightly faster + more accurate

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

struct FuncTable {
    float* vals;
    int len;
    float range;
};

float sig(float x) {
    return 1 / (1 + powf(M_E, -x));
}

/**
 * Linearly samples funct len times across inclusive bounds [-range,range].
 * Stores results in table for fast lookup.
 */
struct FuncTable* allocFuncTable(float (*funct)(float), int len, float range) {
    struct FuncTable* table = malloc(sizeof(struct FuncTable));
    table->len = len;
    table->range = range;

    table->vals = malloc(sizeof(float) * len);
    for (int i = 0; i < len; ++i) {
        table->vals[i] = (*funct)(-range + 2 * range * ((float)i / (len - 1)));
    }
    return table;
}

void freeFuncTable(struct FuncTable* t) {
    free(t->vals);
    free(t);
}

float funcTableLookup(struct FuncTable* table, float x) {
    float r = table->range;
    if (x < -r) {
        x = -r;
    } else if (x > r) {
        return x = r;
    }
    return table->vals[(int)((table->len - 1) * (x + r) / (2 * r) + 0.5f)];
}

int main() {
    clock_t start, end;
    struct FuncTable* sigTable = allocFuncTable(sig, 1024, 2);
    start = clock();
    for (float i = -2; i <= 2; i += (float)1 / 100000) {
        funcTableLookup(sigTable, i);
    }
    end = clock();
    printf("%ld\n", end - start);
    start = clock();
    for (float i = -2; i <= 2; i += (float)1 / 100000) {
        float ans = sig(i);
    }
    end = clock();
    printf("%ld\n", end - start);
    freeFuncTable(sigTable);
    return 0;
}