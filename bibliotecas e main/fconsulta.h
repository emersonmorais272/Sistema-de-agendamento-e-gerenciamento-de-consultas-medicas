#ifndef FCONSULTA_H
#define FCONSULTA_H

#include "fgeral.h" 

// Declaração das funções
void exibir_consulta(medicos *medico, int num_medicos, consultas *consulta, int num_pacientes, pacientes *paciente);
void listarconsultas(medicos *medico, int *num_medicos, int *c, consultas *consulta, int *num_pacientes, pacientes *paciente);
void exibir_consulta(medicos *medico, int num_medicos, consultas *consulta, int num_pacientes, pacientes *paciente);
void atualizarconsulta(int *c, consultas **consulta, medicos *medico, int num_medicos, pacientes *paciente, int num_pacientes);
void remover_consulta(consultas **c, int *tc);
void relatorio(consultas *consulta, int nc, medicos *medico, int nm, pacientes *paciente, int np);
void menu_consultas(medicos **medico, int *num_medicos, int *num_consultas, consultas **consulta, int *idconsulta, int *capacidade_consultas, int *num_pacientes, pacientes **paciente);

#endif // FCONSULTA_H