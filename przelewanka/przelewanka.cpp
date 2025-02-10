// autor: Michał Szydło
// code review: Adam Bęcłowicz

#include <bits/stdc++.h>
using namespace std;

const pair<int,int> p = {37, 1009};
const int Mod = 998244353; // modulo do haszowania

// sprawdza czy stan szklanek jest oczekiwany
bool dobry_stan(const vector<int> &stan, const vector<pair<int, int>> &szklanki) {
	int n = (int)szklanki.size();

	for(int i = 0; i < n; i++) {
		if(stan[i] != szklanki[i].second) {
			return false;
		}
	}

	return true;
}

// operacja 1: wlanie z kranu
vector<int> dolewka(int k, int pojemnosc, const vector<int> &stan) {
	vector<int> nowy_stan = stan;

	nowy_stan[k] = pojemnosc;

	return nowy_stan;
}

// operacja 2: wylanie do zlewu
vector<int> wylewka(int k, const vector<int> &stan) {
	vector<int> nowy_stan = stan;

	nowy_stan[k] = 0;

	return nowy_stan;
}

// operacja 3: przelanie z i-tej szklanki do j-tej
vector<int>przelewka(int i, int j, const vector<int> &stan, const vector<pair<int, int>> &szklanki) {
	vector<int> nowy_stan = stan;

	int przelej = min(stan[i], szklanki[j].first - stan[j]);

	nowy_stan[i] -= przelej;
	nowy_stan[j] += przelej;

	return nowy_stan;
}

// haszowanie wektora
pair<int,int> hasz(const vector<int> &stan) {
	pair<long long, long long> wyn = {0, 0}, P = {1, 1};

	for(int x : stan) {
		wyn.first = (wyn.first + P.first * x) % Mod;
		wyn.second = (wyn.second + P.second * x) % Mod;

		P.first = (P.first * p.first) % Mod;
		P.second = (P.second * p.second) % Mod;
	}

	return wyn;
}

// Algorytm BFS
int solve(int n, const vector<pair<int, int>> &szklanki) {
    map<pair<int,int>, int> odwiedzone; // odwiedzone stany szklanek 
    
    queue<vector<int>> Q;
    
    vector<int> start(n, 0);
    odwiedzone[hasz(start)] = 0;
    Q.push(start);

    while(!Q.empty()) {
        vector<int> stan_teraz = Q.front();
        Q.pop();

        int ruchy = odwiedzone[hasz(stan_teraz)];

        if(dobry_stan(stan_teraz, szklanki)) {
			return ruchy;
		}

        for(int i = 0; i < n; i++) {
            vector<int> nast;

            // Dolewanie wody do szklanki
            nast = dolewka(i, szklanki[i].first, stan_teraz);

            if(odwiedzone.find(hasz(nast)) == odwiedzone.end()) {
                odwiedzone[hasz(nast)] = ruchy + 1;
                Q.push(nast);
            }

            // Wylewanie wody ze szklanki
            nast = wylewka(i, stan_teraz);

			if(odwiedzone.find(hasz(nast)) == odwiedzone.end()) {
                odwiedzone[hasz(nast)] = ruchy + 1;
                Q.push(nast);
            }

            // Przelewanie wody z i-tej do j-tych szklanek
            if(stan_teraz[i] > 0) {
				for(int j = 0; j < n; j++) {
					if(i != j && stan_teraz[j] != szklanki[j].first) {
						nast = przelewka(i, j, stan_teraz, szklanki);

						if(odwiedzone.find(hasz(nast)) == odwiedzone.end()) {
							odwiedzone[hasz(nast)] = ruchy + 1;
							Q.push(nast);
						}
					}
				}
			}
        }
    }
    
    return -1;
}

int main() {
    int n;
    cin >> n;
	
	bool dobre = true; // y == 0 lub x == y
    vector<pair<int, int>> szklanki(n);
    int nwd_x = 0; // nwd pojemnosci szklanek

    for (int i = 0; i < n; i++) {
        cin >> szklanki[i].first >> szklanki[i].second;

        dobre &= (szklanki[i].first == szklanki[i].second || szklanki[i].second == 0);
        nwd_x = __gcd(nwd_x, szklanki[i].first);
    }
	
	if(dobre) {
		int wynik = 0;

		for(int i = 0; i < n; i++) {
			if(szklanki[i].second > 0) {
				wynik++;
			}
		}

		cout << wynik << "\n";

		return 0;
	}
	
    bool ostatni_ruch = false; // czy mozna wykonac w jakis sposob ostatni ruch (xi == yi lub yi == 0)
    bool da_sie_nwd = true; // czy kazdy yi % nwd_x == 0
    
    for (int i = 0; i < n; i++) {
		if(szklanki[i].second % nwd_x != 0) {
			da_sie_nwd = false;
		}

		if(szklanki[i].second == 0 || szklanki[i].second == szklanki[i].first) {
			ostatni_ruch = true;
		}
	}
	
	if(ostatni_ruch == false || da_sie_nwd == false) {
		cout << -1 << "\n";

		return 0;
	}
    
    cout << solve(n, szklanki) << "\n";
}
