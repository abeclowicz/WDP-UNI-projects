#include <stdlib.h>
#include <stdbool.h>
#include "zbior_ary.h"

static int Q;

int modulo(int a, int q) {
    return ((a % q) + q) % q;
}
int min(int a, int b) {
    return a < b ? a : b;
}
int max(int a, int b) {
    return a >= b ? a : b;
}

zbior_ary ciag_arytmetyczny(int a, int q, int b) {
    Q = q;

    zbior_ary A;
    
    A.rozmiar = 1;
    A.zbiory = (ciag*)malloc((size_t)A.rozmiar * sizeof(ciag));
	
    A.zbiory[0].mod = modulo(a, q);
    A.zbiory[0].a = a;
    A.zbiory[0].b = b;
    
    return A;
}

zbior_ary singleton(int a) {
    return ciag_arytmetyczny(a, Q, a);
}

zbior_ary suma(zbior_ary A, zbior_ary B) {
    zbior_ary Suma;

    Suma.rozmiar = A.rozmiar + B.rozmiar;
    Suma.zbiory = (ciag*)malloc((size_t)Suma.rozmiar * sizeof(ciag));
    
    unsigned int i = 0, j = 0, licznik = 0;
    
    while(i < A.rozmiar && j < B.rozmiar) {
        if(A.zbiory[i].mod == B.zbiory[j].mod) {
            int modulo_teraz = A.zbiory[i].mod;
            
            ciag pomocniczy;
            pomocniczy.mod = -1;

            while(i < A.rozmiar && j < B.rozmiar && A.zbiory[i].mod == modulo_teraz && B.zbiory[j].mod == modulo_teraz) {
                if(A.zbiory[i].a <= B.zbiory[j].a) {
                    if(pomocniczy.mod == -1) {
                        pomocniczy = A.zbiory[i++];
                    }
                    else {
                        if(pomocniczy.b + Q >= A.zbiory[i].a) {
                            pomocniczy.b = max(pomocniczy.b, A.zbiory[i++].b);
                        }
                        else {
                            Suma.zbiory[licznik++] = pomocniczy;
                            pomocniczy = A.zbiory[i++];
                        }
                    }
                }
                else {
                    if(pomocniczy.mod == -1) {
                        pomocniczy = B.zbiory[j++];
                    }
                    else {
                        if(pomocniczy.b + Q >= B.zbiory[j].a) {
                            pomocniczy.b = max(pomocniczy.b, B.zbiory[j++].b);
                        }
                        else {
                            Suma.zbiory[licznik++] = pomocniczy;
                            pomocniczy = B.zbiory[j++];
                        }
                    }
                }
            }

            while(i < A.rozmiar && A.zbiory[i].mod == modulo_teraz) {
                if(pomocniczy.b + Q >= A.zbiory[i].a) {
                    pomocniczy.b = max(pomocniczy.b, A.zbiory[i++].b);
                }
                else {
                    Suma.zbiory[licznik++] = pomocniczy;
                    pomocniczy = A.zbiory[i++];
                }
            }

            while(j < B.rozmiar && B.zbiory[j].mod == modulo_teraz) {
                if(pomocniczy.b + Q >= B.zbiory[j].a) {
                    pomocniczy.b = max(pomocniczy.b, B.zbiory[j++].b);
                }
                else {
                    Suma.zbiory[licznik++] = pomocniczy;
                    pomocniczy = B.zbiory[j++];
                }
            }
            if(pomocniczy.mod != -1) {
                Suma.zbiory[licznik++] = pomocniczy;
            }   
        }
        else {
            while(i < A.rozmiar && A.zbiory[i].mod < B.zbiory[j].mod) {
                Suma.zbiory[licznik++] = A.zbiory[i++];
            }

            while(j < B.rozmiar && A.zbiory[i].mod > B.zbiory[j].mod) {
                Suma.zbiory[licznik++] = B.zbiory[j++];
            }
        }
    }

    while(i < A.rozmiar) {
        Suma.zbiory[licznik++] = A.zbiory[i++];
    }

    while(j < B.rozmiar) {
        Suma.zbiory[licznik++] = B.zbiory[j++];
    }
    
    Suma.rozmiar = licznik;
    Suma.zbiory = (ciag*)realloc(Suma.zbiory, (size_t)Suma.rozmiar * sizeof(ciag));

    return Suma;
}

