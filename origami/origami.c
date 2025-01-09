// autor: Michal Szydlo
// code review: Adam Beclowicz

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

#define epsilon 1e-6

typedef struct {
    double x, y;
} Punkt;

// rozmiary figur
unsigned rozmiar_figury(char typ) {
    if (typ == 'P') {
        return 4U;
    }
    if (typ == 'K') {
        return 3U;
    }
    return 5U;
}

// sprawdzenie mniejszosci i rownosci aby uniknac bledow dokladnosci
bool rowny(double x, double y) {
    return fabs(x - y) < epsilon;
}
bool mniejszy(double x, double y) {
    return x < y && !rowny(x, y);
}
bool mniejszy_rowny(double x, double y) {
    return mniejszy(x, y) || rowny(x, y);
}

double iloczyn_wektorowy(Punkt odniesienie, Punkt a, Punkt b) {
    return (a.x - odniesienie.x) * (b.y - odniesienie.y) - (a.y - odniesienie.y) * (b.x - odniesienie.x);
}

// kierunek dwoch wektorow (0 -> wspolinionwe, -1 -> po lewej stronie, 1 -> po prawej stronie)
int wyznacz_kierunek(double x) {
    if (rowny(x, 0.0))
        return 0;
    if (mniejszy(x, 0.0))
        return -1;
    return 1;
}

// odbicie symetryczne punktu wzgledem prostej
Punkt odbicieSymetryczne(Punkt p1, Punkt p2, Punkt sprawdzany) {
    double A = p2.y - p1.y;
    double B = p1.x - p2.x;
    double C = p2.x * p1.y - p1.x * p2.y;

    double D = A * A + B * B; // Norma wektora (A, B)
    double x_r = (B * (B * sprawdzany.x - A * sprawdzany.y) - A * C) / D;
    double y_r = (A * (-B * sprawdzany.x + A * sprawdzany.y) - B * C) / D;

    Punkt odbity;
    odbity.x = 2 * x_r - sprawdzany.x;
    odbity.y = 2 * y_r - sprawdzany.y;

    return odbity;
}

// odleglosc euklidesowa
double odleglosc(Punkt x, Punkt y) {
    return sqrt((x.x - y.x) * (x.x - y.x) + (x.y - y.y) * (x.y - y.y));
}

// czy punkt znajduje sie w prostokacie
bool w_prostokacie(Punkt sprawdzany, Punkt lewy, Punkt prawy) {
    return mniejszy_rowny(lewy.x, sprawdzany.x) && mniejszy_rowny(sprawdzany.x, prawy.x) &&
           mniejszy_rowny(lewy.y, sprawdzany.y) && mniejszy_rowny(sprawdzany.y, prawy.y);
}

// czy punkt znajduje sie w kole
bool w_kole(Punkt sprawdzany, Punkt srodek, double r) {
    return mniejszy_rowny(odleglosc(sprawdzany, srodek), r);
}

int rozw(int k, Punkt P, char *typ, double **Figura) {
    if (typ[k] == 'P') {
        return (int)w_prostokacie(P, (Punkt){Figura[k][0], Figura[k][1]}, (Punkt){Figura[k][2], Figura[k][3]});
    } 
    else if (typ[k] == 'K') {
        return (int)w_kole(P, (Punkt){Figura[k][0], Figura[k][1]}, Figura[k][2]);
    }
    
    int kierunek = wyznacz_kierunek(iloczyn_wektorowy((Punkt){Figura[k][1], Figura[k][2]}, (Punkt){Figura[k][3], Figura[k][4]}, P));
    if (kierunek == -1) { // punkt z prawej strony prostej
        return 0;
    }
    else if (kierunek == 0) { // punkt na prostej
        return rozw((int)Figura[k][0], P, typ, Figura);
    }
    // punkt z lewej strony prostej
    Punkt odbity = odbicieSymetryczne((Punkt){Figura[k][1], Figura[k][2]}, (Punkt){Figura[k][3], Figura[k][4]}, P);
    return rozw((int)Figura[k][0], P, typ, Figura) + rozw((int)Figura[k][0], odbity, typ, Figura);
}

int main() {
    int n, q;
    scanf("%d %d", &n, &q);
	
	// figura - prostokat, kolo, zlozenie
	
    char *typ = (char *)malloc((n + 1) * sizeof(char)); // typ Figury
    double **Figura = (double **)malloc((n + 1) * sizeof(double *)); // Figura (P, K, Z)

    for (int i = 1; i <= n; i++) {
        scanf(" %c", &typ[i]); // wczytanie typu figury

        unsigned rozmiar = rozmiar_figury(typ[i]); // kazda figura ma inny rozmiar inputu
        Figura[i] = (double *)malloc(rozmiar * sizeof(double));
		
		for (unsigned j = 0; j < rozmiar; j++) {
			scanf("%lf", &Figura[i][j]);
		}
    }
	
    for (int i = 1; i <= q; i++) {
        int k;
        Punkt punkt;
        scanf("%d %lf %lf", &k, &punkt.x, &punkt.y);

        int wynik = rozw(k, punkt, typ, Figura); // rekurencyjne generowanie punktow i liczenie wyniku
        printf("%d\n", wynik);
    }

	// czyszczenie pamieci
    for (int i = 1; i <= n; i++) {
        free(Figura[i]);
    }
    free(typ);
    free(Figura);

    return 0;
}
