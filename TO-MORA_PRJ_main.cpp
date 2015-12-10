#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <string>

using namespace std;

// Structure d'un arc
typedef struct s_arc
{
        string prec, succ;
        int val;
} t_arc;

// Structure d'un sommet pour le parcours en profondeur
typedef struct s_sommet
{
    string val;
    bool marque;
    int status;
} t_sommet;

// Structure pour une valuation de l'arc où l'extrémité initial est le sommet défini
typedef struct s_valuation
{
    string sommet;
    int val;
} t_valuation;

// Structure de la valeur Pi
typedef struct s_pi
{
        string par;
        string sommet;
        string val;
} t_pi;

// Fonction int to string
string IntToString(int a)
{
    stringstream ss;
    ss << a;
    return ss.str();
}

// Fonction string to int
int StringToInt(string a)
{
    return atoi(a.c_str());
}

// Ajout d'un arc
t_arc* AjouterArc(t_arc *arcs, int *nbArcs, t_arc e)
{
    t_arc *nouveau = new t_arc[*nbArcs + 1];

    for(int i = 0; i < *nbArcs; i++)
    {
        nouveau[i] = arcs[i];
    }
    nouveau[*nbArcs] = e;
    *nbArcs = *nbArcs + 1;

    delete arcs;
    arcs = NULL;
    return nouveau;
}

// Vérifie si un sommet est alphabétique ou numérique
bool chaineAlpha(string *sommets, int nbSommets)
{
    for(int i = 0; i < nbSommets; i++)
    {
        for(unsigned int j = 0; j < sommets[i].size(); j++)
        {
            if(!isdigit(sommets[i][j])) return true;
        }
    }
    return false;
}

// Ajout d'un sommet
string* AjouterSommet(string *sommets, int *nbSommets, string e)
{
    string *nouveau = new string[*nbSommets + 1];

    for(int i = 0; i < *nbSommets; i++)
    {
        nouveau[i] = sommets[i];
    }
    nouveau[*nbSommets] = e;
    *nbSommets = *nbSommets + 1;

    delete sommets;
    sommets = NULL;
    return nouveau;
}

// Ajout d'une valuation
t_valuation* AjouterValuation(t_valuation *sommets, int *nbSommets, t_valuation e)
{
    t_valuation *nouveau = new t_valuation[*nbSommets + 1];

    for(int i = 0; i < *nbSommets; i++)
    {
        nouveau[i] = sommets[i];
    }
    nouveau[*nbSommets] = e;
    *nbSommets = *nbSommets + 1;

    delete sommets;
    sommets = NULL;
    return nouveau;
}

// Retirer un sommet
string* SupprimerSommet(string *sommets, int *nbSommets, string e, bool copie = false)
{
    string *sommetsT = new string[*nbSommets - 1];
    int j = 0;
    for(int i = 0; i < *nbSommets; i++)
    {
        if(sommets[i] != e)
        {
            sommetsT[j] = sommets[i];
            j++;
        }
    }
    *nbSommets = j;
    if(copie)
    {
        delete sommets;
        sommets = NULL;
    }
    return sommetsT;
}

// Retirer un sommet
t_valuation* SupprimerValuationSommet(t_valuation *sommets, int *nbSommets, string e)
{
    t_valuation *sommetsT = new t_valuation[*nbSommets - 1];
    int j = 0;
    for(int i = 0; i < *nbSommets; i++)
    {
        if(sommets[i].sommet != e)
        {
            sommetsT[j] = sommets[i];
            j++;
        }
    }
    *nbSommets = j;
    delete sommets;
    sommets = NULL;
    return sommetsT;
}

// Rempli les sommets quand celui-ci n'a pas d'arcs
void remplirSommets(string *sommets, int nbSommets)
{
    if(chaineAlpha(sommets, nbSommets))
    {
        if(sommets[0] == "null") sommets[0] = "A";
        for(int i = 1; i < nbSommets; i++)
        {
            if(sommets[i] == "null") sommets[i] = (sommets[i - 1][0] + 1);
        }
    }
    else
    {
        if(sommets[0] == "null") sommets[0] = "0";
        for(int i = 1; i < nbSommets; i++)
        {
            if(sommets[i] == "null") sommets[i] = IntToString(StringToInt(sommets[i - 1]) + 1);
        }
    }
}

