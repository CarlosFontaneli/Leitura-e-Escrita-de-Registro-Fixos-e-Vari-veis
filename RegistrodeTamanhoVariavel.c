/*
  Autores:
    - Bruno Leandro Pereira    - RA 791067 Bacharelado em Ciência da Computação
    - Carlos Eduardo Fontaneli - RA 769949 Bacharelado em Ciência da Computação
  Data: xx/xx/2021
  Trabalho Prático 1
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define TAM_MAX_CAMPO 16

typedef struct endereco
{
  char rua[TAM_MAX_CAMPO];         // tamanho do campo rua
  char num[TAM_MAX_CAMPO];         // tamanho do campo num
  char complemento[TAM_MAX_CAMPO]; // tamanho do campo complemento
} endereco;

typedef struct pessoa
{
  char chave[TAM_MAX_CAMPO];    // tamanho do campo chave
  char primeiro[TAM_MAX_CAMPO]; // tamanho do campo primeiro nome
  char ultimo[TAM_MAX_CAMPO];   // tamanho do campo último nome
  endereco endereco;            // tamanho do campo endereço
  char cidade[TAM_MAX_CAMPO];   // tamanho do campo cidade
  char estado[TAM_MAX_CAMPO];   // tamanho do campo estado
  char cep[TAM_MAX_CAMPO];      // tamanho do campo cep
  char telefone[TAM_MAX_CAMPO]; // tamanho do campo telefone
  int tamRegistro;
} pessoa;

/* DECLARAÇÃO DOS MÉTODOS */

// Escreve registro no arquivo sequencialmente ao final do arquivo
void insereRegistroSequencialmente(FILE *arquivo);

// Permite inserção no arquivo, levar em conta espaço de registros que foram apagados
void insereRegistroReaproveitamento(FILE *arquivo);

// Remoção LÓGICA de registro
void apagaRegistro(FILE *arquivo);

// Le todos os registros
void leituraCompleta(FILE *arquivo);

// Imprime cada campo do registro
void imprimeRegistro(pessoa *registro);

// Leitura de campo de registro
void obterCampo(char *buffer);

// Escolhe a operacao FEITO!
void realizaOperacao();

// Escolhe arquivo FEITO!
FILE *escolheArquivo(char *);

// Pesquisa sequencial por chave
pessoa *pesquisaRegistroChave(FILE *arquivo);

// Pesquisa por nome, usar indice secundário
int pesquisaRegistroNome(int pesquisa);

// Lê o registro à partir da pesquisa por nome
void leituraRegistroNome(int offset, FILE *arquivo);

// Inicia um registro de forma nula;
void iniciaRegistroNulo(pessoa *registro);

// Limpa o lixo dos campos do registro
void limpaCamposRegistro(pessoa *registro);

// Le um registro
pessoa *obterRegistro();

int main()
{
  system("clear");
  printf("Bem vindo ao programa que trata de registros com tamanho variado!\n\n");
  realizaOperacao();
  system("clear");
  printf("\n\nExecução Finalizada!\n\n");
  return 0;
}

void iniciaRegistroNulo(pessoa *registro)
{
  registro->chave[15] = '\0';
  registro->primeiro[15] = '\0';
  registro->ultimo[15] = '\0';
  registro->endereco.rua[15] = '\0';
  registro->endereco.num[15] = '\0';
  registro->endereco.complemento[15] = '\0';
  registro->cidade[15] = '\0';
  registro->telefone[15] = '\0';
  registro->estado[15] = '\0';
  registro->cep[15] = '\0';
}

void limpaCamposRegistro(pessoa *registro)
{
  memset(registro->chave, '\0', 16);
  memset(registro->primeiro, '\0', 16);
  memset(registro->ultimo, '\0', 16);
  memset(registro->endereco.rua, '\0', 16);
  memset(registro->endereco.num, '\0', 16);
  memset(registro->endereco.complemento, '\0', 16);
  memset(registro->cidade, '\0', 16);
  memset(registro->telefone, '\0', 16);
  memset(registro->estado, '\0', 16);
  memset(registro->cep, '\0', 16);
}

