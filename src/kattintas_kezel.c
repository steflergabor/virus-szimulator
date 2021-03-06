#include "kattintas_kezel.h"

/* Itt csak olyan függvények találhatóak amik a különféle kattintásokat kezelik
 * */

extern int W, H;
extern int EX, EY;
extern bool beallit;
extern Bevitel bevitel;

/* Jobb felül a kilépés gomb */
void kilepes(bool* quit){
    *quit = katt(EX, EY, W - 25, W - 5, 0, H * SAV_SZEL);
}

/* Az adott szimuláció beállításának kattintása*/
void beall(Szim* const szim, bool* stop, int* melyiket){
    for (int i = 0; i < SZIMSZAM; ++i) {
        int x = realtoint((W / 4 - W * TART_SZEL) / 2.0) - 50;
        int y = H * SAV_SZEL + 50 + i * (W * TART_SZEL + 50);
        if (szim[i].all &&katt(EX, EY, x,x + 100, y, y + 30)){
            *stop = true;
            beallit = true;
            *melyiket = i;
        }
    }
}

/* Szimuláció törlése gomb */
void torol(Szim* szim){
    for (int i = 0; i < SZIMSZAM; ++i) {
        int x = realtoint((W / 4 - W * TART_SZEL) / 2.0) - 50;
        int y = H * SAV_SZEL + 50 + i * (W * TART_SZEL + 50) + 100;
        if (szim[i].all && katt(EX, EY, x,x + 100, y, y + 30)){
            szim[i].all = false;
        }
    }
}

/* Szimuláció újraindítása gomb */
void ujraindit(Szim* szim){
    for (int i = 0; i < SZIMSZAM; ++i) {
        int x = realtoint((W / 4 - W * TART_SZEL) / 2.0) - 50;
        int y = H * SAV_SZEL + 50 + i * (W * TART_SZEL + 50) + 50;
        if (szim[i].all && katt(EX, EY, x,x + 100, y, y + 30)){
            nepvaltozat(&szim[i]);
        }
    }
}

/* Új szimuláció hozzáadása gomb */
void uj(Szim* const szim, bool* stop, int* melyik){
    for (int i = 0; i < SZIMSZAM; ++i) {
        int x = W / 4 - 30;
        int y = H * SAV_SZEL + 50 + i * (W * TART_SZEL + 50) + (W * TART_SZEL) / 2 - 30;
        if (!szim[i].all && katt(EX, EY, x, x + 60, y, y + 60)){
            beallit = true;
            *melyik = i;
            *stop = true;
        }
    }
}

/* Beállításokban az adott beviteli mezőkre kattintás kezelése */
void bevitel_valaszt(void){
    int x = W / 6 * 4;
    int y = 200;
    if (katt(EX, EY, W / 2 + 140, x - 30, y + 195, y + 230))
        bevitel = Nepesseg;
    else if (katt(EX, EY, W / 2 + 140, x - 30, y + 270, y + 305))
        bevitel = Sugar;
    else if (katt(EX, EY, W / 2 + 140, x - 30, y + 345, y + 380))
        bevitel = Szazalek;
    else if (katt(EX, EY, W / 2 + 140, x - 30, y + 420, y + 455))
        bevitel = Ido;
    else
        bevitel = Semmi;
}

/* Beállításokban a mégse gombra kattintás
 * Ez nem fogja resetelni az eddig bevitt mezőket*/
void megse(void){
    int x2 = W / 6 * 4;
    int y1 = 200;
    int y2 = H - y1;
    if (katt(EX, EY, x2 - 20 - 100, x2 - 20, y2 - 60, y2 - 20)){
        beallit = false;
    }
}

/* Segégdfüggvény ami megkeresi a hibákat a bevitt értékekben
 * és egy Error típussal tér vissza
 * */
static Error hiba_keres(Bevstring const bevstring){
    //minimum kényszerek
    int nep_min = 20;
    int sug_min = R + 1;
    int szaz_min = 1;
    int ido_min = 1;
    int hibak_szama = 0;

    Error hiba;

    if (bevstring.nep[0] == '\0' || atoi(bevstring.nep) < nep_min)
        hiba.hibak[hibak_szama++] = Nepesseg;
    if (bevstring.sug[0] == '\0' || atoi(bevstring.sug) < sug_min)
        hiba.hibak[hibak_szama++] = Sugar;
    if (bevstring.szaz[0] == '\0' || atoi(bevstring.szaz) < szaz_min)
        hiba.hibak[hibak_szama++] = Szazalek;
    if (bevstring.ido[0] == '\0' || atoi(bevstring.ido) < ido_min)
        hiba.hibak[hibak_szama++] = Ido;

    hiba.hibak_szama = hibak_szama;
    return hiba;
}

/* Beállításokban az indítás gomb kezelése
 * Megkapja a szimulációk tömbjét, hogy melyiket módosítsa, a beviteli stringet
 * ami tartalmazza az adatokat, a megállás állapotát jelző változót és a hiba típusú hibaváltozót
 * */
void inditas(Szim* szim, int melyik, Bevstring* bev, bool* stop, Error* hiba){
   int x = W / 6 * 2;
   int y = H - 200;
   *hiba = hiba_keres(*bev);
   if (katt(EX, EY, x + 20, x + 20 + 200, y - 60, y - 20) && !hiba->hibak_szama){
       szim[melyik].all = true;
       szim[melyik].nepmeret = atoi(bev->nep);
       szim[melyik].virus.r = atoi(bev->sug);
       szim[melyik].virus.p = atoi(bev->szaz) / 100.0;
       szim[melyik].virus.recover = atoi(bev->ido);
       nepvaltozat(&szim[melyik]);
       beallit = false;
       *stop = false;
       strcpy(bev->nep, "\0");
       strcpy(bev->sug, "\0");
       strcpy(bev->szaz, "\0");
       strcpy(bev->ido, "\0");
   }
   else if (katt(EX, EY, x + 20, x + 20 + 200, y - 60, y - 20) && hiba->hibak_szama)
       bevitel = Hiba;
}

/* Az svg export gomb megnyomása
 * */
void svg_export_katt(Szim* const szim, int* szamlalo){
    for (int i = 0; i < SZIMSZAM; ++i) {
        int x = realtoint((W / 4 - W * TART_SZEL) / 2.0) - 50;
        int y = H * SAV_SZEL + 50 + i * (W * TART_SZEL + 50) + 150;
        if (szim[i].all && szim[i].graf->utso->fert == 0 && katt(EX, EY, x, x + 100, y, y + 30)){
            SVG_export(szim, i, szamlalo);
        }
    }
}