// Fonction de lecture d'un fichier graphe
t_arc* lectureFichier(const string &graphe, int *nbSommets, int *nbArcs, string **sommets, int **valuation, bool *sommetAlpha, bool *sommetOmega)
{
    string url = "TO-MORA_PRJ_" + graphe + ".txt";

    ifstream fichier(url.c_str(), ios::in);

    if(fichier)
    {
        string ligne;
        // Retourne le nombre de sommets
        getline(fichier, ligne);
        *nbSommets = atoi(ligne.c_str()) + 2;

        // Aucun arc au départ
        *nbArcs = 0;
        t_arc *arcs = NULL;

        *sommets = new string[*nbSommets];
        t_valuation *valuationSommets = new t_valuation[*nbSommets];
        *valuation = new int[*nbSommets];

        for(int i = 0; i < *nbSommets; i++)
        {
            valuationSommets[i].sommet = "null";
            valuationSommets[i].val = -1;
        }

        // Ajout du sommet de départ
        valuationSommets[0].sommet = "0";
        valuationSommets[0].val = 0;

        int i(0), j(1);
        string memoire = "", successeur = "", predecesseur = "";
        char input;

        // Parsage des arcs
        while(fichier.get(input))
        {
            if(input == ' ' || input == '\n')
            {
                i++;
                if(i == 1)
                {
                    successeur = memoire;
                    valuationSommets[j].sommet = successeur;
                }
                else if(i == 2) { valuationSommets[j].val = StringToInt(memoire); j++; }
                else if(i > 2)
                {
                    predecesseur = memoire;

                    // DEBUT : Ajout de l'arc
                    if(predecesseur == "-1" && i == 3)
                    {
                        t_arc temp;
                        temp.succ = successeur;
                        temp.prec = "0";
                        temp.val = 0;
                        arcs = AjouterArc(arcs, nbArcs, temp);
                    }
                    else if(predecesseur != "-1")
                    {
                        t_arc temp;
                        temp.succ = successeur;
                        temp.prec = predecesseur;
                        temp.val = -1;
                        arcs = AjouterArc(arcs, nbArcs, temp);
                    }
                    // FIN : Ajout de l'arc

                    if(input == '\n')
                    {
                        i = 0;
                        predecesseur = "";
                    }
                }
                memoire = "";
            }
            else memoire += input;
        }

        // DEBUT : Remplir le sommet omega
        for(int i = 0; i < *nbSommets; i++)
        {
            (*sommets)[i] = valuationSommets[i].sommet;
        }
        remplirSommets(*sommets, *nbSommets);
        for(int i = 0; i < *nbSommets; i++)
        {
            valuationSommets[i].sommet = (*sommets)[i];
        }
        // FIN : Remplir le sommet omega

        // DEBUT : Retrait du sommet alpha quand aucun arc existe depuis ce sommet
        for(int i = 0; i < *nbArcs; i++)
        {
            if(arcs[i].prec == "0")
            {
                *sommetAlpha = true;
            }
        }
        if(!(*sommetAlpha)) valuationSommets = SupprimerValuationSommet(valuationSommets, nbSommets, "0");
        for(int i = 0; i < *nbSommets; i++)
        {
            (*sommets)[i] = valuationSommets[i].sommet;
        }
        // FIN : Retrait du sommet alpha quand aucun arc existe depuis ce sommet

        for(int i = 0; i < *nbSommets - 1; i++)
        {
            bool prec = false, succ = false;
            for(int j = 0; j < *nbArcs; j++)
            {
                if(valuationSommets[i].sommet == arcs[j].prec) { arcs[j].val = valuationSommets[i].val; prec = true; }
                if(valuationSommets[i].sommet == arcs[j].succ) succ = true;
            }

            if(!prec && succ)
            {
                for(int k = 0; k < *nbSommets; k++)
                {
                    if(valuationSommets[i].sommet == valuationSommets[k].sommet)
                    {
                        t_arc temp;
                        temp.prec = valuationSommets[i].sommet;
                        temp.succ = (*sommets)[*nbSommets - 1];
                        temp.val = valuationSommets[i].val;
                        arcs = AjouterArc(arcs, nbArcs, temp);
                    }
                }
            }
        }

        // DEBUT : Retrait du sommet omega quand aucun arc existe depuis ce sommet
        for(int i = 0; i < *nbArcs; i++)
        {
            if(arcs[i].succ == (*sommets)[*nbSommets - 1])
            {
                *sommetOmega = true;
            }
        }
        if(!(*sommetOmega)) valuationSommets = SupprimerValuationSommet(valuationSommets, nbSommets, (*sommets)[*nbSommets - 1]);
        for(int i = 0; i < *nbSommets; i++)
        {
            (*sommets)[i] = valuationSommets[i].sommet;
            (*valuation)[i] = valuationSommets[i].val;
        }
        // FIN : Retrait du sommet omega quand aucun arc existe depuis ce sommet

        fichier.close();
        return arcs;
    }
    else
    {
        cout << "ERREUR : Fichier " << url << " inexistant" << endl;
        fichier.clear();
        return NULL;
    }
}

