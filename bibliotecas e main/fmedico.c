#include "fmedico.h"
#include "fgeral.h"

void adicionar_medico(medicos **medico, int *m, int *idmedico, int *capacidade_medicos){
    medicos a;
    int turno;
    a.inicioManha.hora = -1;
    a.inicioManha.minuto = -1;
    a.fimManha.hora = -1;
    a.fimManha.minuto = -1;
    a.inicioTarde.hora = -1;
    a.inicioTarde.minuto = -1;
    a.fimTarde.hora = -1;
    a.fimTarde.minuto = -1;

    printf("Iniciando o programa de cadastro de medicos...\n");
    a.id=*idmedico;
    
    do {
        printf("\nadicione o nome do(a) medico(a): ");
        scanf(" %63[^\n]", a.nome);
    } while (!validar_nome(a.nome));

    do {
        printf("\nadicione a especialidade do(a) medico(a):\n0-Clinico\n1-Pediatra\n2-Dermato\n3-Cardio\n4-Psicologo\n");
        scanf("%d", (int*)&a.especialidade);

        if (a.especialidade < 0 || a.especialidade > 4){
            printf("resposta invalida, tente novamente!\n");
        }
    } while (a.especialidade < 0 || a.especialidade > 4);
    
       do {
        printf("\nqual o turno de trabalho?\n1-manha e tarde;\n2-manha;\n3-tarde.\n");
        scanf("%d", &turno);


        switch (turno){
            case 1:
                // Ambos os turnos
                do {
                    do {
                        printf("Digite o horario de inicio da manha (HH MM): ");
                        scanf("%d %d", &a.inicioManha.hora, &a.inicioManha.minuto);
                    } while (!validar_horario(a.inicioManha, 0, 12));
                    do {
                        printf("Digite o horario de fim da manha (HH MM): ");
                        scanf("%d %d", &a.fimManha.hora, &a.fimManha.minuto);
                    } while (!validar_horario(a.fimManha, 0, 12));

                } while (!validar_interalo_horario(a.inicioManha, a.fimManha));

                do {
                    do {
                        printf("Digite o horario de inicio da tarde (HH MM): ");
                        scanf("%d %d", &a.inicioTarde.hora, &a.inicioTarde.minuto);
                    } while (!validar_horario(a.inicioTarde, 14, 18));
                    do {
                        printf("Digite o horario de fim da tarde (HH MM): ");
                        scanf("%d %d", &a.fimTarde.hora, &a.fimTarde.minuto);
                    } while (!validar_horario(a.fimTarde, 14, 18));
                } while (!validar_interalo_horario(a.inicioTarde, a.fimTarde));

                break;

            case 2:
                // Somente Manha
                do {
                    do {
                        printf("Digite o horario de inicio da manha (HH MM): ");
                        scanf("%d %d", &a.inicioManha.hora, &a.inicioManha.minuto);
                    } while (!validar_horario(a.inicioManha, 0, 12));
                    do {
                        printf("Digite o horario de fim da manha (HH MM): ");
                        scanf("%d %d", &a.fimManha.hora, &a.fimManha.minuto);
                    } while (!validar_horario(a.fimManha, 0, 12));

                } while (!validar_interalo_horario(a.inicioManha, a.fimManha));
                break;

            case 3:
                // Somente Tarde
                do {
                    do {
                        printf("Digite o horario de inicio da tarde (HH MM): ");
                        scanf("%d %d", &a.inicioTarde.hora, &a.inicioTarde.minuto);
                    } while (!validar_horario(a.inicioTarde, 14, 18));
                    do {
                        printf("Digite o horario de fim da tarde (HH MM): ");
                        scanf("%d %d", &a.fimTarde.hora, &a.fimTarde.minuto);
                    } while (!validar_horario(a.fimTarde, 14, 18));
                } while (!validar_interalo_horario(a.inicioTarde, a.fimTarde));
                break;
            default:
                printf("Opcao de turno invalida. Tente novamente!\n");
        }

    } while (turno < 1 || turno > 3);

    a.num_atendimentos = 0;
    if (a.inicioManha.hora!=-1) {
        a.num_atendimentos += ((a.fimManha.hora * 60 + a.fimManha.minuto) - (a.inicioManha.hora * 60 + a.inicioManha.minuto))/30;
    }
    if (a.inicioTarde.hora!=-1) {
        a.num_atendimentos+= ((a.fimTarde.hora*60 + a.fimTarde.minuto)-(a.inicioTarde.hora*60+a.inicioTarde.minuto))/30;


    }
    a.atendimentos= (SlotConsulta*) malloc(a.num_atendimentos * sizeof(SlotConsulta));
    if (a.atendimentos==NULL) {
        printf("Erro ao alocar memoria para os slots.\n");
    } else {
        Horario hora_atual;
        int c=0;
        if (a.inicioManha.hora!=-1) {
            hora_atual=a.inicioManha;
            while (c<a.num_atendimentos && (hora_atual.hora<a.fimManha.hora || (hora_atual.hora==a.fimManha.hora && hora_atual.minuto<a.fimManha.minuto))) {
                a.atendimentos[c].inicio=hora_atual;
                a.atendimentos[c].ocupado=false;
                hora_atual = somar_minutos(hora_atual, 30);
                c++;
            }
        }
        if (a.inicioTarde.hora!=-1) {
            hora_atual=a.inicioTarde;
            while (c<a.num_atendimentos && (hora_atual.hora<a.fimTarde.hora || (hora_atual.hora==a.fimTarde.hora && hora_atual.minuto<a.fimTarde.minuto))) {
                a.atendimentos[c].inicio=hora_atual;
                a.atendimentos[c].ocupado=false;
                hora_atual = somar_minutos(hora_atual, 30);
                c++;
            }
        }
    }

    if (*m >= *capacidade_medicos) {
        *capacidade_medicos += 10;
        *medico = (medicos*)realloc(*medico, *capacidade_medicos * sizeof(medicos));
        
        if (*medico == NULL) {
            printf("Erro ao realocar memoria para medicos.\n");
            return;
        }
    }

    (*medico)[*m]=a;
    (*m)++;
    (*idmedico)++;
}

