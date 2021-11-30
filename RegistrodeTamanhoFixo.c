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

// Pesquisa por número do registro
pessoa *pesquisaRegistroNumero(FILE *arquivo);

// Lê o registro à partir da pesquisa por nome
void leituraRegistroNome(int offset, FILE *arquivo);

// Le um registro
pessoa *obterRegistro();

int main()
{
  system("clear");
  printf("Bem vindo ao programa que trata de registros com tamanho fixo!\n\n");
  realizaOperacao();
  system("clear");
  printf("\n\nExecução Finalizada!\n\n");
  return 0;
}

void insereRegistroSequencialmente(FILE *arquivo)
{
  int escrevendo = 0;                        // Variavel para controle de escrita de registro
  pessoa *registro = malloc(sizeof(pessoa)); // Alocação da estrutura pessoa

  FILE *arquivoIndices;
  int offset;
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
    fwrite(&registro->chave, 16, 1, arquivo);
    fwrite(&registro->primeiro, 16, 1, arquivo);
    fwrite(&registro->ultimo, 16, 1, arquivo);
    fwrite(&registro->endereco.rua, 16, 1, arquivo);
    fwrite(&registro->endereco.num, 16, 1, arquivo);
    fwrite(&registro->endereco.complemento, 16, 1, arquivo);
    fwrite(&registro->cidade, 16, 1, arquivo);
    fwrite(&registro->estado, 16, 1, arquivo);
    fwrite(&registro->cep, 16, 1, arquivo);
    fwrite(&registro->telefone, 16, 1, arquivo);

    // Salvar o indice
    fwrite(&registro->primeiro, 16, 1, arquivoIndices);
    fwrite(&offset, 32, 1, arquivoIndices);

    printf("1 - Escrever registro;\n0 - Sair;\n");
    scanf("%d", &escrevendo);
  }

  fclose(arquivo);
  fclose(arquivoIndices);
}

void insereRegistroReaproveitamento(FILE *arquivo)
{
  char chave[16] = "apagado        \0"; // Variável para encontrar o registro apagado

  pessoa *registro = malloc(sizeof(pessoa)); // Alocação de estrutura pessoa

  FILE *arquivoIndices;
  int offset;
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
  // Lendo e comparando o campo
  while (fread(registro->chave, 16, 1, arquivo) && strcmp(registro->chave, chave))
  {
    fseek(arquivo, 144, SEEK_CUR);
    fread(registro->chave, 16, 1, arquivo);
  };
  fseek(arquivo, -16, SEEK_CUR);
  registro = obterRegistro();
  offset = ftell(arquivo);
  fwrite(registro->chave, 16, 1, arquivo);
  fwrite(registro->primeiro, 16, 1, arquivo);
  fwrite(registro->ultimo, 16, 1, arquivo);
  fwrite(registro->endereco.rua, 16, 1, arquivo);
  fwrite(registro->endereco.num, 16, 1, arquivo);
  fwrite(registro->endereco.complemento, 16, 1, arquivo);
  fwrite(registro->cidade, 16, 1, arquivo);
  fwrite(registro->estado, 16, 1, arquivo);
  fwrite(registro->cep, 16, 1, arquivo);
  fwrite(registro->telefone, 16, 1, arquivo);

  fseek(arquivoIndices, 0, SEEK_SET);
  fwrite(registro->primeiro, 16, 1, arquivoIndices);
  fwrite(&offset, 32, 1, arquivoIndices);

  fclose(arquivo);
  fclose(arquivoIndices);
}

void apagaRegistro(FILE *arquivo)
{
  int i;
  char chave[16], *espacoAdic;
  char apagado[16] = "apagado        \0";
  pessoa *registro = malloc(sizeof(pessoa)); // Alocação da estrutura pessoa
  printf("Digite a chave a ser pesquisada: ");
  scanf("%s", chave);
  espacoAdic = (char *)malloc(16 - strlen(chave) + 1);
  for (i = 0; i < 15 - strlen(chave); i++)
  {
    espacoAdic[i] = ' ';
  } // for
  espacoAdic[i] = '\0';
  strcat(chave, espacoAdic);

  fseek(arquivo, 0, SEEK_SET); // Posiciona a posicao de leitura no inicio do arquivo

  while (fread(registro->chave, 16, 1, arquivo))
  {
    if (!strcmp(registro->chave, chave))
    {
      fseek(arquivo, -16, SEEK_CUR);
      fwrite(&apagado, 16, 1, arquivo);
    }
    fseek(arquivo, 144, SEEK_CUR);
  };

  fclose(arquivo);
}

