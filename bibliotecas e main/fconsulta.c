#include "fconsulta.h"
#include "fgeral.h"

void adicionar_consulta(medicos **medico, int *num_medicos, int *c, consultas **consulta, int *idconsulta, int *capacidade_consultas, int *num_pacientes, pacientes *paciente) {
    consultas a;
    int idMedico, idPaciente;
    int atendimento_escolhido = -1;
    medicos *medico_selecionado = NULL;
    pacientes *paciente_selecionado = NULL;
    int prioridade;
    bool consulta_encontrado = false;

    if (*num_medicos == 0) {
        printf("Nao ha medicos cadastrados. Agendamento cancelado.\n");
        return;
    }
    if (*num_pacientes == 0) {
        printf("Nao ha pacientes cadastrados. Agendamento cancelado.\n");
        return;
    }

    printf("\n--- AGENDAMENTO DE CONSULTA ---\n");
    listarmedicos(*medico, *num_medicos);
    printf("Qual o ID do medico para a consulta? ");
    scanf("%d", &idMedico);
    
    medico_selecionado = buscaidm(*medico, *num_medicos, idMedico);
    if (medico_selecionado == NULL) {
        printf("Medico nao encontrado. Agendamento cancelado.\n");
        return;
    }

    listarpacientes(paciente, *num_pacientes);
    printf("Qual o ID do paciente para a consulta? ");
    scanf("%d", &idPaciente);

    paciente_selecionado = buscaidp(paciente, *num_pacientes, idPaciente);
    if (paciente_selecionado == NULL) {
        printf("Paciente nao encontrado. Agendamento cancelado.\n");
        return;
    }

    Data data_temp;
    do {
        printf("Qual a data da consulta? (dd mm aaaa) ");
        scanf("%d %d %d", &data_temp.dia, &data_temp.mes, &data_temp.ano);
    } while (!validar_data(data_temp));

    printf("qual a prioridade do caso?\n0-NORMAL\n1-URGENTE\n");
    scanf("%d", &prioridade);

    if (prioridade != 1){
        printf("\n--- Horarios disponiveis para %s ---\n", medico_selecionado->nome);

        if (medico_selecionado->inicioManha.hora != -1) {
            processar_turno(medico_selecionado, medico_selecionado->inicioManha, medico_selecionado->fimManha);
        }
        if (medico_selecionado->inicioTarde.hora != -1) {
            processar_turno(medico_selecionado, medico_selecionado->inicioTarde, medico_selecionado->fimTarde);
        }

        printf("Escolha o slot de consulta: ");
        scanf("%d", &atendimento_escolhido);
        consulta_encontrado = true;
    } else {
        printf("Caso urgente detectado. Buscando o horario mais cedo possivel...\n");
        for (int i = 0; i < medico_selecionado->num_atendimentos; i++) {
            if (!medico_selecionado->atendimentos[i].ocupado) {
                atendimento_escolhido = i;
                consulta_encontrado = true;
                break;
            }
        }

    }

    if (consulta_encontrado && atendimento_escolhido >= 0 && atendimento_escolhido < medico_selecionado->num_atendimentos && !medico_selecionado->atendimentos[atendimento_escolhido].ocupado) {
        a.id = (*idconsulta);
        a.idMedico = idMedico;
        a.idPaciente = idPaciente;
        a.data = data_temp;
        a.inicio = medico_selecionado->atendimentos[atendimento_escolhido].inicio;
        a.fim = somar_minutos(a.inicio, 30);
        a.status = CONS_AGENDADA;
        a.prioridade = prioridade; 
        a.indice_atendimento = atendimento_escolhido;
        medico_selecionado->atendimentos[atendimento_escolhido].data = data_temp;
        medico_selecionado->atendimentos[atendimento_escolhido].ocupado = true;
        
        if (*c >= *capacidade_consultas) {
            *capacidade_consultas += 10;
            *consulta = (consultas*)realloc(*consulta, *capacidade_consultas * sizeof(consultas));

            if (*consulta == NULL) {
                printf("Erro ao realocar memoria para consultas.\n");
                return;
            }
        }

        (*consulta)[*c] = a;
        (*c)++;
        (*idconsulta)++;

        printf("\nConsulta agendada com sucesso!\n");
    } else {
        printf("Slot de consulta invalido ou ja ocupado. Agendamento cancelado.\n");
    }
}

