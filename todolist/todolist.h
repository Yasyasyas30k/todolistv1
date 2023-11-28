//
// Created by Yassine Boulkenafet on 16/11/2023.
//

#include <stdio.h>
#include <stdlib.h>
#include<math.h>
#include <stddef.h>
#include <string.h>



typedef enum {
    EnAttente,
    EnCours,
    Terminee
} StatutTache; //On utlise une énumeration pour un code clair 

typedef struct {
    char nom[50];
    char dateCreation[20];
    StatutTache statut;
    int joursNecessaires;

} Tache; //Définition de la structure tache

typedef struct ElementListe {
    char nomListe[50];
    Tache tache;
    struct ElementListe* suivant;
    struct ElementListe* precedent;
} ElementListe; //Définition de la structure Liste

ElementListe* createEmptyList(char nomListe[]);
ElementListe* addTask(ElementListe* liste, Tache nouvelleTache);
Tache createNewTask();
void displayTaskList(ElementListe* liste);
int compareDates(char date1[], char date2[]); //Algo intermédiaire
ElementListe* removeTask(ElementListe* liste);
void updateTask(ElementListe* liste);
int countTask(ElementListe* liste); //Algo intermédiaire
void updateAwaitingTasks(ElementListe* liste);
Tache creerTacheAleatoire(); //Algo intermédiaire
void saveTaskListToFile(ElementListe* liste, const char* nomListe, const char* extension);
ElementListe* loadTaskListFromFileOrCreateNew(const char* nomListe, const char* extension);
ElementListe* sortTaskListByDate(ElementListe* liste);
void freeTaskList(ElementListe* liste);
void afficherMenu();
void displayTaskListByName(ElementListe* liste, const char* nomListe);








