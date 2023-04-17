#include "interface.h"


void imprime_arquivo_txt(FILE *arq){
    int i;
    char Linha[100];
    char *result;
    // Abre um arquivo TEXTO para LEITURA
    if (arq == NULL)  // Se houve erro na abertura
    {
        printf("|| Erro ao abrir o arquivo\n");
        return;
    }
    i = 1;
    while (!feof(arq))
    {
        // Lê uma linha (inclusive com o '\n')
        result = fgets(Linha, 100, arq);  // o 'fgets' lê até 99 caracteres ou até o '\n'
        if (result)  // Se foi possível ler
            printf("%s",Linha);
        i++;
    }
}

void MSG_MENU() {
    printf("\n\n>>>>>>>>>>>>>>>>>>>>>>> OPCOES DE MENU <<<<<<<<<<<<<<<<<<<<<<<<");
    printf(" \n|| (1) IMPRIMIR ARQUIVO");
    printf(" \n|| (2) BUSCAR FUNCIONARIO");
    printf(" \n|| (3) REMOVER FUNCIONARIO");
    printf(" \n|| (4) IMPRIMINDO RESULTADOS DA BUSCA");
    printf(" \n|| (0) SAIR");
}


void MENU(FILE *arq, bTree *tree, queue *q0) {
    int codigo;
    FILE* busca;

    int opcao;
    do {
        MSG_MENU();
        printf("\n\n|| Digite uma opcao: ");
        fflush(stdin);
        scanf("%d", &opcao);
        switch (opcao) {
            case 0:
                system("cls");
                printf("\n<<<<<<<<< SAINDO DO PROGRAMA >>>>>>>>>>\n");
                libera_arvore(tree, arq);
                system("PAUSE");
                break;
            case 1:
                system("cls");
                printf("\n|| IMPRIMINDO FUNCIONARIOS DENTRO DO ARQUIVO ||\n");
                imprimir_arvoreB(tree, q0, arq);
                printf("\n");
                system("PAUSE");
                system("cls");
                break;
            case 2:
                system("cls");
                printf("\n|| BUSCANDO FUNCIONARIO ||\n");
                printf("\n|| Informe o codigo do funcionario: ");
                scanf("%d", &codigo);
                func ke2 = {.codigo = codigo};
                int search2 = pesquisar(tree->root, tree->order, ke2, arq);
                if (search2 != -1) {
                    imprimir_func(*tree, ke2, arq);
                } else {
                    printf("Funcionario nao encontrado.\n");
                }
                printf("\n");
                system("PAUSE");
                system("cls");
                break;
            case 3:
                system("cls");
                printf("\n|| REMOVENDO FUNCIONARIO ||\n");
                printf("\n|| Informe o codigo do funcionario que voce deseja remover: ");
                scanf("%d", &codigo);
                func ke = {.codigo = codigo};
                int search = pesquisar(tree->root, tree->order, ke, arq);
                if (search != -1) {
                    int removed = remover(tree, ke, arq);
                    if (removed == 1) {
                        printf("|| Funcionario de codigo %d removido com sucesso!\n", codigo);
                    }
                }else {
                    printf("|| Funcionario de codigo %d nao esta na arvore!\n", codigo);
                }

                printf("\n");
                system("PAUSE");
                system("cls");
                break;
            case 4:
                system("cls");
                printf("\n|| LENDO O RESULTADO DAS BUSCAS ||\n");
                busca = fopen("busca.txt", "r");
                imprime_arquivo_txt(busca);
                printf("\n");
                fclose(busca);
                system("PAUSE");
                system("cls");
                break;
            default:
                system("cls");
                printf("|| Digite uma opcao valida!!!\n");
                system("PAUSE");
                system("cls");
        } // fim do bloco switch
    } while (opcao != 0);
}
