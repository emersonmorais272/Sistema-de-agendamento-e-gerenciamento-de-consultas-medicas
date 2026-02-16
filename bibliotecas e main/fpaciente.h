#ifndef FPACIENTE_H
#define FPACIENTE_H

#include "fgeral.h"

// Declaração das funções
void adicionar_paciente(pacientes **paciente, int *p, int *idpaciente, int *capacidade_pacientes);
void listarpacientes(pacientes *x, int n);
void atualizarpacientes(pacientes *p, int t);
void remover_paciente(pacientes **paciente, int *num_pacientes);
int compararpacientes(const void *a, const void *b);
void ordenarpacientes(pacientes *x, int n);
void menu_pacientes(pacientes **paciente, int *num_pacientes, int *idpaciente, int *capacidade_pacientes);

#endif // PACIENTE_H