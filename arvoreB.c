#include "arvoreB.h"

/*
    Características de uma árvore B de ordem d
        A raiz é uma folha ou tem no mínimo 2 filhos
        Cada nó interno possui no mínimo d+1 filhos
        Cada nó tem no máximo 2d+1 filhos
        Todas as folhas estão no mesmo nível

    Características da raiz
        Tem no mínimo d+1 filhos (ponteiros)
        Tem entre 1 e 2d chaves

    Características do nó interno
        Tem no mínimo d+1 filhos (ponteiros)
        Tem entre d e 2d chaves


    Busca em árvore B
	    Percorra a árvore começando da raiz, se:
        a) encontrou a chave no nó atual, encerra a busca e retorna a chave
        b) se encontrou uma chave maior que a procurada no nó atual, salva ponteiro(pt) a esquerda dessa chave encontrada
        c) se percorreu todas as chaves do nó atual, então salva ponteiro(pt) da direita da última chave do nó atual
        d) se pt = NULL return 0
        e) se não a gente começa outra busca do nó em que o ponteiro (pt) está apontando


    Inserção em árvore B
	    Executa o algoritmo de busca:
        a) Se a chave está no nó retornado pela busca
            Inserção inválida
        b) Se a chave não está no nó retornado pela busca
            Inserir a chave no nó retornado pela busca
                Caso o nó retornado pela busca esteja com a capacidade máxima de 2d, então devemos fazer um particionamento enquanto necessário


    Particionamento
	    Criamos um novo nó
	    Adicionamos as d primeiras chaves no no original
	    Adicionamos as chaves restantes para o novo nó
        A chave d+1 sobe para o pai
	    A chave d+1 aponta para o novo nó
		    Devemos fazer a ordenação do pai por conta da inserção da chave d+1
			    Para fazer isso a gente faz um shift e movemos a chave junto o com o ponteiro a direita dela


    Caso o particionamento chegue até a raiz
	    o nó raiz é particionado normalmente, mas, como a raiz não tem pai, cria-se um novo nó, que passa a ser a nova raiz


    Exclusão
    Existem duas situações possíveis:
        a) A entrada x está em um nó folha
            Neste caso simplesmente removemos a entrada x
                Mas se o número de chaves dentro do nó ficar menor que d, então devemos fazer a concatenação ou redistribuição
        b) A entrada x não está em um nó folha
            Substituir x pela chave y imediatamente maior


    Concatenação
	    Se dois nós(P e Q) são irmãos adjacentes e são apontados por dois ponteiros adjacentes no nó pai então podemos realizar a concatenação, e além disso a soma das chaves tem que ser menor que 2d
	    Pego as chaves Q e adiciono em P
	    No nó pai eu pego a chave que está entre os ponteiros que apontam para P e Q e transfiro ela para P
	    Por fim eliminar o ponteiro que antes apontava para Q
	    Mas caso ao retirar a chave que estava entre os ponteiros no nó pai, ele ficar com um número de chaves menor que d, então eu devo fazer a concatenação novamente


    Redistribuição
	Ocorre quando a soma das entradas de P e de seu irmão adjacente Q é maior ou igual a 2d
		Concatenar P e Q
			Isso resulta em um nó P com mais de 2d chaves
			Particionar o no concatenado, usando Q como novo nó

 */


int calcula_espacamento(int disk, int order) {

    int size_of_btNode = (sizeof(int) * 3) + (sizeof(func) * order - 1) + (sizeof(int) * order);

    return size_of_btNode * disk;    // Retorna a posição do nó no arquivo

}

btNode le_arq(int disk, int order, FILE *fp) {
    btNode read_node;

    int offset = calcula_espacamento(disk, order);
    fseek(fp, offset, SEEK_SET);

    // Lendo as informações do arquivo
    fread(&read_node.num_funcs, sizeof(read_node.num_funcs), 1, fp);
    fread(&read_node.flag_folha, sizeof(read_node.flag_folha), 1, fp);
    fread(&read_node.pos_in_disk, sizeof(read_node.pos_in_disk), 1, fp);

    read_node.funcs = malloc(sizeof(func) * order - 1);
    fread(read_node.funcs, sizeof(func), order - 1, fp);

    read_node.filhos = malloc(sizeof(int) * order);
    fread(read_node.filhos, sizeof(int), order, fp);


    return read_node;
}