void insereRegistroSequencialmente(FILE *arquivo)
{
  int escrevendo = 0, tamCampo, offset;      // Variavel para controle de escrita de registro
  pessoa *registro = malloc(sizeof(pessoa)); // Alocação da estrutura pessoa
  iniciaRegistroNulo(registro);

  FILE *arquivoIndices;
  arquivoIndices = fopen("indices.bin", "ab");
  while (NULL == arquivo)
  {
    char caminhoArquivo[100], modoAbertura[3];

    printf("ERRO AO ABRIR ARQUIVO!\nDigite novamente o caminho(path) até o arquivo: ");
    scanf("%s", caminhoArquivo);
    printf("\nDigite o modo de abertura: ");
    scanf("%s", modoAbertura);

    arquivoIndices = fopen(caminhoArquivo, modoAbertura);
  }

  printf("1 - Escrever registro;\n0 - Sair;\n");
  scanf("%d", &escrevendo);
  // Escrever registro no arquivo
  while (escrevendo)
  {
    registro = obterRegistro();
    offset = ftell(arquivo);
    registro->tamRegistro += sizeof(int) * 10;
    fwrite(&registro->tamRegistro, sizeof(int), 1, arquivo);

    tamCampo = strlen(registro->chave);
    fwrite(&tamCampo, sizeof(int), 1, arquivo);
    fwrite(&registro->chave, tamCampo, 1, arquivo);

    tamCampo = strlen(registro->primeiro);
    fwrite(&tamCampo, sizeof(int), 1, arquivo);
    fwrite(&registro->primeiro, tamCampo, 1, arquivo);

    tamCampo = strlen(registro->ultimo);
    fwrite(&tamCampo, sizeof(int), 1, arquivo);
    fwrite(&registro->ultimo, tamCampo, 1, arquivo);

    tamCampo = strlen(registro->endereco.rua);
    fwrite(&tamCampo, sizeof(int), 1, arquivo);
    fwrite(&registro->endereco.rua, tamCampo, 1, arquivo);

    tamCampo = strlen(registro->endereco.num);
    fwrite(&tamCampo, sizeof(int), 1, arquivo);
    fwrite(&registro->endereco.num, tamCampo, 1, arquivo);

    tamCampo = strlen(registro->endereco.complemento);
    fwrite(&tamCampo, sizeof(int), 1, arquivo);
    fwrite(&registro->endereco.complemento, tamCampo, 1, arquivo);

    tamCampo = strlen(registro->cidade);
    fwrite(&tamCampo, sizeof(int), 1, arquivo);
    fwrite(&registro->cidade, tamCampo, 1, arquivo);

    tamCampo = strlen(registro->estado);
    fwrite(&tamCampo, sizeof(int), 1, arquivo);
    fwrite(&registro->estado, tamCampo, 1, arquivo);

    tamCampo = strlen(registro->cep);
    fwrite(&tamCampo, sizeof(int), 1, arquivo);
    fwrite(&registro->cep, tamCampo, 1, arquivo);

    tamCampo = strlen(registro->telefone);
    fwrite(&tamCampo, sizeof(int), 1, arquivo);
    fwrite(&registro->telefone, tamCampo, 1, arquivo);

    // Salvar o indice
    tamCampo = strlen(registro->primeiro);
    fwrite(&tamCampo, sizeof(int), 1, arquivoIndices);
    fwrite(&registro->primeiro, tamCampo, 1, arquivoIndices);
    fwrite(&offset, 32, 1, arquivoIndices);

    limpaCamposRegistro(registro);
    printf("1 - Escrever registro;\n0 - Sair;\n");
    scanf("%d", &escrevendo);
  }

  fclose(arquivo);
  fclose(arquivoIndices);
}

