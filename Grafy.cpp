#include <iostream>
#include<algorithm>
using namespace std;
class vector
{
    int* tb;
    int wielkosc_tablicy;
    int ilosc_elelemtow;
public:
    vector(int wielkosc_poczatkowa = 10)
    {
        wielkosc_tablicy = wielkosc_poczatkowa;
        ilosc_elelemtow = 0;
        tb = new int[wielkosc_tablicy];
    }
    void push_back(int wartosc)
    {
        if (ilosc_elelemtow == wielkosc_tablicy)
        {
            wielkosc_tablicy *= 2;
            int* pom = new int[wielkosc_tablicy];
            for (int i = 0; i < ilosc_elelemtow; ++i)
            {
                pom[i] = tb[i];
            }
            delete[] tb;
            tb = pom;
        }
        tb[ilosc_elelemtow] = wartosc;
        ++ilosc_elelemtow;
    }
    int& operator[](int index)
    {
        return tb[index];
    }
    int size()
    {
        return ilosc_elelemtow;
    }
    int* begin()
    {
        return tb;
    }
    int* end()
    {
        return tb + ilosc_elelemtow;
    }
    ~vector()
    {
        delete[] tb;
    }
};
class Kolejka {
    int* tb;
    int ilosc_elementow;
    int wielkosc_tablicy;
    int poczatek;
    int koniec;

public:
    Kolejka(int wielkosc_poczatkowa = 2) {
        wielkosc_tablicy = wielkosc_poczatkowa;
        tb = new int[wielkosc_tablicy];
        ilosc_elementow = 0;
        poczatek = 0;
        koniec = -1;
    }

    bool pusty() {
        return ilosc_elementow == 0;
    }

    bool pelny() {
        return ilosc_elementow == wielkosc_tablicy;
    }

    void push(int x) {
        koniec++;
        tb[koniec] = x;
        ilosc_elementow++;
    }

    int pop() {

        int wartosc = tb[poczatek];
        ++poczatek;
        ilosc_elementow--;
        return wartosc;
    }

    int first() {


        return tb[poczatek];
    }