void escreve_arq(btNode node, int order, FILE *fp) {

    int offset = calcula_espacamento(node.pos_in_disk, order);
    fseek(fp, offset, SEEK_SET);

    // Escrevendo no arquivo
    fwrite(&node.num_funcs, sizeof(node.num_funcs), 1, fp);
    fwrite(&node.flag_folha, sizeof(node.flag_folha), 1, fp);
    fwrite(&node.pos_in_disk, sizeof(node.pos_in_disk), 1, fp);
    fwrite(node.funcs, sizeof(func), order - 1, fp);
    fwrite(node.filhos, sizeof(int), order, fp);
}

// A função abaixo cria um no para a árvore, podendo ele ser um no folha ou um no interno,
// isso depende da variavel is_leaf, que diz se vai ser folha ou não
// Além disso ela atribui os valores dos funcionarios dentro do no como -1
btNode novo_no(int order, int is_leaf) {
    btNode n;

    n.num_funcs = 0;
    n.flag_folha = is_leaf;

    n.funcs = malloc((order - 1) * sizeof(func));
    for (int i = 0; i < order - 1; i++) {
        n.funcs[i].codigo = -1;
        n.funcs[i].salario = -1;
    }

    n.filhos = malloc((order) * sizeof(int));
    for (int i = 0; i < order; i++) {
        n.filhos[i] = -1;
    }

    return n;
}

// Função para imprimir os funcionarios dentro de um no
void imprimir_funcionarios(btNode node, int order) {
    printf("\n|| Posicao no disco {%d}", node.pos_in_disk);
    for (int i = 0; i < order - 1; i++) {
        if (node.funcs[i].codigo != -1) {
            printf("\n  || Codigo: %d\n ", node.funcs[i].codigo);
            printf(" || Nome: Fulano\n ");
            printf(" || Salario: R$ %.2f\n ", node.funcs[i].salario);
        }
    }
}

// A função abaixo realiza o particionamento de um no
// split_root é uma flag que indica se o nó a ser dividido é a raiz da árvore
void particionamento(btNode x, int pos, bTree *tree, FILE *fp, int split_root) {

    btNode no = le_arq(x.filhos[pos], tree->order, fp);
    if (split_root == 1) {
        tree->node_count++;
        no.pos_in_disk = tree->node_count;
    }

    // Criando um no vazio
    btNode no_novo = novo_no(tree->order, no.flag_folha);
    // Aumentando a quantidade de nós dentro da árvore
    tree->node_count++;
    // A posição desse novo nó recebe a numero de nós total
    no_novo.pos_in_disk = tree->node_count;

    // Calculando o numero de registros que irao ser transferidos de no para no_novo
    int t = (tree->order / 2);

    // Definindo o numero de funcionarios que irão ser dividos
    if (tree->order % 2 == 0) {
        t--;
    }
    no_novo.num_funcs = t;

    if (tree->order % 2 != 0) {
        t--;
    }

    // Essa linha transfere t registros de no para no_novo, atualiza os valores dos códigos e salários dos registros transferidos em no.
    for (int j = 0; j <= t && (j + t + 1) <= no.num_funcs - 1; j++) {
        no_novo.funcs[j] = no.funcs[j + t + 1];
        no.funcs[j + t + 1].codigo = -1;
        no.funcs[j + t + 1].salario = -1;
    }

    // Se for um no interno ele atribui os valores dos ponteiros para os filhos
    if (no.flag_folha == 0) {
        for (int j = 0; j <= t; j++) {
            no_novo.filhos[j] = no.filhos[j + t + 1];
            no.filhos[j + t + 1] = -1;
        }
    }
    no.num_funcs = t;


    // Se no não for folha, as linhas acima transferem os ponteiros dos filhos de no correspondentes aos registros
    // transferidos para no_novo e atualizam os ponteiros dos filhos correspondentes em no.

    if (split_root == 1) {
        x.filhos[pos] = no.pos_in_disk;
        x.filhos[pos + 1] = no_novo.pos_in_disk;
    } else {
        int j, i, r;
        for (j = 0; j < tree->order; j++) {
            if (x.filhos[j] == no.pos_in_disk) {
                for (i = j + 1; i < tree->order; i += 2) {
                    if (i + 1 < tree->order)
                        x.filhos[i + 1] = x.filhos[i];
                }
                r = j;
            }
        }
        x.filhos[r + 1] = no_novo.pos_in_disk;
    }


    for (int j = pos; j < tree->order - 2; j += 2) {
        x.funcs[j + 1] = x.funcs[j];
    }

    x.funcs[pos] = no.funcs[t];
    no.funcs[t].codigo = -1;
    no.funcs[t].salario = -1;
    x.num_funcs++;

    // Salvando os nos
    escreve_arq(x, tree->order, fp);
    escreve_arq(no, tree->order, fp);
    escreve_arq(no_novo, tree->order, fp);
}

