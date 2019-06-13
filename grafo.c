#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "grafo.h"

void listaPrintar(lista *l);
void listaPrintarAfinidade(lista *l);
usuario *listaRemoverBusca_PosicaoNoFree(lista *l, int posicao);
float verificadorAfinidade (usuario *usuario1, usuario *usuario2);

/* -- Grafo -- */

char *strToLower(char *str) {
    char *copia = malloc(strlen(str) * sizeof(char));
    for (int i = 0; i < strlen(str); i++)
        copia[i] = tolower(str[i]);
    return copia;
}

struct _grafo{
    usuario *cabeca;
    usuario *ultimo;
    int qtd_usuarios;
};

struct _usuario_no{
    int id;
    int idade;
    char *nome;
    char *cidade;
    char *genero_filme;
    char *cor_favorita;
    char *time;

    lista *amizades;
    lista *pedido_amizade;
    usuario *proximo;
};

int ultimo_id = 1;
usuario *usuario_atual = NULL;

grafo *grafoCriar(){
    grafo *g = (grafo*)malloc(sizeof(grafo));
    if(!g) return NULL;

    g->cabeca = (usuario *)malloc(sizeof(usuario));
    if(!g->cabeca){
        free(g);
        return NULL;
    }

    g->ultimo = NULL;
    g->cabeca->proximo = NULL;
    g->qtd_usuarios = 0;

    return g;
}

int grafoVazio(grafo *g){
    return g->qtd_usuarios == 0;
}

void grafoInserirFim(grafo *g, int idade, char *nome, char *cidade, char *genero_filme, char *cor_favorita, char *time){
    usuario *novo = (usuario *) malloc(sizeof(usuario));
    novo->proximo = NULL;

    novo->nome = malloc(strlen(nome));
    novo->cidade = malloc(strlen(cidade));
    novo->genero_filme = malloc(strlen(genero_filme));
    novo->cor_favorita = malloc(strlen(cor_favorita));
    novo->time = malloc(strlen(time));

    // Atribuição das variáveis de um usuário
    novo->id = ultimo_id++;
    novo->idade = idade;
    strcpy(novo->nome, nome);
    strcpy(novo->cidade, cidade);
    strcpy(novo->genero_filme, genero_filme);
    strcpy(novo->cor_favorita, cor_favorita);
    strcpy(novo->time, time);
    novo->amizades = listaCriar();
    novo->pedido_amizade = listaCriar();

    if(grafoVazio(g)) g->cabeca->proximo = novo;
    else g->ultimo->proximo = novo;
    g->ultimo = novo;

    g->qtd_usuarios++;
}

void grafoRemoverBusca_id(grafo *g, int id){
    if(grafoVazio(g)) return;
    usuario *remover = g->cabeca;
    while(remover->proximo != NULL && remover->proximo->id < id)
        remover = remover->proximo;
    if(remover->proximo == NULL || remover->proximo->id > id) return;
    usuario *aux = remover->proximo;
    remover->proximo = remover->proximo->proximo;
    if(aux == g->ultimo) free(g->ultimo);
    free(aux);
    g->qtd_usuarios--;
}

void grafoRemoverInicio(grafo *g){
    if(grafoVazio(g)) return;
    usuario *remover = g->cabeca->proximo;
    g->cabeca->proximo = remover->proximo;
    if(g->qtd_usuarios == 1) g->ultimo = NULL;
    free(remover);
    g->qtd_usuarios--;
}

void grafoApagar(grafo *g){
    if(!g) return;
    while(!grafoVazio(g))
        grafoRemoverInicio(g);
    free(g->cabeca);
    free(g);
}

int grafoBuscarNome(grafo *g, char *nome) {
    usuario *atual = g->cabeca->proximo;
    while(atual && strcmp(atual->nome, nome) != 0)
        atual = atual->proximo;
    if(!atual) return -1;
    return atual->id;
}

void grafoLogin(grafo *g, int id) {
    if(usuario_atual) return;
    usuario *atual = g->cabeca->proximo;
    while(atual && atual->id < id)
        atual = atual->proximo;
    if(!atual || atual->id > id) {
        usuario_atual = NULL;
        return;
    }
    usuario_atual = atual;
    return;
}

void grafoLogout() {
    usuario_atual = NULL;
}

void grafoListarSolicitacoes(grafo *g) {
    if(!g) return;
    int escolha;
    while (usuario_atual) {
        if(listaVazia(usuario_atual->pedido_amizade)){
            printf("Não há solicitações\n");
            return;
        }
        listaPrintarAfinidade(usuario_atual->pedido_amizade);
        printf("Digite o número do usuário que deseja adicionar ou 0 para continuar\n>>");
        scanf("%d%*c", &escolha);
        if (escolha <= 0) break;
        usuario *novo_amigo = listaRemoverBusca_PosicaoNoFree(usuario_atual->pedido_amizade, escolha);
        if(listaInserirOrdenado(usuario_atual->amizades, novo_amigo->id, novo_amigo) && listaInserirOrdenado(novo_amigo->amizades, usuario_atual->id, usuario_atual))
            printf("Adicionado com sucesso!\n");
    }

    return;
}

