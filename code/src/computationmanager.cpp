//     ____  __________     ___   ____ ___  _____ //
//    / __ \/ ____/ __ \   |__ \ / __ \__ \|__  / //
//   / /_/ / /   / / / /   __/ // / / /_/ / /_ <  //
//  / ____/ /___/ /_/ /   / __// /_/ / __/___/ /  //
// /_/    \____/\____/   /____/\____/____/____/   //
// Auteurs : Prénom Nom, Prénom Nom


// A vous de remplir les méthodes, vous pouvez ajouter des attributs ou méthodes pour vous aider
// déclarez les dans ComputationManager.h et définissez les méthodes ici.
// Certaines fonctions ci-dessous ont déjà un peu de code, il est à remplacer, il est là temporairement
// afin de faire attendre les threads appelants et aussi afin que le code compile.

#include "computationmanager.h"


ComputationManager::ComputationManager(int maxQueueSize): MAX_TOLERATED_QUEUE_SIZE(maxQueueSize)
{

}
// Relatif à la méthode put(T item) pour ajouter une requête au buffer
int ComputationManager::requestComputation(Computation c) {

    size_t indexOfComputationType = (size_t)c.computationType;
    int id;
    monitorIn();

    //récupération de l'id de la requête
    requestMutex.lock();
    id = nextId++;
    requestMutex.unlock();

    if (buffers.at(indexOfComputationType).size() >= MAX_TOLERATED_QUEUE_SIZE) {
        // Si le buffer est plein, on attend qu'il y ait de la place
        wait(fulls.at((size_t)c.computationType));
    }
    buffers.at(indexOfComputationType).emplace(c, id);

    // On signale qu'il y a une requête dans le buffer
    signal(empties.at(indexOfComputationType));
    monitorOut();
    return id;
}


void ComputationManager::abortComputation(int id) {
    // TODO
}

Result ComputationManager::getNextResult() {
   monitorIn();
   if(results.empty()){
       // Si le buffer est vide, on attend qu'il y ait un résultat
       wait(resultAvailable);
   }

   //TODO recuperer le resultat dans l'ordre


    // On récupère le résultat - TODO A remplacer par le code qui récupère le résultat dans l'ordre
    Result result = results.front();
    results.pop_front();
    monitorOut();
    return result;
}

Request ComputationManager::getWork(ComputationType computationType) {

    size_t indexOfComputationType = (size_t)computationType;
    monitorIn();

    if (buffers.at(indexOfComputationType).empty()) {
        // Si le buffer est vide, on attend qu'il y ait une requête
        wait(empties.at(indexOfComputationType));
    }
    // On récupère la requête
    Request request = buffers.at(indexOfComputationType).front();
    buffers.at(indexOfComputationType).pop();
    // On signale qu'il y a de la place dans le buffer
    signal(fulls.at(indexOfComputationType));
    monitorOut();

    return  request;
}

bool ComputationManager::continueWork(int id) {
    // TODO
    return true;
}

void ComputationManager::provideResult(Result result) {
    monitorIn();

    //TODO faire un splice pour mettre le resultat directement au bon index
    results.push_back(result);
    results.sort([](const Result &f, const Result &s) { return f.getId() < s.getId(); });

    // On signale qu'il y a un resultat dans le buffer
    signal(resultAvailable);
    monitorOut();
}

void ComputationManager::stop() {
    // TODO
}
