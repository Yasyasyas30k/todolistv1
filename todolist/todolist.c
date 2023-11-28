//
// Created by Yassine Boulkenafet on 16/11/2023.
//

#include "todolist.h"

Tache createNewTask() {
    Tache nouvelleTache;

    // Informations rentrées par l'utilisateur

    printf("\nEntrez le nom de la tache : ");
    scanf("%s", nouvelleTache.nom);

    printf("\nEntrez la date de creation de la tache (DD-MM-YYYY) : ");
    scanf("%s", nouvelleTache.dateCreation);

    printf("\nEntrez le statut de la tache (0 pour EnAttente, 1 pour EnCours, 2 pour Terminee) : ");
    scanf("%d", (int*)&nouvelleTache.statut);

    printf("\nEntrez le nombre de jours necessaires pour la tache : ");
    scanf("%d", &nouvelleTache.joursNecessaires);

    return nouvelleTache;
}

ElementListe* createEmptyList(char nomListe[]) {

    ElementListe* nouvelleListe = (ElementListe*)malloc(sizeof(ElementListe)); //Allocation dynamique de la mémoire
    strcpy(nouvelleListe->nomListe, nomListe); // On lui donne le nom entrée par l'utilisateur
    nouvelleListe->suivant = NULL;
    nouvelleListe->precedent = NULL;
    return nouvelleListe;
}

ElementListe* addTask(ElementListe* liste, Tache nouvelleTache) {
    ElementListe* nouvelElement = (ElementListe*)malloc(sizeof(ElementListe));
    nouvelElement->tache = nouvelleTache;
    nouvelElement->suivant = NULL;
    nouvelElement->precedent = NULL;

    // Si la liste est vide, le nouvel élément devient le premier
    if (liste == NULL) {
        return nouvelElement;
    }

    ElementListe* tmp = liste;
    while (tmp->suivant != NULL && tmp->tache.nom[0] != '\0') {
        tmp = tmp->suivant;
    }

    // Si on a trouvé une tâche nulle, la remplacer
    if (tmp->tache.nom[0] == '\0') {
        tmp->tache = nouvelleTache;
        free(nouvelElement);  // Libérer la mémoire du nouvel élément non utilisé
        return liste;
    }

    // Sinon, ajouter le nouvel élément en fin de liste
    tmp->suivant = nouvelElement;
    nouvelElement->precedent = tmp;

    return liste;
}


void displayTaskList(ElementListe* liste) {
    if (liste == NULL) {
        printf("La liste est vide.\n");
        return;
    }

    // Créer une liste temporaire pour faciliter le tri
    ElementListe* temp = NULL;

    // Copier les éléments de la liste originale dans la liste temporaire
    while (liste != NULL) {
        temp = addTask(temp, liste->tache);
        liste = liste->suivant;
    }

    // Trier la liste temporaire en fonction des dates
    temp = sortTaskListByDate(temp);

    // Afficher les éléments de la liste temporaire triée
    while (temp != NULL) {

        const char* statutString;

        // Convertir le statut en chaîne
        if (temp->tache.statut == EnAttente) {
            statutString = "En attente";
        } else if (temp->tache.statut == EnCours) {
            statutString = "En cours";
        } else if (temp->tache.statut == Terminee) {
            statutString = "Terminée";
        } else {
            // Gérer le cas où le statut n'est pas reconnu
            statutString = "Statut inconnu";
        }
        printf("Statut après conversion : %s\n", statutString);


        printf("Nom: %s, Date de création: %s, Statut: %s, Jours nécessaires: %d\n",
               temp->tache.nom,
               temp->tache.dateCreation,
               statutString,
               temp->tache.joursNecessaires);

        temp = temp->suivant;
    }

    // Libérer la mémoire de la liste temporaire
    freeTaskList(temp);
}


ElementListe* sortedInsert(ElementListe* liste, ElementListe* nouvelElement) {
    if (liste == NULL || compareDates(nouvelElement->tache.dateCreation, liste->tache.dateCreation) <= 0) {
        nouvelElement->suivant = liste;
        return nouvelElement;
    }

    ElementListe* tmp = liste;

    while (tmp->suivant != NULL && compareDates(nouvelElement->tache.dateCreation, tmp->suivant->tache.dateCreation) > 0) {
        tmp = tmp->suivant;
    }

    nouvelElement->suivant = tmp->suivant;
    tmp->suivant = nouvelElement;

    return liste;
}