void grafoBuscarTodosNomes(grafo *g, char *nome) {
    if (!g || !nome) return;
    
    usuario *atual = g->cabeca->proximo;
    lista *encontrados = listaCriar();
    while(atual) {
        if(strncmp(strToLower(atual->nome), strToLower(nome), strlen(nome) < strlen(atual->nome) ? strlen(nome) : strlen(atual->nome)) == 0)
            listaInserirFim(encontrados, atual->id, atual);
        atual = atual->proximo;
    }

    int escolha;
    while(true) {
        if(listaVazia(encontrados)) {
            printf("Não há usuários para enviar solicitação.\n");
            return;
        }
        listaPrintarAfinidade(encontrados);
        printf("Digite o número do usuário que deseja adicionar ou 0 para continuar\n>>");
        scanf("%d%*c", &escolha);
        if(escolha <= 0) break;
        usuario *novo_amigo = listaRemoverBusca_PosicaoNoFree(encontrados, escolha);
        if(listaInserirOrdenado(novo_amigo->pedido_amizade, usuario_atual->id, usuario_atual))
            printf("Solicitação enviada com sucesso.\n");
    }
    return;  
}

void grafoListarAmizades(grafo *g){
    if(!g) return;
    listaPrintar(usuario_atual->amizades);
    printf("\n");
    return;
}

float verificadorAfinidade (usuario *usuario1, usuario *usuario2) {
    if (!usuario1 || !usuario2) return -1;
    //VERIFICAR AMIZADES EM COMUM
    float afinidadeTotal = 100.0;

    if (strcmp(strToLower(usuario1->cidade), strToLower(usuario2->cidade)) != 0)
        afinidadeTotal *= 0.8;
    if (strcmp(strToLower(usuario1->genero_filme), strToLower(usuario2->genero_filme)) == 0)
        afinidadeTotal *= 1.2;
    if (strcmp(strToLower(usuario1->cor_favorita), strToLower(usuario2->cor_favorita)) == 0)
        afinidadeTotal *= 1.1;
    if (strcmp(strToLower(usuario1->time), strToLower(usuario2->time)) != 0)
        afinidadeTotal *= 0.6;
    int deltaIdade = usuario1->idade - usuario2->idade;
    if (deltaIdade > 8)
        afinidadeTotal *= 0.8;

    if (afinidadeTotal >= 100)
        return 100;
    return afinidadeTotal;
}

/* -- Lista -- */

struct LISTA{
    int tamanho;
    nohLista *cabeca;
    nohLista *fim;
};

struct NOHLISTA{
    int id;
    usuario *amigo;

    nohLista *proximo;
};

lista *listaCriar(){
    lista *l = (lista*)malloc(sizeof(lista));
    if(!l) return NULL;

    l->cabeca = (nohLista*)malloc(sizeof(nohLista));
    if(!l->cabeca){
        free(l);
        return NULL;
    }

    l->cabeca->proximo = NULL;
    l->tamanho = 0;

    return l;
}

int listaVazia(lista *l){
    if (!l)
        return 1;
    return l->tamanho == 0;
}

bool listaInserirFim(lista *l, int id, usuario *amigo){

    if (!l || !amigo)
        return false;
    nohLista *novo = (nohLista*)malloc(sizeof(nohLista));
    novo->proximo = NULL;
    
    novo->id = id;
    novo->amigo = amigo;

    if(listaVazia(l)) l->cabeca->proximo = novo;
    else l->fim->proximo = novo;
    l->fim = novo;

    l->tamanho++;
    return true;
}

bool listaInserirOrdenado(lista *l, int id, usuario* amigo){
    if(!l || !amigo) return false;

    if(listaVazia(l) || id > l->fim->id)
        return listaInserirFim(l, id, amigo);;
    nohLista *novo = malloc(sizeof(nohLista));
    novo->proximo = NULL;
    
    novo->id = id;
    novo->amigo = amigo;

    nohLista *p = l->cabeca;

    while(p->proximo != NULL && p->proximo->id < novo->id)
        p = p->proximo;
    novo->proximo = p->proximo;
    p->proximo = novo;
    l->tamanho++;
    return true;
}

void listaRemoverInicio(lista *l){
    if(listaVazia(l)) return;
    nohLista *remover = l->cabeca->proximo;
    l->cabeca->proximo = remover->proximo;
    if(l->tamanho == 1) l->fim = NULL;
    free(remover);
    l->tamanho--;
}

void listaRemoverBusca_id (lista *l, int id){
    if(listaVazia(l)) return;
    nohLista *remover = l->cabeca;
    while(remover->proximo != NULL && remover->proximo->id < id)
        remover = remover->proximo;
    if(remover->proximo == NULL || remover->proximo->id > id) return;
    nohLista *aux = remover->proximo;
    remover->proximo = remover->proximo->proximo;
    if(aux == l->fim) free(l->fim);
    free(aux);
    l->tamanho--;
}

usuario *listaRemoverBusca_PosicaoNoFree (lista *l, int posicao){
    if(listaVazia(l)) return NULL;
    nohLista *remover = l->cabeca;
    for (; remover->proximo != NULL && posicao - 1 > 0; posicao--)
        remover = remover->proximo;

    if(remover->proximo == NULL) return NULL;
    nohLista *aux = remover->proximo;
    remover->proximo = remover->proximo->proximo;
    l->tamanho--;
    return aux->amigo;
}

void listaApagar(lista *l){
    if(!l) return;
    while(!listaVazia(l))
        listaRemoverInicio(l);
    free(l->cabeca);
    free(l);
}

void listaPrintarAfinidade(lista *l) {
    if (!l) return;
    nohLista *nol = l->cabeca->proximo;
    int i = 1;
    while(nol != NULL) {
        printf("%d) Nome: %s - Chances de amizade verdadeira %.2f%%\n", i++, nol->amigo->nome, verificadorAfinidade(usuario_atual, nol->amigo));
        nol = nol->proximo;
    }
    return;
}

void listaPrintar(lista *l) {
    if (!l) return;
    nohLista *nol = l->cabeca->proximo;
    int i = 1;
    while(nol != NULL) {
        printf("%d) Nome: %s\n", i++, nol->amigo->nome);
        nol = nol->proximo;
    }
    return;
}