void insereRegistroReaproveitamento(FILE *arquivo)
{
  char chave[16] = "apagado\0"; // Variável para encontrar o registro apagado
  int offset, tamCampo, tamRegistro;

  pessoa *registro = malloc(sizeof(pessoa)); // Alocação de estrutura pessoa
  iniciaRegistroNulo(registro);

  FILE *arquivoIndices;

  arquivoIndices = fopen("indices.bin", "ab+");
  while (NULL == arquivo)
  {
    char caminhoArquivo[100], modoAbertura[3];

    printf("ERRO AO ABRIR ARQUIVO!\nDigite novamente o caminho(path) até o arquivo: ");
    scanf("%s", caminhoArquivo);
    printf("\nDigite o modo de abertura: ");
    scanf("%s", modoAbertura);

    arquivoIndices = fopen(caminhoArquivo, modoAbertura);
  }

  fseek(arquivo, 0, SEEK_SET); // Posiciona a posição de leitura no inicio do arquivo
  fread(&tamRegistro, sizeof(int), 1, arquivo);
  fread(&tamCampo, sizeof(int), 1, arquivo);
  // Lendo e comparando o campo
  while (fread(registro->chave, tamCampo, 1, arquivo) && strcmp(registro->chave, chave))
  {
    fseek(arquivo, tamRegistro - (tamCampo + sizeof(int)), SEEK_CUR);
    fread(&tamRegistro, sizeof(int), 1, arquivo);
    fread(&tamCampo, sizeof(int), 1, arquivo);
  };
  fseek(arquivo, -(tamCampo + (2 * sizeof(int))), SEEK_CUR);
  registro = obterRegistro();
  offset = ftell(arquivo);
  registro->tamRegistro += sizeof(int) * 10;
  fwrite(&registro->tamRegistro, sizeof(int), 1, arquivo);

  tamCampo = strlen(registro->chave);
  fwrite(&tamCampo, sizeof(int), 1, arquivo);
  fwrite(&registro->chave, tamCampo, 1, arquivo);

  tamCampo = strlen(registro->primeiro);
  fwrite(&tamCampo, sizeof(int), 1, arquivo);
  fwrite(&registro->primeiro, tamCampo, 1, arquivo);

  tamCampo = strlen(registro->ultimo);
  fwrite(&tamCampo, sizeof(int), 1, arquivo);
  fwrite(&registro->ultimo, tamCampo, 1, arquivo);

  tamCampo = strlen(registro->endereco.rua);
  fwrite(&tamCampo, sizeof(int), 1, arquivo);
  fwrite(&registro->endereco.rua, tamCampo, 1, arquivo);

  tamCampo = strlen(registro->endereco.num);
  fwrite(&tamCampo, sizeof(int), 1, arquivo);
  fwrite(&registro->endereco.num, tamCampo, 1, arquivo);

  tamCampo = strlen(registro->endereco.complemento);
  fwrite(&tamCampo, sizeof(int), 1, arquivo);
  fwrite(&registro->endereco.complemento, tamCampo, 1, arquivo);

  tamCampo = strlen(registro->cidade);
  fwrite(&tamCampo, sizeof(int), 1, arquivo);
  fwrite(&registro->cidade, tamCampo, 1, arquivo);

  tamCampo = strlen(registro->estado);
  fwrite(&tamCampo, sizeof(int), 1, arquivo);
  fwrite(&registro->estado, tamCampo, 1, arquivo);

  tamCampo = strlen(registro->cep);
  fwrite(&tamCampo, sizeof(int), 1, arquivo);
  fwrite(&registro->cep, tamCampo, 1, arquivo);

  tamCampo = strlen(registro->telefone);
  fwrite(&tamCampo, sizeof(int), 1, arquivo);
  fwrite(&registro->telefone, tamCampo, 1, arquivo);

  fseek(arquivoIndices, 0, SEEK_SET);
  tamCampo = strlen(registro->primeiro);
  fwrite(&tamCampo, sizeof(int), 1, arquivoIndices);
  fwrite(&registro->primeiro, tamCampo, 1, arquivoIndices);
  fwrite(&offset, 32, 1, arquivoIndices);

  fclose(arquivo);
  fclose(arquivoIndices);
}