void leituraCompleta(FILE *arquivo)
{
  pessoa *registro = malloc(sizeof(pessoa)); // Alocação da estrutura pessoa
  fseek(arquivo, 0, SEEK_SET);               // Posiciona a posição de leitura no inicio do arquivo

  // Lê todos os registros que não estão como apagados
  while (fread(registro->chave, 16, 1, arquivo))
  {
    if (strcmp(registro->chave, "apagado        \0"))
    {
      fread(registro->primeiro, 16, 1, arquivo);
      fread(registro->ultimo, 16, 1, arquivo);
      fread(registro->endereco.rua, 16, 1, arquivo);
      fread(registro->endereco.num, 16, 1, arquivo);
      fread(registro->endereco.complemento, 16, 1, arquivo);
      fread(registro->cidade, 16, 1, arquivo);
      fread(registro->estado, 16, 1, arquivo);
      fread(registro->cep, 16, 1, arquivo);
      fread(registro->telefone, 16, 1, arquivo);
      imprimeRegistro(registro);
    }
    else
    {
      fseek(arquivo, 144, SEEK_CUR);
    }
  }

  fclose(arquivo);
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
  char bufferAux[16]; // buffer utilizado para ler um campo do teclado
  char *espacoAdic;   // espaco restante de um campo que deve armazenado no buffer
  int i;              // variavel utilizada para acessar o array espacoAdic

  fflush(stdin);             // limpar o buffer de entrada e saida
  scanf("%s", bufferAux);    // obtendo dados de um campo da estrutura pessoa
  strcat(buffer, bufferAux); // colocando os dados do campo no fim do buffer

  espacoAdic = (char *)malloc(16 - strlen(bufferAux) + 1);

  for (i = 0; i < 15 - strlen(bufferAux); i++)
  {
    espacoAdic[i] = ' ';
  } // for
  espacoAdic[i] = '\0';

  strcat(buffer, espacoAdic);
}

