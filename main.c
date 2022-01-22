#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "words.h"

#define LETTER_MASK(c) (1U << ((uint32_t)((c) - 'a')))

void printUsage() {
    printf("%s\n",
           "wordbee - an answer generator for the NYTimes Spelling Bee game\n\n"
           "usage: wordbee [letters starting with the central letter]\n\n"
           "For example, if the puzzle is TLCHAE with the central letter O:\n\n"
           "    wordbee OTLCHAE\n");
}

int compareWords(const void* a, const void* b) {
    int lena = strlen((*(const word**)a)->w);
    int lenb = strlen((*(const word**)b)->w);
    return lenb - lena;
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printUsage();
        return EXIT_FAILURE;
    }

    uint32_t m = 0;
    uint32_t required = 0;

    const char* letters = argv[1];
    const int lsize = strlen(argv[1]);
    if (lsize != 7) {
        fprintf(stderr, "Expected 7 characters, got %d\n", lsize);
        return EXIT_FAILURE;
    }

    for (const char* lp = letters; *lp != '\0'; lp++) {
        char c = *lp;
        if (islower(c)) {
            m |= LETTER_MASK(c);
            required = required == 0 ? LETTER_MASK(c) : required;
        } else if (isupper(c)) {
            m |= LETTER_MASK(tolower(c));
            required = required == 0 ? LETTER_MASK(tolower(c)) : required;
        } else {
            fprintf(stderr, "Invalid character: %c\n", c);
            return EXIT_FAILURE;
        }
    }

    int wc = 0;
    int cap = 1024;
    word** results = calloc(cap, sizeof(word*));

    for (int i = 0; i < wordCount; i++) {
        word* w = &words[i];
        if ((w->mask & ~m) != 0) {
            continue;
        } else if ((w->mask & required) == 0) {
            continue;
        }
        results[wc++] = w;

        if (wc == cap) {
            cap *= 2;
            results = realloc(results, cap * sizeof(word*));
            assert(results != NULL);
        }
    }

    qsort(results, wc, sizeof(word*), compareWords);

    for (int i = 0; i < wc; i++) {
        printf("%s\n", results[i]->w);
    }

    free(results);

    return EXIT_SUCCESS;
}
