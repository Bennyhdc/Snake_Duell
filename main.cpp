#include "std_lib_inc.h";

class Bad_argument{};
class Bad_input{};

class Schlangenglied
{
public:
    int x, y;
};

class Point2d
{
public:
    int px, py;
};


// Stellt den aktuellen Spielzustand dar
class Spielzustand
{
public:
    int spielbreite;
    int spielhoehe;
    int punktzahl;
    bool gameOver;
    bool gegessen;
    int lebenspieler;
    int lebennpc;
    Point2d futter;
    vector<Schlangenglied> python;
    vector<Schlangenglied> npc;

    void calcpos(char a, int breite_in, int hoehe_in);
};

int breite = 0;
int breite_in = 0;
int hoehe = 0;
int hoehe_in = 0;
char eing = 0;
bool schlangenpixel = false;
bool zeichnen_erlaubt = true;

// testet, die Eingabe nach Fehlern
int wert_abfragen() {
    int wert = 0;
    cin >> wert;
    if (!cin) {
        throw Bad_input();
    }
    if (wert < 3 || wert > 25) {
        throw Bad_argument();
    }
    return wert + 2;
}
void Spielzustand::calcpos(char a, int breite_in, int hoehe_in) {
    if (a == 'n') {
        npc[0].x = ((python[0].x + lebenspieler * 2) % breite_in) + 1;
        npc[0].y = ((python[0].y + lebenspieler * 3) % hoehe_in) + 1;
    }
    else if (a == 'f') {
        futter.px = ((17 * python[0].x) % breite_in) + 1;
        futter.py = ((13 * python[0].y) % hoehe_in) + 1;
    }
}
// gibt entweder Fehlernmeldung oder gibt den Wert weiter
int input_test() {
    int wert = 0;
    try {
        wert = wert_abfragen();
        return wert;
    }

    catch(Bad_argument&) {
        cout << "Fehler: Eingabe ausserhalb des zulaessigen Bereiches.\n";
        exit(1);
    }
    catch(Bad_input&) {
        cout << "Fehler: Programm wegen fehlender Spielfeldeingabe beendet.\n";
        exit(1);
    }
}


void input_verarbeiten(Spielzustand& spielzustand, char eing) {
    switch (eing) {
        case 'a': -- spielzustand.python[0].x;
            break;
        case 'd': ++ spielzustand.python[0].x;
            break;
        case 'w': -- spielzustand.python[0].y;
            break;
        case 's': ++ spielzustand.python[0].y;
            break;
        case 'q': spielzustand.gameOver = true;
            break;
    }
}
void waende_zeichnen (int breit, int hoch) {
    if (breit == 0 || breit == breite - 1) {
        cout << '|';
    }
    else if (hoch == 0 || hoch == hoehe - 1) {
        if (breit > 0 && breit < breite - 1) {
            cout << '-';
        }
    }
}

void schlange_zeichnen (Spielzustand &spielzustand, int breit, int hoch) {
    for (int k = 0; k < spielzustand.python.size(); ++k) {
        if (spielzustand.python[k].x == breit && spielzustand.python[k].y == hoch) {
            schlangenpixel = true;
            if (k==0) {
                cout << 'O';
            }
            else {
                cout << 'o';
            }
        }
    }
}
void npcschlange_zeichnen (Spielzustand &spielzustand, int breit, int hoch) {
    spielzustand.calcpos('n', breite_in, hoehe_in);
    for (int k = 0; k < spielzustand.npc.size(); ++k) {
        if (spielzustand.npc[k].x == breit && spielzustand.npc[k].y == hoch) {
            schlangenpixel = true;
            if (k==0) {
                cout << 'X';
            }
            else {
                cout << 'x';
            }
        }
    }
}

