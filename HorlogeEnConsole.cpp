#include <iostream>// bibliothèque qui gère les entrée/sortie standard en c++
#include <iomanip>// permet de manipuler le format d'affichage des entrée/sortie
#include <chrono>// permet de manipuler le temps avec une grande précision
#include <thread>// permet d'éxecuter les fonction en parallèle
#include <ctime>// permet de fournir les dates et le temps, nous permet de manipuler l'heure actuel
#include <atomic>//gère les opérarion atomique sans intéruption
#include <string>// permet de manipuler les chaines de caratère 

using namespace std;

// Variables atomic  qui gère l'arrêt des threads
atomic<bool>/*garantir que la  lecture et l'écriture sur stop thread sont sans interruption*/ stopThread(false);

// Fonction pour afficher l'heure en temps réel
void afficherHeure() {
    while (!stopThread)/*tantque stopthread est false le programme continue d'afficher l'heure*/ {
        auto maintenant = chrono::system_clock::now();
        time_t temps = chrono::system_clock::to_time_t(maintenant);// Récupèere l'heure actuel
        tm* heureLocale = localtime(&temps);// convertir l'heure en heure locale

        std::cout << "Heure actuelle : ";
        std::cout << setfill('0') << setw(2) << heureLocale->tm_hour << ":"
             << setfill('0') << setw(2) << heureLocale->tm_min << ":"
             << setfill('0') << setw(2) << heureLocale->tm_sec << std::endl;/* format d'affichage pour garantir que les nombre ont 
             tjr 2 chiffre*/

        this_thread::sleep_for(chrono::seconds(1));// attente d'une seconde(faire la mise à jour apres chaque seconde)
         #ifdef _WIN32
        system("cls");
        #else 
        system("clear");
        #endif // Effacer l'écran après chaque mise à jour
    }
}

// Fonction pour le compte à rebours
void compteARebours(int secondes) {
    stopThread = false;/*initialisation de stop thread( permet de s'assurer que le thread du compte a rebours puisse fonctioner normalement)*/
    while (secondes >= 0 && !stopThread)/*continue tant qu'il reste du temps sec>=0 et que stopthread n'est pas activé*/ {
       //verification pour arreter le thread
        int minutes = secondes / 60;
        int sec = secondes % 60;

        std::cout << "Compte à rebours : ";
        cout << setfill('0') << setw(2) << minutes << ":"
             << setfill('0') << setw(2) << sec << std::endl;// format d'affichage pour guarantir que le temps vas safficher avec 2 chiffre

        this_thread::sleep_for(chrono::seconds(1));// permet d'attendre 1 sec avant la mise a jour 
        --secondes;
       #ifdef _WIN32
        system("cls");
        #else 
        system("clear");
        #endif // Effacer l'écran après chaque mise à jour
    }

    if (!stopThread) {
        std::cout << "Temps écoulé !" << std::endl;// si stop thread n'a pas été activer afficher "temps ecoulee"
    }
}

// Fonction pour lire l'entrée utilisateur
void lireInput()/*permet a l'utilisateur d'entrer un caratère et d'arreter le compte a rebours si il appuie sue 's'*/ {
    char input;
    while (!stopThread){
        cin >> input;
    if (input == 's') {
        stopThread = true;
        break;
    }
    }
}

// Fonction pour le chronomètre
void chronometre() {
    auto debut = chrono::steady_clock::now();// capture l'instant ou le chronomètre commence
    stopThread = false;

    std::cout << "Appuyez sur 's' pour arrêter le chronomètre." << std::endl;

    // Démarrer un thread pour lire l'entrée utilisateur
    thread t(lireInput);

    while (!stopThread) {
        auto maintenant = chrono::steady_clock::now();
        auto duree = chrono::duration_cast<chrono::seconds>(maintenant - debut);// calcule le temps entre début et maintainant
        // afficher le temps en minute et en seconde
        int minutes = duree.count() / 60;
        int secondes = duree.count() % 60;

        std::cout << "Chronomètre : ";
        std::cout << setfill('0') << setw(2) << minutes << ":"
             << setfill('0') << setw(2) << secondes << std::endl;// format d'affichage pour quarrantire que le temps va safficher avec 2 chiffre

        this_thread::sleep_for(chrono::seconds(1));// permet d'attendre 1 seconde avant la mise a jour
        #ifdef _WIN32
        system("cls");
        #else 
        system("clear");
        #endif // Effacer l'écran après chaque mise à jour
    }

    t.join(); // Attendre que le thread d'entrée utilisateur se termine
    std::cout << "Chronomètre arrêté." << std::endl;
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

        if (heureLocale->tm_hour == heure && heureLocale->tm_min == minute)/*recupère l'heure actuel et compare avec l'heure de l'alarme si 
        si l'heure est egale afficher le msg a l'écran*/ {
            std::cout << "ALERTE : L'heure de l'alarme est atteinte !" << std::endl;
            stopThread = true; // Arrêter l'alarme après qu'elle ait sonné
        }

        this_thread::sleep_for(chrono::seconds(1));// permet d'attendre 1 sec avant la mise à jour
    }

    t.join(); // Attendre que le thread d'entrée utilisateur se termine
}

// Fonction principale
int main() {
    int choix;

    while (true) {
        std::cout << "=== Horloge en console ===" << std::endl;
        std::cout << "1. Afficher l'heure en temps réel" << std::endl;
        std::cout << "2. Compte à rebours" << std::endl;
        std::cout << "3. Chronomètre" << std::endl;
        std::cout << "4. Définir une alarme" << std::endl;
        std::cout << "5. Quitter" << std::endl;
        std::cout << "Choisissez une option : ";
        std::cin >> choix;

        switch (choix) {
            case 1: {
                stopThread = false;
                thread t(afficherHeure);// cree un thread qui lance la fonction "afficherHeure"
                std::cout << "Appuyez sur une touche pour revenir au menu principal." << std::endl;
                std::cin.ignore();
                std::cin.get();// attend une entrée de l'utilisateur pour arreter l'affichage de l'heure
                stopThread = true;//stopper le thread
                t.join();// attendre le thread afficheHeure se termine
                break;
            }
            case 2: {
                int secondes;
                std::cout << "Entrez le temps en secondes : ";
                std::cin >> secondes;
                compteARebours(secondes);// lance le copmpte à rebours
                break;
            }
            case 3: {
                stopThread = false;
                thread t(chronometre);// demarrer un tread t pour executer la fonction chronomètre
                t.join(); // Attendre que le chronomètre se termine
                break;
            }
            case 4: {
                int heure, minute;
                std::cout << "Entrez l'heure de l'alarme (HH MM) : ";
                std::cin >> heure >> minute;
                stopThread = false;
                thread t(alarme,heure, minute);
                std::cout << "Appuyez sur 's' pour arrêter l'alarme." << std::endl;
                t.join();
                break;
            }
            case 5:
                std::cout << "Au revoir !" << std::endl;
                return 0;
            default:
                std::cout << "Choix invalide. Réessayez." << std::endl;
        }
    }
}