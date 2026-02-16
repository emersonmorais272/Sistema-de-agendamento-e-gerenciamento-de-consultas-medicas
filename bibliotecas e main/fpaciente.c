#include "fpaciente.h"
#include "fgeral.h"

void adicionar_paciente(pacientes **paciente, int *p, int *idpaciente, int *capacidade_pacientes){
    pacientes a;

    printf("Iniciando o programa de cadastro de pacientes...\n");
    a.id = *idpaciente;
    
    do {
        printf("\nadicione o nome do(a) paciente: ");
        scanf(" %63[^\n]", a.nome);
    } while (!validar_nome(a.nome));

    do {
        printf("\nadicione o contato do(a) paciente (email ou telefone(apenas numeros)): ");
        scanf(" %63[^\n]", a.contato);
    } while (!validar_contato(a.contato));

    if (*p >= *capacidade_pacientes) {
        *capacidade_pacientes += 10;
        *paciente = (pacientes*)realloc(*paciente, *capacidade_pacientes * sizeof(pacientes));
        
        if (*paciente == NULL) {
            printf("Erro ao realocar memoria para pacientes.\n");
            return;
        }
    }

    (*paciente)[*p] = a;
    (*p)++;
    (*idpaciente)++;
}

void listarpacientes(pacientes *x, int n) {
    if (n == 0) {
        printf("Nao existe nenhum paciente cadastrado no sistema!\n");
        return;
    }
    for(int i = 0; i < n; i++) {
        printf("---LISTA DO PACIENTE---\n");
        printf("ID: %d\n", x[i].id);
        printf("Nome: %s\n", x[i].nome);
        printf("Contato: %s\n", x[i].contato);
        printf("-------------------------------\n");
    }
}

void atualizarpacientes(pacientes *p, int t) {
    int idbusca;
    int escolha;

    if (t == 0) {
        printf("Nao existe nenhum pacientes cadastrado no sistema!\n");
        return;
    }
    printf("Digite o id do paciente que voce deseja atualizar as informações: \n");
    scanf("%d", &idbusca);

    pacientes *aux = buscaidp(p, t, idbusca);
    if (aux != NULL) {
        printf("--- PACIENTE ENCONTRADO ---\n");
        printf("ID: %d\n", aux->id);
        printf("Nome: %s\n", aux->nome);
        printf("Contato: %s\n", aux->contato);
        printf("\nO que voce deseja atualizar?\n1 - Nome\n2 - Contato\n");
        printf("Digite sua escolha:\n ");
        scanf("%d", &escolha);

        switch (escolha) {
            case 1:
                printf("Digite o novo nome.\n");
                scanf(" %63[^\n]", aux->nome);
                break;
            case 2:
                printf("Digite o novo contato.\n");
                scanf(" %63[^\n]", aux->contato);
                break;
            default:
                break;
        }
    } else {
        printf("\nPaciente com ID %d nao encontrado.\n", idbusca);
    }
}

void remover_paciente(pacientes **paciente, int *num_pacientes) {
    char nome_temp[64];
    int i;
    int encontrado = 0;

    if ((*num_pacientes) == 0) {
        printf("Nao existe nenhum pacientes cadastrado no sistema!\n");
        return;
    }

    printf("Digite o nome do paciente que sera removido: ");
    scanf(" %63[^\n]", nome_temp);

    for (i = 0; i < *num_pacientes; i++) {
        if (strcmp(nome_temp, (*paciente)[i].nome) == 0) {
            for (int j = i; j < *num_pacientes - 1; j++) {
                (*paciente)[j] = (*paciente)[j + 1];
            }
            (*num_pacientes)--;
            encontrado = 1;
            break;
        }
    }
    
    if (encontrado) {
        printf("Paciente removido com sucesso!\n");
    } else {
        printf("Paciente com o nome '%s' nao encontrado.\n", nome_temp);
    }
}

int compararpacientes(const void *a, const void *b) {
    pacientes *pa = (pacientes *)a;
    pacientes *pb = (pacientes *)b;
    return strcmp(pa->nome, pb->nome);
}

void ordenarpacientes(pacientes *x, int n) {
    if (n == 0) {
        printf("Nao existe nenhum pacientes cadastrado no sistema!\n");
        return;
    }
    qsort(x, n, sizeof(pacientes), compararpacientes);
}

void menu_pacientes(pacientes **paciente, int *num_pacientes, int *idpaciente, int *capacidade_pacientes) {
    int escolhapaciente;
    int idbuscar;
    
    do {
        printf("\nBEM-VINDO A AREA DO PACIENTE, O QUE VOCE DESEJA FAZER?\n0-VOLTAR AO MENU PRINCIPAL\n1-ADICIONAR PACIENTE\n2-LISTAR PACIENTE\n3-ATUALIZAR PACIENTE\n4-REMOVER PACIENTE\n5-BUSCAR ID\n6-ORDENAR PACIENTE POR ORDEM ALFABETICA\nINSIRA O NUMERO CORRESPONDENTE A SUA ESCOLHA!\n");
        scanf("%d", &escolhapaciente);
        limpar_buffer();
        printf("---------------------------\n");
        
        switch (escolhapaciente) {
            case 0:
                break;
            case 1:
                adicionar_paciente(paciente, num_pacientes, idpaciente, capacidade_pacientes);
                printf("\nCadastro de paciente concluido. Total de pacientes: %d\n", *num_pacientes);
                break;
            case 2:
                listarpacientes(*paciente, *num_pacientes); 
                break;
            case 3:
                atualizarpacientes(*paciente, *num_pacientes);
                break;
            case 4:
                remover_paciente(paciente, num_pacientes);
                break;
            case 5:
                if ((*num_pacientes) == 0) {
                    printf("Nao existe nenhum pacientes cadastrado no sistema!\n");
                    break;
                }
                printf("Digite o id que voce deseja buscar: \n");
                scanf("%d", &idbuscar);
                limpar_buffer();
                pacientes *idpaciente_ptr = buscaidp(*paciente, *num_pacientes, idbuscar);
                if (idpaciente_ptr != NULL) {
                    printf("--- PACIENTE ENCONTRADO ---\n");
                    printf("ID: %d\n", idpaciente_ptr->id);
                    printf("Nome: %s\n", idpaciente_ptr->nome);
                    printf("Contato: %s\n", idpaciente_ptr->contato);
                } else {
                    printf("Paciente com ID %d nao encontrado.\n", idbuscar);
                }
                break;
            case 6:
                    ordenarpacientes(*paciente, *num_pacientes);
                    listarpacientes(*paciente, *num_pacientes);
                break;
            default:
                printf("Opcao invalida. Tente novamente!\n");
                break;
        }
    } while (escolhapaciente != 0);
}