// testet, ob der Schlangenkopf die Wand berührt hat
void wand_beruehrt(Spielzustand& spielzustand) {
    if (spielzustand.python[0].x >= breite - 1) {
        spielzustand.python[0].x = 1;
    }
    if (spielzustand.python[0].x <= 0) {
        spielzustand.python[0].x = breite - 2;
    }
    if (spielzustand.python[0].y >= hoehe - 1) {
        spielzustand.python[0].y = 1;
    }
    if (spielzustand.python[0].y <= 0) {
        spielzustand.python[0].y = hoehe - 2;
    }
}
// testet, ob falsches char eingegeben wurde
bool falsche_eingabe(char eingabe) {
    if (eingabe == 'w' || eingabe == 'a' || eingabe == 's' || eingabe == 'd' || eingabe == 'q') {
    return true;
    }
    throw Bad_argument();
}
void karte_zeichnen(Spielzustand& spielzustand) {
    for (int hoehej = 0; hoehej < hoehe; ++hoehej) {
        for (int breitej = 0; breitej < breite; ++breitej) {
            schlangenpixel = false;

            if (spielzustand.futter.px == spielzustand.python[0].x && spielzustand.futter.py == spielzustand.python[0].y) {
                spielzustand.calcpos('f', breite_in, hoehe_in);
                spielzustand.gegessen = true;
            }

            waende_zeichnen(breitej, hoehej);

            if (spielzustand.futter.px == breitej && spielzustand.futter.py == hoehej) {
                cout << '#';
            }
            else {
                schlange_zeichnen(spielzustand, breitej, hoehej);
                npcschlange_zeichnen(spielzustand, breitej, hoehej);
                if (!schlangenpixel && hoehej > 0 && hoehej <= hoehe_in && breitej > 0 && breitej <= breite_in) {
                    cout << ' ';
                }
            }
        }
        cout << '\n';
    }
}
void futter_gegessen(Spielzustand& spielzustand) {
    if (spielzustand.gegessen == true) {
        spielzustand.punktzahl += 10;
        Schlangenglied glied;
        glied.x = spielzustand.python[spielzustand.python.size() - 1].x;
        glied.y = spielzustand.python[spielzustand.python.size() - 1].y;
        spielzustand.python.push_back(glied);
        spielzustand.gegessen = false;
    }
}
void test_gameover(Spielzustand& spielzustand) {
    for (int i = 1; i < spielzustand.python.size(); ++i) {
        if (spielzustand.python[i].x == spielzustand.python[0].x && spielzustand.python[i].y == spielzustand.python[0].y) {
            spielzustand.gameOver = true;
        }
    }
    if (eing == 'q') {
        spielzustand.gameOver = true;
    }
}
void koerper_berechnen(Spielzustand& spielzustand) {
    for (int i = spielzustand.python.size() - 1; i > 0; --i) {
        spielzustand.python[i].x = spielzustand.python[i - 1].x;
        spielzustand.python[i].y = spielzustand.python[i - 1].y;
    }
}
bool wasd_fehler() {
    try {
        falsche_eingabe(eing);
        return true;
    }
    catch (Bad_argument& ) {
        cout << "Fehler: Unzulaessige Eingabe! Nutze w, a, s, d zum Bewegen oder q zum Beenden.\n";
        return false;
    }
}

int main() {
    Spielzustand spielzustand {breite, hoehe,0,false, false, 1, 1};

    breite = input_test();
    breite_in = breite - 2;
    hoehe = input_test();
    hoehe_in = hoehe - 2;

    Schlangenglied kopf {breite_in/2+1, hoehe_in/2+1};
    spielzustand.python.push_back(kopf);

    Schlangenglied kopfn {((spielzustand.python[0].x + spielzustand.lebenspieler * 2) % breite_in) + 1,
        ((spielzustand.python[0].y + spielzustand.lebenspieler * 3) % hoehe_in) + 1};
    spielzustand.npc.push_back(kopfn);

    spielzustand.calcpos('f', breite_in, hoehe_in);;

    while (spielzustand.gameOver == false) {
        if (zeichnen_erlaubt == true) {
            karte_zeichnen(spielzustand);
            futter_gegessen(spielzustand);
            cout << "Punktzahl: " << spielzustand.punktzahl << '\n';
            koerper_berechnen(spielzustand);
        }
        cin >> eing;
        zeichnen_erlaubt = wasd_fehler();

        if (zeichnen_erlaubt==true) {
            input_verarbeiten(spielzustand, eing);
            test_gameover(spielzustand);
            wand_beruehrt(spielzustand);
        }



    }
    cout << "Game Over! Gesamtpunktzahl: " << spielzustand.punktzahl << '\n';
}