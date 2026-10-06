#include "dyspatch_system.h"

void push(queueintervention **top, qintervention *interv)
{
	queueintervention *newnode = calloc(1, sizeof(queueintervention));
	newnode->interv = interv;
	newnode->next = *top;
	*top = newnode;
}

qintervention *pop(queueintervention **top)
{
	if (!*top) {
		return NULL;
	}
	queueintervention *nod = *top;
	*top = nod->next;
	qintervention *INTER = nod->interv;
	free(nod);
	return INTER;
}

qnod *enqueue(queuenod *q, nod *element)
{	////////////////////////////////
	qnod *Nnod = calloc(1, sizeof(qnod));
	Nnod->element = element;
	Nnod->prev = q->tail;
	Nnod->next = q->tail->next;
	q->tail->next = Nnod;
	q->head->prev = Nnod;
	q->tail = Nnod;
	q->size++;
	return q->head;
}

void enqueue_front(queuenod *q, nod *element)
{
	qnod *Nnod = calloc(1, sizeof(qnod));
	Nnod->element = element;
	Nnod->prev = q->head;
	Nnod->next = q->head->next;
	q->head->next->prev = Nnod;
	q->head->next = Nnod;
	if (q->size == 0) {
		q->tail = Nnod;
	}
	q->size++;
}

sistem *initsystem()
{
	sistem *sys = calloc(1, sizeof(sistem));
	sys->incidents = calloc(1, sizeof(nod));
	sys->incidents->element = calloc(1, sizeof(incident));
	sys->incidents->element->id = 0;
	strcpy(sys->incidents->element->status, "solved");
	strcpy(sys->incidents->element->priority, "low");
	sys->incidents->element->description = calloc(15, sizeof(char));
	strcpy(sys->incidents->element->description, "test incident");
	sys->incidents->next = sys->incidents;
	sys->incidents->prev = sys->incidents;
	sys->interventions = calloc(1, sizeof(qintervention));
	sys->interventions->next = sys->interventions;
	sys->interventions->prev = sys->interventions;
	return sys;
}

queuenod initq()
{
	queuenod q;
	q.head = calloc(1, sizeof(qnod));
	q.tail = q.head;
	q.head->next = q.head;
	q.head->prev = q.head;
	q.size = 0;
	q.capacity = 0;
	return q;
}

queueunit initqunit()
{
	queueunit q;
	q.head = calloc(1, sizeof(qunit));
	q.tail = q.head;
	q.head->next = q.head;
	q.head->prev = q.head;
	q.size = 0;
	q.capacity = 0;
	return q;
}

qunit *Uenqueue(queueunit *q, unit *element)
{
	qunit *Nnod = calloc(1, sizeof(qunit));
	Nnod->element = element;
	Nnod->prev = q->tail;
	Nnod->next = q->tail->next;
	q->tail->next = Nnod;
	q->head->prev = Nnod;
	q->tail = Nnod;
	q->size++;
	return q->head;
}

nod *add_incid_syst(sistem *s, int id, char priority[7], char *description)
{
	nod *newnode = calloc(1, sizeof(nod));
	newnode->element = calloc(1, sizeof(incident));
	newnode->element->id = id;
	newnode->element->description = calloc(strlen(description) + 1, 
										   sizeof(char));
	strcpy(newnode->element->description, description);
	strcpy(newnode->element->priority, priority);
	newnode->next = s->incidents;
	s->incidents->prev->next = newnode;
	newnode->prev = s->incidents->prev;
	s->incidents->prev = newnode;
	return newnode;
}

void add_unit_syst(sistem *s, int index, int id, char type, int availability)
{
	s->units[index].id = id;
	s->units[index].type = type;
	s->units[index].availability = availability;
}

void add_intervention_syst(sistem *s, incident *incid, unit *u)
{
	qintervention *inter = calloc(1, sizeof(qintervention));
	inter->element->incident = incid;
	inter->element->unit = u;
	inter->prev = s->interventions;
	s->interventions->next = inter;
}

void add_incident(nod *incid, queuenod *q)
{
	enqueue(q, incid);
	strcpy(incid->element->status, "queued");
}

