#include "interface.h"


int main() {

    FILE *fp;
    fp = fopen("file.dat", "wb+");

    bTree *tree = cria_arvore(4);
    for (int i = 1; i <= 1050; ++i) {
        func n1 = {.codigo = i, .salario = 1300};
        inserir(tree, n1, fp);
    }
    queue *q0 = createQueue(1050);

    MENU(fp, tree, q0);

    fclose(fp);

    return 0;
}