void listarmedicos(medicos *m, int j) {
    if (j == 0) {
        printf("Nao existe nenhum medico cadastrado no sistema!\n");
        return;
    }
    for(int i = 0; i < j; i++) {
        printf("---LISTA DO MEDICO---\n");
        printf("ID: %d\n", m[i].id);
        printf("Nome: %s\n", m[i].nome);

        switch (m[i].especialidade) {
            case ESPEC_CLINICO: printf("Especialidade: Clinico Geral\n"); break;
            case ESPEC_PEDIATRA: printf("Especialidade: Pediatra\n"); break;
            case ESPEC_DERMATO: printf("Especialidade: Dermatologista\n"); break;
            case ESPEC_CARDIO: printf("Especialidade: Cardiologista\n"); break;
            case ESPEC_PSICO: printf("Especialidade: Psicologo\n"); break;
            default: printf("Especialidade: Outra\n"); break;
        }
        if (m[i].inicioManha.hora != -1) {
            printf("Horario: %02d:%02d - %02d:%02d\n", m[i].inicioManha.hora, m[i].inicioManha.minuto, m[i].fimManha.hora, m[i].fimManha.minuto);
        }
        if (m[i].inicioTarde.hora != -1) {
            printf("Horario: %02d:%02d - %02d:%02d\n", m[i].inicioTarde.hora, m[i].inicioTarde.minuto,m[i].fimTarde.hora, m[i].fimTarde.minuto);
        }
        printf("-------------------------------\n");
    }
}