/* IMPLEMENTAÇÃO DOS MÉTODOS */
void realizaOperacao()
{
  int operacao;
  FILE *arquivo;
  pessoa *registro = malloc(sizeof(pessoa));
  int offset;

  do
  {
    printf("1 - Ler todos os registros;\n2 - Pesquisar por um registro por chave;\n3 - Pesquisar registro por numero;\n4 - Pesquisar registro por nome;\n5 - Escrever registro(s) sequencialmente;\n6 - Escrever registro com reaproveitamento de espaço\n7 - Apagar um registro;\n0 - Finalizar programa;\n");
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
    case 3: // Pesquisar registro por numero
      system("clear");
      printf("Realizando pesquisa por número do registro:  \n\n");
      arquivo = escolheArquivo("rb");
      registro = pesquisaRegistroNumero(arquivo);
      imprimeRegistro(registro);
      break;
    case 4: // Pesquisar registro por nome
      system("clear");
      printf("Realizando pesquisa por nome:  \n\n");
      arquivo = escolheArquivo("rb");
      offset = pesquisaRegistroNome(1);
      if (offset != -1)
      {
        leituraRegistroNome(offset, arquivo);
      }
      break;
    case 5: // Escrever registro(s) sequencialmente
      system("clear");
      printf("Realizando escrita sequencial de registros:  \n\n");
      arquivo = escolheArquivo("ab");
      insereRegistroSequencialmente(arquivo);
      break;
    case 6: // Escrever registro com reaproveitamento de espaço
      system("clear");
      printf("Realizando escrita de registros com reaproveitamento:  \n\n");
      arquivo = escolheArquivo("rb+");
      insereRegistroReaproveitamento(arquivo);
      break;
    case 7: // Apagar um registro
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
  int i;
  char chave[16], *espacoAdic;
  pessoa *registro = malloc(sizeof(pessoa)); // Alocação da estrutura pessoa
  printf("Digite a chave a ser pesquisada: ");
  scanf("%s", chave);

  //  Completa o espaço que sobrou no vetor da chave
  espacoAdic = (char *)malloc(16 - strlen(chave) + 1);
  for (i = 0; i < 15 - strlen(chave); i++)
  {
    espacoAdic[i] = ' ';
  } // for
  espacoAdic[i] = '\0';
  strcat(chave, espacoAdic);

  fseek(arquivo, 0, SEEK_SET); // Posiciona a posicao de leitura no inicio do arquivo
  // Lendo o campo
  while (fread(registro->chave, 16, 1, arquivo))
  {
    // Comparando a chave a ser procurada com as que estão no registro
    if (!strcmp(registro->chave, chave))
    {
      printf("Registro encontrado: \n");
      fread(registro->primeiro, 16, 1, arquivo);
      fread(registro->ultimo, 16, 1, arquivo);
      fread(registro->endereco.rua, 16, 1, arquivo);
      fread(registro->endereco.num, 16, 1, arquivo);
      fread(registro->endereco.complemento, 16, 1, arquivo);
      fread(registro->cidade, 16, 1, arquivo);
      fread(registro->estado, 16, 1, arquivo);
      fread(registro->cep, 16, 1, arquivo);
      fread(registro->telefone, 16, 1, arquivo);
      fclose(arquivo);

      return registro;
    }

    fseek(arquivo, 144, SEEK_CUR); // Posiciona a posição de leitura no próximo campo chave
  };
  printf("Registro não encontrado\n");
  return NULL;

  fclose(arquivo);
}

int pesquisaRegistroNome(int pesquisa)
{
  int i;
  char nomePesquisado[16], *espacoAdic;
  char nomeArquivo[16];

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

  //  Completa o espaço que sobrou no vetor do primeiro nome
  espacoAdic = (char *)malloc(16 - strlen(nomePesquisado) + 1);
  for (i = 0; i < 15 - strlen(nomePesquisado); i++)
  {
    espacoAdic[i] = ' ';
  } // for
  espacoAdic[i] = '\0';
  strcat(nomePesquisado, espacoAdic);

  // Lendo o campo
  while (fread(nomeArquivo, 16, 1, arquivo))
  {
    // Comparando o nome a ser procurado com os que estão no registro
    if (!strcmp(nomeArquivo, nomePesquisado) && pesquisa)
    {
      printf("Registro encontrado: \n");
      fread(&offset, 32, 1, arquivo);

      return offset;
    }
    else if (!pesquisa)
    {
      char apagado[16] = "apagado        \0";
      fseek(arquivo, -16, SEEK_CUR);
      fwrite(&apagado, 16, 1, arquivo);

      return 0;
    }
    fseek(arquivo, 32, SEEK_CUR);
  };

  printf("Registro não encontrado\n");
  fclose(arquivo);

  return -1;
}

pessoa *pesquisaRegistroNumero(FILE *arquivo)
{
  int numero, i = 0;
  pessoa *registro = malloc(sizeof(pessoa)); // Alocação da estrutura pessoa
  printf("Digite o numero do registro a ser pesquisado: ");
  scanf("%d", &numero);
  fseek(arquivo, 0, SEEK_SET); // Posiciona a posicao de leitura no inicio do arquivo
  fread(registro->chave, 16, 1, arquivo);
  while (i < numero || !strcmp(registro->chave, "apagado        \0"))
  {
    if (strcmp(registro->chave, "apagado        \0"))
    {
      i++;
      fseek(arquivo, 144, SEEK_CUR);
    }
    else
    {
      fseek(arquivo, 144, SEEK_CUR);
    }
    fread(registro->chave, 16, 1, arquivo);
  }
  fseek(arquivo, -16, SEEK_CUR);
  // Lendo o campo
  if (fread(registro->chave, 16, 1, arquivo))
  {
    printf("Registro encontrado: \n");
    fread(registro->primeiro, 16, 1, arquivo);
    fread(registro->ultimo, 16, 1, arquivo);
    fread(registro->endereco.rua, 16, 1, arquivo);
    fread(registro->endereco.num, 16, 1, arquivo);
    fread(registro->endereco.complemento, 16, 1, arquivo);
    fread(registro->cidade, 16, 1, arquivo);
    fread(registro->estado, 16, 1, arquivo);
    fread(registro->cep, 16, 1, arquivo);
    fread(registro->telefone, 16, 1, arquivo);

    return registro;
  }
  else
  {
    printf("Registro não encontrado\n");
    return NULL;
  }

  fclose(arquivo);
}

void leituraRegistroNome(int offset, FILE *arquivo)
{
  pessoa *registro = malloc(sizeof(pessoa)); // Alocação da estrutura pessoa

  // Posiciona a posição de leitura no byte offset indicado pela pesquisa por nome
  fseek(arquivo, offset, SEEK_SET);
  fread(registro->chave, 16, 1, arquivo);
  fread(registro->primeiro, 16, 1, arquivo);
  fread(registro->ultimo, 16, 1, arquivo);
  fread(registro->endereco.rua, 16, 1, arquivo);
  fread(registro->endereco.num, 16, 1, arquivo);
  fread(registro->endereco.complemento, 16, 1, arquivo);
  fread(registro->cidade, 16, 1, arquivo);
  fread(registro->estado, 16, 1, arquivo);
  fread(registro->cep, 16, 1, arquivo);
  fread(registro->telefone, 16, 1, arquivo);
  imprimeRegistro(registro);
  fclose(arquivo);
}

pessoa *obterRegistro()
{
  pessoa *registro = malloc(sizeof(pessoa)); // Alocação da estrutura pessoa

  printf("Digite a chave do registro: ");
  obterCampo(registro->chave);

  printf("Digite o primeiro nome: ");
  obterCampo(registro->primeiro);

  printf("Digite o ultimo nome: ");
  obterCampo(registro->ultimo);

  printf("Digite a rua: ");
  obterCampo(registro->endereco.rua);

  printf("Digite o número da casa: ");
  obterCampo(registro->endereco.num);

  printf("Digite o complemento do endereço: ");
  obterCampo(registro->endereco.complemento);

  printf("Digite a cidade: ");
  obterCampo(registro->cidade);

  printf("Digite o estado: ");
  obterCampo(registro->estado);

  printf("Digite o CEP: ");
  obterCampo(registro->cep);

  printf("Digite o telefone: ");
  obterCampo(registro->telefone);

  return registro;
}