void listarconsultas(medicos *medico, int *num_medicos, int *c, consultas *consulta, int *num_pacientes, pacientes *paciente){
    int idMedicoaux, idPacienteaux;
    if (*c== 0) {
        printf("Nao existe nenhuma consulta cadastrado no sistema!\n");
        return;
    }
    
    for(int i = 0; i < *c; i++) {
        printf("---LISTA DE CONSULTAS---\n");
        printf("ID da consulta: %d\n", consulta[i].id);

        printf("\n");
        idMedicoaux = consulta[i].idMedico;
        medicos *medico_aux = buscaidm(medico, *num_medicos, idMedicoaux);
        if (medico_aux != NULL) {
            printf("Medico: %s\n", medico_aux->nome);
            printf("ID do medico: %d\n", consulta[i].idMedico);
            switch (medico_aux->especialidade) {
                case ESPEC_CLINICO: printf("Especialidade: Clinico Geral\n"); break;
                case ESPEC_PEDIATRA: printf("Especialidade: Pediatra\n"); break;
                case ESPEC_DERMATO: printf("Especialidade: Dermatologista\n"); break;
                case ESPEC_CARDIO: printf("Especialidade: Cardiologista\n"); break;
                case ESPEC_PSICO: printf("Especialidade: Psicologo\n"); break;
                default: printf("Especialidade: Outra\n"); break;
            }
        }

        printf("\n");
        idPacienteaux = consulta[i].idPaciente;
        pacientes *paciente_aux = buscaidp(paciente, *num_pacientes, idPacienteaux);
        if (paciente_aux != NULL) {
            printf("Paciente: %s\n", paciente_aux->nome);
            printf("ID do paciente: %d\n", paciente_aux->id);
            printf("Contato: %s\n", paciente_aux->contato);
        } else {
            printf("Paciente com ID %d nao encontrado.\n", idPacienteaux);
        }

        printf("\n");        
        printf("Data: %02d/%02d/%4d\n", consulta[i].data.dia, consulta[i].data.mes, consulta[i].data.ano);
        printf("Horario de inicio %02d:%02d\n", consulta[i].inicio.hora, consulta[i].inicio.minuto);
        printf("Horario de fim %02d:%02d\n", consulta[i].fim.hora, consulta[i].fim.minuto);
        switch (consulta[i].status){
            case CONS_AGENDADA:
            printf("Status da consulta: Agendada\n");
            break;
            case CONS_CANCELADA:
            printf("Status da consulta: Cancelada\n");
            break;
            case CONS_CONCLUIDA:
            printf("Status da consulta: Concluida\n");
            break;
            case CONS_FALTA:
            printf("Status da consulta: Faltada\n");
            break;
            default: break;
        }
        printf("-------------------------------\n");
    }
}

