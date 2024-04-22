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

//funções para o selection sort:
void trocar(Aluno* a, Aluno* b); //troca o conteúdo dos espaços de memória contidos em *a e *b

void selection_sort(Aluno lista[], unsigned int tam); //executa o selection sort, dado o vetor "lista" e o tamanho dele "tam"

//funções para o merge sort:
void merge(Aluno lista[], unsigned int e, unsigned int m, unsigned int d); //função para dividir e recolocar dois vetores na ordem os subvetores são E[e,...,m] e o outro D[m+1,...,d]

void merge_sort(Aluno lista[], unsigned int e, unsigned int d); //executa o merge sort, dado o vetor "lista" e os indices das extremidades

//=========================================main===============================================

int main() {

    int T;
    unsigned int N, M;
    scanf("%d", &T); //recebe o método de ordenação desejado
    scanf("%u %u", &N, &M); //recebe o número de alunos e de bolsas disponiveis

    Aluno alunos[N]; //matriz para guardar as infos dos alunos
    fflush(stdin);
    Aluno temp;
    int elim = 0;
    //recebendo medias, numero de rep e nomes
    for (unsigned int i = 0; i < N; i++) {
        scanf(" %f %d ", &temp.media, &temp.num_rep);
        //printf("%.1f %d", alunos[i].media, alunos[i].num_rep);
        fgets(temp.nome, TAM_MAX, stdin);
        temp.nome[strcspn(temp.nome, "\n")] = '\0'; //remove o \n no final de cada nome para facilitar a manipulação
        if(temp.num_rep > 10)
            elim++;
        else{   
            alunos[i-elim] = temp;     
        }           
    }
    N -= elim;
    if(M > N)
        M = N;

    //agora, tendo a lista de alunos com notas e reprovações, devemos ordená-la na ordem de preferência definida
    if(T == 1){
        selection_sort(alunos, N);
    } else if(T == 2){
        merge_sort(alunos, 0, N-1);
    }
    /*
    for (int i = 0; i < N; i++) {
        printf("%s ", alunos[i].nome);
        printf("m = %.1f, r = %d\n", alunos[i].media, alunos[i].num_rep);
    }
    */
    unsigned int extra = 0;
    for(unsigned int i = M - 1; i < N; i++){
        if(alunos[i].media == alunos[i+1].media && alunos[i].num_rep == alunos[i+1].num_rep){
            extra++;
        } else
            break;
    }
    M += extra;
    if(M > N)
        M = N;

    //imprimindo o numero de alunos que ganharam e seus nomes
    printf("%u\n", M);
    for(int i = 0; i < M; i++){
        printf("%s %.1f %d\n", alunos[i].nome, alunos[i].media, alunos[i].num_rep);
    }
    return 0;
}

//===============================implementação de funções=====================================

void trocar(Aluno* a, Aluno* b){
    Aluno aux = *a;
    *a = *b;
    *b = aux;
}

void selection_sort(Aluno lista[], unsigned int tam) {
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

void merge(Aluno lista[], unsigned int e, unsigned int m, unsigned int d) {
    unsigned int n1 = m - e + 1; // tamanho da sub-array 1
    unsigned int n2 = d - m;     // tamanho da sub-array 2
    unsigned int i, j, k; 

    Aluno E[n1], D[n2]; // arrays temporárias (Esquerda e Direita)

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
                unsigned int comp = strcmp(E[i].nome, D[j].nome); // Comparação lexicográfica
                if (comp < 0) { // Se E[i] vem antes de D[j]
                    lista[k] = E[i];
                    i++;
                } else if (comp > 0) { // Se E[i] vem depois de D[j]
                    lista[k] = D[j];
                    j++;
                } else { // Se os nomes são iguais
                    // Ambos os elementos são copiados de volta para o array principal
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

void merge_sort(Aluno lista[], unsigned int e, unsigned int d){
    if(e < d){
        unsigned int m = e + (d - e) / 2; // Decidindo onde será dividida a array entre as duas sub-arrays
        // Com recursão, vamos organizar a primeira e segunda metade
        merge_sort(lista, e, m);
        merge_sort(lista, m + 1, d);

        // Agora juntando de volta as duas
        merge(lista, e, m, d);
    }
}