// Tri numérique des sommets
void triNum(int *sommets, int nbSommets)
{
    int i, j;
    int temp;

    for(i = 1; i < nbSommets; i++)
    {
        temp = sommets[i];
        for (j = i; j > 0 && sommets[j - 1] > temp; j--)
        {
            sommets[j] = sommets[j - 1];
        }
        sommets[j] = temp;
    }
}

// Tri alphabétique des sommets
void triAlpha(string *sommets, int nbSommets)
{
    int i, j;
    string temp;

    for(i = 1; i < nbSommets; i++)
    {
        temp = sommets[i];
        for (j = i; j > 0 && sommets[j - 1] > temp; j--)
        {
            sommets[j] = sommets[j - 1];
        }
        sommets[j] = temp;
    }
}

// Tri des sommets
void trierSommets(string *sommets, int nbSommets)
{
    if(chaineAlpha(sommets, nbSommets))
    {
        triAlpha(sommets, nbSommets);
    }
    else
    {
        int *temp = new int[nbSommets];
        for(int i = 0; i < nbSommets; i++)
        {
            temp[i] = StringToInt(sommets[i]);
        }
        triNum(temp, nbSommets);
        for(int i = 0; i < nbSommets; i++)
        {
            sommets[i] = IntToString(temp[i]);
        }
        delete temp;
        temp = NULL;
    }
}

// Vérifie si sommet en est un
bool isSommet(string *sommets, int nbSommets, string sommet)
{
    for(int i = 0; i < nbSommets; i++)
    {
        if(sommets[i] == sommet) return true;
    }
    return false;
}

// Copier les arcs
t_arc* CopieArcs(t_arc *arcs, int nbArcs)
{
    t_arc *arcsT = new t_arc[nbArcs];

    for(int i = 0; i < nbArcs; i++)
    {
        arcsT[i] = arcs[i];
    }
    return arcsT;
}

// Copier les sommets
string* CopieSommets(string *sommets, int nbSommets)
{
    string *sommetsT = new string[nbSommets];

    for(int i = 0; i < nbSommets; i++)
    {
        sommetsT[i] = sommets[i];
    }
    return sommetsT;
}

// Retirer un arc
t_arc* SupprimerArc(t_arc *arcs, int *nbArcs, t_arc e, bool copie = false)
{
    int j = 0;
    for(int i = 0; i < *nbArcs; i++)
    {
        if(arcs[i].prec != e.prec || arcs[i].succ != e.succ || arcs[i].val != e.val)
        {
            j++;
        }
    }

    t_arc *arcsT = new t_arc[j];
    j = 0;
    for(int i = 0; i < *nbArcs; i++)
    {
        if(arcs[i].prec != e.prec || arcs[i].succ != e.succ || arcs[i].val != e.val)
        {
            arcsT[j] = arcs[i];
            j++;
        }
    }
    *nbArcs = j;

    if(copie)
    {
        delete arcs;
        arcs = NULL;
    }
    return arcsT;
}

// Vérifie la condition d'arrêt de Bellman (ligne K = ligne K-1)
bool verifStop(t_pi **pi, int nbSommets, int k)
{
    for(int j = 0; j < nbSommets; j++)
    {
        if(pi[k][j].sommet != pi[k - 1][j].sommet) return false;
        if(pi[k][j].par != pi[k - 1][j].par) return false;
        if(pi[k][j].val != pi[k - 1][j].val) return false;
    }
    return true;
}

// Retourne l'indice correspondant à un sommet
int retournerK(t_pi **pi, int nbSommets, int k, string sommet)
{
    for(int i = 0; i < nbSommets; i++)
    {
        if(pi[k][i].sommet == sommet) return i;
    }
    return -1;
}

// Donne une liste des arcs en inversant leur orientation
t_arc* InverserTousLesArcs(t_arc *arcs, int nbArcs)
{
    t_arc *arcsT = new t_arc[nbArcs];
    for(int i = 0; i < nbArcs; i++)
    {
        arcsT[i].prec = arcs[i].succ;
        arcsT[i].succ = arcs[i].prec;
        arcsT[i].val = arcs[i].val;
    }
    return arcsT;
}

// Algorithme de Bellman // Calendrier au plus tôt
int Bellman(t_pi **pi, int nbSommets, t_arc *arcs, int nbArcs)
{
    int k(0), J(-1);
    string temp;
    do
    {
        k++;
        for(int i = 0; i < nbSommets; i++)
        {
            temp = pi[k - 1][i].val;

            // Copie de Pi(k-1) sur Pi(k)
            if(pi[k][i].sommet == pi[k - 1][i].sommet)
            {
                pi[k][i].par = pi[k - 1][i].par;
                pi[k][i].val = pi[k - 1][i].val;
            }

            for(int j = 0; j < nbArcs; j++)
            {
                if(pi[k][i].sommet == arcs[j].succ)
                {
                    J = retournerK(pi, nbSommets, k - 1, arcs[j].prec);

                    if(pi[k - 1][J].val != "+oo" && (temp == "+oo" || (temp != "+oo" && (StringToInt(pi[k - 1][J].val) + arcs[j].val) > StringToInt(temp))))
                    {
                        temp = IntToString(StringToInt(pi[k - 1][J].val) + arcs[j].val);
                        pi[k][i].par = arcs[j].prec;
                        pi[k][i].val = temp;
                    }
                }
            }
        }
    } while(k < (nbSommets - 1) && !verifStop(pi, nbSommets, k));

    return k;
}

