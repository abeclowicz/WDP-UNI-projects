// Autor: Michał Szydło grupa 14
// Code review: Adam Bęcłowicz

#include <stdio.h>
#include <stdlib.h>


// Funkcje matematyczne
int min(int a, int b) {
    return (a <= b) ? a : b;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

// Funkcja do odleglosci podpunkt 1
int odleglosc_1(int *odl, int a, int b, int c) {
    return max(odl[b] - odl[a], odl[c] - odl[b]);
}

// Funkcja do odleglosci podpunkt 2
int odleglosc_2(int *motel, int *odl, int a, int b, int c) {
    if(motel[a] == motel[b] || motel[a] == motel[c] || motel[b] == motel[c]) {
        return -1;
    }
    return min(odl[b] - odl[a], odl[c] - odl[b]);
}

// znalezienie pierwszego kandydata na 'srodkowy' hotel
int Find_j(int *motel, int k) {
    int j = k - 1;
    while(motel[j - 1] == motel[j]) {
        j--;
    }
    return j;
}

// znalezienie pierwszego kandydata na 'lewy' hotel
int Find_i(int *motel, int j, int k) {
    int i = j - 1;
    while(motel[i] == motel[k] || motel[i] == motel[j]) {
        i--;
    }
    return i;
}

int main() {
    int n;
    scanf("%d", &n);

    int *motel = (int *)malloc((size_t)(n + 1) * sizeof(int));
    int *odl = (int *)malloc((size_t)(n + 1) * sizeof(int));

	// dla wygody indeksowanie od 1 do n
    for (int i = 1; i <= n; i++) {
        scanf("%d %d", &motel[i], &odl[i]);
    }
    
    /// Podpunkt 1
    // i - index motelu najblizszego poczatku autostrady, j - srodkowy, k - najbardziej oddalony (mozliwe ze i == j lub j == k)
    int ans_1 = odl[n] - odl[1];
    int i = 1;
    int j = i;
    
    while(j <= n && motel[j] == motel[i]) {
        j++;
    }
    
    int k = j;
    
    while(k <= n && (motel[k] == motel[i] || motel[k] == motel[j])) {
        k++;
    }
   
    
    // brak 3 rodzai roznych hoteli
	if(k == n + 1) {
		printf("%d %d\n", 0, 0);
		return 0;
	}
	
	// sprawdzam wszyskie pomiedzy i-tym a k-tym. Kazdy miedzy jest taki sam jak j-ty hotel
    while (k <= n) {
        j = Find_j(motel, k); 
        i = Find_i(motel, j, k);

		// sprawdzenie wszystkich pomiedzy
        ans_1 = min(ans_1, odleglosc_1(odl, i, j, k));
        while(motel[j + 1] == motel[j]) {
            j++;
            ans_1 = min(ans_1, odleglosc_1(odl, i, j, k));
        }
        while(k <= n && (motel[k] == motel[j + 1] || motel[k] == motel[j])) {
            k++;
        }
    }
	
	// Zlozonosc tego algorytmu jest liniowa, poniewaz k-tym pointerem ide jedynie w prawa strone i odwiedze kazdy element dokladnie raz
	// j-tym pointerem ide w lewa strone i odwiedze kazdy element maksymalnie 2 razy (raz idac w lewo i raz idac w prawo)
	// i-ty pointer za kazdym razem idzie jedynie w prawa strone w kazdej iteracji petli
	
    /// Podpunkt 2
    // znajduje 3 motele na prefiksie o roznych markach najbardziej blisko poczatku autostrady (i, j, k) i najbardizej blisko ostatniego motelu (ii, jj, kk)
    // dla kazdego srodka sprawdzam zawsze stala liczbe (wszystkie mozliwe poczatki i wszystkie mozliwe konce)
    // Zlozonosc algorytmu jest zatem liniowa
    
    int ans_2 = 0;
    i = 1;
    j = i;
    while(motel[j] == motel[i]) {
        j++;
    }
    k = j;
    while(motel[k] == motel[i] || motel[k] == motel[j]) {
        k++;
    }

    int ii = n;
    int jj = n - 1;
    while(motel[jj] == motel[ii]) {
        jj--;
    }
    int kk = jj - 1;
    while(motel[kk] == motel[ii] || motel[kk] == motel[jj]) {
        kk--;
	}
    for(int srodek = 1; srodek <= n; srodek++) {
        ans_2 = max(ans_2, odleglosc_2(motel, odl, i, srodek, ii));
        ans_2 = max(ans_2, odleglosc_2(motel, odl, i, srodek, jj));
        ans_2 = max(ans_2, odleglosc_2(motel, odl, i, srodek, kk));
        ans_2 = max(ans_2, odleglosc_2(motel, odl, j, srodek, ii));
        ans_2 = max(ans_2, odleglosc_2(motel, odl, j, srodek, jj));
        ans_2 = max(ans_2, odleglosc_2(motel, odl, j, srodek, kk));
        ans_2 = max(ans_2, odleglosc_2(motel, odl, k, srodek, ii));
        ans_2 = max(ans_2, odleglosc_2(motel, odl, k, srodek, jj));
        ans_2 = max(ans_2, odleglosc_2(motel, odl, k, srodek, kk));
    }

    printf("%d %d\n", ans_1, ans_2);

    free(motel);
    free(odl);

    return 0;
}