void apagaRegistro(FILE *arquivo)
{
  int tamCampo, tamRegistro;
  char chave[16];
  char apagado[16] = "apagado\0";
  pessoa *registro = malloc(sizeof(pessoa)); // Alocação da estrutura pessoa
  iniciaRegistroNulo(registro);

  printf("Digite a chave a ser pesquisada: ");
  scanf("%s", chave);

  fseek(arquivo, 0, SEEK_SET); // Posiciona a posicao de leitura no inicio do arquivo

  fread(&tamRegistro, sizeof(int), 1, arquivo);
  fread(&tamCampo, sizeof(int), 1, arquivo);
  while (fread(registro->chave, tamCampo, 1, arquivo))
  {
    if (!strcmp(registro->chave, chave))
    {
      printf("Registrado encontrado!\n");
      fseek(arquivo, -(tamCampo + sizeof(int)), SEEK_CUR);
      tamCampo = strlen(apagado);
      fwrite(&tamCampo, sizeof(int), 1, arquivo);
      fwrite(apagado, tamCampo, 1, arquivo);
      printf("Registrado apagado!\n");
      fclose(arquivo);
      return;
    }
    fseek(arquivo, tamRegistro - (tamCampo + sizeof(int)), SEEK_CUR);
    for (int i = 0; i < tamCampo; i++)
    {
      registro->chave[i] = '\0';
    }
    fread(&tamRegistro, sizeof(int), 1, arquivo);
    fread(&tamCampo, sizeof(int), 1, arquivo);
  };

  fclose(arquivo);
}

void leituraCompleta(FILE *arquivo)
{
  pessoa *registro = malloc(sizeof(pessoa)); // Alocação da estrutura pessoa
  iniciaRegistroNulo(registro);

  fseek(arquivo, 0, SEEK_SET); // Posiciona a posição de leitura no inicio do arquivo
  int tamRegistro, tamCampo;

  // Lê todos os registros que não estão como apagados

  while (fread(&tamRegistro, sizeof(int), 1, arquivo))
  {
    fread(&tamCampo, sizeof(int), 1, arquivo);
    fread(registro->chave, tamCampo, 1, arquivo);
    if (strcmp(registro->chave, "apagado\0"))
    {
      fread(&tamCampo, sizeof(int), 1, arquivo);
      fread(registro->primeiro, tamCampo, 1, arquivo);

      fread(&tamCampo, sizeof(int), 1, arquivo);
      fread(registro->ultimo, tamCampo, 1, arquivo);

      fread(&tamCampo, sizeof(int), 1, arquivo);
      fread(registro->endereco.rua, tamCampo, 1, arquivo);

      fread(&tamCampo, sizeof(int), 1, arquivo);
      fread(registro->endereco.num, tamCampo, 1, arquivo);

      fread(&tamCampo, sizeof(int), 1, arquivo);
      fread(registro->endereco.complemento, tamCampo, 1, arquivo);

      fread(&tamCampo, sizeof(int), 1, arquivo);
      fread(registro->cidade, tamCampo, 1, arquivo);

      fread(&tamCampo, sizeof(int), 1, arquivo);
      fread(registro->estado, tamCampo, 1, arquivo);

      fread(&tamCampo, sizeof(int), 1, arquivo);
      fread(registro->cep, tamCampo, 1, arquivo);

      fread(&tamCampo, sizeof(int), 1, arquivo);
      fread(registro->telefone, tamCampo, 1, arquivo);

      imprimeRegistro(registro);
      limpaCamposRegistro(registro);
    }
    else
    {
      fseek(arquivo, tamRegistro - (tamCampo + sizeof(int)), SEEK_CUR);
      for (int i = 0; i < tamCampo; i++)
      {
        registro->chave[i] = '\0';
      }
    }
  }
}

