#include <iostream>
#include <iomanip>
#include <chrono>
#include <thread>
#include <ctime>
#include <atomic>
#include <string>

using namespace std;

// Variables globales pour gérer l'arrêt des threads
atomic<bool> stopThread(false);

// Fonction pour afficher l'heure en temps réel
void afficherHeure() {
    while (!stopThread) {
        auto maintenant = chrono::system_clock::now();
        time_t temps = chrono::system_clock::to_time_t(maintenant);
        tm* heureLocale = localtime(&temps);

        cout << "Heure actuelle : ";
        cout << setfill('0') << setw(2) << heureLocale->tm_hour << ":"
             << setfill('0') << setw(2) << heureLocale->tm_min << ":"
             << setfill('0') << setw(2) << heureLocale->tm_sec << endl;

        this_thread::sleep_for(chrono::seconds(1));
        system("clear"); // Effacer l'écran après chaque mise à jour
    }
}

// Fonction pour le compte à rebours
void compteARebours(int secondes) {
    stopThread = false;
    while (secondes >= 0 && !stopThread) {
        int minutes = secondes / 60;
        int sec = secondes % 60;

        cout << "Compte à rebours : ";
        cout << setfill('0') << setw(2) << minutes << ":"
             << setfill('0') << setw(2) << sec << endl;

        this_thread::sleep_for(chrono::seconds(1));
        --secondes;
        system("clear"); // Effacer l'écran après chaque mise à jour
    }

    if (!stopThread) {
        cout << "Temps écoulé !" << endl;
    }
}

// Fonction pour lire l'entrée utilisateur
void lireInput() {
    char input;
    cin >> input;
    if (input == 's') {
        stopThread = true;
    }
}

// Fonction pour le chronomètre
void chronometre() {
    auto debut = chrono::steady_clock::now();
    stopThread = false;

    cout << "Appuyez sur 's' pour arrêter le chronomètre." << endl;

    // Démarrer un thread pour lire l'entrée utilisateur
    thread t(lireInput);

    while (!stopThread) {
        auto maintenant = chrono::steady_clock::now();
        auto duree = chrono::duration_cast<chrono::seconds>(maintenant - debut);

        int minutes = duree.count() / 60;
        int secondes = duree.count() % 60;

        cout << "Chronomètre : ";
        cout << setfill('0') << setw(2) << minutes << ":"
             << setfill('0') << setw(2) << secondes << endl;

        this_thread::sleep_for(chrono::seconds(1));
        #ifdef _WIN32
        system("cls");
        #else 
        system("clear");
        #endif // Effacer l'écran après chaque mise à jour
    }

    t.join(); // Attendre que le thread d'entrée utilisateur se termine
    cout << "Chronomètre arrêté." << endl;
}

// Fonction pour l'alarme
void alarme(int heure, int minute) {
    stopThread = false;

    // Démarrer un thread pour lire l'entrée utilisateur
    thread t(lireInput);

    while (!stopThread) {
        auto maintenant = chrono::system_clock::now();
        time_t temps = chrono::system_clock::to_time_t(maintenant);
        tm* heureLocale = localtime(&temps);

        if (heureLocale->tm_hour == heure && heureLocale->tm_min == minute) {
            cout << "ALERTE : L'heure de l'alarme est atteinte !" << endl;
            stopThread = true; // Arrêter l'alarme après qu'elle ait sonné
        }

        this_thread::sleep_for(chrono::seconds(1));
    }

    t.join(); // Attendre que le thread d'entrée utilisateur se termine
}

// Fonction principale
int main() {
    int choix;

    while (true) {
        cout << "=== Horloge en console ===" << endl;
        cout << "1. Afficher l'heure en temps réel" << endl;
        cout << "2. Compte à rebours" << endl;
        cout << "3. Chronomètre" << endl;
        cout << "4. Définir une alarme" << endl;
        cout << "5. Quitter" << endl;
        cout << "Choisissez une option : ";
        cin >> choix;

        switch (choix) {
            case 1: {
                stopThread = false;
                thread t(afficherHeure);
                cout << "Appuyez sur une touche pour revenir au menu principal." << endl;
                cin.ignore();
                cin.get();
                stopThread = true;
                t.join();
                break;
            }
            case 2: {
                int secondes;
                cout << "Entrez le temps en secondes : ";
                cin >> secondes;
                compteARebours(secondes);
                break;
            }
            case 3: {
                stopThread = false;
                thread t(chronometre);
                t.join(); // Attendre que le chronomètre se termine
                break;
            }
            case 4: {
                int heure, minute;
                cout << "Entrez l'heure de l'alarme (HH MM) : ";
                cin >> heure >> minute;
                stopThread = false;
                thread t(alarme, heure, minute);
                cout << "Appuyez sur 's' pour arrêter l'alarme." << endl;
                t.join();
                break;
            }
            case 5:
                cout << "Au revoir !" << endl;
                return 0;
            default:
                cout << "Choix invalide. Réessayez." << endl;
        }
    }
}