void freeTaskList(ElementListe* liste) {
    while (liste != NULL) {
        ElementListe* temp = liste;
        liste = liste->suivant;
        free(temp);
    }
}
ElementListe* sortTaskListByDate(ElementListe* liste) {
    if (liste == NULL || liste->suivant == NULL) {
        return liste; // La liste est déjà triée ou vide
    }

    ElementListe* sortedList = NULL;
    ElementListe* tmp = liste;

    while (tmp != NULL) {
        ElementListe* current = tmp;
        tmp = tmp->suivant;

        if (sortedList == NULL || compareDates(current->tache.dateCreation, sortedList->tache.dateCreation) <= 0) {
            // Insérer en début de liste
            current->suivant = sortedList;
            current->precedent = NULL;
            if (sortedList != NULL) {
                sortedList->precedent = current;
            }
            sortedList = current;
        } else {
            // Trouver l'endroit où insérer dans la liste triée
            ElementListe* traverse = sortedList;
            while (traverse->suivant != NULL && compareDates(current->tache.dateCreation, traverse->suivant->tache.dateCreation) > 0) {
                traverse = traverse->suivant;
            }

            // Insérer après traverse
            current->suivant = traverse->suivant;
            current->precedent = traverse;
            if (traverse->suivant != NULL) {
                traverse->suivant->precedent = current;
            }
            traverse->suivant = current;
        }
    }

    return sortedList;
}

int compareDates(char date1[], char date2[]) {
    int jour1, mois1, annee1, jour2, mois2, annee2;
    sscanf(date1, "%d-%d-%d", &jour1, &mois1, &annee1);
    sscanf(date2, "%d-%d-%d", &jour2, &mois2, &annee2);
    //Conversion au format DD-MM-YYYY

    if (annee1 != annee2) {
        return annee1 - annee2;
    } else if (mois1 != mois2) {
        return mois1 - mois2;
    } else {
        return jour1 - jour2;
    } // Comparaison des dates
}

ElementListe* removeTask(ElementListe* liste) {
    // Vérifier si la liste est vide
    if (liste == NULL) {
        printf("La liste est vide.\n");
        return NULL;
    }

    // Parcourir la liste et retirer les tâches terminées
    ElementListe* tmp = liste;
    ElementListe* suivant;
    while (tmp != NULL) {
        suivant = tmp->suivant;

        // Vérifier si la tâche est terminée
        if (tmp->tache.statut == Terminee) {
            // Retirer la tâche de la liste
            if (tmp->precedent != NULL) {
                tmp->precedent->suivant = tmp->suivant;
            } else {
                // Si la tâche est la première de la liste
                liste = tmp->suivant;
            }

            if (tmp->suivant != NULL) {
                tmp->suivant->precedent = tmp->precedent;
            }

            // Libérer la mémoire associée à la tâche
            free(tmp);
        }

        tmp = suivant;
    }

    return liste;
}


void updateTask(ElementListe* liste) {
    // Parcourez la liste
    ElementListe* tmp = liste;
    while (tmp != NULL) {
        // Vérifiez si la tâche est en cours et si le nombre de jours est égal à zéro
        if (tmp->tache.statut == EnCours && tmp->tache.joursNecessaires == 0) {
            // Mettez à jour le statut à Terminée
            tmp->tache.statut = Terminee;
        }
        tmp = tmp->suivant;
    }
}


//Algo intermédiaire pour compter les tâches en cours
int countTask(ElementListe* liste) {
    int compteur = 0;
    while (liste != NULL) { // Parcourir la liste
        if (liste->tache.statut == EnCours) {
            compteur++; // Compter les tâches "En_cours"
        }
        liste = liste->suivant;
    }
    return compteur;
}

// Fonction pour changer les tâches de "En_attente" à "En_cours" si nécessaire