// Algorithme de Bellman // Calendrier au plus tard
int BellmanTard(t_pi **pi, int nbSommets, t_arc *arcs, int nbArcs)
{
    t_arc *arcsInverse = InverserTousLesArcs(arcs, nbArcs);
    int k(0), J(-1);
    string temp;
    do
    {
        k++;
        for(int i = 0; i < nbSommets; i++)
        {
            temp = pi[k - 1][i].val;

            // Copie de Pi(k-1) sur Pi(k)
            if(pi[k][i].sommet == pi[k - 1][i].sommet)
            {
                pi[k][i].par = pi[k - 1][i].par;
                pi[k][i].val = pi[k - 1][i].val;
            }

            for(int j = 0; j < nbArcs; j++)
            {
                if(pi[k][i].sommet == arcsInverse[j].succ)
                {
                    J = retournerK(pi, nbSommets, k - 1, arcsInverse[j].prec);

                    if(pi[k - 1][J].val != "+oo" && (temp == "+oo" || (temp != "+oo" && (StringToInt(pi[k - 1][J].val) - arcsInverse[j].val) > StringToInt(temp))))
                    {
                        temp = IntToString(StringToInt(pi[k - 1][J].val) - arcsInverse[j].val);
                        pi[k][i].par = arcsInverse[j].prec;
                        pi[k][i].val = temp;
                    }
                }
            }
        }
    } while(k < (nbSommets - 1) && !verifStop(pi, nbSommets, k));
    delete arcsInverse;
    arcsInverse = NULL;
    return k;
}

// Affichage du tableau de l'algorithme de Bellman
void afficherTabPi(int nbSommets, int k, t_pi **pi, string *sommets)
{
    cout << "\n" << endl;

    string sep_c = "  ";
    string sep_l = "";

    for(int i = 0; i < nbSommets; i++)
    {
        sep_c = sep_c + "\t " + sommets[i];
    }

    for(int i = 0; i <= nbSommets + 1; i++)
    {
        sep_l = sep_l + "-------";
    }

    cout << sep_c << endl;
    for(int i = 0; i < k + 1; i++)
    {
        cout << sep_l << "\nK=" << i;

        for(int j = 0; j <  nbSommets ; j++)
        {
            if(pi[i][j].par != "-1")
            {
                cout <<"  \t" << pi[i][j].val << " " << pi[i][j].par;
            }
            else
            {
                cout << "\t+oo";
            }
        }
        cout << "\n";
    }
    cout << sep_l << endl;
}

// Affichage du calendrier
void afficherCalendrier(int nbSommets, int k, t_pi **pi, string *sommets)
{
    cout << "\n" << endl;

    string sep_c = "";
    string sep_l = "";

    for(int i = 0; i < nbSommets; i++)
    {
        sep_c = sep_c + sommets[i] + "     ";
    }

    for(int i = 0; i < nbSommets; i++)
    {
        sep_l = sep_l + "------";
    }

    cout << sep_c << endl;
    cout << sep_l << endl;
    for(int i = 0; i <  nbSommets; i++)
    {
        cout << pi[k][i].val;
        for(unsigned int j = 0; j < 6 - pi[k][i].val.size(); j++) cout << " ";
    }
    cout << "\n";
}

// Transformer les arcs pour rendre le graphe non orienté
t_arc* GenererArcsGrapheNonOriente(t_arc *arcs, int nbArcs)
{
    t_arc *arcsT = new t_arc[2 * nbArcs];
    int j = 0;
    for(int i = 0; i < nbArcs; i++)
    {
        arcsT[j] = arcs[i];
        j++;
    }
    for(int i = 0; i < nbArcs; i++)
    {
        t_arc temp;
        temp.prec = arcs[i].succ;
        temp.succ = arcs[i].prec;
        temp.val = arcs[i].val;

        arcsT[j] = temp;
        j++;
    }

    return arcsT;
}

// Affiche les arcs
void AfficherArcs(t_arc *arcs, int nbArcs)
{
    for(int i = 0; i < nbArcs; i++)
    {
        cout << arcs[i].prec << "\t" << arcs[i].succ << "\t" << arcs[i].val << endl;
    }
}

