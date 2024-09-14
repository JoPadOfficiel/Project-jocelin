#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
#include <chrono>
#include <thread> //pause du de l'écran

using namespace std;

const int LIGNES = 4;
const int COLONNES = 4;

// Structure pour représenter une carte individuelle
struct Carte {
    char valeur;
    bool estRetournee;
};

// Structure pour représenter un joueur
struct Joueur {
    string nom;
    int essaisRestants;
    int pairesTrouvees;
};

// Structure pour représenter le plateau de jeu
struct Plateau {
    Carte cartes[LIGNES][COLONNES];
};

// Fonction pour initialiser le plateau avec des paires prédéfinies
void initialiserPlateau(Plateau &plateau) {
    char paires[16] = {'A', 'A', 'B', 'B', 'C', 'C', 'D', 'D','E', 'E', 'F', 'F', 'G', 'G', 'H', 'H'};

    // Mélanger les paires de cartes
    srand(time(0));
    for (int i = 15; i > 0; --i) {
        int j = rand() % (i + 1);
        swap(paires[i], paires[j]);
    }

    // Remplir le plateau avec les paires mélangées
    int index = 0;
    for (int i = 0; i < LIGNES; ++i) {
        for (int j = 0; j < COLONNES; ++j) {
            plateau.cartes[i][j].valeur = paires[index++];
            plateau.cartes[i][j].estRetournee = false;
        }
    }
}

// Fonction pour afficher le plateau de jeu
void afficherPlateau(const Plateau &plateau) {
    cout << endl;
    cout << "  ";
    for (int j = 0; j < COLONNES; ++j) {
        cout << j + 1 << " ";
    }
    cout << endl;

    for (int i = 0; i < LIGNES; ++i) {
        cout << i + 1 << " ";
        for (int j = 0; j < COLONNES; ++j) {
            if (plateau.cartes[i][j].estRetournee) {
                cout << plateau.cartes[i][j].valeur << " ";
            } else {
                cout << "* ";
            }
        }
        cout << endl;
    }
}

// Fonction pour jouer un tour
void jouerTour(Plateau &plateau, Joueur &joueur) {
    int x1, y1, x2, y2;
    cout << endl;
    cout << joueur.nom
         << ", entrez les coordonnées de la première carte à retourner (ligne, colonne): ";
    cin >> x1 >> y1;
    --x1;
    --y1;

    cout << joueur.nom
         << ", entrez les coordonnées de la deuxième carte à retourner (ligne, colonne): ";
    cin >> x2 >> y2;
    --x2;
    --y2;

    if (x1 == x2 && y1 == y2) {
        cout << "Les coordonnées des deux cartes sont identiques. Veuillez choisir deux cartes différentes." << endl;
        return;
    }

    plateau.cartes[x1][y1].estRetournee = true;
    plateau.cartes[x2][y2].estRetournee = true;
    afficherPlateau(plateau);

    if (plateau.cartes[x1][y1].valeur == plateau.cartes[x2][y2].valeur) {
        cout << "Bravo, vous avez trouvé une paire !" << endl;
        joueur.pairesTrouvees++;
    } else {
        cout << "Les cartes ne correspondent pas. Essayez encore." << endl;
        plateau.cartes[x1][y1].estRetournee = false;
        plateau.cartes[x2][y2].estRetournee = false;
        joueur.essaisRestants--;

        // Attendre x secondes avant d'effacer l'écran
        this_thread::sleep_for(chrono::seconds(5));
        system("clear"); // Linux et MacOS
        // system("cls"); // Windows
    }
}

// Fonction pour jouer la partie
void jouer(Plateau &plateau, Joueur &joueur1, Joueur &joueur2, int nbJoueurs) {
    int totalPaires = 8;

    while ((joueur1.essaisRestants > 0 || (nbJoueurs == 2 && joueur2.essaisRestants > 0)) &&
           (joueur1.pairesTrouvees + joueur2.pairesTrouvees) < totalPaires) {
        afficherPlateau(plateau);
        jouerTour(plateau, joueur1);

        if (nbJoueurs == 2 && joueur2.essaisRestants > 0 && (joueur1.pairesTrouvees + joueur2.pairesTrouvees) < totalPaires) {
            afficherPlateau(plateau);
            jouerTour(plateau, joueur2);
        }
    }

    cout << "Fin de la partie." << endl;
    cout << joueur1.nom << " a trouvé " << joueur1.pairesTrouvees << " paires avec " << joueur1.essaisRestants << " essais restants." << endl;
    if (nbJoueurs == 2) {
        cout << joueur2.nom << " a trouvé " << joueur2.pairesTrouvees << " paires avec " << joueur2.essaisRestants << " essais restants." << endl;
    }
}

int main() {
    Plateau plateau;
    Joueur joueur1, joueur2;
    int nbJoueurs;

    do {
        cout << "Veuillez saisir le nombre de joueurs (1 ou 2) : ";
        cin >> nbJoueurs;
    } while (nbJoueurs < 1 || nbJoueurs > 2);

    cout << "Veuillez saisir le prénom du joueur 1 : ";
    cin >> joueur1.nom;
    joueur1.essaisRestants = 10;
    joueur1.pairesTrouvees = 0;

    if (nbJoueurs == 2) {
        cout << "Veuillez saisir le prénom du joueur 2 : ";
        cin >> joueur2.nom;
        joueur2.essaisRestants = 10;
        joueur2.pairesTrouvees = 0;
    }

    initialiserPlateau(plateau);
    jouer(plateau, joueur1, joueur2, nbJoueurs);

    return 0;
}