// Inseri um novo funcionario no nó node
btNode inserir_no_nao_cheio(btNode node, func key, bTree *tree, FILE *fp) {

    int pos = node.num_funcs;

    // Verifica se é um no folha ou não
    // Se for um nó folha ele realiza uma busca pela posição onde o no ficará ordenado
    if (node.flag_folha == 1) {
        int i = pos - 1;
        while (i >= 0 && key.codigo < node.funcs[i].codigo) {
            node.funcs[i + 1] = node.funcs[i];
            node.funcs[i].codigo = -1;
            node.funcs[i].salario = -1;
            i--;
        }
        if (i + 1 != pos) {
            node.funcs[i + 1] = key;
        } else {
            node.funcs[pos] = key;
        }
        node.num_funcs++;
        escreve_arq(node, tree->order, fp);
        return node;



        // Se for um no interno realizo uma busca recursiva pelo filho correto, se o nó onde o funcionario deve ser
        // inserido estiver cheio eu devo realizar o particionamento
        // O meu particionamento ao inves dele particionar e adicionar o novo elemento de forma ordenada ele so divide,
        // por isso logo apos o particionamento eu chamo essa mesma função por meio de recursão para assim fazer a inserção do funcionáriop de forma ordenada
    } else {
        int n_pd = node.pos_in_disk;
        int i = pos - 1;
        while (key.codigo < node.funcs[i].codigo && i >= 0) {
            i--;
            pos--;
        }

        btNode x = le_arq(node.filhos[pos], tree->order, fp);
        if (x.num_funcs == tree->order - 1) {
            particionamento(node, pos, tree, fp, 0);
            btNode x1 = le_arq(n_pd, tree->order, fp);
            if (key.codigo > x1.funcs[pos].codigo)
                pos++;
        }
        btNode x1 = le_arq(n_pd, tree->order, fp);
        btNode x2 = le_arq(x1.filhos[pos], tree->order, fp);
        inserir_no_nao_cheio(x2, key, tree, fp);
    }
}


func deleta_max(btNode node, int order, FILE *fp) {
    if (node.flag_folha == 1) {
        node.funcs[node.num_funcs - 1].codigo = -1;
        node.funcs[node.num_funcs - 1].salario = -1;
        node.num_funcs--;
        escreve_arq(node, order, fp);
        return node.funcs[node.num_funcs - 1];
    } else {
        btNode x = le_arq(node.filhos[node.num_funcs], order, fp);
        deleta_max(x, order, fp);
    }
}

func deleta_min(btNode node, int order, FILE *fp) {
    if (node.flag_folha == 1) {
        func x = node.funcs[0];
        for (int j = 0; j < node.num_funcs; j++)
            node.funcs[j] = node.funcs[j + 1];
        node.num_funcs--;
        escreve_arq(node, order, fp);
        return x;
    } else {

        btNode x = le_arq(node.filhos[0], order, fp);
        deleta_min(x, order, fp);
    }
}