void atualizarm(medicos *m, int t) {
    int idbusca;
    int escolha;
    int turnoo;

    if (t == 0) {
        printf("Nao existe nenhum medico cadastrado no sistema!\n");
        return;
    }
    printf("Digite o ID do medico que voce deseja atualizar as informacoes: \n");
    scanf("%d", &idbusca);

    medicos *aux = buscaidm(m, t, idbusca);
    if (aux != NULL) {
        printf("--- MEDICO ENCONTRADO ---\n");
        printf("ID: %d\n", aux->id);
        printf("Nome: %s\n", aux->nome);

        switch (aux->especialidade) {
            case ESPEC_CLINICO: printf("Especialidade: Clinico Geral\n"); break;
            case ESPEC_PEDIATRA: printf("Especialidade: Pediatra\n"); break;
            case ESPEC_DERMATO: printf("Especialidade: Dermatologista\n"); break;
            case ESPEC_CARDIO: printf("Especialidade: Cardiologista\n"); break;
            case ESPEC_PSICO: printf("Especialidade: Psicologo\n"); break;
            default: printf("Especialidade: Outra\n"); break;
        }
        if (aux->inicioManha.hora != -1) {
            printf("Horario (manha): %02d:%02d - %02d:%02d\n", aux->inicioManha.hora, aux->inicioManha.minuto, aux->fimManha.hora, aux->fimManha.minuto);
        }
        if (aux->inicioTarde.hora != -1) {
            printf("Horario (tarde): %02d:%02d - %02d:%02d\n", aux->inicioTarde.hora, aux->inicioTarde.minuto, aux->fimTarde.hora, aux->fimTarde.minuto);
        }
        printf("\nO que voce deseja atualizar?\n1 - Nome\n2 - Especialidade\n3 - Horario\n");
        printf("Digite sua escolha:\n ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                printf("Digite o novo nome.\n");
                scanf(" %63[^\n]", aux->nome);
                break;
            case 2:
                printf("Digite a nova especialidade (0-Clinico, 1-Pediatra, 2-Dermato, 3-Cardio).\n");
                scanf("%d", (int*)&aux->especialidade);
                break;
            case 3:
                printf("Digite o novo horario (1-Manha e tarde, 2-Manha, 3-tarde\n");
                scanf("%d", &turnoo);
                switch (turnoo) {
                    case 1:
                        printf("Digite o novo horario inicio manha (HH MM): \n");
                        scanf("%d %d", &aux->inicioManha.hora, &aux->inicioManha.minuto);
                        printf("Digite o novo horario fim manha (HH MM): \n");
                        scanf("%d %d", &aux->fimManha.hora, &aux->fimManha.minuto);
                        printf("Digite o novo horario inicio tarde (HH MM): \n");
                        scanf("%d %d", &aux->inicioTarde.hora, &aux->inicioTarde.minuto);
                        printf("Digite o novo horario fim tarde (HH MM): \n");
                        scanf("%d %d", &aux->fimTarde.hora, &aux->fimTarde.minuto);
                        break;
                    case 2:
                        printf("Digite o novo horario de inicio da manha (HH MM): ");
                        scanf("%d %d", &aux->inicioManha.hora, &aux->inicioManha.minuto);
                        printf("Digite o novo horario de fim da manha (HH MM): ");
                        scanf("%d %d", &aux->fimManha.hora, &aux->fimManha.minuto);
                        aux->inicioTarde.hora = -1;
                        break;
                    case 3:
                        printf("Digite o novo horario de inicio da tarde (HH MM): ");
                        scanf("%d %d", &aux->inicioTarde.hora, &aux->inicioTarde.minuto);
                        printf("Digite o novo horario de fim da tarde (HH MM): ");
                        scanf("%d %d", &aux->fimTarde.hora, &aux->fimTarde.minuto);
                        aux->inicioManha.hora = -1;
                        break;
                }
                break;
            default:
                break;
        }
    } else {
        printf("\nMedico com ID %d nao encontrado.\n", idbusca);
    }
}

void remover_medico(medicos **medico, int *num_medicos) {
    char nome_temp[64];
    int i;
    int encontrado = 0;

    if ((*num_medicos) == 0) {
        printf("Nao existe nenhum medico cadastrado no sistema!\n");
        return;
    }

    printf("Digite o nome do medico que sera removido: ");
    scanf(" %63[^\n]", nome_temp);

    for (i = 0; i < *num_medicos; i++) {
        if (strcmp(nome_temp, (*medico)[i].nome) == 0) {
            free((*medico)[i].atendimentos);
            for (int j = i; j < *num_medicos - 1; j++) {
                (*medico)[j] = (*medico)[j + 1];
            }
            (*num_medicos)--;
            encontrado = 1;
            break;
        }
    }
    
    if (encontrado) {
        printf("Medico removido com sucesso!\n");
    } else {
        printf("Medico com o nome '%s' nao encontrado.\n", nome_temp);
    }
}

int comparar_medicos(const void *a, const void *b) {
    medicos *ma = (medicos *)a;
    medicos *mb = (medicos *)b;
    return strcmp(ma->nome, mb->nome);
}

void ordenar_medicos(medicos *x, int n) {
    qsort(x, n, sizeof(medicos), comparar_medicos);
}

