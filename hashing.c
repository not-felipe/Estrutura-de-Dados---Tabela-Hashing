#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct aluno {
  int matricula;
  char nome[50];
  char email[100];
  int disponibilidade;
} Aluno;

int tamanho = 0;

void inicializar(char *nomeArq){
  FILE *arq = fopen(nomeArq, "wb");
  if(arq == NULL){
    printf("ERROR: Erro ao abrir o arquivo.\n");
    return;
  }
  Aluno a;
  a.disponibilidade = 1;
  for(int i = 0; i < tamanho; i++)
    fwrite(&a, sizeof(Aluno), 1, arq);
  fclose(arq);
}

int hash(int matricula){
  return matricula % tamanho;
}

int acharPosicao(char *nomeArq, int matricula){
  FILE *arq = fopen(nomeArq, "rb");
  Aluno a;
  int pos = hash(matricula);
  fseek(arq, pos*sizeof(Aluno), SEEK_SET);
  fread(&a, sizeof(Aluno), 1, arq);
  while(a.disponibilidade == 0 && a.matricula != matricula){
    pos = (pos + 1) % tamanho; //tratamento de colisao
    fseek(arq, pos*sizeof(Aluno), SEEK_SET);
    fread(&a, sizeof(Aluno), 1, arq);
  }
  fclose(arq);
  return pos;
}

void inserirAluno(char *nomeArq, Aluno a){
  FILE *arq = fopen(nomeArq, "r+b");

  printf("Digite o nome do aluno: ");
  scanf("%s", a.nome);

  printf("Digite o email do aluno: ");
  scanf("%s", a.email);

  printf("Digite a matricula do aluno: ");
  scanf("%d", &a.matricula);
  a.disponibilidade = 0;

  int pos = acharPosicao(nomeArq, a.matricula);

  fseek(arq, pos*sizeof(Aluno), SEEK_SET);
  fwrite(&a, sizeof(Aluno), 1, arq);
  fclose(arq);
}

void imprimirAluno(char *nomeArq, int matricula){
  FILE *arq = fopen(nomeArq,"r");

  Aluno a;
  int pos = acharPosicao(nomeArq, matricula);

  fseek(arq, pos*sizeof(Aluno), SEEK_SET);
  fread(&a, sizeof(Aluno), 1, arq);

  if(a.disponibilidade == 0 && a.matricula == matricula){
    printf("Aluno encontrado:\n");
    printf("Matricula: %d\n", a.matricula);
    printf("Nome: %s\n", a.nome);
    printf("Email: %s\n", a.email);
  } else {
    printf("Aluno nao encontrado.\n");
  }

  fclose(arq);
}

void imprimirTabela(char *nomeArq) {
  FILE *arq = fopen(nomeArq, "rb");

  Aluno a;

  printf("Tabela de dispersao:\n");

  for(int i = 0; i < tamanho; i++) {
    fseek(arq, i*sizeof(Aluno), SEEK_SET);
    fread(&a, sizeof(Aluno), 1, arq);

    printf("Posicao %d:", i);

    if(a.disponibilidade == 0) {
      printf(" Matricula: %d, Nome: %s, Email: %s\n", a.matricula, a.nome, a.email);
    } else {
      printf(" Vazio\n");
    }
  }

  fclose(arq);
}

int main(void){
  Aluno a;
  char nomeArquivo[50];
  int opcao;
  int matricula;

  printf("Digite o tamanho da tabela: ");
  scanf("%d", &tamanho);

  printf("Digite o nome do arquivo a ser lido: ");
  scanf("%s", nomeArquivo);

  inicializar(nomeArquivo);

  do {
    printf("\nMenu:\n");
    printf("1. Inserir um novo aluno\n");
    printf("2. Imprimir as informacoes de um determinado aluno\n");
    printf("3. Imprimir a tabela de dispersao\n");
    printf("4. Sair\n");
    printf("Escolha uma opcao: ");
    scanf("%d", &opcao);

    switch(opcao){
      case 1:
        inserirAluno(nomeArquivo, a);
        break;
      case 2:
        printf("Digite a matricula do aluno que deseja buscar: ");
        scanf("%d", &matricula);
        imprimirAluno(nomeArquivo, matricula);
        break;
      case 3:
        imprimirTabela(nomeArquivo);
        break;
      case 4:
        printf("Encerrando o programa...\n");
        break;
      default:
        printf("Opcao invalida!\n");
    }
  } while(opcao != 4);

  return 0;
}