// Vérifie si un graphe contient au moins un cycle sur un graphe orienté
bool contientCycle(t_arc *arcs, int nbArcs, string *sommets, int nbSommets)
{
    t_arc *arcs_temp = CopieArcs(arcs, nbArcs);
    string *sommets_temp = CopieSommets(sommets, nbSommets);
    bool traitement = true;

    while(nbSommets > 0 && traitement)
    {
        traitement = false;
        for(int i = 0; i < nbSommets; i++)
        {
            bool prec = false;
            t_arc temp;
            for(int j = 0; j < nbArcs; j++)
            {
                if(sommets_temp[i] == arcs_temp[j].prec)
                {
                    prec = true;
                    temp.prec = sommets_temp[i];
                }
            }

            if(!prec)
            {
                for(int j = 0; j < nbArcs; j++)
                {
                    if(sommets_temp[i] == arcs_temp[j].succ)
                    {
                        arcs_temp = SupprimerArc(arcs_temp, &nbArcs, arcs_temp[j]);
                        j = 0;
                    }
                }
                sommets_temp = SupprimerSommet(sommets_temp, &nbSommets, sommets_temp[i]);
                i = 0;
                traitement = true;
            }
        }
    }
    /*
    cout << "nb sommets : " << nbSommets << endl;
    cout << "traitement : " << traitement << endl;
    */
    if(nbSommets > 0) return true;
    return false;
}

bool estMarque(t_sommet *sommets, int nbSommets, string a)
{
    for(int i = 0; i < nbSommets; i++)
    {
        if(sommets[i].val == a && sommets[i].marque) return true;
    }
    return false;
}

// Parcours en profondeur
void DFS(t_arc *arcs, int nbArcs, t_sommet *sommets, int nbSommets, string a)
{
    // Marquer le sommet
    for(int i = 0; i < nbSommets; i++)
    {
        if(sommets[i].val == a) sommets[i].marque = true;
    }

    for(int i = 0; i < nbArcs; i++)
    {
        if(a == arcs[i].prec && !estMarque(sommets, nbSommets, arcs[i].succ))
        {
            DFS(arcs, nbArcs, sommets, nbSommets, arcs[i].succ);
        }
        if(a == arcs[i].succ && !estMarque(sommets, nbSommets, arcs[i].prec))
        {
            DFS(arcs, nbArcs, sommets, nbSommets, arcs[i].prec);
        }
    }
}

// Vérifie si le graphe est connexe
bool connexite(t_arc *arcs, int nbArcs, string *sommets, int nbSommets)
{
    t_sommet *a = new t_sommet[nbSommets];
    // Copie des sommets
    for(int i = 0; i < nbSommets; i++)
    {
        a[i].val = sommets[i];
        a[i].marque = false;
    }

    DFS(arcs, nbArcs, a, nbSommets, a[0].val);

    bool connexe = true;
    for(int i = 0; i < nbSommets; i++)
    {
        if(!a[i].marque) connexe = false;
    }
    delete a;
    a = NULL;
    return connexe;
}

// Classer les arcs selon leur valuation
void ClasserArcs(t_arc *arcs, int nbArcs, string ordre = "croissant")
{
    int i, j;
    t_arc temp;

    if(ordre == "decroissant")
    {
        for(i = 1; i < nbArcs; i++)
        {
            temp = arcs[i];
            for (j = i; j > 0 && arcs[j - 1].val < temp.val; j--)
            {
                arcs[j] = arcs[j - 1];
            }
            arcs[j] = temp;
        }
    }
    else
    {
        for(i = 1; i < nbArcs; i++)
        {
            temp = arcs[i];
            for (j = i; j > 0 && arcs[j - 1].val > temp.val; j--)
            {
                arcs[j] = arcs[j - 1];
            }
            arcs[j] = temp;
        }
    }
}

// Vérifie si un arc est présent dans la liste
bool ArcExist(t_arc *arcs, int nbArcs, t_arc e)
{
    for(int i = 0; i < nbArcs; i++)
    {
        if(arcs[i].prec == e.prec && arcs[i].succ == e.succ && arcs[i].val == e.val) return true;
    }
    return false;
}

