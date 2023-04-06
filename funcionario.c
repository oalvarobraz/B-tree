#include "funcionario.h"
#include "windows.h"


// Imprime funcionario
void imprime(TFunc *func) {
    printf("\nFuncionario de codigo ");
    printf("%d", func->cod);
    printf("\nNome: ");
    printf("%s", func->nome);
    printf("\nSalario: ");
    printf("R$ %4.2f", func->salario);
    printf("\n\n********************************************************************************************");
}

// Cria funcionario. Lembrar de usar free(func)
TFunc *funcionario(int cod, char *nome, double salario) {
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    //inicializa espaço de memória com ZEROS
    if (func) memset(func, 0, sizeof(TFunc));
    //copia valores para os campos de func
    func->cod = cod;
    strcpy(func->nome, nome); // utiliza strcpy pq é uma string
    func->salario = salario;
    return func;
}

// Salva funcionario no arquivo out, na posicao atual do cursor
void salva(TFunc *func, FILE *out) {
    fwrite(&func->cod, sizeof(int), 1, out);
    //func->nome ao invés de &func->nome, pois string já é ponteiro
    fwrite(func->nome, sizeof(char), sizeof(func->nome), out);
    fwrite(&func->salario, sizeof(double), 1, out);
}

// Le um funcionario do arquivo in na posicao atual do cursor
// Retorna um ponteiro para funcionario lido do arquivo
TFunc *le(FILE *in) {
    TFunc *func = (TFunc *) malloc(sizeof(TFunc));
    if (0 >= fread(&func->cod, sizeof(int), 1, in)) {
        free(func);
        return NULL;
    }
    fread(func->nome, sizeof(char), sizeof(func->nome), in);
    fread(&func->salario, sizeof(double), 1, in);
    return func;
}

// Retorna tamanho do funcionario em bytes
int tamanho() {
    return sizeof(int)  //cod
           + sizeof(char) * 50 //nome
           + sizeof(double); //salario
}

// Imprime arquivo
void imprime_arquivo(FILE *arq) {
    //le o arquivo e coloca no vetor
    rewind(arq); //posiciona cursor no inicio do arquivo
    TFunc *f = le(arq);
    while (!feof(arq)) {
        imprime(f);
        f = le(arq);
    }
}

// Retorna o tamanho do arquivo
int tamanho_arquivo(FILE *arq) {
    fseek(arq, 0, SEEK_END);
    int tam = trunc(ftell(arq) / tamanho());
    return tam;
}