// A função abaixo realiza a concatenação de dois nós filhos adjacentes
void concatenacao(btNode node, int pos, int order, FILE *fp) {
    int t = (order / 2);

    // Lendo os dois nos irmãos
    btNode filhos_esq = le_arq(node.filhos[pos], order, fp);
    btNode filho_dir = le_arq(node.filhos[pos + 1], order, fp);

    filhos_esq.funcs[t - 1] = node.funcs[pos];
    node.funcs[pos].codigo = -1;
    node.funcs[pos].salario = -1;

    // Movendo as chaves do nó a direita para o nó a esquerda
    for (int j = 0; j < t - 1; j++) {
        filhos_esq.funcs[t + j] = filho_dir.funcs[j];
    }

    // Se o nó não for uma folha eu copio os filhos do nó da direita para o da esquerda
    if (filhos_esq.flag_folha == 0) {
        for (int j = 0; j < t; j++) {
            filhos_esq.filhos[t + j] = filho_dir.filhos[j];
        }
    }

    // Atualizando o numero de "chaves"
    filhos_esq.num_funcs = order - 1;
    for (int j = pos + 1; j < node.num_funcs; j++) {
        node.funcs[j - 1] = node.funcs[j];
    }
    for (int j = pos + 2; j < node.num_funcs + 1; j++) {
        node.filhos[j - 1] = node.filhos[j];
    }
    node.num_funcs--;

    // Escrevendo o nós
    escreve_arq(filhos_esq, order, fp);
    escreve_arq(node, order, fp);
}

// A função recebe como parâmetros um nó node, um inteiro pos que indica a posição do filho esquerdo a ser mesclado,
// um inteiro order que representa a ordem da árvore B, e um arquivo fp usado para leitura e escrita na árvore B.
// Primeiro, a função lê os nós dos filhos esquerdo e direito de node na posição pos e pos + 1, respectivamente.
// Em seguida, move a chave do nó node na posição pos para a posição t - 1 do nó esquerdo y, onde t = (order / 2) é o
// tamanho mínimo permitido. Depois, copia as chaves do nó direito z para o nó esquerdo y, começando da posição t.
// Se o nó node não for uma folha, a função também copia os filhos de z para y, começando da posição t.

void redistribuicao_esq(btNode node, int pos, int order, FILE *fp) {
    int t = (order / 2);

    // Lendo os filhos
    btNode filho_esq = le_arq(node.filhos[pos], order, fp);
    btNode filho_dir = le_arq(node.filhos[pos - 1], order, fp);

    for (int j = t - 1; j > 0; j--) {
        filho_esq.funcs[j] = filho_esq.funcs[j - 1];
    }

    // movendo a chave do nó node para o filho a esquerda
    filho_esq.funcs[0] = node.funcs[pos - 1];
    node.funcs[pos - 1] = filho_dir.funcs[filho_dir.num_funcs - 1];

    filho_dir.funcs[filho_dir.num_funcs - 1].codigo = -1;
    filho_dir.funcs[filho_dir.num_funcs - 1].salario = -1;

    // Se o no node não for folha eu copio os filhos do filho da direita para o da esquerda
    if (filho_esq.flag_folha == 0) {
        for (int j = t; j > 1; j--) {
            filho_esq.filhos[j + 1] = filho_esq.filhos[j];
        }
        filho_esq.filhos[1] = filho_dir.filhos[filho_dir.num_funcs + 1];
    }
    filho_esq.num_funcs = t;
    filho_dir.num_funcs--;
    escreve_arq(filho_dir, order, fp);
    escreve_arq(filho_esq, order, fp);
    escreve_arq(node, order, fp);
}

void redistribuicao_dir(btNode node, int pos, int order, FILE *fp) {
    int t = (order / 2);
    btNode y = le_arq(node.filhos[pos], order, fp);
    btNode z = le_arq(node.filhos[pos + 1], order, fp);

    y.funcs[y.num_funcs] = node.funcs[pos];
    node.funcs[pos] = z.funcs[0];

    for (int j = 0; j < z.num_funcs; j++) {
        if (j + 1 == z.num_funcs) {
            z.funcs[j].codigo = -1;
            z.funcs[j].salario = -1;
        } else
            z.funcs[j] = z.funcs[j + 1];
    }


    if (y.flag_folha == 0) {
        for (int j = t; j > 1; j--) {
            y.filhos[j + 1] = y.filhos[j];
        }
        y.filhos[1] = z.filhos[z.num_funcs + 1];
    }
    y.num_funcs = t;
    z.num_funcs--;
    escreve_arq(z, order, fp);
    escreve_arq(y, order, fp);
    escreve_arq(node, order, fp);
}

// A função começa procurando a posição da chave a ser excluída no nó atual. Se a chave estiver em um nó folha,
// ela é excluída do nó. Se o nó não é uma folha, a chave é substituída pela chave máxima ou mínima do filho esquerdo
// ou direito, respectivamente, desde que o número de chaves do filho seja maior do que o mínimo permitido.
// Caso contrário, a função faz uma redistribuição ou concatenação de nós para garantir que haja pelo menos t - 1 chaves
// em cada nó não folha, onde t é o número mínimo de chaves permitido em cada nó não raiz.