// Kruskal destructif
t_arc* KruskalDestructif(t_arc *arcs, int *nbArcs, string *sommets, int nbSommets)
{
    int nbArcsTemp = *nbArcs;
    t_arc *arcs_temp = CopieArcs(arcs, nbArcsTemp);
    ClasserArcs(arcs_temp, nbArcsTemp, "decroissant");

    cout << "Etape 0 : Tri par ordre decroissant des arretes" << endl;
    AfficherArcs(arcs_temp, nbArcsTemp);
    cout << endl;
    int etape = 1;

    t_arc *arcsGardes = new t_arc[0];
    int nbArcsGardes = 0;

    for(int i = 0; i < nbArcsTemp; i++)
    {
        /* Partie test */
        int nbA = nbArcsTemp;

        t_arc *arcs_test = CopieArcs(arcs_temp, nbArcsTemp);
        arcs_test = SupprimerArc(arcs_test, &nbA, arcs_test[i], true);

        if(connexite(arcs_test, nbA, sommets, nbSommets))
        {
            cout << "Etape " << etape << " : suppression de l'arrete " << arcs_temp[i].prec << " <-> " << arcs_temp[i].succ << " (" << arcs_temp[i].val << ")" << endl;
            etape++;
            /* Partie suppression */
            arcs_temp = SupprimerArc(arcs_temp, &nbArcsTemp, arcs_temp[i], true);
            i = -1;
        }
        else if(!ArcExist(arcsGardes, nbArcsGardes, arcs_temp[i]))
        {
            arcsGardes = AjouterArc(arcsGardes, &nbArcsGardes, arcs_temp[i]);
            cout << "Etape " << etape << " : on garde l'arrete " << arcs_temp[i].prec << " <-> " << arcs_temp[i].succ << " (" << arcs_temp[i].val << ")" << endl;
            etape++;
        }
        delete arcs_test;
        arcs_test = NULL;
    }
    *nbArcs = nbArcsTemp;
    delete arcsGardes;
    arcsGardes = NULL;
    return arcs_temp;
}

t_pi** InitBellman(string *sommets, int nbSommets, string sommetDepart = "0", string valeurDepart = "0")
{
    t_pi **pi = new t_pi*[nbSommets];
    for(int i = 0; i < nbSommets; i++)
    {
            pi[i] = new t_pi[nbSommets];

            for(int j = 0; j < nbSommets; j++)
            {
                    pi[i][j].par = IntToString(-1);
                    pi[i][j].sommet = sommets[j];

                    // Mise à 0 du Pi du sommet de départ
                    if(i == 0 && sommets[j] == sommetDepart)
                    {
                        pi[i][j].par = pi[i][j].sommet;
                        pi[i][j].val = valeurDepart;
                    }
                    else pi[i][j].val = "+oo";
            }
    }
    return pi;
}

// Vidage du buffer
void viderBuffer()
{
    if(cin.bad() || cin.eof() || cin.fail())
    {
        cin.clear();
        cin.seekg(0, ios::end);
        if(!cin.fail()) cin.ignore(10000, '\n');
        else cin.clear();
    }
}

t_arc LectureArc(string saisie, string *sommets, int *valuationSommets, int nbSommets)
{
    string memoire = "";
    int j = 0;
    t_arc nouvelArc;
    for(unsigned int i = 0; i < saisie.size(); i++)
    {
        if(saisie[i] != ' ') memoire += saisie[i];
        else if(saisie[i] == ' ')
        {
            j++;
            if(j == 2) nouvelArc.succ = memoire;
            memoire = "";
        }
    }
    nouvelArc.prec = memoire;
    // Recherche de la durée de la tache
    for(int k = 0; k < nbSommets; k++)
    {
        if(sommets[k] == nouvelArc.prec) nouvelArc.val = valuationSommets[k];
    }
    return nouvelArc;
}

// Renvoie le nouveau dernier sommet
string NouveauDernierSommet(string *sommets, int nbSommets)
{
    if(chaineAlpha(sommets, nbSommets))
    {
        if(nbSommets == 0) return "A";
        else return IntToString(sommets[nbSommets - 1][0] + 1);
    }
    else
    {
        if(nbSommets == 0) return "0";
        else return IntToString(StringToInt(sommets[nbSommets - 1]) + 1);
    }
}

