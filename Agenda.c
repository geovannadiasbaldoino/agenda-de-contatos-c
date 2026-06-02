#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <locale.h>

#define ARQUIVO "contatos.dat"

// Definição da estrutura para armazenar contatos
typedef struct {
    int dia, mes, ano;
} Data;

typedef struct {
    char nome[50];
    char telefone[20];
    char email[50];
    Data nascimento;
} Contato;

// Função para limpar o buffer do teclado
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Função para capturar data de nascimento
void capturarData(Data *data) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    int anoAtual = tm.tm_year + 1900;
    
    system("cls");
    printf("Digite o dia de nascimento (1-31): ");
    scanf("%d", &data->dia);
    limparBuffer();

    printf("Digite o mês de nascimento (1-12): ");
    scanf("%d", &data->mes);
    limparBuffer();

    printf("Digite o ano de nascimento (1900-%d): ", anoAtual);
    scanf("%d", &data->ano);
    limparBuffer();
}

// Função para criar um novo contato
void criarContato() {
    system("cls");
    Contato contato;
    FILE *arq = fopen(ARQUIVO, "ab");
    if (!arq) {
        printf("Erro ao abrir o arquivo!\n");
        return;
    }

    printf("\nNome: ");
    fgets(contato.nome, sizeof(contato.nome), stdin);
    contato.nome[strcspn(contato.nome, "\n")] = '\0';

    printf("Telefone (DDD + número): ");
    fgets(contato.telefone, sizeof(contato.telefone), stdin);
    contato.telefone[strcspn(contato.telefone, "\n")] = '\0';

    printf("E-mail: ");
    fgets(contato.email, sizeof(contato.email), stdin);
    contato.email[strcspn(contato.email, "\n")] = '\0';

    capturarData(&contato.nascimento);
    
    fwrite(&contato, sizeof(Contato), 1, arq);
    fclose(arq);
    printf("Contato salvo com sucesso!\n");
}

// Função para carregar os contatos do arquivo e ordenar
int carregarContatos(Contato contatos[], int max) {
    FILE *arq = fopen(ARQUIVO, "rb");
    if (!arq) return 0;
    
    int count = 0;
    while (fread(&contatos[count], sizeof(Contato), 1, arq) && count < max) {
        count++;
    }
    fclose(arq);
    
    // Ordenação alfabética por nome
    int i, j;
    for (i = 0; i < count - 1; i++) {
        for (j = i + 1; j < count; j++) {
            if (strcmp(contatos[i].nome, contatos[j].nome) > 0) {
                Contato temp = contatos[i];
                contatos[i] = contatos[j];
                contatos[j] = temp;
            }
        }
    }
    return count;
}

// Função para listar todos os contatos
void listarContatos() {
    system("cls");
    FILE *arq = fopen(ARQUIVO, "rb");
    if (!arq) {
        printf("Nenhum contato encontrado!\n");
        return;
    }
    
    Contato contatos[100];
    int count = 0, i, j;
    
    while (fread(&contatos[count], sizeof(Contato), 1, arq) && count < 100) {
        count++;
    }
    fclose(arq);
    
    // Ordenar contatos por nome (Bubble Sort simples)
    for (i = 0; i < count - 1; i++) {
        for (j = 0; j < count - i - 1; j++) {
            if (strcmp(contatos[j].nome, contatos[j + 1].nome) > 0) {
                Contato temp = contatos[j];
                contatos[j] = contatos[j + 1];
                contatos[j + 1] = temp;
            }
        }
    }
    
    // Exibir tabela formatada
    printf("\n%-30s %-15s %-15s %-30s\n", "Nome", "Telefone", "Nascimento", "E-mail");
    printf("%s\n", "--------------------------------------------------------------------------------------------------------");
    
    for (i = 0; i < count; i++) {
        printf("%-30s %s %-2d/%-2d/%-4d %-30s\n", 
               contatos[i].nome, contatos[i].telefone, 
               contatos[i].nascimento.dia, contatos[i].nascimento.mes, contatos[i].nascimento.ano, 
               contatos[i].email);
    }
    
    getch();
}

// Função para excluir contato
void excluirContato() {
    system("cls");
    char nomeBusca[50];
    printf("Digite o nome do contato a excluir: ");
    fgets(nomeBusca, sizeof(nomeBusca), stdin);
    nomeBusca[strcspn(nomeBusca, "\n")] = '\0';

    FILE *arq = fopen(ARQUIVO, "rb");
    FILE *temp = fopen("temp.dat", "wb");
    if (!arq || !temp) {
        printf("Erro ao acessar arquivos!\n");
        return;
    }

    Contato contato;
    bool encontrado = false;
    while (fread(&contato, sizeof(Contato), 1, arq)) {
        if (strcmp(contato.nome, nomeBusca) != 0) {
            fwrite(&contato, sizeof(Contato), 1, temp);
        } else {
            encontrado = true;
        }
    }

    fclose(arq);
    fclose(temp);
    remove(ARQUIVO);
    rename("temp.dat", ARQUIVO);

    if (encontrado) printf("Contato excluído com sucesso!\n");
    else printf("Contato não encontrado!\n"); getch ();
}

// Menu principal
int main() {
    setlocale (LC_ALL, "");
    int opcao;
    do {
        system("cls");
        printf("\nAgenda de Contatos\n");
        printf("1. Adicionar Contato\n");
        printf("2. Listar Contatos\n");
        printf("3. Excluir Contato\n");
        printf("4. Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        limparBuffer();

        switch (opcao) {
            case 1:
                criarContato();
                break;
            case 2:
                listarContatos();
                break;
            case 3:
                excluirContato();
                break;
            case 4:
                printf("Saindo...\n");
                break;
            default:
                printf("Opção inválida!\n");
        }
    } while (opcao != 4);
    return 0;
}