void exibir_consulta(medicos *medico, int num_medicos, consultas *consulta, int num_pacientes, pacientes *paciente){
 
    printf("--- CONSULTA ENCONTRADA ---\n");
    printf("ID: %d\n", consulta->id);

    printf("\n");
    
    medicos *medico_encontrado = buscaidm(medico, num_medicos, consulta->idMedico);
    if (medico_encontrado != NULL){
        printf("Medico: %s\n", medico_encontrado->nome);
        printf("ID do medico: %d\n", medico_encontrado->id);
        switch (medico_encontrado->especialidade) {
            case ESPEC_CLINICO: printf("Especialidade: Clinico Geral\n"); break;
            case ESPEC_PEDIATRA: printf("Especialidade: Pediatra\n"); break;
            case ESPEC_DERMATO: printf("Especialidade: Dermatologista\n"); break;
            case ESPEC_CARDIO: printf("Especialidade: Cardiologista\n"); break;
            case ESPEC_PSICO: printf("Especialidade: Psicologo\n"); break;
            default: printf("Especialidade: Outra\n"); break;
        }
    } else {
        printf("Medico (ID: %d) nao encontrado.\n", consulta->idMedico);
    }
    
    printf("\n");
    pacientes *paciente_escolhido = buscaidp(paciente, num_pacientes, consulta->idPaciente);
    if (paciente_escolhido != NULL){
        printf("Paciente: %s\n", paciente_escolhido->nome);
        printf("ID do paciente: %d\n", paciente_escolhido->id);
        printf("Contato: %s\n", paciente_escolhido->contato);
    } else {
        printf("Paciente (ID: %d) nao encontrado.\n", consulta->idPaciente);
    }

    printf("\n");
    printf("Data: %02d/%02d/%04d\n", consulta->data.dia, consulta->data.mes, consulta->data.ano);
    printf("Horario de inicio %02d:%02d\n", consulta->inicio.hora, consulta->inicio.minuto);
    printf("Horario de fim %02d:%02d\n", consulta->fim.hora, consulta->fim.minuto);
    switch (consulta->status){
        case CONS_AGENDADA: printf("Status da consulta: Agendada\n"); break;
        case CONS_CANCELADA: printf("Status da consulta: Cancelada\n"); break;
        case CONS_CONCLUIDA: printf("Status da consulta: Concluida\n"); break;
        case CONS_FALTA: printf("Status da consulta: Faltada\n"); break;
        default: break;
    }
    printf("-------------------------------\n");
}

void atualizarconsulta(int *c, consultas **consulta, medicos *medico, int num_medicos, pacientes *paciente, int num_pacientes){
    consultas *cauxi;
    int idaux, escolha, novo_status;
    Data data_aux;
    int escolha_horario;


    if ((*c) == 0) {
        printf("Nao existe nenhuma consulta cadastrado no sistema!\n");
        return;
    }

    printf("qual o ID da consulta que sera atualizada?\n");
    scanf("%d", &idaux);
    cauxi = buscaidc(*consulta, c, idaux);
    medicos *medico_c = buscaidm(medico, num_medicos, cauxi->idMedico);
    
    if (cauxi != NULL) {

        exibir_consulta(medico, num_medicos, cauxi, num_pacientes, paciente);
        printf("o que voce quer modificar?\n1-DATA\n2-HORARIO\n3-STAUTS DA CONSULTA\n INSIRA O NUMERO CORRESPONDENTE A SUA ESCOLHA: ");
        scanf("%d", &escolha);

        switch (escolha){
        case 1:
                do {
                    printf("qual a data nova?");
                    scanf("%d %d %d", &data_aux.dia, &data_aux.mes, &data_aux.ano);
                } while (!validar_data(data_aux));
                cauxi->data = data_aux;
                printf("Data da consulta atualizada com sucesso!\n");
            break;
        case 2:
                if (medico_c != NULL) {
                    printf("\n--- Horarios disponiveis para %s ---\n", medico_c->nome);

                    if (medico_c->inicioManha.hora != -1) {
                        processar_turno(medico_c, medico_c->inicioManha, medico_c->fimManha);
                    }
                    if (medico_c->inicioTarde.hora != -1) {
                        processar_turno(medico_c, medico_c->inicioTarde, medico_c->fimTarde);
                    }

                    printf("\nEscolha o slot de consulta: ");
                    scanf("%d", &escolha_horario);

                    int indice_atendimento_novo = escolha_horario;

                    if (indice_atendimento_novo >= 0 && indice_atendimento_novo < medico_c->num_atendimentos && !medico_c->atendimentos[indice_atendimento_novo].ocupado) {
            
                        if (cauxi->indice_atendimento >= 0 && cauxi->indice_atendimento < medico_c->num_atendimentos) {
                            medico_c->atendimentos[cauxi->indice_atendimento].ocupado = false;
                        }

                        cauxi->inicio = medico_c->atendimentos[indice_atendimento_novo].inicio;
                        cauxi->fim = somar_minutos(cauxi->inicio, 30);
                        cauxi->indice_atendimento = indice_atendimento_novo;
                        
                
                        medico_c->atendimentos[indice_atendimento_novo].ocupado = true;
                        medico_c->atendimentos[indice_atendimento_novo].data = cauxi->data;

                        printf("Horario da consulta atualizado com sucesso para %02d:%02d!\n", cauxi->inicio.hora, cauxi->inicio.minuto);
                    } else {
                        printf("Opcao de horario invalida ou ja ocupada. Nenhum dado foi alterado.\n");
                    }
                } else {
                    printf("Erro: Medico da consulta nao encontrado.\n");
            }
                break;
        case 3:
            printf("qual o novo status da consulta?\n1-AGENDADA\n2-CANCELADA\3-");
            scanf("%d", &novo_status);
            switch (novo_status){
                case 1: cauxi->status=CONS_AGENDADA; break;
                case 2: cauxi->status=CONS_CANCELADA; break;
                case 3: cauxi->status=CONS_CONCLUIDA; break;
                case 4: cauxi->status=CONS_FALTA; break;
                default: break;
            }
        default:
            break;
        }
    } else {
        printf("Consulta com ID %d nao encontrada.\n", idaux);
    }
}

