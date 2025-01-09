#ifndef KOL_H
#define KOL_H

#include <vector>
#include <cstdlib>

struct interesant {
    interesant* nastepnik;
    interesant* poprzednik;
    int numerek_interesanta; 
};

struct Kolejka{
    interesant *przod_kolejki, *tyl_kolejki;
    void stworz_kolejke() {
		przod_kolejki = (interesant *) malloc(sizeof(interesant));
		tyl_kolejki = (interesant *) malloc(sizeof(interesant));
		przod_kolejki -> numerek_interesanta = -1; // numerek poczatku kolejki
		tyl_kolejki -> numerek_interesanta = -2; // numerek konca kolejki
		przod_kolejki -> nastepnik = tyl_kolejki;
		tyl_kolejki -> nastepnik = przod_kolejki;
		przod_kolejki -> poprzednik = przod_kolejki;
		tyl_kolejki -> poprzednik = tyl_kolejki;
	}
    bool pusta() {
		return przod_kolejki -> nastepnik == tyl_kolejki;
	}
};

// Funkcje globalne
void otwarcie_urzedu(int m);
interesant* nowy_interesant(int k);
int numerek(interesant* i);
interesant* obsluz(int k);
void zmiana_okienka(interesant* i, int k);
void zamkniecie_okienka(int k1, int k2);
std::vector<interesant*> fast_track(interesant* i1, interesant* i2);
void naczelnik(int k);
std::vector<interesant*> zamkniecie_urzedu();

#endif // KOL_H