// Fonction intermédiaire pour créer une tâche aléatoire
Tache creerTacheAleatoire() {
    Tache nouvelleTache;

    // Génération d'un nom aléatoire (à des fins de démonstration)
    snprintf(nouvelleTache.nom, sizeof(nouvelleTache.nom), "Tache%d", rand() % 1000);

    // Génération d'une date aléatoire (à des fins de démonstration)
    snprintf(nouvelleTache.dateCreation, sizeof(nouvelleTache.dateCreation), "%02d-%02d-%04d",
             rand() % 31 + 1, rand() % 12 + 1, rand() % (2023 - 2000 + 1) + 2000);

    // Attribution aléatoire d'un statut
    nouvelleTache.statut = rand() % 3;

    // Attribution d'un nombre de jours nécessaires aléatoire
    nouvelleTache.joursNecessaires = rand() % 10 + 1; // Entre 1 et 10 jours

    return nouvelleTache;
}
void updateAwaitingTasks(ElementListe* liste) {
    // Compter le nombre de tâches "En_cours"
    int nombreTachesEnCours = countTask(liste);

    // Si moins de 5 tâches "En_cours", changer les tâches "En_attente" à "En_cours"
    if (nombreTachesEnCours < 5) {
        ElementListe* tmp = liste;
        while (tmp != NULL) {
            if (tmp->tache.statut == EnAttente) {
                tmp->tache.statut = EnCours;
                nombreTachesEnCours++;
                if (nombreTachesEnCours == 5) {
                    // On a atteint le nombre maximum de tâches "En_cours"
                    break;
                }
            }
            tmp = tmp->suivant;
        }
    }
}
void saveTaskListToFile(ElementListe* liste, const char* nomListe, const char* extension) {
    char fileName[256];
    snprintf(fileName, sizeof(fileName), "%s.%s", nomListe, extension);

    FILE* file = fopen(fileName, "w"); // Ouvre le fichier en mode écriture

    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier %s\n", fileName);
        perror("Erreur");
        return;
    }

    ElementListe* tmp = liste;

    // Parcourir la liste et ajouter chaque tâche au fichier
    while (tmp != NULL) {
        fprintf(file, "Nom de la tache : %s / Date de création : %s / Statut : %d / Jours nécessaires : %d\n",
                tmp->tache.nom, tmp->tache.dateCreation,
                tmp->tache.statut,
                tmp->tache.joursNecessaires);



        tmp = tmp->suivant;
    }

    if (fclose(file) != 0) {
        fprintf(stderr, "Erreur lors de la fermeture du fichier %s\n", fileName);
        perror("Erreur");
    }
}


ElementListe* loadTaskListFromFileOrCreateNew(const char* nomListe, const char* extension) {
    char fileName[256];
    snprintf(fileName, sizeof(fileName), "%s.%s", nomListe, extension);

    FILE* file = fopen(fileName, "r");

    if (file == NULL) {
        fprintf(stderr, "Le fichier %s n'existe pas. Une nouvelle liste sera créée.\n", fileName);
        return createEmptyList(nomListe);
    }

    ElementListe* loadedList = NULL;
    Tache task;
    // Lire le fichier ligne par ligne
    // On omet les virgules pour simplifier la lecture
    while (fscanf(file, "Nom de la tache : %49[^/]/ Date de création : %49[^/]/ Statut : %d / Jours nécessaires : %d\n", task.nom, task.dateCreation, (int*)&task.statut, &task.joursNecessaires) == 4) {
        printf("Lue à partir du fichier : Nom=%s, Date=%s, Statut=%d, Jours=%d\n", task.nom, task.dateCreation, task.statut, task.joursNecessaires);
        loadedList = addTask(loadedList, task);
    }

    fclose(file); // Fermer le fichier

    return loadedList;
}

void afficherMenu() {
    printf("\n===== Menu Principal =====\n");
    printf("1. Creer une liste de taches\n");
    printf("2. Ajouter une tache à une liste existante\n");
    printf("3. Afficher les taches d'une liste (ordre croissant des dates)\n");
    printf("4. Retirer les taches terminees d'une liste\n");
    printf("5. Mettre a jour les taches en cours d'une liste\n");
    printf("6. Changer les taches En_attente a En_cours dans une liste\n");
    printf("7. Quitter\n");
    printf("Choix : ");
}
void displayTaskListByName(ElementListe* liste, const char* nomListe) {
    printf("\nListe de taches pour '%s' :\n", nomListe);

    // Copier les éléments de la liste originale dans la liste temporaire
    ElementListe* temp = NULL;
    while (liste != NULL) {
        if (strcmp(liste->nomListe, nomListe) == 0) {
            temp = addTask(temp, liste->tache);
        }
        liste = liste->suivant;
    }

    // Trier la liste temporaire en fonction des dates
    temp = sortTaskListByDate(temp);

    // Afficher les éléments de la liste temporaire triée
    while (temp != NULL) {
        printf("Nom: %s, Date de création: %s, Statut: %d, Jours nécessaires: %d\n",
               temp->tache.nom,
               temp->tache.dateCreation,
               temp->tache.statut,
               temp->tache.joursNecessaires);

        temp = temp->suivant;
    }

    // Libérer la mémoire de la liste temporaire
    freeTaskList(temp);
}
