#ifndef NHF_KATTINTAS_KEZEL_H
#define NHF_KATTINTAS_KEZEL_H

#include "../def.h"
#include "../globalis_fvek.h"
#include "adat_kezel.h"
#include "FILE/SVG.h"

void kilepes(bool* quit);
void valt(Szim* const szim, bool* stop, int* melyiket);
void torol(Szim* szim);
void ujraindit(Szim* szim);
void uj(Szim* szim, bool* stop, int* melyik);
void bevitel_valaszt(void);
void inditas(Szim* szim, int melyik, Bevstring* bevstring, bool* stop);
void svg_export_katt(Szim* const szim);

#endif