#include "fgeral.h"
#include "fmedico.h"


// Implementação das funções

void limpar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

int validar_nome(const char *n) {
    if (n==NULL || strlen(n)==0) {
        printf("Erro: O campo nome e obrigatorio!\n");
        return 0;
    }
    for (int i=0; n[i] != '\0'; i++) {
        if (!isalpha(n[i]) && n[i] !=' ') {
            printf("Erro: O nome so pode conter letras e espacos.\n");
            return 0;
        }
    }
    return 1;
}

int validar_horario(Horario horario, int minh, int maxh) {
    if (horario.hora < minh || horario.hora > maxh) {
        printf("Horario invalido! A hora deve estar entre %dh a %dh.\n", minh, maxh);
        return 0;
    }
    if (horario.minuto <0 || horario.minuto > 59) {
        printf("Horario invalido! A hora deve estar entre 0 a 59.\n");
        return 0;
    }
    return 1;
}

int validar_interalo_horario(Horario horainic, Horario horafim) {

    if (horainic.hora > horafim.hora) {
        printf("Horario invalido! Digite novavamente!\n");
        return 0;
    }
    if (horainic.hora==horafim.hora) {
        printf("Horario invalido! Os horarios nao podem ser iguais, digite novavamente!\n");
        return 0;
    }
    return 1;
}

int validar_contato(const char *contato) {
    if (contato == NULL || strlen(contato) == 0) {
        printf("Erro: O campo de contato e obrigatorio!\n");
        return 0;
    }
    if (strchr(contato, '@') != NULL) {
        if (strchr(contato, '.')) {
            return 1;
        } else {
            printf("Erro: formato do email errado, esta faltando '.'.\n");
            return 0;
        }

    } else {
        for (int i=0; contato[i] != '\0'; i++) {
            if (!isdigit(contato[i])) { //dar uma olhada depois
                printf("Erro: formato do contato invalido, digite o seu contato novamente!\n");
                return 0;
            }
        }
    }
    return 1;
}

Horario somar_minutos(Horario h, int minutos) {
    h.minuto += minutos;
    h.hora += h.minuto / 60;
    h.minuto %= 60;
    return h;
}

bool anobissexto(int ano) {
    if ((ano%400==0) || (ano%4==0 && ano%100!=0)) {
        return true;
    }
    return false;
}

bool validar_data(Data d) {
    if (d.dia<1 || d.dia >31) {
        printf("Erro: o dia esta fora do intervalo, prencha o campo dd com numeros entre 1 a 31.\n");
        return false;
    }

    if (d.mes<1 || d.mes>12) {
        printf("Erro: o mes esta fora do intervalo, prencha o campo mm com numeros entre 1 a 12.\n");
        return false;
    }

    if (d.ano<2025) {
        printf("O agendamento das consultas so sao marcadas a partir de 2025.\n");
        return false;
    }

    if (d.mes==2) {
        if (anobissexto(d.ano)) {
            if (d.dia>29) {
                printf("Erro: o dia esta fora do intervalo, prencha o campo dd com numeros entre 1 a 29.\n");
                return false;
            }
        } else {
            if (d.dia>28) {
                printf("Erro: o dia esta fora do intervalo, prencha o campo dd com numeros entre 1 a 28.\n");
                return false;
            }
        }
    } else if (d.mes==4 || d.mes==6 || d.mes==9 || d.mes==11) {
        if (d.dia>30) {
            printf("Erro: o dia esta fora do intervalo, prencha o campo dd com numeros entre 1 a 30.\n");
            return false;
        }
    }
    return true;
}

void processar_turno(medicos *medico, Horario inicio_turno, Horario fim_turno) {
    Horario hora_atual = inicio_turno;
    int atendiemnto_count = 0;
    while (hora_atual.hora < fim_turno.hora || (hora_atual.hora == fim_turno.hora && hora_atual.minuto < fim_turno.minuto)) {
        if (atendiemnto_count >= medico->num_atendimentos) {
            break;
        }
        if (!medico->atendimentos[atendiemnto_count].ocupado) {
            printf("%d: %02d:%02d\n", atendiemnto_count, hora_atual.hora, hora_atual.minuto);
            medico->atendimentos[atendiemnto_count].inicio = hora_atual;
        }
        hora_atual = somar_minutos(hora_atual, 30);
        atendiemnto_count++;
    }
}