zbior_ary iloczyn(zbior_ary A, zbior_ary B) {
    zbior_ary Iloczyn;
    
    Iloczyn.rozmiar = A.rozmiar + B.rozmiar;
    Iloczyn.zbiory = (ciag*)malloc((size_t)Iloczyn.rozmiar * sizeof(ciag));
    
    unsigned int i = 0, j = 0, licznik = 0;

    while(i < A.rozmiar && j < B.rozmiar) {
        if(A.zbiory[i].mod == B.zbiory[j].mod) {
            int modulo_teraz = A.zbiory[i].mod;

            while(i < A.rozmiar && j < B.rozmiar && A.zbiory[i].mod == modulo_teraz && B.zbiory[j].mod == modulo_teraz) {
                int maksia = max(A.zbiory[i].a, B.zbiory[j].a);
                int minib = min(A.zbiory[i].b, B.zbiory[j].b);
                
                if(maksia <= minib) {
                    Iloczyn.zbiory[licznik].mod = modulo_teraz;
                    Iloczyn.zbiory[licznik].a = maksia;
                    Iloczyn.zbiory[licznik].b = minib;
                    
                    licznik++;
                }
                if(A.zbiory[i].b <= B.zbiory[j].b) {
                    i++;
                }
                else {
                    j++;
                }
            }
        }
        else {
            while(i < A.rozmiar && A.zbiory[i].mod < B.zbiory[j].mod) {
                i++;
            }

            while(j < B.rozmiar && A.zbiory[i].mod > B.zbiory[j].mod) {
                j++;
            }
        }
    }
    
    Iloczyn.rozmiar = licznik;
    Iloczyn.zbiory = (ciag*)realloc(Iloczyn.zbiory, (size_t)Iloczyn.rozmiar * sizeof(ciag));
    
    return Iloczyn;
}

// A - B = A - Iloczyn(A, B)
zbior_ary roznica(zbior_ary A, zbior_ary B) {
    zbior_ary Iloczyn = iloczyn(A, B);
    zbior_ary Roznica;

    Roznica.rozmiar = A.rozmiar + Iloczyn.rozmiar;
    Roznica.zbiory = (ciag*)malloc((size_t)Roznica.rozmiar * sizeof(ciag));
    
    unsigned int j = 0, licznik = 0;
    
    for(unsigned int i = 0; i < A.rozmiar; i++) {
        ciag pomocniczy = A.zbiory[i];
        
        while(j < Iloczyn.rozmiar && Iloczyn.zbiory[j].mod == pomocniczy.mod && Iloczyn.zbiory[j].b <= pomocniczy.b) {
            if(pomocniczy.a < Iloczyn.zbiory[j].a) {
                Roznica.zbiory[licznik].mod = pomocniczy.mod;
                Roznica.zbiory[licznik].a = pomocniczy.a;
                Roznica.zbiory[licznik].b = Iloczyn.zbiory[j].a - Q;
                
                licznik++;
            }

            pomocniczy.a = Iloczyn.zbiory[j].b + Q;
            j++;
        }
        if(pomocniczy.a <= pomocniczy.b) {
            Roznica.zbiory[licznik++] = pomocniczy;
        }
    }

    Roznica.rozmiar = licznik;
    Roznica.zbiory = (ciag*)realloc(Roznica.zbiory, (size_t)Roznica.rozmiar * sizeof(ciag));

    return Roznica;
}

bool nalezy(zbior_ary A, int b) {
    int mod = modulo(b, Q);
    unsigned int lewo = 0, prawo = A.rozmiar;

    while(lewo != prawo) {
        unsigned int srodek = (lewo + prawo) / 2;

        if(A.zbiory[srodek].mod < mod || A.zbiory[srodek].b < b) {
            lewo = srodek + 1;
        }
        else {
            prawo = srodek;
        }
    }

    if(A.zbiory[lewo].mod != mod) {
        return false;
    }

    return (A.zbiory[lewo].a <= b && b <= A.zbiory[lewo].b);
}

unsigned moc(zbior_ary A) {
    unsigned Moc = 0;

    for(unsigned int i = 0; i < A.rozmiar; i++) {
        Moc += (((unsigned)(A.zbiory[i].b) - (unsigned)(A.zbiory[i].a)) / (unsigned)Q + 1);
    }

    return Moc;
}

unsigned ary(zbior_ary A) {
    return (unsigned)A.rozmiar;
}
