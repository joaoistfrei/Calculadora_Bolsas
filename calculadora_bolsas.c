//============================== informações relevantes ======================================

#define ALUNO_AUTOR "Joao Pedro Machado Medeiros"
#define NUSP 14582462

//=============================  bibliotecas utilizadas ======================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TAM_MAX 100

//============================= criando o objeto "aluno" =====================================

typedef struct {
    char nome[TAM_MAX];
    float media;
    int num_rep;
} Aluno;

//============================= declaração das funções =======================================

// Funções para o selection sort:
void trocar(Aluno* a, Aluno* b); // Troca o conteúdo dos espaços de memória contidos em *a e *b

void selection_sort(Aluno* lista, int tam); // Executa o selection sort, dado o vetor "lista" e o tamanho dele "tam"

// Funções para o merge sort:
void merge(Aluno* lista, int e, int m, int d); // Função para dividir e recolocar dois vetores na ordem os subvetores são E[e,...,m] e o outro D[m+1,...,d]

void merge_sort(Aluno* lista, int e, int d); // Executa o merge sort, dado o vetor "lista" e os indices das extremidades

//======================================= main ===============================================

int main() {

    int T, N, M; // T = metodo de ordenação escolhido | N = número inicial de alunos | M = número inicial de bolsas
    scanf("%d", &T); // Recebe o método de ordenação desejado
    scanf("%d %d", &N, &M); // Recebe o número de alunos e de bolsas disponiveis

    // Usando calloc para alocar memória para a matriz de alunos
    Aluno *alunos = (Aluno *)calloc(N, sizeof(Aluno));
    if (alunos == NULL) {
        printf("Erro ao alocar memória.\n");
        return 1;
    }
    Aluno temp; // Variável auxiliar para verificar se o aluno está nas condições necessárias (<10 reprovações)
    int valido = 0; // Variável que recebe o número de alunos aptos a receber a bolsa
    // Recebendo medias, numero de rep e nomes
    for (int i = 0; i < N; i++) {
        scanf("%f %d %s", &temp.media, &temp.num_rep, temp.nome);
        if(temp.num_rep <= 10){ // Se o aluno puder receber bolsa, é posto na lista "alunos"
            alunos[valido].media = temp.media;
            alunos[valido].num_rep = temp.num_rep;
            strcpy(alunos[valido].nome, temp.nome);
            valido++;
        }
    }
    N = valido; // Agora N representa o número de alunos aptos a receber bolsa

    // Tendo a lista de alunos com notas e reprovações, devemos ordená-la na ordem de preferência definida
    if(T == 1){
        selection_sort(alunos, N);
    } else if(T == 2){
        merge_sort(alunos, 0, N-1);
    }

    if(M > N) // Como não podem haver mais alunos do que bolsas dadas, é seguro verificar se isso acontece
        M = N;

    // Devemos checar se há empate nos alunos que estão no limite de ganhar a bolsa
    while(alunos[M].media == alunos[M-1].media && alunos[M].num_rep == alunos[M-1].num_rep){
        M++;
    }

    if(M > N)
        M = N;

    // Imprimindo o numero de alunos que ganharam, seguido de seus nomes
    printf("%d\n", M);
    for(int i = 0; i < M; i++){
        printf("%s\n", alunos[i].nome);
    }
    return 0;
}

//===============================implementação de funções=====================================

void trocar(Aluno* a, Aluno* b){
    Aluno aux = *a;
    *a = *b;
    *b = aux;
}

void selection_sort(Aluno* lista, int tam) {
    for (int etapa = 0; etapa < tam - 1; etapa++) {
        int max = etapa;
        for (int i = etapa + 1; i < tam; i++) {

            // Primeiro critério: ordenar por notas em ordem decrescente
            if (lista[i].media > lista[max].media) {
                max = i;
            }
            // Em caso de notas iguais, ordenar por número de reprovações em ordem crescente
            else if (lista[i].media == lista[max].media && lista[i].num_rep < lista[max].num_rep) {
                max = i;
            }
            // Se ainda houver empate, ordene em ordem alfabética
            else if (lista[i].media == lista[max].media && lista[i].num_rep == lista[max].num_rep && strcmp(lista[i].nome, lista[max].nome) < 0) {
                max = i;
            }
        }
        // Trocar os elementos se necessário
        if (max != etapa) {
            trocar(&lista[max], &lista[etapa]);
        }
    }
}

void merge(Aluno* lista, int e, int m, int d) {
    int n1 = m - e + 1; // Tamanho da sub-array da esquerda
    int n2 = d - m;     // Tamanho da sub-array da direita
    int i, j, k; 

    Aluno* E  = (Aluno*)calloc(n1, sizeof(Aluno)); // Arrays temporárias (Esquerda e Direita)
    Aluno* D  = (Aluno*)calloc(n2, sizeof(Aluno)); // Nesse caso foi usado calloc para nao sobrecarregar a memória stack

    // Copiando o conteúdo da array principal para as sub-arrays
    for (i = 0; i < n1; i++)
        E[i] = lista[e + i];

    for (j = 0; j < n2; j++)
        D[j] = lista[m + 1 + j];

    i = 0;
    j = 0;
    k = e;

    // Comparação e ordenação por notas
    while (i < n1 && j < n2) {
        if (E[i].media > D[j].media) {
            lista[k] = E[i];
            i++;
        } else if (E[i].media < D[j].media) {
            lista[k] = D[j];
            j++;
        } else { // Em caso de empate nas notas
            // Desempate pelo menor número de reprovações
            if (E[i].num_rep < D[j].num_rep) {
                lista[k] = E[i];
                i++;
            } else if (E[i].num_rep > D[j].num_rep) {
                lista[k] = D[j];
                j++;
            } else { // Em caso de empate no número de reprovações
                // Desempate em ordem alfabética
                int comp = strcmp(E[i].nome, D[j].nome); // Comparando em ordem alfabética
                if (comp < 0) { // Se E[i] vem antes de D[j]
                    lista[k] = E[i];
                    i++;
                } else if (comp > 0) { // Se E[i] vem depois de D[j]
                    lista[k] = D[j];
                    j++;
                } else { // Se os nomes são iguais, ambos os elementos são copiados de volta para o array principal
                    lista[k] = D[j];
                    j++;
                }   
            }
        }
        k++;
    }

    // Copiar os elementos restantes da sub-array esquerda, se houver
    while (i < n1) {
        lista[k] = E[i];
        i++;
        k++;
    }

    // Copiar os elementos restantes da sub-array direita, se houver
    while (j < n2) {
        lista[k] = D[j];
        j++;
        k++;
    }
}

void merge_sort(Aluno* lista, int e, int d){
    if(e < d){
        int m = e + (d - e) / 2; // Decidindo onde será dividida a array entre as duas sub-arrays
        // Com recursão, vamos organizar a primeira e segunda metade
        merge_sort(lista, e, m);
        merge_sort(lista, m + 1, d);

        // Finalmente, juntando de volta as duas partes
        merge(lista, e, m, d);
    }
}