void imprimeRegistro(pessoa *registro)
{
  if (registro != NULL)
  {
    printf("-------------------------------------\n");
    printf("Nome: %s\n", registro->primeiro);
    printf("Sobrenome: %s\n", registro->ultimo);
    printf("Endereço: \n");
    printf("Estado: %s\n", registro->estado);
    printf("Cidade: %s\n", registro->cidade);
    printf("Rua: %s Número: %s Complemento: %s\n", registro->endereco.rua, registro->endereco.num, registro->endereco.complemento);
    printf("CEP: %d\n", atoi(registro->cep));
    printf("Telefone: %d\n", atoi(registro->telefone));
    printf("--------------------------------------\n");
  }
}

void obterCampo(char *buffer)
{
  fflush(stdin);       // limpar o buffer de entrada e saida
  scanf("%s", buffer); // obtendo dados de um campo da estrutura pessoa
}

/* IMPLEMENTAÇÃO DOS MÉTODOS */
void realizaOperacao()
{
  int operacao;
  FILE *arquivo;
  pessoa *registro = malloc(sizeof(pessoa));
  iniciaRegistroNulo(registro);

  int offset;

  do
  {
    printf("1 - Ler todos os registros;\n2 - Pesquisar por um registro por chave;\n3 - Pesquisar registro por nome;\n4 - Escrever registro(s) sequencialmente;\n5 - Escrever registro com reaproveitamento de espaço\n6 - Apagar um registro;\n0 - Finalizar programa;\n");
    printf("Opção escolhida: ");
    scanf("%d", &operacao);
    switch (operacao)
    {
    case 1: // Ler todos os registros
      system("clear");
      printf("Realizando leitura completa:  \n\n");
      arquivo = escolheArquivo("rb");
      leituraCompleta(arquivo);
      break;
    case 2: // Pesquisar por um registro por chave
      system("clear");
      printf("Realizando pesquisa por chave:  \n\n");
      arquivo = escolheArquivo("rb");
      registro = pesquisaRegistroChave(arquivo);
      imprimeRegistro(registro);
      break;
    case 3: // Pesquisar registro por nome
      system("clear");
      printf("Realizando pesquisa por nome:  \n\n");
      arquivo = escolheArquivo("rb");
      offset = pesquisaRegistroNome(1);
      if (offset != -1)
      {
        leituraRegistroNome(offset, arquivo);
      }
      break;
    case 4: // Escrever registro(s) sequencialmente
      system("clear");
      printf("Realizando escrita sequencial de registros:  \n\n");
      arquivo = escolheArquivo("ab");
      insereRegistroSequencialmente(arquivo);
      break;
    case 5: // Escrever registro com reaproveitamento de espaço
      system("clear");
      printf("Realizando escrita de registros com reaproveitamento:  \n\n");
      arquivo = escolheArquivo("rb+");
      insereRegistroReaproveitamento(arquivo);
      break;
    case 6: // Apagar um registro
      system("clear");
      printf("Realizando exclusão de registro:  \n\n");
      arquivo = escolheArquivo("rb+");
      apagaRegistro(arquivo);
      break;
    case 0: // Finalizar Programa
      break;
    default:
      printf("Valor digitado inválido!\n");
      break;
    }
  } while (operacao != 0);
}

FILE *escolheArquivo(char *modoAbertura)
{
  char caminhoArquivo[100]; // Variável que armazena o caminho até o arquivo
  FILE *arquivo;

  printf("Digite o caminho(path) até o arquivo: ");
  scanf("%s", caminhoArquivo);

  arquivo = fopen(caminhoArquivo, modoAbertura);

  while (NULL == arquivo)
  {
    printf("ERRO AO ABRIR ARQUIVO!\nDigite novamente o caminho(path) até o arquivo: ");
    scanf("%s", caminhoArquivo);

    arquivo = fopen(caminhoArquivo, modoAbertura);
  }

  return arquivo;
}