int comparar_medicos_esp(const void *a, const void *b) {
    medicos *m1 = (medicos *)a;
    medicos *m2 = (medicos *)b;
    if (m1->especialidade != m2->especialidade) {
        return m1->especialidade - m2->especialidade;
    }
    return strcmp(m1->nome, m2->nome); // Bug corrigido aqui
}

void ordenar_medicos_esp(medicos *x, int n) {
    qsort(x, n, sizeof(medicos), comparar_medicos_esp);
}

void menu_medicos(medicos **medico, int *num_medicos, int *idmedico, int *capacidade_medicos) {
    int escolhamedico;
    int idbusca;
    int ordem_medica;

		
    do {
        printf("\nBEM-VINDO A AREA MEDICA, O QUE VOCE DESEJA FAZER?\n0-VOLTAR AO MENU PRINCIPAL\n1-ADICIONAR MEDICO\n2-LISTAR MEDICO\n3-ATUALIZAR MEDICO \n4-REMOVER MEDICO\n5-BUSCAR ID\n6-ORDENAR MEDICO\nINSIRA O NUMERO CORRESPONDENTE A SUA ESCOLHA!\n");
        scanf("%d", &escolhamedico);
        limpar_buffer();
        printf("---------------------------\n");
        
        switch (escolhamedico) {
            case 0:
                break;
            case 1:
                adicionar_medico(medico, num_medicos, idmedico, capacidade_medicos); 
                printf("\nCadastro de medico concluido. Total de medicos: %d\n", *num_medicos);
                break;
            case 2:
                listarmedicos(*medico, *num_medicos); 
                break;
            case 3:
                atualizarm(*medico, *num_medicos); 
                break;
            case 4:
                remover_medico(medico, num_medicos);
                break;
            case 5:
                if ((*num_medicos) == 0) {
                    printf("Nao existe nenhum medico cadastrado no sistema!\n");
                    break;
                }
                printf("Digite o id que voce deseja buscar: \n");
                scanf("%d", &idbusca);
                medicos *idmedico = buscaidm(*medico, *num_medicos, idbusca);
                if (idmedico !=NULL) {
                    printf("--- MEDICO ENCONTRADO ---\n");
                    printf("ID: %d\n", idmedico->id);
                    printf("Nome: %s\n", idmedico->nome);

                    switch (idmedico->especialidade) {
                        case ESPEC_CLINICO:
                            printf("Especialidade: Clinico Geral\n");
                            break;
                        case ESPEC_PEDIATRA:
                            printf("Especialidade: Pediatra\n");
                            break;
                        case ESPEC_DERMATO:
                            printf("Especialidade: Dermatologista\n");
                            break;
                        case ESPEC_CARDIO:
                            printf("Especialidade: Cardiologista\n");
                            break;
                        case ESPEC_PSICO:
                            printf("Especialidade: Psicologa\n");
                            break;
                    }
                    if (idmedico->inicioManha.hora != -1) {
                        printf("Horario (manha): %02d:%02d - %02d:%02d\n", idmedico->inicioManha.hora, idmedico->inicioManha.minuto, idmedico->fimManha.hora, idmedico->fimManha.minuto);
                    }
                    if (idmedico->inicioTarde.hora != -1) {
                        printf("Horario (tarde): %02d:%02d - %02d:%02d\n", idmedico->inicioTarde.hora, idmedico->inicioTarde.minuto, idmedico->fimTarde.hora, idmedico->fimTarde.minuto);
                    }
                }
                break;
            case 6:
                if ((*num_medicos) == 0) {
                    printf("Nao existe nenhum medico cadastrado no sistema!\n");
                    break;
                }
                printf("\n1-Ordem alfabetica.\n2-Por ordem de especialidade.\n");
                scanf("%d", &ordem_medica);
                switch (ordem_medica) {
                    case 1:
                        ordenar_medicos(*medico, *num_medicos);
                        listarmedicos(*medico, *num_medicos);
                        break;
                    case 2:
                        ordenar_medicos_esp(*medico, *num_medicos);
                        listarmedicos(*medico, *num_medicos);
                        break;
                    default:
                        printf("Opcao invalida. Tente novamente!\n");
                        break;
                }
        }
    } while (escolhamedico != 0);
}
