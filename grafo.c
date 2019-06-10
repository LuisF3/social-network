#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

/* -- Grafo -- */

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

void grafoBuscarId(grafo *g, int id) {
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

void grafoListarSolicitacoes(grafo *g, int id_usuario_logado) {
    
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
    return l->tamanho == 0;
}

void listaInserirFim(lista *l, int id, usuario *amigo){
    nohLista *novo = (nohLista*)malloc(sizeof(nohLista));
    novo->proximo = NULL;
    
    novo->id = id;
    novo->amigo = amigo;

    if(listaVazia(l)) l->cabeca->proximo = novo;
    else l->fim->proximo = novo;
    l->fim = novo;

    l->tamanho++;
}

void listaInserirOrdenado(lista *l, int id, usuario* amigo){
    if(listaVazia(l) || id > l->fim->id)
        listaInserirFim(l,id, amigo);

    nohLista *novo = (nohLista*)malloc(sizeof(nohLista));
    novo->proximo = NULL;
    
    novo->id = id;
    novo->amigo = amigo;

    nohLista *p = l->cabeca;

    while(p->proximo != NULL && p->proximo->id < novo->proximo->id)
        p = p->proximo;
    novo->proximo = p->proximo;
    p->proximo = novo;
    l->tamanho++;
}

void listaRemoverInicio(lista *l){
    if(listaVazia(l)) return;
    nohLista *remover = l->cabeca->proximo;
    l->cabeca->proximo = remover->proximo;
    if(l->tamanho == 1) l->fim = NULL;
    free(remover);
    l->tamanho--;
}

void listaRemoverBusca_id (lista *l, int id, usuario *amigo){
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

void listaApagar(lista *l){
    if(!l) return;
    while(!listaVazia(l))
        listaRemoverInicio(l);
    free(l->cabeca);
    free(l);
}