void remove_aux(btNode node, func key, int order, FILE *fp) {
    int t = (order / 2);
    int borrowed;
    int pos = 0;
    // Procura a posição da chave a ser excluída no nó atual
    while (pos <= node.num_funcs - 1 && key.codigo > node.funcs[pos].codigo)
        pos++;
    if (pos <= node.num_funcs && key.codigo == node.funcs[pos].codigo) {
        // Se a chave estiver em um nó folha, ela é excluída do nó
        if (node.flag_folha == 1) {
            // Verifica se o nó é o último nó da árvore
            if (node.pos_in_disk == 0 && node.num_funcs == 1) {
                node.num_funcs = 0;
                escreve_arq(node, order, fp);
                return;
            }

            // Exclui a chave correspondente do nó
            for (int j = pos; j < node.num_funcs; j++)
                node.funcs[j] = node.funcs[j + 1];
            if (pos == node.num_funcs - 1) {
                node.funcs[pos].codigo = -1;
                node.funcs[pos].salario = -1;
            }
            node.num_funcs--;
            escreve_arq(node, order, fp);
        } else {
            // Se o nó não é uma folha, a chave é substituída pela chave máxima ou mínima do filho esquerdo
            // ou direito, respectivamente, desde que o número de chaves do filho seja maior do que o mínimo permitido.
            btNode y = le_arq(node.filhos[pos], order, fp);
            if (y.num_funcs > t - 1) {
                node.funcs[pos] = deleta_max(y, order, fp);
                escreve_arq(node, order, fp);
            } else {

                btNode z = le_arq(node.filhos[pos + 1], order, fp);
                if (z.num_funcs > t - 1) {
                    node.funcs[pos] = deleta_min(z, order, fp);
                    escreve_arq(node, order, fp);
                } else {
                    concatenacao(node, pos, order, fp);
                    btNode node_child = le_arq(node.filhos[pos], order, fp);
                    remove_aux(node_child, key, order, fp);
                }
            }
        }
    } else if (node.flag_folha == 0) {
        int m = pos; //default
        btNode y = le_arq(node.filhos[pos], order, fp);
        if (y.num_funcs == t - 1) {
            borrowed = 0;
            // Caso contrário, a função faz uma redistribuição ou concatenação de nós para garantir que haja pelo menos t - 1 chaves
            // em cada nó não folha, onde t é o número mínimo de chaves permitido em cada nó não raiz.
            if (pos > 0) {
                btNode z = le_arq(node.filhos[pos - 1], order, fp);
                if (z.num_funcs > t - 1) {
                    redistribuicao_esq(node, pos, order, fp);
                    borrowed = 1;
                } else {
                    m = pos - 1;
                }
            }
            if (borrowed == 0 && pos <= node.num_funcs && node.filhos[pos + 1] != -1) {
                btNode z = le_arq(node.filhos[pos + 1], order, fp);
                if (z.num_funcs > t - 1) {
                    redistribuicao_dir(node, pos, order, fp);
                    borrowed = 1;
                } else {
                    m = pos;
                }
            }
            if (borrowed == 0) {
                concatenacao(node, m, order, fp);
                btNode x = le_arq(node.filhos[m], order, fp);
                y = x;
            }
        }
        if (m != pos) {
            remove_aux(y, key, order, fp);
        } else {
            btNode new_y = le_arq(node.filhos[pos], order, fp);
            remove_aux(new_y, key, order, fp);
        }
    }
}


bTree *cria_arvore(int order) {

    bTree *tree;
    if ((tree = malloc(sizeof(bTree))) == NULL)
        return NULL;

    btNode root = novo_no(order, true);
    root.pos_in_disk = 0;

    tree->order = order;
    tree->root = root;
    tree->node_count = 0;

    return tree;

}

