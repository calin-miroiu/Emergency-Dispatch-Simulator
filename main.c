#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "dispatch_system.h"

int main()
{
	queuenod queue_high = initq();
	queuenod queue_medium = initq();
	queuenod queue_low = initq();
	sistem *sys = initsystem();
	queueintervention *istoric = NULL;
	char s[256];
	int n;
	FILE *fin = fopen("dispatch.in", "r");
	FILE *fout = fopen("dispatch.out", "w");
	queueunit Uqueue = initqunit();
	fscanf(fin, "%d ", &n);
	int nr = n;
	sys->units = calloc(n, sizeof(unit));
	citire_echipaje(n, sys, &Uqueue, fin);
	fscanf(fin, "%d ", &n);
	for (int i = 0; i < n; i++) {
		fgets(s, 256, fin);
		if (strstr(s, "CHECK_UNITS_AVAILABILITY") != 0) {
			fprintf(fout, "Number of available units: %d\n", 
				   check_units_availability(&Uqueue));
		} else if (strstr(s, "ADD_INCIDENT") != 0) {
			func_add_incident(s, sys, &queue_high, &queue_medium, 
							  &queue_low);
		} else if (strstr(s, "UNDO_LAST_DISPATCH") != 0) {
			qintervention *interventie = NULL;
			while (istoric) {
				interventie = pop(&istoric);
				if (strcmp(interventie->element->incident->status,
							"intervened") == 0) {
					break;
				}
			}
			if (!interventie || 
				strcmp(interventie->element->incident->status, 
						"intervened") != 0) {
				fprintf(fout, "INVALID OPERATION! ERROR 404\n");
			} else {
				restore_intervention(sys, interventie, &Uqueue, 
									 &queue_high, &queue_medium, 
									 &queue_low);
			}
		} else if (strstr(s, "DISPATCH") != 0) {
			if (check_units_availability(&Uqueue) == 0 ||
				(queue_high.size == 0 && queue_medium.size == 0 && 
				 queue_low.size == 0)) {
				fprintf(fout, "INVALID OPERATION! ERROR 404\n");
			} else {
				dispatch(sys, &Uqueue, &queue_high, &queue_medium, 
						 &queue_low, &istoric);
			}
		} else if (strstr(s, "SOLVED_INCIDENT") != 0) {
			int id = atoi(s + 16);
			solved_incident(id, sys, &Uqueue, fout);
		} else if (strstr(s, "SHOW_UNIT") != 0) {
			int id = atoi(s + 10);
			show_unit(id, sys, nr, fout);
		} else if (strstr(s, "SHOW_INCIDENT") != 0) {
			int id = atoi(s + 14);
			show_incident(id, sys, fout);
		} else if (strstr(s, "SHOW_INTERVENTIONS") != 0) {
			show_interventions(sys, fout);
		}
	}
	free_all(sys, &queue_high, &queue_medium, &queue_low, &Uqueue, &istoric);
	fclose(fin);
	fclose(fout);
	return 0;
}