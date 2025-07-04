#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_OS 100
#define MAX_PECA 5
#define TAM_STR 100

typedef struct {
    int id;
    char cliente[TAM_STR];
    char problema[TAM_STR];
    char pecas[MAX_PECA][TAM_STR];
    int num_pecas;
    char tempo_estimado[TAM_STR];
    char data[TAM_STR];
    char status[TAM_STR];
    char diagnostico[TAM_STR];
    char prazo[TAM_STR];
    int ativa; // 1 = ativa, 0 = cancelada
} OrdemServico;

OrdemServico os_lista[MAX_OS];
OrdemServico historico[MAX_OS];
int total_os = 0;
int total_canceladas = 0;

// Função para limpar o buffer do teclado
void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para criar uma OS
void criar_os() {
    if (total_os >= MAX_OS) {
        printf("?? Limite de OS atingido.\n");
        return;
    }

    OrdemServico nova;
    nova.id = total_os + 1;
    nova.ativa = 1;

    printf("Nome do cliente: ");
    fgets(nova.cliente, TAM_STR, stdin); nova.cliente[strcspn(nova.cliente, "\n")] = '\0';

    printf("Descrição do problema: ");
    fgets(nova.problema, TAM_STR, stdin); nova.problema[strcspn(nova.problema, "\n")] = '\0';

    printf("Número de peças: ");
    scanf("%d", &nova.num_pecas);
    limpar_buffer();

    for (int i = 0; i < nova.num_pecas && i < MAX_PECA; i++) {
        printf("Peça %d: ", i + 1);
        fgets(nova.pecas[i], TAM_STR, stdin); nova.pecas[i][strcspn(nova.pecas[i], "\n")] = '\0';
    }

    printf("Tempo estimado (ex: 3 dias): ");
    fgets(nova.tempo_estimado, TAM_STR, stdin); nova.tempo_estimado[strcspn(nova.tempo_estimado, "\n")] = '\0';

    printf("Data de criação (ex: 2025-07-03): ");
    fgets(nova.data, TAM_STR, stdin); nova.data[strcspn(nova.data, "\n")] = '\0';

    strcpy(nova.status, "Aberta");
    strcpy(nova.diagnostico, "");
    strcpy(nova.prazo, "");

    os_lista[total_os++] = nova;
    printf("? OS criada com sucesso! ID: %d\n", nova.id);
}

// Função para consultar OS
void consultar_os() {
    int opcao;
    char termo[TAM_STR];

    printf("\nConsultar por:\n1. Status\n2. Cliente\n3. Data\nEscolha: ");
    scanf("%d", &opcao);
    limpar_buffer();

    printf("Digite o termo de busca: ");
    fgets(termo, TAM_STR, stdin); termo[strcspn(termo, "\n")] = '\0';

    int encontrou = 0;
    for (int i = 0; i < total_os; i++) {
        if (!os_lista[i].ativa) continue;

        int match = 0;
        if (opcao == 1 && strcasecmp(os_lista[i].status, termo) == 0) match = 1;
        else if (opcao == 2 && strcasecmp(os_lista[i].cliente, termo) == 0) match = 1;
        else if (opcao == 3 && strcmp(os_lista[i].data, termo) == 0) match = 1;

        if (match) {
            encontrou = 1;
            printf("\nID: %d\nCliente: %s\nProblema: %s\nStatus: %s\n", os_lista[i].id, os_lista[i].cliente, os_lista[i].problema, os_lista[i].status);
        }
    }

    if (!encontrou)
        printf("?? Nenhuma OS encontrada com esse critério.\n");
}

// Função para atualizar OS
void atualizar_os() {
    int id;
    printf("Digite o ID da OS a atualizar: ");
    scanf("%d", &id);
    limpar_buffer();

    for (int i = 0; i < total_os; i++) {
        if (os_lista[i].id == id && os_lista[i].ativa) {
            OrdemServico *os = &os_lista[i];

            printf("Novo status (%s): ", os->status);
            fgets(os->status, TAM_STR, stdin); os->status[strcspn(os->status, "\n")] = '\0';

            printf("Diagnóstico: ");
            fgets(os->diagnostico, TAM_STR, stdin); os->diagnostico[strcspn(os->diagnostico, "\n")] = '\0';

            printf("Nova quantidade de peças: ");
            scanf("%d", &os->num_pecas);
            limpar_buffer();

            for (int j = 0; j < os->num_pecas && j < MAX_PECA; j++) {
                printf("Peça %d: ", j + 1);
                fgets(os->pecas[j], TAM_STR, stdin); os->pecas[j][strcspn(os->pecas[j], "\n")] = '\0';
            }

            printf("Prazo final: ");
            fgets(os->prazo, TAM_STR, stdin); os->prazo[strcspn(os->prazo, "\n")] = '\0';

            printf("? OS atualizada com sucesso.\n");
            return;
        }
    }

    printf("? OS não encontrada ou já cancelada.\n");
}

// Função para cancelar OS
void cancelar_os() {
    int id;
    printf("Digite o ID da OS a cancelar: ");
    scanf("%d", &id);
    limpar_buffer();

    for (int i = 0; i < total_os; i++) {
        if (os_lista[i].id == id && os_lista[i].ativa) {
            os_lista[i].ativa = 0;
            historico[total_canceladas++] = os_lista[i];
            printf("? OS cancelada com sucesso.\n");
            return;
        }
    }

    printf("? OS não encontrada ou já cancelada.\n");
}

// Ver histórico de OS canceladas
void ver_historico() {
    if (total_canceladas == 0) {
        printf("?? Nenhuma OS cancelada até agora.\n");
        return;
    }

    printf("\n?? Histórico de OS canceladas:\n");
    for (int i = 0; i < total_canceladas; i++) {
        printf("ID: %d | Cliente: %s | Problema: %s\n", historico[i].id, historico[i].cliente, historico[i].problema);
    }
}

// Menu principal
void menu() {
    int opcao;
    do {
        printf("\n=== MENU - Sistema de Ordens de Serviço ===\n");
        printf("1. Criar nova OS\n");
        printf("2. Consultar OS\n");
        printf("3. Atualizar OS\n");
        printf("4. Cancelar/Excluir OS\n");
        printf("5. Ver histórico de cancelamentos\n");
        printf("0. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limpar_buffer();

        switch (opcao) {
            case 1: criar_os(); break;
            case 2: consultar_os(); break;
            case 3: atualizar_os(); break;
            case 4: cancelar_os(); break;
            case 5: ver_historico(); break;
            case 0: printf("?? Encerrando o sistema.\n"); break;
            default: printf("? Opção inválida.\n");
        }
    } while (opcao != 0);
}

// Função principal
int main() {
    menu();
    return 0;
}
 
