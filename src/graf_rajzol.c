#include "graf_rajzol.h"

extern int W, H;
extern Betutipus *betutipus;

SDL_Color vilagos = {220, 220, 220};

/* Visszatérési értéke egy Inteket tartalmazó tömb amik a grafikon görbéjének
 * koordinátáit tartalmazza, a Melyik paraméterrel tudjuk kiválasztani, hogy
 * a fertőzöttek vagy gyógyultak y vagy (mivel mindkettőnek ugyan az az x) az x
 * koordinátáit akarjuk belerekani, ezen kívűl megkap több olyan paramétert is
 * ami a grafikon képernyőbeli helyének a kiszámításához kell
 * arany: a grafikon y tengelyének a magassága / népesség mérete
 * novelo: a besztások közötti távolság
 * x: a képernyőn az a hely ahol a grafikonban x = 0(ahol az y tengely van)
 * y: -----------------""--------------------- x = 0 és y = 0
 * y1: ------------------""------------------- x = 0 és y = max
 * */
Sint16* belerak(Grafikon* const graf, enum Melyik melyik, double const arany, int const szeleseg, double const novelo, int const x, int const y, int const y1){
    int meret = graf->meret;
    Sint16* tmp = (Sint16*)malloc(sizeof(Sint16) * (graf->meret + 2));

    GrafLista* fedex;
    int szamlalo = 1;
    /* Bejárjuk a láncolt listát
     * */
    for (fedex = graf->elso; fedex != NULL; fedex = fedex->kov)
    {
        //Az arányoknak megfelelő koordinátákat rakunk be
        if (melyik == Fert){
            tmp[szamlalo++] = y - (Sint16)(fedex->fert * arany);
        }
        else if (melyik == Gyogy){
            tmp[szamlalo++] = y1 + (Sint16)(fedex->gogyult * arany);
        }
        else if (melyik == X_koo){
            tmp[szamlalo++] = x + (Sint16)(novelo * szamlalo);
        }
    }
    /* Az első és az utolsó helyre pedig berakjuk az y = 0
     * Hogy a grafikonunk teljes legyen
     * */
    if (melyik == Fert)
    {
        tmp[0] = y;
        tmp[meret+1] = y;
    }

    else if (melyik == Gyogy){
        tmp[0] = y1;
        tmp[meret+1] = y1;
    }

    /* Itt pedig az első helyre az x = 0
     * az utolsóra pedig az x = max-ot
     * */
    else if (melyik == X_koo)
    {
        tmp[0] = x;
        tmp[meret+1] = x + szeleseg;
    }
    return tmp;
}

/* Kirajzolja a grafikont a képernyőre
 * A fentebb látott belerak fv-hez hasonlóan majdnem ugyan azokat a paramétereket kapja
 * hiszen ez a függvény hívja majd meg a 'belerak'ot
 * */
static void gorbe_rajzol(SDL_Renderer* renderer, Grafikon* const graf, int const nepmeret, int const x, int const y, int const y1){
    //Kiszámítja a 'belerak'nak fontos értékeket
    double novelo = (double)(W * TART_SZEL * 2) / graf->meret;
    double arany = W * TART_SZEL / (double)nepmeret;

    //Feltölti a koordináták tömbjét a belerak fv segítségével
    Sint16* f_y = belerak(graf, Fert, arany, realtoint(W * TART_SZEL * 2), novelo, x, y, y1);
    Sint16* gy_y = belerak(graf, Gyogy, arany, realtoint(W * TART_SZEL * 2), novelo, x, y, y1);
    Sint16* f_gy_x = belerak(graf, X_koo, arany, realtoint(W * TART_SZEL * 2), novelo, x, y, y1);

    //Kirajoljuk a görbét
    filledPolygonRGBA(renderer, f_gy_x, f_y, graf->meret + 2, 255, 50, 50, 255);
    filledPolygonRGBA(renderer, f_gy_x, gy_y, graf->meret + 2, 50, 255, 50, 255);

    //Takarítunk
    free(f_y);
    free(gy_y);
    free(f_gy_x);
}

/*
Ez a függvény rajzolja ki a kis beosztás vonalakat
Úgy van tervezve, hogy másodpercenként hívódjon meg
És akkor a beosztások a másodperceket jelölik
*/
static void beosztas_rajzol(SDL_Renderer* renderer, int const meret, int const x, int const y){
    //változók a vonal pozíciójának
    int x1, y1, y2;
    //az elhelyezkedés igénye szerinti értékadás
    y1 = y - 3;
    y2 = y + 3;
    x1 = x;

    //ez a távolság két bosztási vonal között, dinamikusan változik az idő elteltével
    double novelo = (double)(W * TART_SZEL * 2) / meret;

    /*
    végigmegyünk a másodperceken és kirajzoljuk a megfelelő mennyiségű volnalat
    "novelo" távolságonként
     */
    for (int i = 1; i <= meret; i++)
    {
        double hol = x1 + novelo * i;
        lineRGBA(renderer, hol, y1, hol, y2, 255, 255, 255, 255);
    }
}

/* Kiírja hogy mennyi idő telt el a grafikon x egyenesének a végén
 * */
