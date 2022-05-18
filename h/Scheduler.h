#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "PCB.h"

class Scheduler {
private:
    static PCB *head, *tail; // pokazuje na pocetak i kraj liste spremnih procesa(u strukturama se nalaze next pokazivaci)
    static PCB* idleProcess;

    // zabrana bilo kakvog pravljenja objekata
    Scheduler() = default;
public:
    // stavlja process na kraj liste za cekanje u Scheduleru
    static void put(PCB* proccess);

    /* uzima proces sa pocetka liste za cekanje na Scheduleru, uvek ce postojati neka nit koja moze da se pozove
     Ako ne postoji nijedna druga, vraca se idle nit koja uposleno ceka na promenu konteksta
    */
    static PCB* get();
};


#endif //SCHEDULER_H