#include "fgeral.h"
#include "fmedico.h"
#include "fpaciente.h"
#include "fconsulta.h"

int main(){
    medicos *medico = NULL;
    pacientes *paciente = NULL;
    consultas *consulta = NULL;
    int idmedico = 1, num_medicos = 0;
    int idpaciente = 1, num_pacientes = 0;
    int idconsulta = 1, num_consultas = 0;
    int escolha;
    int capacidade_medicos = 10;
    int capacidade_pacientes = 10;
    int capacidade_consultas = 10;
    FILE *arq_medico, *arq_paciente, *arq_consulta;
    char temp[100];

    medico = (medicos*) malloc(capacidade_medicos * sizeof(medicos));
    paciente = (pacientes*) malloc(capacidade_pacientes * sizeof(pacientes));
    consulta = (consultas*) malloc(capacidade_consultas * sizeof(consultas));


    if (medico == NULL || paciente == NULL || consulta == NULL) {
        printf("Erro ao alocar memoria inicial. O programa sera encerrado.\n");
        return 1;
    }

    //leitura de medicos

    arq_medico = fopen("arq_medico.txt", "r");
    if (arq_medico == NULL) {
        printf("ERRO! Nao foi possivel abrir o arquivo medico para leitura!\n");
        return 1;
    }

    fgets(temp, sizeof(temp), arq_medico);
    fgets(temp, sizeof(temp), arq_medico); 

    while (fscanf(arq_medico, "ID: %d\n", &medico[num_medicos].id) == 1) {
        fscanf(arq_medico, "Nome: %[^\n]\n\n", medico[num_medicos].nome);

        fgets(temp, sizeof(temp), arq_medico);

        if (strstr(temp, "Clinico") != NULL) medico[num_medicos].especialidade = 0;
        else if (strstr(temp, "Pediatra") != NULL) medico[num_medicos].especialidade = 1;
        else if (strstr(temp, "Dermatologista") != NULL) medico[num_medicos].especialidade = 2;
        else if (strstr(temp, "Cardiologista") != NULL) medico[num_medicos].especialidade = 3;
        else if (strstr(temp, "Psicologo") != NULL) medico[num_medicos].especialidade = 4;

        fscanf(arq_medico, "Horario: %d:%d - %d:%d\n", &medico[num_medicos].inicioManha.hora, &medico[num_medicos].inicioManha.minuto, &medico[num_medicos].fimManha.hora, &medico[num_medicos].fimManha.minuto);
        fscanf(arq_medico, "Horario: %d:%d - %d:%d\n", &medico[num_medicos].inicioTarde.hora, &medico[num_medicos].inicioTarde.minuto, &medico[num_medicos].fimTarde.hora, &medico[num_medicos].fimTarde.minuto);

        fgets(temp, sizeof(temp), arq_medico);

        medico[num_medicos].num_atendimentos = 0;
    if (medico[num_medicos].inicioManha.hora != -1) {
        medico[num_medicos].num_atendimentos += ((medico[num_medicos].fimManha.hora * 60 + medico[num_medicos].fimManha.minuto) -
        (medico[num_medicos].inicioManha.hora * 60 + medico[num_medicos].inicioManha.minuto)) / 30;
    }
    if (medico[num_medicos].inicioTarde.hora != -1) {
        medico[num_medicos].num_atendimentos += ((medico[num_medicos].fimTarde.hora * 60 + medico[num_medicos].fimTarde.minuto) -
        (medico[num_medicos].inicioTarde.hora * 60 + medico[num_medicos].inicioTarde.minuto)) / 30;
    }

    medico[num_medicos].atendimentos = malloc(medico[num_medicos].num_atendimentos * sizeof(SlotConsulta));

    Horario hora_atual;
    int c = 0;
    if (medico[num_medicos].inicioManha.hora != -1) {
        hora_atual = medico[num_medicos].inicioManha;
        while (c < medico[num_medicos].num_atendimentos &&
            (hora_atual.hora < medico[num_medicos].fimManha.hora ||
            (hora_atual.hora == medico[num_medicos].fimManha.hora && hora_atual.minuto < medico[num_medicos].fimManha.minuto))) {
            medico[num_medicos].atendimentos[c].inicio = hora_atual;
            medico[num_medicos].atendimentos[c].ocupado = false;
            hora_atual = somar_minutos(hora_atual, 30);
            c++;
        }
    }
    if (medico[num_medicos].inicioTarde.hora != -1) {
        hora_atual = medico[num_medicos].inicioTarde;
        while (c < medico[num_medicos].num_atendimentos &&
            (hora_atual.hora < medico[num_medicos].fimTarde.hora ||
            (hora_atual.hora == medico[num_medicos].fimTarde.hora && hora_atual.minuto < medico[num_medicos].fimTarde.minuto))) {
            medico[num_medicos].atendimentos[c].inicio = hora_atual;
            medico[num_medicos].atendimentos[c].ocupado = false;
            hora_atual = somar_minutos(hora_atual, 30);
            c++;
        }
    }

        num_medicos++;
        idmedico++;
    }

    fclose(arq_medico);

    printf("\nMedicos lidos do arquivo\n");

    //leitura de pacientes

    arq_paciente = fopen("arq_paciente.txt", "r");
    if (arq_paciente == NULL) {
        printf("ERRO! Nao foi possivel abrir o arquivo paciente para leitura!\n");
        return 1;
    }

    fgets(temp, sizeof(temp), arq_paciente);
    fgets(temp, sizeof(temp), arq_paciente); 

    while (fscanf(arq_paciente, "ID: %d\n", &paciente[num_pacientes].id) == 1) {
        fscanf(arq_paciente, "Nome: %[^\n]\n", paciente[num_pacientes].nome);
        fscanf(arq_paciente, "contato: %[^\n]\n", paciente[num_pacientes].contato);

        fgets(temp, sizeof(temp), arq_paciente);
        idpaciente++;
        num_pacientes++;
    }

    fclose(arq_paciente);

    printf("\nPacientes lidos do arquivo\n");

    // leitura da consulta

    arq_consulta = fopen("arq_consulta.txt", "r");
if (arq_consulta == NULL) {
    printf("ERRO! Nao foi possivel abrir o arquivo consulta para leitura!\n");
    return 1;
}

fgets(temp, sizeof(temp), arq_consulta);
fgets(temp, sizeof(temp), arq_consulta);

char status_str[50];
while (fscanf(arq_consulta, "ID: %d\n", &consulta[num_consultas].id) == 1) {
    fscanf(arq_consulta, "ID do medico: %d\n", &consulta[num_consultas].idMedico);
    fscanf(arq_consulta, "ID do paciente: %d\n", &consulta[num_consultas].idPaciente);
    fscanf(arq_consulta, "Data: %d/%d/%d\n", &consulta[num_consultas].data.dia, &consulta[num_consultas].data.mes, &consulta[num_consultas].data.ano);
    fscanf(arq_consulta, "Horario de inicio %d:%d\n", &consulta[num_consultas].inicio.hora, &consulta[num_consultas].inicio.minuto);
    fscanf(arq_consulta, "Horario de fim %d:%d\n", &consulta[num_consultas].fim.hora, &consulta[num_consultas].fim.minuto);
    fscanf(arq_consulta, "Status da consulta: %[^\n]\n", status_str);

    // converter status_str -> enum
    if (strstr(status_str, "Agendada") != NULL) consulta[num_consultas].status = CONS_AGENDADA;
    else if (strstr(status_str, "Cancelada") != NULL) consulta[num_consultas].status = CONS_CANCELADA;
    else if (strstr(status_str, "Concluida") != NULL) consulta[num_consultas].status = CONS_CONCLUIDA;
    else if (strstr(status_str, "Falta") != NULL) consulta[num_consultas].status = CONS_FALTA;

    medicos *medico_consulta = buscaidm(medico, num_medicos, consulta[num_consultas].idMedico);
    if (medico_consulta != NULL) {
        // procurar o slot de horario correspondente
        for (int i = 0; i < medico_consulta->num_atendimentos; i++) {
            if (medico_consulta->atendimentos[i].inicio.hora == consulta[num_consultas].inicio.hora &&
                medico_consulta->atendimentos[i].inicio.minuto == consulta[num_consultas].inicio.minuto) {
                medico_consulta->atendimentos[i].ocupado = true;
                medico_consulta->atendimentos[i].data = consulta[num_consultas].data;
                consulta[num_consultas].indice_atendimento = i;
                break;
            }
        }
    }

    // pula linha separadora
    fgets(temp, sizeof(temp), arq_consulta);

    num_consultas++;
    idconsulta++;
}

fclose(arq_consulta);

printf("\nConsultas lidas do arquivo\n");


    do {
        printf("\nBEM-VINDO, O QUE VOCE DESEJA FAZER?\n0-SAIR DO SISTEMA\n1-AREA MEDICA\n2-AREA DO PACIENTE\n3-AREA DAS CONSULTAS\nINSIRA O NUMERO CORRESPONDENTE A SUA ESCOLHA!\n");
        scanf("%d", &escolha);
        limpar_buffer();
        printf("---------------------------\n");

       switch (escolha) {
            case 0:
                printf("Saindo do sistema. Ate mais!\n");
                break;
            case 1:
                menu_medicos(&medico, &num_medicos, &idmedico, &capacidade_medicos);
                break;
            case 2:
                menu_pacientes(&paciente, &num_pacientes, &idpaciente, &capacidade_pacientes);
                break;
            case 3:
                menu_consultas(&medico, &num_medicos, &num_consultas, &consulta, &idconsulta, &capacidade_consultas, &num_pacientes, &paciente);
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
                break;
        }


    } while (escolha != 0);

    arq_medico = fopen("arq_medico.txt", "w");
    if(arq_medico == NULL){
		printf("ERRO! arquivo medico nao foi aberto!");
	}
	else{
        fprintf(arq_medico, "medicos cadastrados:\n");
        fprintf(arq_medico, "---------------------------\n");

        for (int i = 0; i < num_medicos; i++) {
            fprintf(arq_medico, "ID: %d\n", medico[i].id);
            fprintf(arq_medico, "Nome: %s\n", medico[i].nome);
            switch (medico[i].especialidade) {
                case ESPEC_CLINICO: fprintf(arq_medico,"Especialidade: Clinico Geral\n"); break;
                case ESPEC_PEDIATRA: fprintf(arq_medico,"Especialidade: Pediatra\n"); break;
                case ESPEC_DERMATO: fprintf(arq_medico,"Especialidade: Dermatologista\n"); break;
                case ESPEC_CARDIO: fprintf(arq_medico,"Especialidade: Cardiologista\n"); break;
                case ESPEC_PSICO: fprintf(arq_medico,"Especialidade: Psicologo\n"); break;
                default: break;
            }

        if (medico[i].inicioManha.hora != -1) {
            fprintf(arq_medico,"Horario: %02d:%02d - %02d:%02d\n", medico[i].inicioManha.hora, medico[i].inicioManha.minuto, medico[i].fimManha.hora, medico[i].fimManha.minuto);
        } else {
            fprintf(arq_medico,"Horario: -1:-1 - -1:-1\n");
        }
        if (medico[i].inicioTarde.hora != -1) {
            fprintf(arq_medico,"Horario: %02d:%02d - %02d:%02d\n", medico[i].inicioTarde.hora, medico[i].inicioTarde.minuto,medico[i].fimTarde.hora, medico[i].fimTarde.minuto);
        } else {
            fprintf(arq_medico,"Horario: -1:-1 - -1:-1\n");
        }
            fprintf(arq_medico, "---------------------------\n");	
	    }
        fclose(arq_medico);
    }

    arq_paciente = fopen("arq_paciente.txt", "w");
    if(arq_paciente == NULL){
        printf("ERRO! o arquivo paciente nao foi aberto");
    } else {
        fprintf(arq_paciente, "pacientes cadastrados: %d\n", num_medicos);
        fprintf(arq_paciente, "---------------------------\n");

        for (int i = 0; i < num_pacientes; i++){
            fprintf(arq_paciente, "ID: %d\n", paciente[i].id);
            fprintf(arq_paciente, "Nome: %s\n", paciente[i].nome);
            fprintf(arq_paciente, "contato: %s\n", paciente[i].contato);
            fprintf(arq_paciente, "---------------------------\n");
        }
        fclose(arq_paciente);
    }

    arq_consulta = fopen("arq_consulta.txt", "w");
    if(arq_consulta == NULL){
        printf("ERRO! o arquivo consulta nao foi aberto");
    } else {
        fprintf(arq_consulta, "consultas cadastradas\n");
        fprintf(arq_consulta, "---------------------------\n");

        for (int i = 0; i < num_consultas; i++){
            fprintf(arq_consulta, "ID: %d\n", consulta[i].id);
            fprintf(arq_consulta, "ID do medico: %d\n", consulta[i].idMedico);
            fprintf(arq_consulta, "ID do paciente: %d\n", consulta[i].idPaciente);
            fprintf(arq_consulta, "Data: %02d/%02d/%04d\n", consulta[i].data.dia, consulta[i].data.mes, consulta[i].data.ano);
            fprintf(arq_consulta, "Horario de inicio %02d:%02d\n", consulta[i].inicio.hora, consulta[i].inicio.minuto);
            fprintf(arq_consulta, "Horario de fim %02d:%02d\n", consulta[i].fim.hora, consulta[i].fim.minuto);
            switch (consulta[i].status){
                case CONS_AGENDADA: fprintf(arq_consulta, "Status da consulta: Agendada\n"); break;
                case CONS_CANCELADA: fprintf(arq_consulta, "Status da consulta: Cancelada\n"); break;
                case CONS_CONCLUIDA: fprintf(arq_consulta, "Status da consulta: Concluida\n"); break;
                case CONS_FALTA: fprintf(arq_consulta, "Status da consulta: Falta\n"); break;
                default: break;
            }
            fprintf(arq_consulta, "---------------------------\n");
        }
        fclose(arq_consulta);
    }
    
    free(medico);
    free(paciente);
    free(consulta);
    
    return 0;
}