    ~Kolejka() {
        delete[] tb;
    }
};
struct Pair
{
    int first;
    int second;
    bool operator<(const Pair& other)
    {
        if (first < other.first)
            return 1;
        else if (first == other.first && second < other.second)
            return 1;
        else
            return 0;
    }
};
template <typename T>
void merge(T tb[], T pom[], int l, int s, int r) 
{
    int i, j;
    for (i = s+1; i > l; --i)
        pom[i -1] = tb[i-1];

    for (j = s; j < r; ++j)
        pom[r + s - j] = tb[j + 1];

    for (int k = l; k <= r; ++k) 
    {
        if (pom[j] < pom[i])
        {
            tb[k] = pom[j];
            --j;
        }
        else
        {
            tb[k] = pom[i];
            ++i;
        }
    }
}
template <typename T>
void mergesort(T tb[], T pom[], int l, int r) {
    if (l >= r)
        return;
    int s = (l + r) / 2;
    mergesort(tb, pom, s + 1, r);
    mergesort(tb, pom, l, s);
    merge(tb, pom, l, s, r);
}
void DFS(int wiercholki, int v, int odwiedzone[], vector graf[])
{
    odwiedzone[v] = 1;
    for (int j : graf[v])
        if (odwiedzone[j] == 0)
            DFS(wiercholki, j, odwiedzone, graf);
}
int DFS_dwudzielnosc(vector graf[], int v, int odwiedzone[], int kolor)
{
    odwiedzone[v] = kolor;
    for (int j : graf[v])
    {
        if (odwiedzone[j] == kolor)
            return 0;
        if (odwiedzone[j] == 0)
            if (DFS_dwudzielnosc(graf, j, odwiedzone, 3 - kolor) == 0)
                return 0;
    }
    return 1;
}
int BFS(vector graf[], int n, int wierzcholki, int odwiedzone[], int odleglosc[]) 
{
    Kolejka kolejka(wierzcholki);
    odwiedzone[n] = n;
    odleglosc[n] = 0;
    int wynik = 0;
    kolejka.push(n);
    int licznik = 1;
    while (!kolejka.pusty() && licznik<wierzcholki) 
    {
        int v = kolejka.pop();
        for (int u : graf[v]) 
        {
            if (odwiedzone[u] !=n) 
            {
                odwiedzone[u] = n;
                odleglosc[u] = odleglosc[v] + 1;
                kolejka.push(u);
                ++licznik;
                wynik = odleglosc[u];
            }
        }
    }
    return wynik;
}
void acentrycznosc(vector graf[], int wierzcholki)
{
    int* odwiedzone = new int[wierzcholki + 3];
    int* odleglosc = new int[wierzcholki + 3];
    for (int i = 1; i <= wierzcholki; ++i)
    odwiedzone[i] = -1;
    for (int start = 1; start <= wierzcholki; ++start)
        cout<<BFS(graf, start, wierzcholki, odwiedzone, odleglosc)<<" ";
    delete[] odwiedzone;
    delete[] odleglosc;
}
char dwudzielnosc(vector graf[], int wiercholki)
{
    int* odwiedzone = new int[wiercholki + 3];
    for (int i = 0; i <= wiercholki + 1; ++i)
        odwiedzone[i] = 0;

    for (int i = 1; i <= wiercholki; ++i)
        if (odwiedzone[i] == 0)
            if (DFS_dwudzielnosc(graf, i, odwiedzone, 1) == 0)
            {
                delete[] odwiedzone;
                return 'F';
            }

    delete[] odwiedzone;
    return 'T';
}
int liczba_skladowych_spojnosci(int wierzcholki, vector graf[])
{
    int* odwiedzone = new int[wierzcholki + 3];
    for (int i = 0; i <= wierzcholki + 1; i++)
        odwiedzone[i] = 0;
    int ilosc = 0;
    for (int i = 1; i <= wierzcholki; i++)
        if (odwiedzone[i] == 0)
        {
            ilosc++;
            DFS(wierzcholki, i, odwiedzone, graf);
        }

    delete[] odwiedzone;
    return ilosc;
}
void ciag_stopniowy(int wierzchoki, vector graf[])
{
    int* tb = new int[wierzchoki + 1];
    int* pom = new int[wierzchoki + 1];
    for (int i = 1; i <= wierzchoki; i++)
        tb[i] = graf[i].size();

    mergesort(tb, pom, 1, wierzchoki);
    for (int i = wierzchoki; i > 0; i--)
        cout << tb[i] << " ";


    delete[]tb;
    delete[]pom;
}
long long krawedzie_dopelnienia(int wiercholki, vector graf[])
{
    long long w = wiercholki;
    long long graf_pelny = w * (w - 1) / 2;
    long long graf_org = 0;
    for (int i = 1; i <= wiercholki; ++i)
    {
        graf_org += graf[i].size();
    }
    return (graf_pelny - (graf_org / 2));
}
void kolorowanie_naiwne(int wierzcholki, vector graf[]) {
    int* kolory = new int[wierzcholki + 1];
    int wielkosc = 0;

    for (int i = 1; i <= wierzcholki; ++i) {
        if (graf[i].size() > wielkosc)
            wielkosc = graf[i].size();
    }
    int* dostepne_kolory = new int[wielkosc + 1];

    for (int i = 0; i <= wierzcholki; ++i)
        kolory[i] = -1;

    kolory[1] = 1;

    for (int i = 2; i <= wierzcholki; ++i) {
        for (int j = 1; j <= wielkosc; j++)
            dostepne_kolory[j] = 1;

        for (int j : graf[i]) {
            if (kolory[j] != -1)
                dostepne_kolory[kolory[j]] = 0;
        }
        int licznik = 1;
        while (!dostepne_kolory[licznik] && licznik <= wielkosc)
            ++licznik;
        kolory[i] = licznik;
    }

    cout << endl;
    for (int i = 1; i <= wierzcholki; ++i)
        cout << kolory[i] << " ";


    delete[] kolory;
    delete[] dostepne_kolory;
}
void kolorowanie_lf(int wierzcholki, vector graf[])
{
    Pair* para = new Pair[wierzcholki + 1];
    Pair* pom = new Pair[wierzcholki + 1];
    for (int i = 1; i <= wierzcholki; ++i)
    {
        para[i].first = graf[i].size() * (-1);
        para[i].second = i;
    }
    mergesort(para, pom, 1, wierzcholki);

    int* kolory = new int[wierzcholki + 1];
    int wielkosc = 0;

    for (int i = 1; i <= wierzcholki; ++i)
    {
        if (graf[i].size() > wielkosc)
            wielkosc = graf[i].size();
    }
    int* dostepne_kolory = new int[wielkosc + 1];

    for (int i = 0; i <= wierzcholki; ++i)
        kolory[i] = -1;

    kolory[para[1].second] = 1;

    for (int i = 2; i <= wierzcholki; ++i) {
        for (int j = 1; j <= wielkosc; j++)
            dostepne_kolory[j] = 1;

        for (int j : graf[para[i].second]) {
            if (kolory[j] != -1)
                dostepne_kolory[kolory[j]] = 0;
        }
        int licznik = 1;
        while (!dostepne_kolory[licznik] && licznik <= wielkosc)
            ++licznik;
        kolory[para[i].second] = licznik;
    }

    cout << endl;
    for (int i = 1; i <= wierzcholki; ++i)
        cout << kolory[i] << " ";


    delete[] kolory;
    delete[] dostepne_kolory;
}
int main()
{
    int ile_grafow;
    cin >> ile_grafow;
    for (int k = 0; k < ile_grafow; k++)
    {
        int wierzchoki;
        int ile_polaczen;
        int pom = 1;

        cin >> wierzchoki;
        vector* graf = new vector[wierzchoki + 10];
        for (int i = 0; i < wierzchoki; ++i)
        {
            cin >> ile_polaczen;
            for (int j = 0; j < ile_polaczen; ++j)
            {
                int polaczenie;
                cin >> polaczenie;
                graf[pom].push_back(polaczenie);
            }
            pom++;
        }
        ciag_stopniowy(wierzchoki, graf);
        cout << endl << liczba_skladowych_spojnosci(wierzchoki, graf);
        cout << endl << dwudzielnosc(graf, wierzchoki);
        cout << endl;
        acentrycznosc(graf, wierzchoki);
        cout  << endl << '?';
        kolorowanie_naiwne(wierzchoki, graf);
        kolorowanie_lf(wierzchoki, graf);
        cout << endl << '?' << endl << '?' << endl;
        cout << krawedzie_dopelnienia(wierzchoki, graf) << endl;

        delete[] graf;
    }
}