pessoa *pesquisaRegistroChave(FILE *arquivo)
{
  char chave[16];
  int tamCampo, tamRegistro;
  pessoa *registro = malloc(sizeof(pessoa)); // Alocação da estrutura pessoa
  iniciaRegistroNulo(registro);

  printf("Digite a chave a ser pesquisada: ");
  scanf("%s", chave);

  fseek(arquivo, 0, SEEK_SET); // Posiciona a posicao de leitura no inicio do arquivo
  // Lendo o campo
  while (fread(&tamRegistro, sizeof(int), 1, arquivo))
  {
    fread(&tamCampo, sizeof(int), 1, arquivo);
    fread(registro->chave, tamCampo, 1, arquivo);
    if (!strcmp(registro->chave, chave))
    {
      fread(&tamCampo, sizeof(int), 1, arquivo);
      fread(registro->primeiro, tamCampo, 1, arquivo);

      fread(&tamCampo, sizeof(int), 1, arquivo);
      fread(registro->ultimo, tamCampo, 1, arquivo);

      fread(&tamCampo, sizeof(int), 1, arquivo);
      fread(registro->endereco.rua, tamCampo, 1, arquivo);

      fread(&tamCampo, sizeof(int), 1, arquivo);
      fread(registro->endereco.num, tamCampo, 1, arquivo);

      fread(&tamCampo, sizeof(int), 1, arquivo);
      fread(registro->endereco.complemento, tamCampo, 1, arquivo);

      fread(&tamCampo, sizeof(int), 1, arquivo);
      fread(registro->cidade, tamCampo, 1, arquivo);

      fread(&tamCampo, sizeof(int), 1, arquivo);
      fread(registro->estado, tamCampo, 1, arquivo);

      fread(&tamCampo, sizeof(int), 1, arquivo);
      fread(registro->cep, tamCampo, 1, arquivo);

      fread(&tamCampo, sizeof(int), 1, arquivo);
      fread(registro->telefone, tamCampo, 1, arquivo);

      return registro;
    }
    else
    {
      fseek(arquivo, tamRegistro - tamCampo - sizeof(int), SEEK_CUR);
      for (int i = 0; i < tamCampo; i++)
      {
        registro->chave[i] = '\0';
      }
    }
  }
  printf("Registro não encontrado\n");
  return NULL;

  fclose(arquivo);
}

int pesquisaRegistroNome(int pesquisa)
{
  int tamCampo;
  char nomePesquisado[16] = {'\0'};
  char nomeArquivo[16] = {'\0'};

  FILE *arquivo;
  int offset;
  arquivo = fopen("indices.bin", "rb+");

  while (NULL == arquivo)
  {
    char caminhoArquivo[100], modoAbertura[3];

    printf("ERRO AO ABRIR ARQUIVO!\nDigite novamente o caminho(path) até o arquivo: ");
    scanf("%s", caminhoArquivo);
    printf("\nDigite o modo de abertura: ");
    scanf("%s", modoAbertura);

    arquivo = fopen(caminhoArquivo, modoAbertura);
  }
  printf("Digite o nome do registro a ser pesquisado: ");
  scanf("%s", nomePesquisado);

  // Lendo o campo
  while (fread(&tamCampo, sizeof(int), 1, arquivo))
  {
    fread(nomeArquivo, tamCampo, 1, arquivo);
    // Comparando o nome a ser procurado com os que estão no registro
    if (!strcmp(nomeArquivo, nomePesquisado) && pesquisa)
    {
      printf("Registro encontrado: \n");
      fread(&offset, 32, 1, arquivo);

      return offset;
    }
    else if (!pesquisa)
    {
      char apagado[16] = "apagado\0";
      fseek(arquivo, -(tamCampo + sizeof(int)), SEEK_CUR);
      tamCampo = strlen(apagado);
      fwrite(&tamCampo, sizeof(int), 1, arquivo);
      fwrite(&apagado, tamCampo, 1, arquivo);

      return 0;
    }
    fseek(arquivo, 32, SEEK_CUR);
  };

  printf("Registro não encontrado\n");
  fclose(arquivo);

  return -1;
}