void remover_consulta(consultas **c, int *tc) {
    int buscaidd;
    int encont = 0;

    if (*tc==0) {
        printf("Nenhuma consultada foi marcada!\n");
        return;
    }
    printf("Digite o id da consulta que voce deseja remover: \n");
    scanf("%d", &buscaidd);

    for (int i=0; i<*tc; i++) {
        if ((*c)[i].id==buscaidd) {
            for (int j=i; j<*tc-1; j++) {
                (*c)[j]=(*c)[j+1];
            }
            (*tc)--;
            encont=1;
            printf("Consulta com ID %d removida com sucesso!\n", buscaidd);
            break;
        }
    }
    if (!encont) {
        printf("Nao foi encontrado a onsulta com ID %d.\n", buscaidd);
    }

}

void relatorio(consultas *consulta, int nc, medicos *medico, int nm, pacientes *paciente, int np) {

    int idm;
    bool encontrei=false;
    int prioridade_filtro;
    StatusConsulta filtrocons;
    Data filtro_data;
    if (nc==0) {
        printf("Nao ha consultas agendadas para gerar relatorio.\n");
        return;
    }
    int filtro;
    printf("\n--- GERAR RELATORIO DE CONSULTAS ---\n");
    printf("Escolha o criterio de filtragem:\n");
    printf("1 - Por Medico\n");
    printf("2 - Por Paciente\n");
    printf("3 - Por Status\n");
    printf("4 - Por Especialidade\n");
    printf("5 - Por data\n");
    printf("6 - Por Prioridade\n");
    printf("Opcao: ");
    scanf("%d", &filtro);

    switch (filtro) {
        case 1:
                printf("Digite o id do medico: \n");
                scanf("%d", &idm);
                for (int i=0; i<nc; i++) {
                    if (consulta[i].idMedico== idm) {
                        exibir_consulta(medico,nm, &consulta[i], np, paciente);
                        encontrei = true;
                    }
                }
                break;
        case 2:
            printf("Digite o id do paciente: \n");
            scanf("%d", &idm);
            for (int i=0; i<nc; i++) {
                if (consulta[i].idPaciente==idm) {
                    exibir_consulta(medico,nm, &consulta[i], np, paciente);
                    encontrei = true;
                }
            }
            break;
        case 3:
            printf("Escolha o status:\n");
            printf("0 - Agendada\n1 - Concluida\n2 - Cancelada\n3 - Falta\n");
            printf("Opcao: ");
            scanf ("%d", (int *)&filtrocons);
            for (int i=0; i<nc; i++) {
                if (consulta[i].status==filtrocons) {
                    exibir_consulta(medico,nm, &consulta[i], np, paciente);
                    encontrei = true;
                }
            }
            break;
            case 4:
            printf("Escolha a especialidade:\n");
            printf("0 - Clinico\n1 - Pediatra\n2 - Dermato\n3 - Cardio\n4 - Psico\n");
            printf("Opcao: ");
            int esp_filtro;
            scanf("%d", &esp_filtro);
            for (int i=0; i<nc; i++) {
                medicos *medico_esp = buscaidm(medico, nm, consulta[i].idMedico);
                if (medico_esp != NULL && medico_esp->especialidade==esp_filtro) {
                    exibir_consulta(medico,nm, &consulta[i], np, paciente);
                    encontrei = true;
                }
            }
            break;
        case 5:
            do {
                printf("Digite a data: dd mm aaaa \n");
                scanf("%d %d %d", &filtro_data.dia, &filtro_data.mes, &filtro_data.ano);
                if (!validar_data(filtro_data)) {
                    printf("A data inserida e invalida. Por favor, tente novamente.\n");
                    return;
                }
            } while (!validar_data(filtro_data));

            printf("\n--- Consultas na data %02d/%02d/%04d---\n", filtro_data.dia, filtro_data.mes, filtro_data.ano);
            for (int i=0; i<nc; i++) {
                if (consulta[i].data.dia==filtro_data.dia && consulta[i].data.mes==filtro_data.mes && consulta[i].data.ano==filtro_data.ano) {
                    exibir_consulta(medico,nm, &consulta[i], np, paciente);
                    encontrei = true;
                }
            }
            if (!encontrei) {
                printf("Nenhuma consulta foi encontrada nessa data.\n");
            }
            break;
        case 6:
            printf("Escolha o status:\n");
            printf("\n0 - NORMAL\n1 - URGENCIA\n");
            printf("Opcao: ");
            scanf("%d", &prioridade_filtro);

            for (int i = 0; i < nc; i++) {
                    if (consulta[i].prioridade == prioridade_filtro) {
                        exibir_consulta(medico, nm, &consulta[i], np, paciente);
                        encontrei = true;
                    }
                }
            break;
        case 7: 
        default:
            break;
    }
}

