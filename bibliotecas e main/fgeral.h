#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// declaracao de structs e enums principais

typedef enum {
    ESPEC_CLINICO, ESPEC_PEDIATRA, ESPEC_DERMATO, ESPEC_CARDIO, ESPEC_PSICO
} Especialidade;

typedef struct {
    int hora;
    int minuto;
} Horario;

typedef struct {
    int dia;
    int mes;
    int ano;
} Data;

typedef struct {
    Data data;
    Horario inicio;
    bool ocupado;
} SlotConsulta;

typedef struct {
    int id;
    char nome[64];
    Especialidade especialidade;
    Horario inicioManha, fimManha;
    Horario inicioTarde, fimTarde;
    int num_atendimentos;
    SlotConsulta *atendimentos;
} medicos;

typedef struct {
    int id;
    char nome[64];
    char contato[64];
} pacientes;

typedef enum {
    CONS_AGENDADA, CONS_CONCLUIDA, CONS_CANCELADA, CONS_FALTA
} StatusConsulta;

typedef struct {
    int id;
    int idMedico;
    int idPaciente;
    Data data;
    Horario inicio;
    Horario fim;
    int indice_atendimento;
    StatusConsulta status;
    int prioridade;
} consultas;

// funcoes

void limpar_buffer();
bool anobissexto(int ano);
Horario somar_minutos(Horario h, int minutos);
void processar_turno(medicos *medico, Horario inicio_turno, Horario fim_turno);

// validacoes

int validar_nome(const char *n);
int validar_horario(Horario horario, int minh, int maxh);
int validar_interalo_horario(Horario horainic, Horario horafim);
int validar_contato(const char *contato);
bool validar_data(Data d);

// buscas por id

medicos* buscaidm(medicos *x, int tam, int y);
pacientes* buscaidp(pacientes *a, int t, int b);
consultas* buscaidc(consultas *consulta, int *c, int idcerto);