void leituraRegistroNome(int offset, FILE *arquivo)
{
  pessoa *registro = malloc(sizeof(pessoa)); // Alocação da estrutura pessoa
  iniciaRegistroNulo(registro);

  int tamCampo, tamRegistro;
  // Posiciona a posição de leitura no byte offset indicado pela pesquisa por nome
  fseek(arquivo, offset, SEEK_SET);
  fread(&tamRegistro, sizeof(int), 1, arquivo);

  fread(&tamCampo, sizeof(int), 1, arquivo);
  fread(registro->chave, tamCampo, 1, arquivo);

  fread(&tamCampo, sizeof(int), 1, arquivo);
  fread(registro->primeiro, tamCampo, 1, arquivo);

  fread(&tamCampo, sizeof(int), 1, arquivo);
  fread(registro->ultimo, tamCampo, 1, arquivo);

  fread(&tamCampo, sizeof(int), 1, arquivo);
  fread(registro->endereco.rua, tamCampo, 1, arquivo);

  fread(&tamCampo, sizeof(int), 1, arquivo);
  fread(registro->endereco.num, tamCampo, 1, arquivo);

  fread(&tamCampo, sizeof(int), 1, arquivo);
  fread(registro->endereco.complemento, tamCampo, 1, arquivo);

  fread(&tamCampo, sizeof(int), 1, arquivo);
  fread(registro->cidade, tamCampo, 1, arquivo);

  fread(&tamCampo, sizeof(int), 1, arquivo);
  fread(registro->estado, tamCampo, 1, arquivo);

  fread(&tamCampo, sizeof(int), 1, arquivo);
  fread(registro->cep, tamCampo, 1, arquivo);

  fread(&tamCampo, sizeof(int), 1, arquivo);
  fread(registro->telefone, tamCampo, 1, arquivo);

  imprimeRegistro(registro);
}

pessoa *obterRegistro()
{
  pessoa *registro = malloc(sizeof(pessoa)); // Alocação da estrutura pessoa
  iniciaRegistroNulo(registro);

  registro->tamRegistro = 0;

  printf("Digite a chave do registro: ");
  obterCampo(registro->chave);
  registro->tamRegistro += strlen(registro->chave);

  printf("Digite o primeiro nome: ");
  obterCampo(registro->primeiro);
  registro->tamRegistro += strlen(registro->primeiro);

  printf("Digite o ultimo nome: ");
  obterCampo(registro->ultimo);
  registro->tamRegistro += strlen(registro->ultimo);

  printf("Digite a rua: ");
  obterCampo(registro->endereco.rua);
  registro->tamRegistro += strlen(registro->endereco.rua);

  printf("Digite o número da casa: ");
  obterCampo(registro->endereco.num);
  registro->tamRegistro += strlen(registro->endereco.num);

  printf("Digite o complemento do endereço: ");
  obterCampo(registro->endereco.complemento);
  registro->tamRegistro += strlen(registro->endereco.complemento);

  printf("Digite a cidade: ");
  obterCampo(registro->cidade);
  registro->tamRegistro += strlen(registro->cidade);

  printf("Digite o estado: ");
  obterCampo(registro->estado);
  registro->tamRegistro += strlen(registro->estado);

  printf("Digite o CEP: ");
  obterCampo(registro->cep);
  registro->tamRegistro += strlen(registro->cep);

  printf("Digite o telefone: ");
  obterCampo(registro->telefone);
  registro->tamRegistro += strlen(registro->telefone);

  return registro;
}