
#include"todolist.h"

int main() {
    ElementListe* listeDeTaches = NULL;
    ElementListe* listeAModifier = NULL;

    char nomListe[50];

    int nombreTaches;

    int choix;
    do {
        afficherMenu();
        scanf("%d", &choix);

        switch (choix) {
            case 1:
                printf("\nCreer une nouvelle liste de taches :\n");
                char nomListe[50];
                printf("Entrez le nom de la liste de taches à créer, ou existante : ");
                scanf("%s", nomListe);

                // Vérifiez si la liste existe déjà
                ElementListe* listeDeTaches = loadTaskListFromFileOrCreateNew(nomListe, "txt");

                printf("Combien de taches souhaitez-vous y ajouter ? ");
                scanf("%d", &nombreTaches);

                for (int i = 0; i < nombreTaches; ++i) {
                    Tache nouvelleTache = createNewTask();
                    listeDeTaches = addTask(listeDeTaches, nouvelleTache);
                }

                // Enregistrez la liste mise à jour dans le fichier
                saveTaskListToFile(listeDeTaches, nomListe, "txt");
                printf("Liste créée avec succès. Verifiez vos fichiers.\n");

                // Libérez la mémoire de la liste
                freeTaskList(listeDeTaches);

                break;

            case 2:
                printf("\nAjouter une nouvelle tache :\n");
                printf("Entrez le nom de la liste de taches à modifier: ");
                scanf("%s", nomListe);

                // Chargez la liste de tâches à partir du fichier ou créez une nouvelle si elle n'existe pas
                listeDeTaches = loadTaskListFromFileOrCreateNew(nomListe, "txt");

                // Vérifiez si la listeDeTaches a été correctement chargée ou créée
                if (listeDeTaches == NULL) {
                    printf("La liste de tache est pour l'instant vide.\n");;
                }

                // Affichez la liste actuelle pour déboguer
                printf("Liste actuelle : \n");
                displayTaskList(listeDeTaches);

                // Ajoutez la nouvelle tâche à la liste
                Tache nouvelleTache = createNewTask();
                listeDeTaches = addTask(listeDeTaches, nouvelleTache);

                // Affichez la liste mise à jour pour déboguer
                printf("\nListe mise à jour : \n");
                displayTaskList(listeDeTaches);

                // Enregistrez la liste mise à jour dans le fichier
                saveTaskListToFile(listeDeTaches, nomListe, "txt");
                printf("Liste mise à jour avec succès. Vérifiez votre fichier %s.txt\n", nomListe);

                // Libérez la mémoire de la liste
                freeTaskList(listeDeTaches);

                break;

            case 3:
                printf("\nAfficher les taches d'une liste (ordre croissant des dates) :\n");
                printf("Entrez le nom de la liste de taches à afficher: ");
                scanf("%s", nomListe);
                 listeDeTaches = loadTaskListFromFileOrCreateNew(nomListe, "txt");
                 printf("\nListe de taches :\n\n");
                displayTaskList(listeDeTaches);
                break;

            case 4:
                printf("\nRetirer les taches terminees d'une liste :\n");
                printf("Entrez le nom de la liste de taches à modifier : ");
                scanf("%s", nomListe);
                listeAModifier = loadTaskListFromFileOrCreateNew(nomListe, "txt");

                // Vérifier si la liste existe
                while (listeAModifier != NULL) {
                    // Retirer les tâches terminées de la liste
                    listeAModifier = removeTask(listeAModifier);

                    // Afficher la liste mise à jour

                    // Enregistrer la liste mise à jour dans un fichier

                    listeAModifier = listeAModifier->suivant;

                }
                saveTaskListToFile(listeAModifier, nomListe, "txt");

                displayTaskList(listeAModifier);
                printf("\nListe mise à jour avec succès.Verifiez votre fichier %s.txt\n", nomListe);

                break;

            case 5:
                printf("\nMettre a jour les taches en cours à terminée d'une liste, si 0jours\n");
                printf("Entrez le nom de la liste de taches à modifier : ");
                scanf("%s", nomListe);
                 listeAModifier = loadTaskListFromFileOrCreateNew(nomListe, "txt");

                if (listeAModifier == NULL) {
                    printf("La liste est vide ou n'existe pas.\n");
                    break;
                }

                // Afficher la liste avant la mise à jour
                printf("\n\nListe avant la mise à jour : \n");
                displayTaskList(listeAModifier);

                // Mettre à jour les tâches
                updateTask(listeAModifier);

                // Afficher la liste mise à jour
                printf("\n\nListe Mise à jour : \n");
                displayTaskList(listeAModifier);

                // Enregistrer la liste mise à jour dans un fichier
                saveTaskListToFile(listeAModifier, nomListe, "txt");

                // Libérer la mémoire de la liste
                freeTaskList(listeAModifier);

                break;

            case 6:
                printf("\nChanger les taches En_attente a En_cours dans une liste\n");
                printf("Entrez le nom de la liste de taches à modifier : ");
                scanf("%s", nomListe);

                listeAModifier = loadTaskListFromFileOrCreateNew(nomListe, "txt");

                if (listeAModifier == NULL) {
                    printf("La liste est vide ou n'existe pas.\n");
                    break;
                }

                // Afficher la liste avant les modifications
                printf("\n\nListe avant la mise à jour : \n");
                displayTaskList(listeAModifier);
                // Mettre à jour les tâches
                updateAwaitingTasks(listeAModifier);

                // Afficher la liste mise à jour
                printf("\n\nListe Mise à jour : \n");
                displayTaskList(listeAModifier);

                // Enregistrer la liste mise à jour dans un fichier
                saveTaskListToFile(listeAModifier, nomListe, "txt");

                break;

            case 7:
                printf("\nAu revoir !\n");
                break;

            default:
                printf("\nChoix invalide. Veuillez entrer un chiffre entre 1 et 7.\n");
        }

    } while (choix != 7);

    // Libérer la mémoire avant de quitter le programme
    ElementListe* temp;
    while (listeDeTaches != NULL) {
        temp = listeDeTaches;
        listeDeTaches = listeDeTaches->suivant;
        free(temp);
    }

    return 0;
}

  /*

    for (int i = 0; i < 20; i++) {
        Tache nouvelleTache = creerTacheAleatoire();
        listeDeTaches = addTask(listeDeTaches, nouvelleTache);
    }





    printf("\nListe initiale :\n");

    displayTaskList(listeDeTaches);
    printf("\nListe apres avoir retire les taches terminees :\n");
    listeDeTaches = removeTask(listeDeTaches);

    displayTaskList(listeDeTaches);

    updateAwaitingTasks(listeDeTaches);

    // Afficher la liste après le changement des tâches "En_attente" à "En_cours"
    printf("\nListe apres le changement des taches EnAttente a EnCours si necessaire :\n");
    displayTaskList(listeDeTaches);


    printf("\nListe apres la mise a jour des taches En cours :\n");
    updateTask(listeDeTaches);
    displayTaskList(listeDeTaches);

    saveTaskListToFile(listeDeTaches, nomListe, "txt");
    ElementListe* loadedList = loadTaskListFromFileOrCreateNew(nomListe, "txt");

    printf("\n Un fichier portant le nom %s.txt a été crée et contient votre liste. S'il existe, il a été modifié.\n",nomListe);









    return 0; */

