// autor: Michał Szydło
// code review: Adam Bęcłowicz

#include <bits/stdc++.h>
#include "kol.h"

// zmienne globalne
static int m, licznik_numerkow;
std::vector<Kolejka *>okienka;

void otwarcie_urzedu(int _m) {
	m = _m;
	for(int i = 0; i < m; i++) {
		Kolejka *nowa_kolejka = (Kolejka*) malloc(sizeof(Kolejka));
		nowa_kolejka -> stworz_kolejke();
		okienka.push_back(nowa_kolejka);
	}
}

void polacz_poprzednik_nastepnik(interesant* koniec1, interesant *koniec2, interesant *nowy){
	if(koniec1 == nullptr) // nie ma tego interesanta lub wartownika
		return;
	(koniec1 -> poprzednik == koniec2) ? koniec1 -> poprzednik = nowy : koniec1 -> nastepnik = nowy;
}

void usun_interesanta(interesant *usuwany){
    interesant * sasiad1 = usuwany -> poprzednik, *sasiad2 = usuwany -> nastepnik;
    polacz_poprzednik_nastepnik(sasiad1, usuwany, sasiad2);
	polacz_poprzednik_nastepnik(sasiad2, usuwany, sasiad1);
}

void dodaj_do_ktej_kolejki(Kolejka *k, interesant *dodawany) {
	// dodanie miedzy wartownika tylniego oraz jego nastepnika
	interesant *wartownik = k -> tyl_kolejki;
	interesant *ostatni = wartownik -> nastepnik;
	
	// przepinanie
	dodawany -> poprzednik = wartownik;
	dodawany -> nastepnik = ostatni;
	
	polacz_poprzednik_nastepnik(wartownik, ostatni, dodawany);
	polacz_poprzednik_nastepnik(ostatni, wartownik, dodawany);
}

interesant* usun_z_ktej_kolejki(Kolejka *k){
    if (k -> pusta()) {
		return nullptr;
	}
    interesant* i = k -> przod_kolejki -> nastepnik;
    usun_interesanta(k -> przod_kolejki -> nastepnik);
    return i;
}

interesant* obsluz(int k) {
	return usun_z_ktej_kolejki(okienka[k]);
}	

interesant* nowy_interesant(int k) {
	interesant *nowy = (interesant*) malloc(sizeof(interesant));
    nowy -> numerek_interesanta = licznik_numerkow++;
    dodaj_do_ktej_kolejki(okienka[k], nowy);
    return nowy;
}

int numerek(interesant* i) {
	return i -> numerek_interesanta;
}

void zmiana_okienka(interesant* i, int k) {
	usun_interesanta(i);
    dodaj_do_ktej_kolejki(okienka[k], i);
}

void zamkniecie_okienka(int k1, int k2) {
	if(k1 == k2 || okienka[k1] -> pusta())
		return;
	
    // potrzebni wartownicy do przepinania
    interesant* wartownik_k2_tyl = okienka[k2] -> tyl_kolejki;
    interesant* wartownik_k1_przod = okienka[k1] -> przod_kolejki;
    interesant* wartownik_k1_tyl = okienka[k1] -> tyl_kolejki;

	// pierwszy k1 oraz ostatni k2
	interesant* przodni_k1 = wartownik_k1_przod -> nastepnik;
    interesant* tylni_k2 = wartownik_k2_tyl -> nastepnik;
   
	// przepinanie
    polacz_poprzednik_nastepnik(tylni_k2, wartownik_k2_tyl, przodni_k1);
    polacz_poprzednik_nastepnik(przodni_k1, wartownik_k1_przod, tylni_k2);
    
    okienka[k2] -> tyl_kolejki = wartownik_k1_tyl;
    okienka[k1] -> tyl_kolejki = wartownik_k2_tyl;
    
    wartownik_k1_przod -> nastepnik = wartownik_k2_tyl;
    wartownik_k2_tyl -> nastepnik = wartownik_k1_przod;
}

void naczelnik(int k) {    
    std::swap(okienka[k] -> przod_kolejki, okienka[k] -> tyl_kolejki);
}

std::vector<interesant*> fast_track(interesant *i1, interesant *i2){
	std::vector<interesant*> wynik;
    if (i1 == i2) {
		wynik.push_back(i1);
    }
    else {
		interesant *lewo = i1 -> poprzednik, *prawo = i1 -> nastepnik, *byl_lewym = i1, *byl_prawym = i1;
		 
		std::vector<interesant*> sciezka_lewa, sciezka_prawa;
		sciezka_lewa.push_back(i1);
		sciezka_prawa.push_back(i1);
		
		while (lewo != i2 && prawo != i2){
			interesant* bufor_lewo = lewo, *bufor_prawo = prawo;
			
			if(lewo -> numerek_interesanta >= 0) {
				sciezka_lewa.push_back(lewo);
				(lewo -> poprzednik != byl_lewym) ? lewo = lewo -> poprzednik : lewo = lewo -> nastepnik;
				byl_lewym = bufor_lewo;
			}
			
			if (prawo -> numerek_interesanta >= 0) {
				sciezka_prawa.push_back(prawo);
				(prawo -> nastepnik != byl_prawym) ? prawo = prawo -> nastepnik : prawo = prawo -> poprzednik;
				byl_prawym = bufor_prawo;
			} 
		}
		
		wynik = ((lewo == i2) ? sciezka_lewa : sciezka_prawa);
		wynik.push_back(i2);
	}
	
	for (interesant* x : wynik) {
		usun_interesanta(x);
	}
	
	return wynik;
}


std::vector<interesant*> zamkniecie_urzedu(){
    std::vector<interesant*> pozostali;
    
    for (int i = 0; i < m; i++){
        while (!(okienka[i] -> pusta())) {
            pozostali.push_back(usun_z_ktej_kolejki(okienka[i]));
        }
        free(okienka[i] -> przod_kolejki);
        free(okienka[i] -> tyl_kolejki);
        free(okienka[i]);
    }
    
    return pozostali;
}

