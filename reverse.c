#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void tulosta_virhe_ja_poistu(const char *virheviesti) {
    fprintf(stderr, "%s\n", virheviesti);
    exit(1);
}

void kaanna_rivit(FILE *syote, FILE *tuloste) {
    char *rivi = NULL;
    size_t pituus = 0;
    char **rivit = NULL;
    size_t maara = 0;

    while (getline(&rivi, &pituus, syote) != -1) {
        char **uudet_rivit = realloc(rivit, (maara + 1) * sizeof(char *));
        if (!uudet_rivit) {
            free(rivi);
            free(rivit);
            tulosta_virhe_ja_poistu("malloc failed");
        }
        rivit = uudet_rivit;
        rivit[maara++] = strdup(rivi);
        if (!rivit[maara - 1]) {
            free(rivi);
            free(rivit);
            tulosta_virhe_ja_poistu("malloc failed");
        }
    }
    free(rivi);

    for (ssize_t i = maara - 1; i >= 0; i--) {
        fprintf(tuloste, "%s", rivit[i]);
        free(rivit[i]);
    }
    free(rivit);
}

FILE *avaa_tiedosto(const char *tiedostonimi, const char *tila) {
    FILE *tiedosto = fopen(tiedostonimi, tila);
    if (!tiedosto) {
        fprintf(stderr, "error: cannot open file '%s'\n", tiedostonimi);
        exit(1);
    }
    return tiedosto;
}

int main(int argc, char *argv[]) {
    FILE *syote = stdin;
    FILE *tuloste = stdout;

    if (argc > 3) {
        tulosta_virhe_ja_poistu("usage: reverse <input> <output>");
    }

    if (argc >= 2) {
        syote = avaa_tiedosto(argv[1], "r");
    }

    if (argc == 3) {
        if (strcmp(argv[1], argv[2]) == 0) {
            tulosta_virhe_ja_poistu("Input and output file must differ");
        }
        tuloste = avaa_tiedosto(argv[2], "w");
    }

    kaanna_rivit(syote, tuloste);

    if (syote != stdin) fclose(syote);
    if (tuloste != stdout) fclose(tuloste);

    return 0;
}
