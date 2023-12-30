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

    int idA;
    int idB;
    int idC;

    switch (c.computationType) {
        case ComputationType::A:
            monitorIn();
            //récupération de l'id de la requête
            mutex.lock();
            idA = nextId++;
            mutex.unlock();

            if (bufferA.size() >= MAX_TOLERATED_QUEUE_SIZE) {
                // Si le buffer est plein, on attend qu'il y ait de la place
                wait(fullA);
            }
            bufferA.emplace(c, idA);
            // On signale qu'il y a une requête dans le buffer
            signal(emptyA);
            monitorOut();
            return idA;

        case ComputationType::B:
            monitorIn();
            //récupération de l'id de la requête
            mutex.lock();
            idB = nextId++;
            mutex.unlock();
            if (bufferB.size() >= MAX_TOLERATED_QUEUE_SIZE) {
                // Si le buffer est plein, on attend qu'il y ait de la place
                wait(fullB);
            }
            bufferB.emplace(c, idB);
            // On signale qu'il y a une requête dans le buffer
            signal(emptyB);
            monitorOut();
            return idB;
        case ComputationType::C:
            monitorIn();
            //récupération de l'id de la requête
            mutex.lock();
            idC = nextId++;
            mutex.unlock();

            if (bufferC.size() >= MAX_TOLERATED_QUEUE_SIZE) {
                // Si le buffer est plein, on attend qu'il y ait de la place
                wait(fullC);
            }
            bufferC.emplace(c, idC);
            // On signale qu'il y a une requête dans le buffer
            signal(emptyC);
            monitorOut();
            return idC;
    }
    monitorOut();
    return -1;
}


void ComputationManager::abortComputation(int id) {
    // TODO
}

Result ComputationManager::getNextResult() {
    // TODO
    // Replace all of the code below by your code

    // Filled with some code in order to make the thread in the UI wait
    monitorIn();
    auto c = Condition();
    wait(c);
    monitorOut();

    return Result(-1, 0.0);
}

Request ComputationManager::getWork(ComputationType computationType) {

    switch (computationType) {
        case ComputationType::A:{
            monitorIn();
            if (bufferA.empty()) {
                // Si le buffer est vide, on attend qu'il y ait une requête
                wait(emptyA);
            }
            // On récupère la requête
            Request request = bufferA.front();
            bufferA.pop();
            // On signale qu'il y a de la place dans le buffer
            signal(fullA);
            monitorOut();
            return request;
        }
        case ComputationType::B:{
            monitorIn();
            if (bufferB.empty()) {
                // Si le buffer est vide, on attend qu'il y ait une requête
                wait(emptyB);
            }
            // On récupère la requête
            Request request = bufferB.front();
            bufferB.pop();
            // On signale qu'il y a de la place dans le buffer
            signal(fullB);
            monitorOut();
            return request;
        }
        case ComputationType::C:{
            monitorIn();
            if (bufferC.empty()) {
                // Si le buffer est vide, on attend qu'il y ait une requête
                wait(emptyC);
            }
            // On récupère la requête
            Request request = bufferC.front();
            bufferC.pop();
            // On signale qu'il y a de la place dans le buffer
            signal(fullC);
            monitorOut();
            return request;
        }
    }
    return Request(Computation(computationType), -1);
}

bool ComputationManager::continueWork(int id) {
    // TODO
    return true;
}

void ComputationManager::provideResult(Result result) {
    // TODO
}

void ComputationManager::stop() {
    // TODO
}
