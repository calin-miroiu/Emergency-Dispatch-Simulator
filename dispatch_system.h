#ifndef DISPATCH_SYSTEM_H
#define DISPATCH_SYSTEM_H
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct unit {
	int id;
	char type;
	int availability;
} unit;

typedef struct incident {
	int id;
	char priority[7];
	char *description;
	char status[11];
} incident;

typedef struct nod {
	incident *element;
	struct nod *next;
	struct nod *prev;
} nod;

typedef struct qnod {
	nod *element;
	struct qnod *next;
	struct qnod *prev;
} qnod;

typedef struct queuenod {
	qnod *head;
	qnod *tail;
	int size;
	int capacity;
} queuenod;

typedef struct intervention {
	incident *incident;
	unit *unit;
} intervention;

typedef struct qintervention {
	intervention *element;
	struct qintervention *next;
	struct qintervention *prev;
} qintervention;

typedef struct qunit {
	unit *element;
	struct qunit *next;
	struct qunit *prev;
} qunit;

typedef struct queueunit {
	qunit *head;
	qunit *tail;
	int size;
	int capacity;
} queueunit;

typedef struct queueintervention {
	qintervention *interv;
	struct queueintervention *next;
} queueintervention;

typedef struct system {
	unit *units;
	nod *incidents;
	qintervention *interventions;
} sistem;

void push(queueintervention **top, qintervention *interv);
qintervention* pop(queueintervention **top);
qnod* enqueue(queuenod *q, nod *element);
void enqueue_front(queuenod *q, nod* element);
sistem *initsystem();
queuenod initq();
queueunit initqunit();
qunit *Uenqueue(queueunit *q, unit *element);
nod *add_incid_syst(sistem *s, int id, char priority[7], char *description);
void add_unit_syst(sistem *s, int index, int id, char type, int availability);
void add_intervention_syst(sistem *s, incident *incid, unit *u);
void add_incident(nod *incid, queuenod *q);
int check_units_availability(queueunit *q);
nod *dequeue(queuenod *q);
unit *Udequeue(queueunit *q);
void citire_echipaje(int n, sistem *sys, queueunit *q, FILE *fin);
void solved_incident(int id, sistem *sys, queueunit *Uqueue, FILE *fout);
void show_interventions(sistem *sys, FILE *fout);
void show_incident(int id, sistem* sys, FILE *fout);
void show_unit(int id, sistem *sys, int nr, FILE *fout);
void adaugare_coada(queuenod *queue_high, queuenod *queue_medium, 
					queuenod *queue_low, nod *iter, int ok);
void dispatch(sistem *sys, queueunit *u_queue, queuenod *queue_high, 
			  queuenod *queue_medium, queuenod *queue_low, 
			  queueintervention **istoric);
void restore_intervention(sistem *sys, qintervention *interventie, 
						  queueunit *Uqueue, queuenod *queue_high, 
						  queuenod *queue_medium, queuenod *queue_low);
void func_add_incident(char *s, sistem *sys, queuenod *queue_high, 
					   queuenod *queue_medium, queuenod *queue_low);
void free_all(sistem *sys, queuenod *queue_high, queuenod *queue_medium, 
			  queuenod *queue_low, queueunit *Uqueue, 
			  queueintervention **istoric);
#endif