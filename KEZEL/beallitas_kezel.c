#include "beallitas_kezel.h"

extern Bevitel bevitel;
extern Betutipus *betutipus;

char billentyutochar(Uint32 const be){
    if (be == SDLK_0 || be == SDLK_KP_0)
        return '0';
    if (be == SDLK_1 || be == SDLK_KP_1)
        return '1';
    if (be == SDLK_2 || be == SDLK_KP_2)
        return '2';
    if (be == SDLK_3 || be == SDLK_KP_3)
        return '3';
    if (be == SDLK_4 || be == SDLK_KP_4)
        return '4';
    if (be == SDLK_5 || be == SDLK_KP_5)
        return '5';
    if (be == SDLK_6 || be == SDLK_KP_6)
        return '6';
    if (be == SDLK_7 || be == SDLK_KP_7)
        return '7';
    if (be == SDLK_8 || be == SDLK_KP_8)
        return '8';
    if (be == SDLK_9 || be == SDLK_KP_9)
        return '9';
}

static void str_masol(char *ebbe, char* const ezt){
    size_t i;
    for (i = 0; ezt[i] != '\0'; ++i)
        ebbe[i] = ezt[i];
    ebbe[i] = '\0';
}

static void egy_karakter_hozzafuz(char* ehhez, char const ezt){
    ehhez[strlen(ehhez)] = ezt;
    ehhez[strlen(ehhez) + 1] = '\0';
}

static void str_eggyel_csokkent(char* be){
    if (strlen(be) <= 0)
        return;
    be[strlen(be) - 1] = '\0';
}

static void szam_bevisz(char* ebbe, Uint32 const szam_be, int const kenyszer){
    if (szam_be == SDLK_BACKSPACE){
        //levágjuk a legutsó elemet
        str_eggyel_csokkent(ebbe);
    }
    else{
        //tesztelő string
        char tesztelo[5 + 1];
        tesztelo[0] = '\0';
        //tesztelőbe belemásoljuk a beviteli stringet
        str_masol(tesztelo, ebbe);
        //hozzáfűzűkk a tesztelőhöz a bevitt számot
        egy_karakter_hozzafuz(tesztelo, billentyutochar(szam_be));
        //ha teljesöl a kényszer akkor beleírjuk a beviteli stringbe
        if (atoi(tesztelo) <= kenyszer){
            str_masol(ebbe, tesztelo);
        }
        else {
            ebbe[0] = '\0';
        }
    }
}

void bevisz(Szim* szim, Bevstring* bevstring, Uint32 szam_be){
    //kényszerek az értékekre
    int nep_max = 2000;
    int sug_max = 50;
    int szaz_max = 100;
    int ido_max = 100;
    //***********************

    //A beállításokban az enter gomb kezelése
    if (szam_be == SDLK_RETURN){
        switch (bevitel) {
            case Nepesseg:
                bevitel = Sugar;
                break;
            case Sugar:
                bevitel = Szazalek;
                break;
            case Szazalek:
                bevitel = Ido;
                break;
            case Ido:
                bevitel = Semmi;
                break;
        }
        return;
    }

    switch (bevitel) {
        case Nepesseg:
                szam_bevisz(bevstring->nep, szam_be, nep_max);
            break;
        case Sugar:
                szam_bevisz(bevstring->sug, szam_be, sug_max);
            break;
        case Szazalek:
                szam_bevisz(bevstring->szaz, szam_be, szaz_max);
            break;
        case Ido:
                szam_bevisz(bevstring->ido, szam_be, ido_max);
            break;
    }
}