static void ido_kiir(SDL_Renderer* renderer, Szim* const szim, int i, int x_x1, int x_y1){
    int x = x_x1 + (W * TART_SZEL * 2) - 20;
    int y = x_y1 + 10;
    char ido[9];
    sprintf(ido, "t: %d", szim[i].graf->meret);
    kiir(renderer, betutipus[1].tipus, vilagos, ido, x, y);
}

/* Kiírja az adott grafikon népességének a méretét
 * */
static void meret_kiir(SDL_Renderer* renderer, Szim* const szim, int i){
    int x = W / 2 + 80;
    int y = H * SAV_SZEL + 25 + i * (W * TART_SZEL + 50);
    char nepesseg[15];
    sprintf(nepesseg, "%d egyed", szim[i].nepmeret);
    kiir(renderer, betutipus[1].tipus, vilagos, nepesseg, x, y);
}

/* Kirajzolja az y tengeyre vastagon a fertőzöttek és a gyógyultak arányát
 * És azok mellé még számmal és % is kiírja az értékeket
 * */
static void arany_rajzol(SDL_Renderer* renderer, Szim* const szim, int i, int x1, int y1){
    int gy_meret = szim[i].graf->utso->gogyult;
    int fert_meret = szim[i].graf->utso->fert;
    int y2 = y1 + W * TART_SZEL;
    double arany = (W * TART_SZEL) / szim[i].nepmeret;
    if (fert_meret != 0) {
        thickLineRGBA(renderer, x1, y2, x1, y2 - (fert_meret * arany), 10, 255, 0, 0, 255);
    }
    if (gy_meret != 0) {
        thickLineRGBA(renderer, x1, y1, x1, y1 + (gy_meret * arany), 10, 0, 255, 0, 255);
    }
    //#fertozottek
    char fertozottek[5];
    sprintf(fertozottek, "%d", szim[i].graf->utso->fert);
    kiir(renderer, betutipus[1].tipus, vilagos, fertozottek, x1 - 125, realtoint(y2 - (fert_meret * arany)));
    //#fert szazalek
    char fert_szazalek[8];
    sprintf(fert_szazalek, "(%.1f%%)", (double)szim[i].graf->utso->fert/szim[i].nepmeret*100);
    kiir(renderer, betutipus[1].tipus, vilagos, fert_szazalek, x1 - 80, realtoint(y2 - (fert_meret * arany)));
    //#gyogyultak
    char gyogy[5];
    sprintf(gyogy, "%d", szim[i].graf->utso->gogyult);
    kiir(renderer, betutipus[1].tipus, vilagos, gyogy, x1 - 125, realtoint(y1 + (gy_meret * arany) - 15));
    //#gyogy szazalek
    char gyogy_szazalek[8];
    sprintf(gyogy_szazalek, "(%.1f%%)", (double)szim[i].graf->utso->gogyult/szim[i].nepmeret*100);
    kiir(renderer, betutipus[1].tipus, vilagos, gyogy_szazalek, x1 - 80, realtoint(y1 + (gy_meret * arany) - 15));
}

/*
static void r_kiir(SDL_Renderer * renderer, Szim* const szim, int i, int x, int y){
    char r0[15];
    char r0_max[15];
    sprintf(r0, "R = %.2f", szim[i].R0);
    sprintf(r0_max, "R max = %.2f", szim[i].rmax);
    kiir(renderer, betutipus[0].tipus, vilagos, r0, x, y);
   // kiir(renderer, betutipus[0].tipus, vilagos, r0_max, x, y + 30);
}*/

/* Meghívja az összes fentebb lévő függvényt és így a teljes grafikon kirajzolódik
 * */
void graf_alap_rajzol(SDL_Renderer * renderer, Szim* const szim, int i){
    int y_x1 = W / 2 + 100;
    int y_y1 = (i * (int)(W * TART_SZEL + 50)) + (int)(H * SAV_SZEL + 50);
    int x_x1 = y_x1;
    int x_y1 = y_y1 + (int)(W * TART_SZEL);
    Grafikon* graf = szim[i].graf;
    gorbe_rajzol(renderer, graf, szim[i].nepmeret, x_x1, x_y1, y_y1);
    beosztas_rajzol(renderer, graf->meret, x_x1, x_y1);
    meret_kiir(renderer, szim, i);
    ido_kiir(renderer, szim, i, x_x1, x_y1);
    //r_kiir(renderer, szim, i, x_x1 + W * TART_SZEL * 2 + 50, y_y1 + W / 2 * TART_SZEL);
    lineRGBA(renderer, y_x1, y_y1, y_x1, y_y1 + (W * TART_SZEL), 255, 255, 255, 255);
    lineRGBA(renderer, x_x1, x_y1, x_x1 + (W * TART_SZEL * 2), x_y1, 255, 255, 255, 255);
    //lineRGBA(renderer, y_x1 - 5, y_y1, y_x1 + 5, y_y1, 255, 255, 255, 255);
    lineRGBA(renderer, x_x1  + (W * TART_SZEL * 2), x_y1 - 5, x_x1 + (W * TART_SZEL * 2), x_y1 + 5, 255, 255, 255, 255);
    arany_rajzol(renderer, szim, i, y_x1, y_y1);
}