int check_units_availability(queueunit *q)
{
	return q->size;
}

nod *dequeue(queuenod *q)
{
	qnod *nod_sters = q->head->next;
	nod *incident = nod_sters->element;
	q->head->next = nod_sters->next;
	nod_sters->next->prev = q->head;
	if (q->tail == nod_sters) {
		q->tail = q->head;
	}
	q->size--;
	free(nod_sters);
	return incident;
}

unit *Udequeue(queueunit *q)
{
	qunit *nod_sters = q->head->next;
	unit *echipaj = nod_sters->element;
	q->head->next = nod_sters->next;
	nod_sters->next->prev = q->head;
	if (q->tail == nod_sters) {
		q->tail = q->head;
	}
	q->size--;
	free(nod_sters);
	return echipaj;
}

void citire_echipaje(int n, sistem *sys, queueunit *q, FILE *fin)
{
	int id;
	char c;
	for (int i = 0; i < n; i++) {
		fscanf(fin, "%d %c", &id, &c);
		add_unit_syst(sys, i, id, c, 1);
		Uenqueue(q, &sys->units[i]);
	}
}

void solved_incident(int id, sistem *sys, queueunit *Uqueue, FILE *fout)
{
	nod *iter = sys->incidents->next;
	while (iter != sys->incidents) {
		if (iter->element->id == id) {
			break;
		}
		iter = iter->next;
	}
	if (iter == sys->incidents || 
		strcmp(iter->element->status, "intervened") != 0) {
		fprintf(fout, "INVALID OPERATION! ERROR 404\n");
	} else {
		strcpy(iter->element->status, "solved");
		qintervention *ITER = sys->interventions->next;
		while (ITER != sys->interventions) {
			if (ITER->element->incident->id == id) {
				ITER->element->unit->availability = 1;
				Uenqueue(Uqueue, ITER->element->unit);
				break;
			}
			ITER = ITER->next;
		}
	}
}

void show_interventions(sistem *sys, FILE *fout)
{
	if (sys->interventions->next == sys->interventions) {
		fprintf(fout, "No intervention has been initiated\n");
		return;
	}
	qintervention *iter = sys->interventions->next;
	while (iter != sys->interventions) {
		fprintf(fout, "Incident %d was assigned to unit %d, and has the "
			   "following status: \"%s\"\n",
			   iter->element->incident->id, iter->element->unit->id, 
			   iter->element->incident->status);
		iter = iter->next;
	}
}

void show_incident(int id, sistem *sys, FILE *fout)
{
	nod *iter = sys->incidents->next;
	while (iter != sys->incidents) {
		if (iter->element->id == id) {
			fprintf(fout, "Incident %d has %s priority, the following "
				   "description: \"%s\" and is %s\n",
				   id, iter->element->priority, 
				   iter->element->description, iter->element->status);
			return;
		}
		iter = iter->next;
	}
	fprintf(fout, "INVALID OPERATION! ERROR 404\n");
}

void show_unit(int id, sistem *sys, int nr, FILE *fout)
{
	for (int i = 0; i < nr; i++) {
		if (sys->units[i].id == id) {
			fprintf(fout, "Unit %d is type %c and is %s\n", id, 
				   sys->units[i].type, (sys->units[i].availability == 1) 
				   ? "available" : "unavailable");
			return;
		}
	}
	fprintf(fout, "INVALID OPERATION! ERROR 404\n");
}

void adaugare_coada(queuenod *queue_high, queuenod *queue_medium, 
					queuenod *queue_low, nod *iter, int ok)
{
	if (strcmp(iter->element->priority, "high") == 0) {
		if (ok == 1) {
			enqueue(queue_high, iter);
		} else {
			enqueue_front(queue_high, iter);
		}
	} else if (strcmp(iter->element->priority, "medium") == 0) {
		if (ok == 1) {
			enqueue(queue_medium, iter);
		} else {
			enqueue_front(queue_medium, iter);
		}
	} else if (strcmp(iter->element->priority, "low") == 0) {
		if (ok == 1) {
			enqueue(queue_low, iter);
		} else {
			enqueue_front(queue_low, iter);
		}
	}
	strcpy(iter->element->status, "queued");
}

