#include<iostream>
#include <string>
#include <vector>
#include <fstream>
#include<algorithm>
using namespace std;
#define MAX_SIZE 300
#define MAX_GRAF 30
void DFS_czerwony(int v, int odwiedzone[MAX_SIZE], vector <int> polaczenia[MAX_SIZE], int indeksowanie, int wielkosc, char& kto, char graf[MAX_GRAF][MAX_GRAF])
{
    if (v >= indeksowanie - wielkosc)
    {
        kto = 'r';
        return;
    }
    odwiedzone[v] = 1;
    for (int j : polaczenia[v])
        if (odwiedzone[j] == 0 && graf[j / wielkosc][j % wielkosc] == 'r')
            DFS_czerwony(j, odwiedzone, polaczenia, indeksowanie, wielkosc, kto, graf);
}
void DFS_niebieski(int v, int odwiedzone[MAX_SIZE], vector <int> polaczenia[MAX_SIZE], int indeksowanie, int wielkosc, char& kto, char graf[MAX_GRAF][MAX_GRAF])
{
    if (v % wielkosc == wielkosc - 1)
    {
        kto = 'b';
        return;
    }
    odwiedzone[v] = 1;
    for (int j : polaczenia[v])
        if (odwiedzone[j] == 0 && graf[j / wielkosc][j % wielkosc] == 'b')
            DFS_niebieski(j, odwiedzone, polaczenia, indeksowanie, wielkosc, kto, graf);
}
void Wielkosc_Planszy(string input, int& BOARD_SIZE)
{
    if ((input[0] < 'A' || input[0]>'Z') && input[0] != '\0')
        BOARD_SIZE++;

    if (input[0] == 'B')
    {
        cout << BOARD_SIZE / 2 << endl;
        BOARD_SIZE = 0;
    }
    else if (input[0] > 'A' && input[0] < 'Z')
        BOARD_SIZE = 0;
}
void Ilosc_Pionkow(string input, int& PAWNS)
{
    if (input[0] == 'P')
    {
        cout << PAWNS << endl;
        PAWNS = 0;
    }
    int pom = 0;
    while (input[pom] != '\0')
    {
        if (input[pom] >= 'a' && input[pom] <= 'z')
            PAWNS++;
        pom++;
    }
}
void Ilosc_Czerwonych(string input, int& CZERWONE)
{
    int pom = 0;
    while (input[pom] != '\0')
    {
        if (input[pom] == 'r')
            CZERWONE++;
        pom++;
    }
}
void Ilosc_Niebieskich(string input, int& NIEBIESKIE)
{

    int pom = 0;
    while (input[pom] != '\0')
    {
        if (input[pom] == 'b')
            NIEBIESKIE++;
        pom++;
    }
}
int Board_Correct(int& NIEBIESKIE, int& CZERWONE)
{
    if (CZERWONE == NIEBIESKIE || CZERWONE - 1 == NIEBIESKIE)
        return 1;
    else
        return 0;
}
void zerowanie_grafu(char graf_pom[MAX_GRAF][MAX_GRAF], int wielkosc)
{
    for (int i = 0; i < wielkosc * 2 - 1; i++)
        for (int j = 0; j < wielkosc * 2 - 1; j++)
            graf_pom[i][j] = ' ';
}
void wypelnienie_grafu_orginalnego(int& pom, int& licznikliter, int& start, int wielkosc, char litery[MAX_SIZE], char graf_pom[MAX_GRAF][MAX_GRAF])
{
    while (pom <= licznikliter)
    {
        for (int i = 0; i < wielkosc; i++)
        {
            int pozycja = start;
            for (int j = 0; j < i + 1; j++)
            {
                graf_pom[i][pozycja] = litery[pom];
                pozycja += 2;
                pom++;

            }
            start--;
        }
        start = 1;
        for (int i = wielkosc; i < wielkosc * 2 - 1; i++)
        {
            int pozycja = start;
            for (int j = wielkosc * 2 - 1 - i; j > 0; j--)
            {
                graf_pom[i][pozycja] = litery[pom];
                pozycja += 2;
                pom++;
            }
            start++;
        }

    }
}
void wypelnienie_noweg_grafu(int wielkosc, char graf[MAX_GRAF][MAX_GRAF], char graf_pom[MAX_GRAF][MAX_GRAF])
{
    int wiersz = wielkosc - 1;
    int kolumna = 0;
    for (int i = 0; i < wielkosc; i++)
    {
        int pom_wiersz = wiersz;
        int pom_kolumna = kolumna;
        for (int j = 0; j < wielkosc; j++)
        {
            graf[i][j] = graf_pom[pom_wiersz][pom_kolumna];
            pom_kolumna += 1;
            pom_wiersz -= 1;
        }
        wiersz++;
        kolumna++;
    }
}
void zamiana_grafu(string input, char litery[MAX_SIZE], char graf_pom[MAX_GRAF][MAX_GRAF], char graf[MAX_GRAF][MAX_GRAF], int& licznikliter, int& wielkosc_planszy)
{

    if (input[0] >= 'A' && input[0] <= 'Z' && licznikliter >= 0)
    {
        int wielkosc = sqrt(licznikliter + 1);
        int pom = 0;
        int start = wielkosc - 1;
        wielkosc_planszy = wielkosc;
        zerowanie_grafu(graf_pom, wielkosc);
        wypelnienie_grafu_orginalnego(pom, licznikliter, start, wielkosc, litery, graf_pom);
        wypelnienie_noweg_grafu(wielkosc, graf, graf_pom);
        for (int i = 0; i <= licznikliter; ++i)
            litery[i] = 'x';
        licznikliter = -1;
    }
    else
    {
        for (int i = 0; i < 80; i++)
        {
            if (input[i] == '\0')
                break;
            if (input[i] == '<')
                licznikliter++;
            if (input[i] > 'a' && input[i] < 'z')
                litery[licznikliter] = input[i];
            if (input[i] == '>')
                if (litery[licznikliter] != 'b' && litery[licznikliter] != 'r')
                    litery[licznikliter] = 'x';
        }
    }
}
void tworzenie_polaczenia(string input, int wielkosc_planszy, char graf[MAX_GRAF][MAX_GRAF], vector <int> polaczenia[500], int& indeksowanie)
{
    if (input[0] >= 'A' && input[0] <= 'Z')
    {
        indeksowanie = 0;
        for (int i = 0; i < wielkosc_planszy; i++)
        {
            for (int j = 0; j < wielkosc_planszy; j++)
            {
                if (j + 1 < wielkosc_planszy)
                    polaczenia[indeksowanie].push_back(indeksowanie + 1);
                if (j - 1 >= 0)
                    polaczenia[indeksowanie].push_back(indeksowanie - 1);
                if (i - 1 >= 0)
                    polaczenia[indeksowanie].push_back(indeksowanie - wielkosc_planszy);
                if (i + 1 < wielkosc_planszy)
                    polaczenia[indeksowanie].push_back(indeksowanie + wielkosc_planszy);
                if (i + 1 < wielkosc_planszy && j - 1 >= 0)
                    polaczenia[indeksowanie].push_back(indeksowanie + wielkosc_planszy - 1);
                if (j + 1 < wielkosc_planszy && i - 1 >= 0)
                    polaczenia[indeksowanie].push_back(indeksowanie - wielkosc_planszy + 1);
                indeksowanie++;
            }
        }
    }
}
char Kto_wygral(int wielkosc_planszy, char graf[MAX_GRAF][MAX_GRAF], vector <int> polaczenia[MAX_SIZE], int& indeksowanie, int odwiedzone[MAX_SIZE], char& kto, int NIEBIESKIE, int CZERWONE)
{
    for (int i = 0; i < indeksowanie; i++)
        odwiedzone[i] = 0;
    kto = ' ';
    for (int i = 0; i < wielkosc_planszy; i++)
    {
        if (graf[0][i] == 'r')
            DFS_czerwony(i, odwiedzone, polaczenia, indeksowanie, wielkosc_planszy, kto, graf);
    }
    for (int i = 0; i < wielkosc_planszy; i++)
    {
        if (graf[i][0] == 'b')
            DFS_niebieski(i * wielkosc_planszy, odwiedzone, polaczenia, indeksowanie, wielkosc_planszy, kto, graf);
    }
    if (kto == ' ' || Board_Correct(NIEBIESKIE, CZERWONE) == 0)
        return 'n';
    else if (kto == 'r')
        return 'r';
    else
        return 'b';

}
int czy_red_win(int wielkosc_planszy, char graf[MAX_GRAF][MAX_GRAF], vector <int> polaczenia[MAX_SIZE], int& indeksowanie, int odwiedzone[MAX_SIZE], char& kto)
{
    for (int i = 0; i < indeksowanie; i++)
        odwiedzone[i] = 0;

    kto = ' ';
    for (int i = 0; i < wielkosc_planszy; i++)
    {
        if (graf[0][i] == 'r')
            DFS_czerwony(i, odwiedzone, polaczenia, indeksowanie, wielkosc_planszy, kto, graf);
    }
    if (kto == 'r')
        return 1;
    else
        return 0;
}
int czy_blue_win(int wielkosc_planszy, char graf[MAX_GRAF][MAX_GRAF], vector <int> polaczenia[MAX_SIZE], int& indeksowanie, int odwiedzone[MAX_SIZE], char& kto)
{
    for (int i = 0; i < indeksowanie; i++)
        odwiedzone[i] = 0;

    kto = ' ';
    for (int i = 0; i < wielkosc_planszy; i++)
    {
        if (graf[i][0] == 'b')
            DFS_niebieski(i * wielkosc_planszy, odwiedzone, polaczenia, indeksowanie, wielkosc_planszy, kto, graf);
    }
    if (kto == 'b')
        return 1;
    else
        return 0;
}
int czy_dwoch_wygranych(int wielkosc_planszy, char graf[MAX_GRAF][MAX_GRAF], vector <int> polaczenia[MAX_SIZE], int& indeksowanie, int odwiedzone[MAX_SIZE], char& kto, int NIEBIESKIE, int CZERWONE)
{
    for (int i = 0; i < indeksowanie; i++)
        odwiedzone[i] = 0;
    int czy_red = 0;
    int czy_blue = 0;
    for (int i = 0; i < wielkosc_planszy; i++)
    {
        if (graf[0][i] == 'r')
            DFS_czerwony(i, odwiedzone, polaczenia, indeksowanie, wielkosc_planszy, kto, graf);
    }
    if (kto == 'r')
        czy_red = 1;
    for (int i = 0; i < wielkosc_planszy; i++)
    {
        if (graf[i][0] == 'b')
            DFS_niebieski(i * wielkosc_planszy, odwiedzone, polaczenia, indeksowanie, wielkosc_planszy, kto, graf);
    }
    if (kto == 'b')
        czy_blue = 1;
    return czy_blue + czy_red;

}
int IS_GAME_POSSIBLE(int wielkosc_planszy, char graf[MAX_GRAF][MAX_GRAF], vector <int> polaczenia[MAX_SIZE], int& indeksowanie, int odwiedzone[MAX_SIZE], char& kto, int NIEBIESKIE, int CZERWONE)
{
    if (Board_Correct(NIEBIESKIE, CZERWONE) == 0)
        return 0;

    if (czy_dwoch_wygranych(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) == 2)
        return 0;

    char zwyciezca = Kto_wygral(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE);
    if (zwyciezca == 'r')
        if (CZERWONE - NIEBIESKIE != 1)
            return 0;
    if (zwyciezca == 'b')
        if (CZERWONE - NIEBIESKIE != 0)
            return 0;
    if (zwyciezca == 'n')
        return 1;

    for (int i = 0; i < wielkosc_planszy; i++)
        for (int j = 0; j < wielkosc_planszy; j++)
        {
            if (graf[i][j] == zwyciezca)
            {
                graf[i][j] = 'x';
                if (Kto_wygral(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) != zwyciezca)
                {
                    graf[i][j] = zwyciezca;
                    return 1;
                }
                graf[i][j] = zwyciezca;

            }
        }
    return 0;

}
char kogo_ruch(int NIEBIESKIE, int CZERWONE)
{
    if (CZERWONE > NIEBIESKIE)
        return 'b';
    else
        return 'r';
}
int can_red_1_naive(int wielkosc_planszy, char graf[MAX_GRAF][MAX_GRAF], vector <int> polaczenia[MAX_SIZE], int& indeksowanie, int odwiedzone[MAX_SIZE], char& kto, int NIEBIESKIE, int CZERWONE)
{
    if (IS_GAME_POSSIBLE(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) == 0)
        return 0;
    if (Kto_wygral(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) != 'n')
        return 0;
    int wolnemiejsca = 0;

    for (int i = 0; i < wielkosc_planszy; ++i)
    {
        for (int j = 0; j < wielkosc_planszy; j++)
        {
            if (graf[i][j] == 'x')
                wolnemiejsca++;
        }
    }
    if (wolnemiejsca < 1)
        return 0;

    if (kogo_ruch(NIEBIESKIE, CZERWONE) == 'b')
    {
        if (wolnemiejsca < 2)
            return 0;
    }


    for (int i = 0; i < wielkosc_planszy; ++i)
        for (int j = 0; j < wielkosc_planszy; j++)
        {
            if (graf[i][j] == 'x')
            {
                char poprzednia = graf[i][j];
                graf[i][j] = 'r';
                if (Kto_wygral(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) == 'r')
                {
                    graf[i][j] = poprzednia;
                    return 1;
                }
                graf[i][j] = poprzednia;
            }

        }
    return 0;
}
int can_blue_1_naive(int wielkosc_planszy, char graf[MAX_GRAF][MAX_GRAF], vector <int> polaczenia[MAX_SIZE], int& indeksowanie, int odwiedzone[MAX_SIZE], char& kto, int NIEBIESKIE, int CZERWONE)
{
    if (IS_GAME_POSSIBLE(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) == 0)
        return 0;
    if (Kto_wygral(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) != 'n')
        return 0;
    int wolnemiejsca = 0;

    for (int i = 0; i < wielkosc_planszy; ++i)
        for (int j = 0; j < wielkosc_planszy; j++)
        {
            if (graf[i][j] == 'x')
                wolnemiejsca++;
        }
    if (wolnemiejsca < 1)
        return 0;
    if (kogo_ruch(NIEBIESKIE, CZERWONE) == 'r')
    {
        if (wolnemiejsca < 2)
            return 0;
    }
    for (int i = 0; i < wielkosc_planszy; ++i)
        for (int j = 0; j < wielkosc_planszy; j++)
        {
            if (graf[i][j] == 'x')
            {
                char poprzednia = graf[i][j];
                graf[i][j] = 'b';
                if (Kto_wygral(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) == 'b')
                {
                    graf[i][j] = poprzednia;
                    return 1;
                }
                graf[i][j] = poprzednia;
            }

        }
    return 0;
}
int can_red_2_naive(int wielkosc_planszy, char graf[MAX_GRAF][MAX_GRAF], vector <int> polaczenia[MAX_SIZE], int& indeksowanie, int odwiedzone[MAX_SIZE], char& kto, int NIEBIESKIE, int CZERWONE)
{
    if (IS_GAME_POSSIBLE(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) == 0)
        return 0;
    if (Kto_wygral(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) != 'n')
        return 0;
    int wolnemiejsca = 0;

    for (int i = 0; i < wielkosc_planszy; ++i)
        for (int j = 0; j < wielkosc_planszy; j++)
        {
            if (graf[i][j] == 'x')
                wolnemiejsca++;
        }
    if (wolnemiejsca < 3)
        return 0;
    if (kogo_ruch(NIEBIESKIE, CZERWONE) == 'b')
    {
        if (wolnemiejsca < 4)
            return 0;
    }

    for (int i = 0; i < wielkosc_planszy; ++i)
    {
        for (int j = 0; j < wielkosc_planszy; j++)
        {
            if (graf[i][j] == 'x')
            {
                char poprzednia = graf[i][j];
                graf[i][j] = 'r';
                if (Kto_wygral(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) == 'r')
                {
                    graf[i][j] = poprzednia;
                }
                else
                {
                    for (int k = 0; k < wielkosc_planszy; ++k)
                    {
                        for (int q = 0; q < wielkosc_planszy; q++)
                        {
                            if (graf[k][q] == 'x')
                            {
                                char poprzednia2 = graf[k][q];
                                graf[k][q] = 'r';

                                if (Kto_wygral(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) == 'r')
                                {
                                    graf[i][j] = poprzednia;
                                    graf[k][q] = poprzednia2;
                                    return 1;
                                }
                                graf[k][q] = poprzednia2;
                            }
                        }
                    }
                    graf[i][j] = poprzednia;
                }
            }
        }
    }
    return 0;
}
int can_blue_2_naive(int wielkosc_planszy, char graf[MAX_GRAF][MAX_GRAF], vector <int> polaczenia[MAX_SIZE], int& indeksowanie, int odwiedzone[MAX_SIZE], char& kto, int NIEBIESKIE, int CZERWONE)
{
    if (IS_GAME_POSSIBLE(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) == 0)
        return 0;
    if (Kto_wygral(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) != 'n')
        return 0;
    int wolnemiejsca = 0;

    for (int i = 0; i < wielkosc_planszy; ++i)
        for (int j = 0; j < wielkosc_planszy; j++)
        {
            if (graf[i][j] == 'x')
                wolnemiejsca++;
        }
    if (wolnemiejsca < 3)
        return 0;
    if (kogo_ruch(NIEBIESKIE, CZERWONE) == 'r')
    {
        if (wolnemiejsca < 4)
            return 0;
    }

    for (int i = 0; i < wielkosc_planszy; ++i)
    {
        for (int j = 0; j < wielkosc_planszy; j++)
        {
            if (graf[i][j] == 'x')
            {
                char poprzednia = graf[i][j];
                graf[i][j] = 'b';
                if (Kto_wygral(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) == 'b')
                {
                    graf[i][j] = poprzednia;
                }
                else
                {
                    for (int k = 0; k < wielkosc_planszy; ++k)
                    {
                        for (int q = 0; q < wielkosc_planszy; q++)
                        {
                            if (graf[k][q] == 'x')
                            {
                                char poprzednia2 = graf[k][q];
                                graf[k][q] = 'b';
                                if (Kto_wygral(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) == 'b')
                                {
                                    graf[i][j] = poprzednia;
                                    graf[k][q] = poprzednia2;
                                    return 1;
                                }
                                graf[k][q] = poprzednia2;
                            }
                        }
                    }
                    graf[i][j] = poprzednia;
                }
            }

        }
    }
    return 0;
}
int ile_wolnych_miejsc(int wielkosc_planszy, char graf[MAX_GRAF][MAX_GRAF])
{
    int wolnemiejsca = 0;

    for (int i = 0; i < wielkosc_planszy; ++i)
        for (int j = 0; j < wielkosc_planszy; j++)
            if (graf[i][j] == 'x')
                wolnemiejsca++;
    return wolnemiejsca;
}
int minimax(int wielkosc_planszy, char graf[MAX_GRAF][MAX_GRAF], vector <int> polaczenia[MAX_SIZE], int& indeksowanie, int odwiedzone[MAX_SIZE], char& kto, int NIEBIESKIE, int CZERWONE, char kogo_kolej, int ilosc_ruchow, char zwyciezca, int alfa, int beta)
{
    if (ilosc_ruchow == 0)
    {
        if (zwyciezca == 'r')
            return czy_red_win(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto);
        else
            return czy_blue_win(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto);

    }

    if (kogo_kolej == 'r')
        if (czy_blue_win(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto) == 1)
            return 0;
    if (kogo_kolej == 'b')
        if (czy_red_win(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto) == 1)
            return 0;
    if (kogo_kolej == zwyciezca)
    {
        int maxilosc = -3;
        for (int i = 0; i < wielkosc_planszy; i++)
        {
            for (int j = 0; j < wielkosc_planszy; j++)
            {
                if (graf[i][j] == 'x')
                {
                    graf[i][j] = zwyciezca;
                    int ocenapozycji = 0;
                    if (zwyciezca == 'r')
                        ocenapozycji = minimax(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE, 'b', ilosc_ruchow - 1, zwyciezca, alfa, beta);
                    else
                        ocenapozycji = minimax(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE, 'r', ilosc_ruchow - 1, zwyciezca, alfa, beta);
                    graf[i][j] = 'x';
                    maxilosc = max(maxilosc, ocenapozycji);
                    alfa = max(alfa, ocenapozycji);
                    if (beta <= alfa)
                        break;

                }
            }
        }
        return maxilosc;
    }
    else
    {
        int minilosc = 3;
        for (int i = 0; i < wielkosc_planszy; i++)
        {
            for (int j = 0; j < wielkosc_planszy; j++)
            {
                if (graf[i][j] == 'x')
                {
                    graf[i][j] = kogo_kolej;
                    int ocenapozycji = 0;
                    if (kogo_kolej == 'r')
                        ocenapozycji = minimax(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE, 'b', ilosc_ruchow - 1, zwyciezca, alfa, beta);
                    else
                        ocenapozycji = minimax(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE, 'r', ilosc_ruchow - 1, zwyciezca, alfa, beta);
                    graf[i][j] = 'x';
                    minilosc = min(minilosc, ocenapozycji);
                    beta = min(beta, ocenapozycji);
                    if (beta <= alfa)
                        break;
                }
            }
        }
        return minilosc;
    }
}
void perfect(string& input, int wielkosc_planszy, char graf[MAX_GRAF][MAX_GRAF], vector <int> polaczenia[MAX_SIZE], int& indeksowanie, int odwiedzone[MAX_SIZE], char& kto, int NIEBIESKIE, int CZERWONE)
{
    if (input == "CAN_RED_WIN_IN_1_MOVE_WITH_PERFECT_OPPONENT")
    {

        if (IS_GAME_POSSIBLE(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) == 1)
        {
            char ktozaczyna = kogo_ruch(NIEBIESKIE, CZERWONE);
            if (ktozaczyna == 'b')
            {
                if (ile_wolnych_miejsc(wielkosc_planszy, graf) < 2)
                    cout << "NO" << endl;
                else if (minimax(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE, ktozaczyna, 2, 'r', -5, 10) == 1)
                    cout << "YES" << endl;
                else
                    cout << "NO" << endl;
            }
            else
            {
                if (ile_wolnych_miejsc(wielkosc_planszy, graf) < 1)
                    cout << "NO" << endl;

                else if (minimax(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE, ktozaczyna, 1, 'r', -5, 10) == 1)
                    cout << "YES" << endl;
                else
                    cout << "NO" << endl;
            }
        }
        else
            cout << "NO" << endl;

    }
    if (input == "CAN_BLUE_WIN_IN_1_MOVE_WITH_PERFECT_OPPONENT")
    {
        if (IS_GAME_POSSIBLE(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) == 1)
        {
            char ktozaczyna = kogo_ruch(NIEBIESKIE, CZERWONE);
            if (ktozaczyna == 'b')
                if (ile_wolnych_miejsc(wielkosc_planszy, graf) < 1)
                    cout << "NO" << endl;

                else if (minimax(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE, ktozaczyna, 1, 'b', -5, 10) == 1)
                    cout << "YES" << endl;
                else
                    cout << "NO" << endl;
            else
                if (ile_wolnych_miejsc(wielkosc_planszy, graf) < 2)
                    cout << "NO" << endl;

                else if (minimax(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE, ktozaczyna, 2, 'b', -5, 10) == 1)
                    cout << "YES" << endl;
                else
                    cout << "NO" << endl;
        }
        else
            cout << "NO" << endl;
    }
    if (input == "CAN_RED_WIN_IN_2_MOVES_WITH_PERFECT_OPPONENT")
    {
        if (IS_GAME_POSSIBLE(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) == 1)
        {
            char ktozaczyna = kogo_ruch(NIEBIESKIE, CZERWONE);
            if (ktozaczyna == 'b')
                if (ile_wolnych_miejsc(wielkosc_planszy, graf) < 4)
                    cout << "NO" << endl;
                else if (minimax(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE, ktozaczyna, 4, 'r', -5, 10) == 1)
                    cout << "YES" << endl;
                else
                    cout << "NO" << endl;
            else
                if (ile_wolnych_miejsc(wielkosc_planszy, graf) < 3)
                    cout << "NO" << endl;
                else if (minimax(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE, ktozaczyna, 3, 'r', -5, 10) == 1)
                    cout << "YES" << endl;
                else
                    cout << "NO" << endl;
        }
        else
            cout << "NO" << endl;
    }
    if (input == "CAN_BLUE_WIN_IN_2_MOVES_WITH_PERFECT_OPPONENT")
    {
        if (IS_GAME_POSSIBLE(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) == 1)
        {
            char ktozaczyna = kogo_ruch(NIEBIESKIE, CZERWONE);
            if (ktozaczyna == 'b')
                if (ile_wolnych_miejsc(wielkosc_planszy, graf) < 3)
                    cout << "NO" << endl;
                else if (minimax(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE, ktozaczyna, 3, 'b', -5, 10) == 1)
                    cout << "YES" << endl;
                else
                    cout << "NO" << endl;
            else
                if (ile_wolnych_miejsc(wielkosc_planszy, graf) < 4)
                    cout << "NO" << endl;
                else if (minimax(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE, ktozaczyna, 4, 'b', -5, 10) == 1)
                    cout << "YES" << endl;
                else
                    cout << "NO" << endl;
        }
        else
            cout << "NO" << endl;
    }

}
int main()
{
    fstream dane;
    string input;
    int BOARD_SIZE = 0;
    int PAWNS = 0;
    int CZERWONE = 0;
    int NIEBIESKIE = 0;
    int licznikliter = -1;
    int wielkosc_planszy = 0;
    int pom = 0;
    char kto = ' ';
    int indeksowanie = 0;
    char graf[MAX_GRAF][MAX_GRAF];
    char graf_pom[MAX_GRAF][MAX_GRAF];
    char litery[MAX_SIZE];
    int odwiedzone[MAX_SIZE];
    vector <int> polaczenia[MAX_SIZE];
    int czy_pytanie = 0;

    //  dane.open("q.txt");
    while (getline(cin, input))
    {
        if (input[0] >= 'A' && input[0] <= 'Z')
            czy_pytanie = 1;
        if (input[0] < 'A' || input[0]>'Z')
        {
            if (czy_pytanie == 1)
            {
                for (int i = 0; i < indeksowanie; i++)
                    odwiedzone[i] = 0;
                CZERWONE = 0;
                NIEBIESKIE = 0;
            }
            czy_pytanie = 0;
        }
        Wielkosc_Planszy(input, BOARD_SIZE);
        Ilosc_Pionkow(input, PAWNS);
        Ilosc_Czerwonych(input, CZERWONE);
        Ilosc_Niebieskich(input, NIEBIESKIE);
        if (input == "IS_BOARD_CORRECT")
        {
            if (Board_Correct(NIEBIESKIE, CZERWONE) == 1)
                cout << "YES" << endl;
            else
                cout << "NO" << endl;
        }
        zamiana_grafu(input, litery, graf_pom, graf, licznikliter, wielkosc_planszy);
        tworzenie_polaczenia(input, wielkosc_planszy, graf, polaczenia, indeksowanie);
        if (input == "IS_GAME_OVER")
        {
            char odp = Kto_wygral(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE);
            if (odp == 'n')
                cout << "NO" << endl;
            else if (odp == 'r')
                cout << "YES RED" << endl;
            else if (odp == 'b')
                cout << "YES BLUE" << endl;
        }
        if (input == "IS_BOARD_POSSIBLE")
        {
            pom++;
            if (IS_GAME_POSSIBLE(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) == 1)
                cout << "YES" << endl << endl;
            else
                cout << "NO" << endl << endl;
        }
        if (input == "CAN_RED_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT")
        {
            if (can_red_1_naive(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) == 1)
                cout << "YES" << endl;
            else
                cout << "NO" << endl;
        }
        if (input == "CAN_BLUE_WIN_IN_1_MOVE_WITH_NAIVE_OPPONENT")
        {
            if (can_blue_1_naive(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) == 1)
                cout << "YES" << endl;
            else
                cout << "NO" << endl;
        }
        if (input == "CAN_RED_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT")
        {
            if (can_red_2_naive(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) == 1)
                cout << "YES" << endl;
            else
                cout << "NO" << endl;
        }
        if (input == "CAN_BLUE_WIN_IN_2_MOVES_WITH_NAIVE_OPPONENT")
        {
            if (can_blue_2_naive(wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE) == 1)
                cout << "YES" << endl;
            else
                cout << "NO" << endl;
        }
        perfect(input, wielkosc_planszy, graf, polaczenia, indeksowanie, odwiedzone, kto, NIEBIESKIE, CZERWONE);

        for (int i = 0; i < indeksowanie; i++)
            polaczenia[i].clear();

    }
    return 0;
}