void menu_consultas(medicos **medico, int *num_medicos, int *num_consultas, consultas **consulta, int *idconsulta, int *capacidade_consultas, int *num_pacientes, pacientes **paciente) {
    int escolha_consulta;
    int escolha_id;
    do {
        printf("\nBEM-VINDO A AREA DAS CONSULTAS, O QUE VOCE DESEJA FAZER?\n0-VOLTAR AO MENU PRINCIPAL\n1-ADICIONAR CONSULTA\n2-LISTAR CONSULTAS\n3-ATUALIZAR CONSULTA\n4-REMOVER CONSULTA\n5-BUSCAR ID\n6-MENU DE RELATORIOS\nINSIRA O NUMERO CORRESPONDENTE A SUA ESCOLHA!\n");
        scanf("%d", &escolha_consulta);
        limpar_buffer();
        
        switch (escolha_consulta){
        case 0:
            break;
        case 1:
            adicionar_consulta(medico, num_medicos, num_consultas, consulta, idconsulta, capacidade_consultas, num_pacientes, *paciente);
            break;
        case 2:
            listarconsultas(*medico, num_medicos, num_consultas, *consulta, num_pacientes, *paciente);
            break;
        case 3:
            atualizarconsulta(num_consultas, consulta, *medico, *num_medicos, *paciente, *num_pacientes);
            break;
        case 4:
            remover_consulta(consulta, num_consultas);
            break;
        case 5:
            if ((*num_consultas) == 0) {
                printf("Nao existe nenhuma consulta cadastrado no sistema!\n");
                break;
            }
            printf("Digite o id que voce deseja buscar: \n");
            scanf("%d", &escolha_id);
            consultas *consulta_encontrada = buscaidc(*consulta, num_consultas, escolha_id);
            
            if (consulta_encontrada != NULL) {
                exibir_consulta(*medico, *num_medicos, consulta_encontrada, *num_pacientes, *paciente);
            } else {
                printf("Consulta com ID %d nao encontrada.\n", escolha_id);
            }
            break;
        
         case 6:
                relatorio(*consulta,*num_consultas,*medico, *num_medicos, *paciente, *num_pacientes);
                break;
        default:
            printf("Opcao invalida. Tente novamente!\n");
            break;
        }
    } while (escolha_consulta != 0);
}
