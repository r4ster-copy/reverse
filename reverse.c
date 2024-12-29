//Otsikkotiedot
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Apufunktio, joka tulostaa virheviestin ja lopettaa ohjelman
void tulosta_virhe_ja_poistu(const char *virheviesti) {
    fprintf(stderr, "%s\n", virheviesti);
    exit(1);
}

// Funktio, joka kääntää rivit käänteiseen järjestykseen
void kaanna_rivit(FILE *syote, FILE *tuloste) {
    char *rivi = NULL;
    size_t pituus = 0;
    char **rivit = NULL;
    size_t maara = 0;
    // Lue kaikki rivit syötteestä
    while (getline(&rivi, &pituus, syote) != -1) {
        // Kasvata taulukon kokoa yhdellä rivillä
        char **uudet_rivit = realloc(rivit, (maara + 1) * sizeof(char *));
        if (!uudet_rivit) {
            free(rivi);
            free(rivit);
            tulosta_virhe_ja_poistu("malloc failed");
        }
        rivit = uudet_rivit;

	// Kopioi rivi taulukkoon
        rivit[maara++] = strdup(rivi);
        if (!rivit[maara - 1]) {
            free(rivi);
            free(rivit);
            tulosta_virhe_ja_poistu("malloc failed");
        }
    }
    free(rivi); //Vapauta getline:n varaama muisti

    //Tulosta rivit käänteisessä järjestyksessä
    for (ssize_t i = maara - 1; i >= 0; i--) {
        fprintf(tuloste, "%s", rivit[i]);
        free(rivit[i]); //Vapauta yksittäisen rivin muisti
    }
    free(rivit); //Vapauta taulukon osoittimen muisti
}

//Apufunktio, joka avaa tiedoston ja tarkistaa mahdolliset virheet
FILE *avaa_tiedosto(const char *tiedostonimi, const char *tila) {
    FILE *tiedosto = fopen(tiedostonimi, tila);
    if (!tiedosto) {
        fprintf(stderr, "error: cannot open file '%s'\n", tiedostonimi);
        exit(1);
    }
    return tiedosto;
}

//Pääohjelma, joka määrittää toimintatavan ja käsittelee syötteen
int main(int argc, char *argv[]) {
    FILE *syote = stdin; //Standardisyöte
    FILE *tuloste = stdout; //Standardituloste

    // Tarkista argumenttien määrä (jos yli kolme, niin tulosta virhe ja poistu)
    if (argc > 3) {
        tulosta_virhe_ja_poistu("usage: reverse <input> <output>");
    }

    // Avaa syötetiedosto, jos argumentteja vähintään 1
    if (argc >= 2) {
        syote = avaa_tiedosto(argv[1], "r");
    }

    //Avaa tulostetiedosto, jos argumentteja on 2
    if (argc == 3) {
        if (strcmp(argv[1], argv[2]) == 0) {
            tulosta_virhe_ja_poistu("Input and output file must differ");
        }
        tuloste = avaa_tiedosto(argv[2], "w");
    }

    // Käännä rivit ja tulosta
    kaanna_rivit(syote, tuloste);

    // Sulje tiedostot, jos ne eivät ole stdin tai stdout
    if (syote != stdin) fclose(syote);
    if (tuloste != stdout) fclose(tuloste);

    return 0;
}