void dispatch(sistem *sys, queueunit *u_queue, queuenod *queue_high, 
			  queuenod *queue_medium, queuenod *queue_low, 
			  queueintervention **istoric)
{
	nod *incident = NULL;
	if (queue_high->size > 0) {
		incident = dequeue(queue_high);
	} else if (queue_medium->size > 0) {
		incident = dequeue(queue_medium);
	} else if (queue_low->size > 0) {
		incident = dequeue(queue_low);
	}
	unit *u = Udequeue(u_queue);
	strcpy(incident->element->status, "intervened");
	u->availability = 0;
	qintervention *interv = calloc(1, sizeof(qintervention));
	interv->element = calloc(1, sizeof(intervention));
	interv->element->incident = incident->element;
	interv->element->unit = u;
	interv->prev = sys->interventions->prev;
	interv->next = sys->interventions;
	sys->interventions->prev->next = interv;
	sys->interventions->prev = interv;
	push(istoric, interv);
}

void restore_intervention(sistem *sys, qintervention *interventie, 
						  queueunit *Uqueue, queuenod *queue_high, 
						  queuenod *queue_medium, queuenod *queue_low)
{
	incident *incid = interventie->element->incident;
	unit *u = interventie->element->unit;
	u->availability = 1;
	Uenqueue(Uqueue, u); 
	strcpy(incid->status, "queued");
	nod *iter = sys->incidents->next;
	while (iter->element->id != incid->id) {
		iter = iter->next;
	}
	adaugare_coada(queue_high, queue_medium, queue_low, iter, 0); 
	interventie->prev->next = interventie->next;
	interventie->next->prev = interventie->prev;
	free(interventie->element);
	free(interventie);
}

void func_add_incident(char *s, sistem *sys, queuenod *queue_high, 
					   queuenod *queue_medium, queuenod *queue_low)
{
	char priority[7], *p, operation[20], description[256] = "";
	p = strtok(s, " ");
	strcpy(operation, p);
	p = strtok(NULL, " ");
	int id = atoi(p);
	p = strtok(NULL, " ");
	strcpy(priority, p);
	p = strtok(NULL, "\n");
	memmove(p, p + 1, strlen(p));
	p[strlen(p) - 1] = '\0';
	strcpy(description, p);
	nod *tmp = add_incid_syst(sys, id, priority, description);
	adaugare_coada(queue_high, queue_medium, queue_low, tmp, 1);
}

void free_all(sistem *sys, queuenod *queue_high, queuenod *queue_medium,
			  queuenod *queue_low, queueunit *Uqueue, 
			  queueintervention **istoric)
{
	while (*istoric != NULL) {
		pop(istoric);
	}
	qnod *top = queue_high->head->next;
	while (top != queue_high->head) {
		qnod *tmp = top;
		top = top->next;
		free(tmp);
	}
	free(queue_high->head);
	top = queue_medium->head->next;
	while (top != queue_medium->head) {
		qnod *tmp = top;
		top = top->next;
		free(tmp);
	}
	free(queue_medium->head);
	top = queue_low->head->next;
	while (top != queue_low->head) {
		qnod *tmp = top;
		top = top->next;
		free(tmp);
	}
	free(queue_low->head);
	qunit *iter = Uqueue->head->next;
	while (iter != Uqueue->head) {
		qunit *temp = iter;
		iter = iter->next;
		free(temp);
	}
	free(Uqueue->head);
	qintervention *icurent = sys->interventions->next;
	while (icurent != sys->interventions) {
		qintervention *temp = icurent;
		icurent = icurent->next;
		free(temp->element);
		free(temp);
	}
	free(sys->interventions);
	nod *inc_curent = sys->incidents->next;
	while (inc_curent != sys->incidents) {
		nod *temp = inc_curent;
		inc_curent = inc_curent->next;
		free(temp->element->description);
		free(temp->element);
		free(temp);
	}
	free(sys->incidents->element->description);
	free(sys->incidents->element);
	free(sys->incidents);
	free(sys->units);
	free(sys);
}