int main()
{
    while(1)
    {
        t_arc *arcs = NULL;
        int nbSommets, nbArcs;
        string graphe;
        string *sommets = NULL;
        int *valuationSommets = NULL;
        bool sommetAlpha = false, sommetOmega = false;
        bool modif = false;
        int reponse;

        while(arcs == NULL)
        {
            cout << "Nom du graphe : ";
            cin >> graphe;
            cin.ignore();
            arcs = lectureFichier(graphe, &nbSommets, &nbArcs, &sommets, &valuationSommets, &sommetAlpha, &sommetOmega);
        }

        // Tri des sommets pour une présentation plus claire
        trierSommets(sommets, nbSommets);

        ClasserArcs(arcs, nbArcs, "croissant");

        t_valuation *valuationSommets2 = new t_valuation[nbSommets];
        for(int i = 0; i < nbSommets; i++)
        {
            valuationSommets2[i].sommet = sommets[i];
            valuationSommets2[i].val = valuationSommets[i];
        }

        cout << "Graphe : TO-MORA_PRJ_" << graphe << ".txt" << endl;
        cout << "NB de sommets : " << nbSommets << endl;
        cout << "NB d'arcs : " << nbArcs << endl;

        do
        {
            // Détection de circuit
            bool circuit, action;
            while((circuit = contientCycle(arcs, nbArcs, sommets, nbSommets)) || modif)
            {
                modif = false;
                cout << "\nArcs lus :\n\n";
                AfficherArcs(arcs, nbArcs);
                if(circuit) cout << "\nDetection d'un circuit : oui" << endl;
                else  cout << "\nDetection d'un circuit : non" << endl;
                action = false;
                do
                {
                    string saisie, memoire = "";
                    action = false;
                    cout << "Exemples de commandes :" << endl;
                    cout << "1 2 3 : ajouter la tache 3 en contrainte de la tache 2\n" <<
                            "-1 2 3 : supprimer la tache 3 des contraintes de la tache 2\n" <<
                            "0 2 3 : verifier si la tache 3 est une contrainte de la tache 2\n\nCommande : ";
                    getline(cin, saisie);

                    // Vidage buffer
                    viderBuffer();

                    // Lecture de l'arc
                    t_arc nouvelArc = LectureArc(saisie, sommets, valuationSommets, nbSommets);

                    // Action à effectuer
                    switch(saisie[0])
                    {
                        case '1': // Ajout
                            memoire = "Ajout de la tache " + nouvelArc.prec + " en contrainte de la tache " + nouvelArc.succ + " : ";
                            if(isSommet(sommets, nbSommets, nouvelArc.prec) && isSommet(sommets, nbSommets, nouvelArc.succ))
                            {
                                if(sommetAlpha && (sommets[0] == nouvelArc.prec || sommets[0] == nouvelArc.succ))
                                {
                                    memoire += "ERREUR\nImpossible d'ajouter une contrainte sur le sommet alpha (ici = 0)\n\n";
                                }
                                else if(sommetOmega && (sommets[nbSommets - 1] == nouvelArc.prec || sommets[nbSommets - 1] == nouvelArc.succ))
                                {
                                    memoire += "ERREUR\nImpossible d'ajouter une contrainte sur le sommet omega (ici = " + sommets[nbSommets - 1] + ")\n\n";
                                }
                                else if(!ArcExist(arcs, nbArcs, nouvelArc))
                                {
                                    arcs = AjouterArc(arcs, &nbArcs, nouvelArc);
                                    action = true;
                                    memoire += "OK\n";
                                }
                                else memoire += "ERREUR\nCette tache existe deja\n\n";
                            }
                            else memoire += "ERREUR\nL'une de ces taches n'existe pas\n\n";
                            break;
                        case '-': // Suppression
                            if(saisie[1] == '1')
                            {
                                memoire = "Suppression de la tache " + nouvelArc.prec + " en contrainte de la tache " + nouvelArc.succ + " : ";
                                if(sommetAlpha && (sommets[0] == nouvelArc.prec || sommets[0] == nouvelArc.succ))
                                {
                                    memoire += "ERREUR\nImpossible de supprimer une contrainte sur le sommet alpha (ici = 0)\n\n";
                                }
                                else if(sommetOmega && (sommets[nbSommets - 1] == nouvelArc.prec || sommets[nbSommets - 1] == nouvelArc.succ))
                                {
                                    memoire += "ERREUR\nImpossible de supprimer une contrainte sur le sommet omega (ici = " + sommets[nbSommets - 1] + ")\n\n";
                                }
                                else if(ArcExist(arcs, nbArcs, nouvelArc))
                                {
                                    arcs = SupprimerArc(arcs, &nbArcs, nouvelArc, true);
                                    action = true;
                                    memoire += "OK\n";
                                }
                                else memoire += "ERREUR\nL'une de ces taches n'existe pas\n\n";
                            }
                            break;
                        case '0': // Verification
                            action = false;
                            memoire = "Verification : la tache " + nouvelArc.prec + " est une contrainte de la tache " + nouvelArc.succ + " : ";

                            if(ArcExist(arcs, nbArcs, nouvelArc)) memoire += "Oui\n\n";
                            else memoire += "Non\n\n";
                            break;
                    }
                    cout << memoire;
                } while(!action);
                cout << "\n";
                system("pause");

                // Suppression de alpha & omega
                if(sommetAlpha || sommetOmega)
                {
                    for(int i = 0; i < nbArcs; i++)
                    {
                        if(sommetAlpha && sommets[0] == arcs[i].prec)
                        {
                            arcs = SupprimerArc(arcs, &nbArcs, arcs[i], true);
                            i = 0;
                        }
                        if(sommetOmega && sommets[nbSommets - 1] == arcs[i].succ)
                        {
                            arcs = SupprimerArc(arcs, &nbArcs, arcs[i], true);
                            i = -1;
                        }
                    }
                    if(sommetAlpha)
                    {
                        valuationSommets2 = SupprimerValuationSommet(valuationSommets2, &nbSommets, valuationSommets2[0].sommet);
                    }
                    if(sommetOmega)
                    {
                        valuationSommets2 = SupprimerValuationSommet(valuationSommets2, &nbSommets, valuationSommets2[nbSommets - 1].sommet);
                    }
                    sommetAlpha = false;
                    sommetOmega = false;
                }

                string DernierSommet;

                delete sommets;
                sommets = new string[nbSommets];
                for(int i = 0; i < nbSommets; i++)
                {
                    sommets[i] = valuationSommets2[i].sommet;
                }

                // Ajout de alpha et omega si c'est possible
                if(!sommetAlpha || !sommetOmega)
                {
                    for(int i = 0; i < nbSommets; i++)
                    {
                        bool prec = true;
                        bool succ = true;
                        for(int j = 0; j < nbArcs; j++)
                        {
                            if(valuationSommets2[i].sommet == arcs[j].succ) prec = false;
                            if(valuationSommets2[i].sommet == arcs[j].prec) succ = false;
                        }
                        if(prec)
                        {
                            t_arc ajout;
                            ajout.prec = "0";
                            ajout.val = 0;
                            ajout.succ = valuationSommets2[i].sommet;
                            if(ajout.prec != ajout.succ)
                            {
                                arcs = AjouterArc(arcs, &nbArcs, ajout);
                                sommetAlpha = true;
                            }
                        }
                        if(succ)
                        {
                            t_arc ajout;
                            ajout.prec = valuationSommets2[i].sommet;
                            ajout.val = valuationSommets2[i].val;
                            DernierSommet = NouveauDernierSommet(sommets, nbSommets);
                            ajout.succ = DernierSommet;
                            if(ajout.prec != ajout.succ)
                            {
                                arcs = AjouterArc(arcs, &nbArcs, ajout);
                                sommetOmega = true;
                            }
                        }
                    }
                }

                if(sommetAlpha)
                {
                    t_valuation ajoutValuation;
                    ajoutValuation.sommet = "0";
                    ajoutValuation.val = 0;
                    valuationSommets2 = AjouterValuation(valuationSommets2, &nbSommets, ajoutValuation);
                }
                if(sommetOmega)
                {
                    t_valuation ajoutValuation;
                    ajoutValuation.sommet = DernierSommet;
                    ajoutValuation.val = -1;
                    valuationSommets2 = AjouterValuation(valuationSommets2, &nbSommets, ajoutValuation);
                }

                delete sommets;
                delete valuationSommets;
                sommets = new string[nbSommets];
                valuationSommets = new int[nbSommets];
                for(int i = 0; i < nbSommets; i++)
                {
                    sommets[i] = valuationSommets2[i].sommet;
                    valuationSommets[i] = valuationSommets2[i].val;
                }
                trierSommets(sommets, nbSommets);
                for(int i = 0; i < nbSommets; i++)
                {
                    for(int j = 0; j < nbSommets; j++)
                    {
                        if(sommets[i] == valuationSommets2[j].sommet) valuationSommets[i] = valuationSommets2[j].val;
                    }
                }

                ClasserArcs(arcs, nbArcs, "croissant");
            }

            cout << "\nArcs lus :\n\n";
            AfficherArcs(arcs, nbArcs);
            cout << "\n";

            if(circuit) cout << "Detection d'un circuit : oui" << endl;
            else cout << "Detection d'un circuit : non" << endl;

            cout << "\nDates au plus tot";
            // Initialisation du tableau de Bellman
            t_pi **pi = InitBellman(sommets, nbSommets, sommets[0], "0");
            int k = Bellman(pi, nbSommets, arcs, nbArcs);
            afficherCalendrier(nbSommets, k, pi, sommets);

            cout << "\nDates au plus tard";
            // Initialisation du tableau de Bellman
            pi = InitBellman(sommets, nbSommets, sommets[nbSommets - 1], pi[k][retournerK(pi, nbSommets, k, sommets[nbSommets - 1])].val);
            k = BellmanTard(pi, nbSommets, arcs, nbArcs);
            // Redéfinition de la durée du sommet 0
            pi[k][retournerK(pi, nbSommets, k, sommets[0])].val = "0";
            afficherCalendrier(nbSommets, k, pi, sommets);

            cout << "\nModifier le tableau de contraintes ? (1 = oui/0 = non)\n";
            cin >> reponse;
            cin.ignore();
            if(reponse == 1) modif = true;
            else modif = false;
        } while(reponse == 1);

        delete arcs;
        arcs = NULL;
        delete sommets;
        sommets = NULL;
    }

    return 0;
}