void inserir(bTree *tree, func key, FILE *fp) {
    if (tree->node_count > 0)
        tree->root = le_arq(0, tree->order, fp);
    btNode root = tree->root;

    if (root.num_funcs == tree->order - 1) {
        btNode s = novo_no(tree->order, 0);
        s.filhos[0] = root.pos_in_disk;
        particionamento(s, 0, tree, fp, 1);
        s = le_arq(0, tree->order, fp);
        tree->root = s;
        inserir_no_nao_cheio(s, key, tree, fp);
    } else {
        tree->root = inserir_no_nao_cheio(root, key, tree, fp);
    }

}

int pesquisar(btNode node, int order, func key, FILE *fp) {

    int pos = 0;
    while (pos < node.num_funcs && key.codigo > node.funcs[pos].codigo) {
        pos++;
    }
    if (pos <= node.num_funcs && key.codigo == node.funcs[pos].codigo) {
        return node.pos_in_disk;
    } else if (node.flag_folha == 1) {
        return -1;
    } else {
        btNode x = le_arq(node.filhos[pos], order, fp);
        return pesquisar(x, order, key, fp);
    }

}

int remover(bTree *tree, func key, FILE *fp) {

    btNode root = tree->root;
    remove_aux(root, key, tree->order, fp);
    btNode new_root = le_arq(0, tree->order, fp);
    if (new_root.num_funcs == 0 && (new_root.flag_folha == 0)) {
        btNode x = le_arq(new_root.filhos[0], tree->order, fp);
        x.pos_in_disk = 0;
        escreve_arq(x, tree->order, fp);
        tree->root = x;
    } else {
        tree->root = new_root;
    }
    return 1;
}

func chave_max(btNode node, int order, FILE *fp) {
    if (node.flag_folha == 1) {
        return node.funcs[node.num_funcs - 1];
    } else {
        btNode x = le_arq(node.filhos[node.num_funcs], order, fp);
        chave_max(x, order, fp);
    }
}

func chave_min(btNode node, int order, FILE *fp) {
    if (node.flag_folha == 1) {
        return node.funcs[0];
    } else {
        btNode x = le_arq(node.filhos[0], order, fp);
        chave_min(x, order, fp);
    }
}

void imprimir_arvoreB(bTree *tree, queue *q, FILE *fp) {
    if (tree->root.num_funcs == 0) {
        printf("\n|| A arvore B esta vazia\n");
    } else {
        btNode end = {.num_funcs = -1};
        insert(q, tree->root);
        int item_count = 1;
        while (!isEmpty(q)) {
            btNode no_atual = removeData(q);
            if (no_atual.num_funcs == -1) {
                printf("\n");
                insert(q, end);
                if (item_count == 0)
                    break;
            } else {
                item_count--;
                imprimir_funcionarios(no_atual, tree->order);
                if (no_atual.pos_in_disk == 0)
                    insert(q, end);
                for (int i = 0; i < tree->order; i++) {
                    if (no_atual.filhos[i] != -1) {
                        btNode x = le_arq(no_atual.filhos[i], tree->order, fp);
                        insert(q, x);
                        item_count++;
                    }
                }
            }
        }
    }
}

void libera_arvore(bTree *tree, FILE *fp) {
    free(tree);
    fclose(fp);
    remove("file.dat");
}

void imprimir_func(bTree tree, func key, FILE *fp) {
    btNode node = pesq_no(tree.root, tree.order, key, fp);

    if (node.pos_in_disk == -1) {
        printf("Funcionario nao encontrado.\n");
        return;
    }

    int i = 0;
    while (i < node.num_funcs && node.funcs[i].codigo < key.codigo) {
        i++;
    }

    if (node.funcs[i].codigo == key.codigo) {
        printf("|| Funcionario encontrado:\n|| Nome: Fulano\n|| Codigo = %d\n|| Salario = %.2f\n", node.funcs[i].codigo,
               node.funcs[i].salario);
    } else {
        printf("Funcionario nao encontrado.\n");
    }
}

btNode pesq_no(btNode node, int order, func key, FILE *fp) {
    int pos = 0;
    while (pos < node.num_funcs && key.codigo > node.funcs[pos].codigo) {
        pos++;
    }

    if (pos <= node.num_funcs && key.codigo == node.funcs[pos].codigo) {
        return node;
    } else if (node.flag_folha == 1) {
        btNode null_node = {0};
        null_node.pos_in_disk = -1;
        return null_node;
    } else {
        btNode x = le_arq(node.filhos[pos], order, fp);
        return pesq_no(x, order, key, fp);
    }
}
