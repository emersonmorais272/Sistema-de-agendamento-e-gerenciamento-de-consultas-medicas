#ifndef FMEDICO_H
#define FMEDICO_H

#include "fgeral.h" 

// Declaração das funções
void adicionar_medico(medicos **medico, int *m, int *idmedico, int *capacidade_medicos);
void listarmedicos(medicos *m, int j);
void atualizarm(medicos *m, int t);
void remover_medico(medicos **medico, int *num_medicos);
int comparar_medicos(const void *a, const void *b);
void ordenar_medicos(medicos *x, int n);
int comparar_medicos_esp(const void *a, const void *b);
void ordenar_medicos_esp(medicos *x, int n);
void menu_medicos(medicos **medico, int *num_medicos, int *idmedico, int *capacidade_medicos);

#endif // MEDICO_H