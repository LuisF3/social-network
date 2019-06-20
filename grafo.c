#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "grafo.h"

void listaPrintar(lista *l, int qtdDePrints);
void listaPrintarAfinidade(lista *l, int qtdDePrints, int afinidadeMinima);
usuario *listaRemoverBusca_Posicao(lista *l, int posicao);
float verificadorAfinidade (usuario *usuario1, usuario *usuario2);
lista *recomendacaoNamorado();
lista *recomendacaoAmigos();

/* -- Grafo -- */

/*
    recebe uma string e retorna uma cópia com todos os caractéres em minúsculo
    str - string 
*/
char *strToLower(char *str){
    char *copia = malloc(strlen(str) * sizeof(char));
    for (int i = 0; i < strlen(str); i++)
        copia[i] = tolower(str[i]);
    return copia;
}

struct _cidade{
    char nome[65];
    char estado[3];
    float latitude;
    float longitude;
};

struct _cor{
    char nome[65];
    float red;
    float green;
    float blue;
};

struct _grafo{
    usuario *ultimo;            // último termo da lista encadeada
    usuario *cabeca;            // início da lista encadeada
    int qtd_usuarios;           // quantidade de elementos presentes no grafo
    cidade todas_cidades[5559]; // conjunto com os dados de todas as cidades do Brasil
    cor todas_cores[865];       // conjunto dos dados de todas as cores disponíveis
};

struct _usuario_no{
    int id;             //identificador do usuário
    int idade;
    char *nome;
    cidade cidade_info;
    cor cor_info;
    char *genero_filme;
    char *time;

    usuario *proximo;       // próximo no encademento do grafo
    lista *amizades;        // amizades do indivíduo
    lista *pedido_amizade;  // solicitações de amizades
};

int ultimo_id = 1;                 // contador para atribuição do código de identificação de um novo usuário
usuario *usuario_atual = NULL;     // ponteiro para o usuário atualmente autenticado no sistema

void arquivoCidadesLer(grafo *g){
    FILE *fp = fopen("cidades.csv","r");
    int pos = 0;

    fscanf(fp,"%*s%*c");
    while(!feof(fp)){
        fscanf(fp,"%64[^,]%*c%2[^,]%*c%f%*c%f%*c", g->todas_cidades[pos].nome, g->todas_cidades[pos].estado, &(g->todas_cidades[pos].longitude), &(g->todas_cidades[pos].latitude));
        pos++;
    }

    fclose(fp); 
}

void arquivoCoresLer(grafo *g){
    FILE *fp = fopen("cores.csv","r");
    int pos = 0;

    fscanf(fp,"%*s%*c");
    while(!feof(fp)){
        fscanf(fp,"%64[^,]%*c%f%*c%f%*c%f%*c", g->todas_cores[pos].nome, &(g->todas_cores[pos].red), &(g->todas_cores[pos].green), &(g->todas_cores[pos].blue));
        pos++;
    }

    fclose(fp); 
}

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

    arquivoCidadesLer(g);
    arquivoCoresLer(g);

    return g;
}

int grafoVazio(grafo *g){
    return g->qtd_usuarios == 0;
}

void grafoInserirFim(grafo *g, int idade, char *nome, char *nome_cidade, char *estado, float latitude, float longitude, char *genero_filme, char *cor, float red, float green, float blue,char *time){
    usuario *novo = (usuario *) malloc(sizeof(usuario));
    novo->proximo = NULL;

    novo->nome = malloc(strlen(nome));
    novo->genero_filme = malloc(strlen(genero_filme));
    novo->time = malloc(strlen(time));

    // Atribuição das variáveis de um usuário
    novo->id = ultimo_id++;
    novo->idade = idade;
    strcpy(novo->nome, nome);
    // Atribuição das variáveis relacionadas à cidade
    strcpy(novo->cidade_info.nome, nome_cidade);
    strcpy(novo->cidade_info.estado, estado);
    novo->cidade_info.latitude = latitude;
    novo->cidade_info.longitude = longitude;
    // Atribuição de outras variáveis  
    strcpy(novo->genero_filme, genero_filme);
    strcpy(novo->time, time);
    // Atribuição das variáveis relacionadas à cor
    strcpy(novo->cor_info.nome,cor);
    novo->cor_info.red = red;
    novo->cor_info.green = green;
    novo->cor_info.blue = blue;
    // Criação das lista empregadas no armazenamento de informações referentes a outros usuários
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
    free(remover->nome);
    free(remover->genero_filme);
    free(remover->time);
    listaApagar(remover->amizades);
    listaApagar(remover->pedido_amizade);
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

int grafoBuscarNome(grafo *g, char *nome){
    usuario *atual = g->cabeca->proximo;
    while(atual && strcmp(atual->nome, nome) != 0)
        atual = atual->proximo;
    if(!atual) return -1;
    return atual->id;
}

void grafoLogin(grafo *g, int id){
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

void grafoLogout(){
    usuario_atual = NULL;
}

void grafoListarSolicitacoes(grafo *g){
    if(!g) return;
    int escolha;

    while(usuario_atual){
        if(listaVazia(usuario_atual->pedido_amizade)){
            printf("Não há solicitações\n");
            return;
        }
        listaPrintarAfinidade(usuario_atual->pedido_amizade, listaTamanho(usuario_atual->pedido_amizade), 0);
        printf("Digite o número do usuário que deseja adicionar ou 0 para continuar\n>>");
        scanf("%d%*c", &escolha);
        if(escolha < 0) break;
        else if(escolha == 0){
            while(usuario_atual){
                if(listaVazia(usuario_atual->pedido_amizade)){
                    printf("Não há solicitações\n");
                    return;
                }   
                listaPrintarAfinidade(usuario_atual->pedido_amizade, listaTamanho(usuario_atual->pedido_amizade), 0);
                printf("Digite o número do usuário que deseja recusar ou 0 para continuar\n>>");
                scanf("%d%*c", &escolha);
                if(escolha <= 0) return;
                if (escolha <= listaTamanho(usuario_atual->pedido_amizade))
                    listaRemoverBusca_Posicao(usuario_atual->pedido_amizade, escolha);   
                else
                    printf("Opção invalida\n");
            }
        }
        if (escolha <= listaTamanho(usuario_atual->pedido_amizade))
        {
            usuario *novo_amigo = listaRemoverBusca_Posicao(usuario_atual->pedido_amizade, escolha);
            if (listaInserirOrdenado(usuario_atual->amizades, novo_amigo->id, novo_amigo) && listaInserirOrdenado(novo_amigo->amizades, usuario_atual->id, usuario_atual))
                printf("Adicionado com sucesso!\n");
        }else
            printf("Opção invalida\n");
    }

    return;
}

void grafoBuscarTodosNomes(grafo *g, char *nome){
    if (!g || !nome) return;
    
    usuario *atual = g->cabeca->proximo;
    char *atualLower;
    char *nomeLower;
    lista *encontrados = listaCriar();
        while(atual){
        atualLower = strToLower(atual->nome);
        nomeLower = strToLower(nome);
        if(strncmp(atualLower, nomeLower, strlen(nome) < strlen(atual->nome) ? strlen(nome) : strlen(atual->nome)) == 0 && strcmp(usuario_atual->nome, atual->nome) && !listaIsNaLista(usuario_atual->amizades, atual->id))
            listaInserirFim(encontrados, atual->id, atual);
        atual = atual->proximo;
        free(atualLower);
        free(nomeLower);
    }

    int escolha;
    while(true) {
        if(listaVazia(encontrados)) {
            printf("Não há usuários para enviar solicitação.\n");
            return;
        }
        listaPrintarAfinidade(encontrados, listaTamanho(encontrados), 0);
        printf("Digite o número do usuário que deseja adicionar ou 0 para continuar.\n>>");
        scanf("%d%*c", &escolha);
        if(escolha <= 0) break;
        if(escolha > listaTamanho(encontrados))
            printf("Entrada de valor inválido.\n");
        else{
            usuario *novo_amigo = listaRemoverBusca_Posicao(encontrados, escolha);
            if (listaInserirOrdenado(novo_amigo->pedido_amizade, usuario_atual->id, usuario_atual))
                printf("Solicitação enviada com sucesso.\n");
        }
    }
    return;  
}

void grafoListarAmizades(){
    printf("==================\n");
    listaPrintar(usuario_atual->amizades, listaTamanho(usuario_atual->amizades));
    printf("==================\n");
    return;
}

void grafoRemoverAmizades(){
    int selecao = -1;
    
    while(selecao) {
        if (listaVazia(usuario_atual->amizades)){
            printf("Sem amigos para remover.\n");
            return;
        }
        listaPrintar(usuario_atual->amizades, listaTamanho(usuario_atual->amizades));
        printf("Digite o número do usuário que deseja remover ou 0 para continuar\n>>");
        scanf("%d%*c", &selecao);
        if(selecao <= 0) break;
        usuario *novo_amigo = listaRemoverBusca_Posicao(usuario_atual->amizades, selecao);
        listaRemoverBusca_id(novo_amigo->amizades, usuario_atual->id);
        printf("%s removido com sucesso.\n", novo_amigo->nome);
    }
}

void grafoAmizadesIndevidas(){
    int selecao = -1;
    lista *amizadesFracas = listaBuscaAmizadesFracas(usuario_atual->amizades);
    
    while(selecao){
        if(listaVazia(amizadesFracas)){
            printf("Sem amigos a se remover \n");
            return;
        }
        listaPrintarAfinidade(amizadesFracas, listaTamanho(amizadesFracas), 0);
        printf("Digite o número do usuário que deseja remover ou 0 para continuar\n>>");
        scanf("%d%*c", &selecao);
        if(selecao <= 0) break;
        if (selecao <= listaTamanho(amizadesFracas))
        {
            usuario *novo_amigo = listaRemoverBusca_Posicao(amizadesFracas, selecao);
            listaRemoverBusca_id(usuario_atual->amizades, novo_amigo->id);
            listaRemoverBusca_id(novo_amigo->amizades, usuario_atual->id);
            printf("%s Removido com sucesso.\n", novo_amigo->nome);
        }else{
            printf("Opção invalida\n");
        }
    }
}

int distanciaPercentual(float latitude, float longitude, float latitude2, float longitude2){
    float lat = (latitude - latitude2) * (latitude - latitude2);
    float log = (longitude - longitude2) * (longitude - longitude2);
    

    float per = (lat + log) / ((46 * 46) + (79 * 79));
    
    return per;
}

int corPercentual(float red, float blue, float green, float red2, float blue2, float green2){
    float redr = (red - red2) * (red - red2);
    float bluer = (blue - blue2) * (blue - blue2);
    float greenr = (green - green2) * (green - green2);
    

    float per = (redr + bluer + greenr) / (256 * 256 * 3);
    
    return per;
}

float verificadorAfinidade(usuario *usuario1, usuario *usuario2){
    if (!usuario1 || !usuario2) return -1;
    //VERIFICAR AMIZADES EM COMUM
    float afinidadeTotal = 100.0;

    // if (strcmp(strToLower(usuario1->cidade), strToLower(usuario2->cidade)) != 0)
    //     afinidadeTotal *= 0.8;
   
    
    if (strcmp(strToLower(usuario1->genero_filme), strToLower(usuario2->genero_filme)) == 0)
        afinidadeTotal *= 1.2;
    if (strcmp(strToLower(usuario1->time), strToLower(usuario2->time)) != 0)
        afinidadeTotal *= 0.6;
    int deltaIdade = usuario1->idade - usuario2->idade;
    if (deltaIdade > 8)
        afinidadeTotal *= 0.8;

    if (afinidadeTotal >= 100)
        return 100;
    return 100 - usuario2->idade;
}

void grafoRecomendacoes(){
    if(listaVazia(usuario_atual->amizades)) {
        printf("Adicione alguns amigos antes de tentar essa funcionalidade!\n");
        return;
    }
    int selecao = -1;
    lista *recomendacoes = recomendacaoAmigos();
    
    while(selecao) {
        if (listaVazia(recomendacoes)){
            printf("Nenhuma recomendação encontrada. Tente adicionar mais alguns amigos!\n");
            return;
        }
        listaPrintarAfinidade(recomendacoes, 15, 0);
        printf("Digite o número do usuário que deseja adicionar ou 0 para continuar\n>>");
        scanf("%d%*c", &selecao);
        if(selecao <= 0) break;
        if (selecao <= 15)
        {
            usuario *novo_amigo = listaRemoverBusca_Posicao(recomendacoes, selecao);
            listaInserirOrdenado(novo_amigo->pedido_amizade, usuario_atual->id, usuario_atual);
            printf("Pedido de amizade enviado a %s com sucesso.\n", novo_amigo->nome);
        }else{
            printf("Opção invalida\n");
        }
    }
}

void grafoEncontrarNamorado(){
    if(listaVazia(usuario_atual->amizades)) {
        printf("Adicione alguns amigos antes de tentar essa funcionalidade!");
        return;
    }
    int selecao = -1;
    lista *recomendacoes = recomendacaoNamorado();

    if (listaVazia(recomendacoes)){
        printf("Nenhum(a) namorado(a) encontrado(a). Tente adicionar mais alguns amigos!\n");
        return;
    }
    listaPrintarAfinidade(recomendacoes, 1, 80);
    if (listaIsNaLista(usuario_atual->amizades, listaRemoverInicio(recomendacoes)->id)) {
        printf("O namorado ideal já é seu amigo\n");
        return;
    }
    else
        printf("Digite 1 para mandar uma solicitação de amizade para o seu possível namorado! (ou 0 para continuar)\n>>");

    scanf("%d%*c", &selecao);
    if(selecao != 1) return;
    usuario *novo_amigo = listaRemoverBusca_Posicao(recomendacoes, selecao);
    listaInserirOrdenado(novo_amigo->pedido_amizade, usuario_atual->id, usuario_atual);
    printf("Pedido de amizade enviado a %s com sucesso.\n", novo_amigo->nome);

    return;
}

void grafoAdicionarTodos(grafo *g){
    if(!g) return;
    //Iterar pelo grafo
    usuario *atual = g->cabeca->proximo;
    while(atual) {
        //Iterar pelo grafo novamente
        usuario *atual2 = atual->proximo;
        while(atual2){
            // sorteia se o vai adicionar a pessoa ou não
            if(rand() % 10 < 5) {
                listaInserirOrdenado(atual->amizades, atual2->id, atual2);
                listaInserirOrdenado(atual2->amizades, atual->id, atual);
            }
            atual2 = atual2->proximo;
        }
        atual = atual->proximo;
    }
}

bool busca_binaria_cidade(grafo *g, char *nome, char *estado, float *latitude, float *longitude){
    int inicio = 0;
    int fim = sizeof(g->todas_cidades) / sizeof(cidade);
    int meio;
    int cmp;

    char *cidadeLower = strToLower(nome);
    char *atualLower;

    while(inicio <= fim){
        meio = (fim + inicio)/2;
        atualLower = strToLower(g->todas_cidades[meio].nome);
        cmp = strncmp(cidadeLower, atualLower, strlen(nome) < strlen(g->todas_cidades[meio].nome) ? strlen(nome) : strlen(g->todas_cidades[meio].nome));
        if(cmp == 0){
            while(strncmp(cidadeLower, atualLower, strlen(nome) < strlen(g->todas_cidades[meio].nome) ? strlen(nome) : strlen(g->todas_cidades[meio].nome)) == 0 && meio >= 0){
                meio -= 1;
                free(atualLower);
                atualLower = strToLower(g->todas_cidades[meio].nome);
            }
            free(atualLower);
            meio += 1;
            while(true){
                int i;
                for(i = meio; strncmp(cidadeLower, atualLower = strToLower(g->todas_cidades[i].nome), strlen(nome) < strlen(g->todas_cidades[i].nome) ? strlen(nome) : strlen(g->todas_cidades[i].nome)) == 0 && i < sizeof(g->todas_cidades) / sizeof(cidade); i++){
                    printf("%02d) %s, %s\n", i - meio + 1, g->todas_cidades[i].nome, g->todas_cidades[i].estado);
                    free(atualLower);
                 }
                int selecao = -1;
                printf("Selecione uma das cidades abaixo:\n>>");
                scanf("%d%*c", &selecao);
                if(selecao <= 0 || selecao > i - meio){
                    printf("Valor inválido\n");
                    continue;
                }
                strcpy(estado,g->todas_cidades[selecao + meio - 1].estado);
                *latitude = g->todas_cidades[selecao + meio - 1].latitude;
                *longitude = g->todas_cidades[selecao + meio - 1].longitude;
                printf("Cidade %s selecionada com sucesso!\n", g->todas_cidades[selecao + meio - 1].nome);
                free(cidadeLower);
                free(atualLower);
                return true;
            }
        }
        else if(cmp < 0) fim = meio - 1; 
        else inicio = meio + 1;
        free(atualLower);
    }
    printf("Nenhuma cidade encontrada. Você quis dizer %s?\n",g->todas_cidades[meio].nome);
    free(cidadeLower);
    return false;
}

bool busca_binaria_cor(grafo *g, char *cor, float *red, float *green, float *blue){
    int inicio = 0;
    int fim = sizeof(g->todas_cores) / sizeof(cor);
    fim /= 10;
    int meio;
    int cmp;

    char *corLower = strToLower(cor);
    char *atualLower;

    while(inicio <= fim){
        meio = (fim + inicio)/2;
        atualLower = strToLower(g->todas_cores[meio].nome);
        cmp = strncmp(corLower, atualLower, strlen(cor) < strlen(g->todas_cores[meio].nome) ? strlen(cor) : strlen(g->todas_cores[meio].nome));
        if(cmp == 0){
            while(strncmp(corLower, atualLower, strlen(cor) < strlen(g->todas_cores[meio].nome) ? strlen(cor) : strlen(g->todas_cores[meio].nome)) == 0 && meio >= 0){
                meio -= 1;
                free(atualLower);
                atualLower = strToLower(g->todas_cores[meio].nome);
            }
            free(atualLower);
            meio += 1;
            while(true){
                int i;
                for(i = meio; strncmp(corLower, atualLower = strToLower(g->todas_cores[i].nome), strlen(cor) < strlen(g->todas_cores[i].nome) ? strlen(cor) : strlen(g->todas_cores[i].nome)) == 0 && i < sizeof(g->todas_cores) / sizeof(cidade); i++){
                    printf("%02d) %s\n", i - meio + 1, g->todas_cores[i].nome);
                    free(atualLower);
                }
                int selecao = -1;
                printf("Selecione uma das cores abaixo:\n>>");
                scanf("%d%*c", &selecao);
                if(selecao <= 0 || selecao > i - meio){
                    printf("Valor inválido\n");
                    continue;
                }
                *red = g->todas_cores[selecao + meio - 1].red;
                *green = g->todas_cores[selecao + meio - 1].green;
                *blue = g->todas_cores[selecao + meio - 1].blue;
                printf("Cor %s selecionada com sucesso!\n", g->todas_cores[selecao + meio - 1].nome);
                free(corLower);
                free(atualLower);
                return true;
            }
        }
        else if(cmp < 0) fim = meio - 1; 
        else inicio = meio + 1;
        free(atualLower);
    }
    printf("Nenhuma cor encontrada. Você quis dizer %s?\n",g->todas_cores[meio].nome);
    free(corLower);
    return false;
}

bool busca_nome(char *nome, grafo *g){
    usuario *atual = g->cabeca->proximo;
    while(atual){
        if(strcmp(atual->nome,nome) == 0){
            printf("Nome já cadastrado\n");
            return true;
        }
        atual = atual->proximo;
    }
    return false;
}

/* -- Lista -- */

/*
* lista de usuários 
* armazena, normalmente a afinidade entre dois 
* vértices, no caso, a amizade entre dois amigos
*/
struct LISTA{
    nohLista *cabeca;
    nohLista *fim;
    int tamanho;
};

struct NOHLISTA{
    int id;
    float afinidade;
    usuario *amigo;     //ponteiro para um dos usuários (vertices do Grafo)

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

// checa se a lista está vazia
int listaVazia(lista *l){
    if (!l)
        return 1;
    return l->tamanho == 0;
}

// insere no inicio da lista
bool listaInserirInicio(lista *l, int id, usuario *amigo){

    if (!l || !amigo)
        return false;
    nohLista *novo = malloc(sizeof(nohLista));
    novo->proximo = NULL;
    
    novo->id = id;
    novo->afinidade = -1;
    novo->amigo = amigo;

    if(listaVazia(l)) l->fim = novo;
    else novo->proximo = l->cabeca->proximo;
    l->cabeca->proximo = novo;

    l->tamanho++;
    return true;
}

//insere no fim da lista
bool listaInserirFim(lista *l, int id, usuario *amigo){
    if (!l || !amigo)
        return false;
    nohLista *novo = malloc(sizeof(nohLista));
    novo->proximo = NULL;
    
    novo->id = id;
    novo->afinidade = -1;
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
    novo->afinidade = -1;
    novo->amigo = amigo;

    nohLista *p = l->cabeca;

    while(p->proximo != NULL && p->proximo->id < novo->id)
        p = p->proximo;
    novo->proximo = p->proximo;
    p->proximo = novo;
    l->tamanho++;
    return true;
}

usuario *listaRemoverInicio(lista *l){
    if(listaVazia(l)) return NULL;
    nohLista *remover = l->cabeca->proximo;
    l->cabeca->proximo = remover->proximo;
    if(l->tamanho == 1) l->fim = NULL;
    usuario *removido = remover->amigo;
    free(remover);
    l->tamanho--;
    return removido;
}

usuario *listaInicio(lista *l) {
    if (!l) return NULL;
    return l->cabeca->proximo->amigo;
}

void listaRemoverBusca_id(lista *l, int id){
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

usuario *listaRemoverBusca_Posicao(lista *l, int posicao){
    if(listaVazia(l)) return NULL;
    nohLista *remover = l->cabeca;
    for (; remover->proximo != NULL && posicao - 1 > 0; posicao--)
        remover = remover->proximo;

    if(remover->proximo == NULL) return NULL;
    nohLista *aux = remover->proximo;
    remover->proximo = remover->proximo->proximo;
    l->tamanho--;
    usuario *amigo = aux->amigo;
    free(aux);
    return amigo;
}

void listaApagar(lista *l){
    if(!l) return;
    while(!listaVazia(l))
        listaRemoverInicio(l);
    free(l->cabeca);
    free(l);
}

void listaPrintarAfinidade(lista *l, int qtdDePrints, int afinidadeMinima){
    if (!l) return;
    nohLista *nol = l->cabeca->proximo;
    int i = 1;
    while(nol != NULL && i <= qtdDePrints) {
        float afinidade = verificadorAfinidade(usuario_atual, nol->amigo);
        if (afinidade >= afinidadeMinima)
        printf("%02d) Chances de amizade verdadeira: %.2f%% - Nome: %s\n", i++, afinidade, nol->amigo->nome);
        nol = nol->proximo;
    }
    return;
}

void listaPrintar(lista *l, int qtdDePrints){
    if (!l) return;
    nohLista *nol = l->cabeca->proximo;
    int i = 1;
    while(nol != NULL && i <= qtdDePrints) {
        printf("%02d) Nome: %s\n", i++, nol->amigo->nome);
        nol = nol->proximo;
    }
    return;
}

usuario *listaBusca_id(lista *l, int id){
    if(listaVazia(l)) return NULL;
    nohLista *busca = l->cabeca->proximo;
    while(busca && busca->id < id)
        busca = busca->proximo;
    if(!busca) return NULL;
    return busca->amigo;
}

lista *listaBuscaAmizadesFracas(lista *l){
    if (!l) return NULL;
    nohLista *aux = l->cabeca->proximo;
    lista *amizadesFracas = listaCriar();


    while (aux) {
        if (verificadorAfinidade(usuario_atual, aux->amigo) < minAmizade)
            listaInserirOrdenado(amizadesFracas, aux->amigo->id, aux->amigo);
        
        aux = aux->proximo;
    }
    return amizadesFracas;
}

bool listaIsNaLista(lista *l, int id){
    if (!l) return false;
    nohLista *aux =  l->cabeca->proximo;

    while (aux)
    {
        if (aux->id == id) return true;
        aux = aux->proximo;
    }
    return false;
}

int listaTamanho(lista *l) {
    if (!l) return 0;
    return l->tamanho;
}

lista *recomendacaoAmigos(){
    if (!usuario_atual) return NULL;
    lista *queue = listaCriar(), *recomendacoes = listaCriar();
    bool *vis = malloc(sizeof(bool) * ultimo_id);
    memset(vis, false, ultimo_id * sizeof(bool));
    listaInserirFim(queue, usuario_atual->id, usuario_atual);

    while (!listaVazia(queue)) {
        usuario *atual = listaRemoverInicio(queue);
        nohLista *it = atual->amizades->cabeca->proximo;
        while (it) {
            if (!vis[it->amigo->id]) {
                listaInserirFim(queue, it->amigo->id, it->amigo);
                if (usuario_atual->id != it->amigo->id && !listaIsNaLista(usuario_atual->amizades, it->amigo->id))
                    listaInserirOrdenado(recomendacoes, -1 * verificadorAfinidade(usuario_atual, it->amigo), it->amigo);
            }
            vis[it->amigo->id] = true;
            it = it->proximo;
        }
    }

    free(vis);
    listaApagar(queue);
    return recomendacoes;
}

lista *recomendacaoNamorado(){
    if (!usuario_atual) return NULL;
    lista *queue = listaCriar(), *recomendacoes = listaCriar();
    bool *vis = malloc(sizeof(bool) * ultimo_id);
    memset(vis, false, ultimo_id * sizeof(bool));
    listaInserirFim(queue, usuario_atual->id, usuario_atual);

    while (!listaVazia(queue)) {
        usuario *atual = listaRemoverInicio(queue);
        nohLista *it = atual->amizades->cabeca->proximo;
        while (it) {
            if (!vis[it->amigo->id]) {
                listaInserirFim(queue, it->amigo->id, it->amigo);
                if (usuario_atual->id != it->amigo->id && !listaIsNaLista(it->amigo->pedido_amizade, usuario_atual->id))
                    listaInserirOrdenado(recomendacoes, -1 * verificadorAfinidade(usuario_atual, it->amigo), it->amigo);
            }
            vis[it->amigo->id] = true;
            it = it->proximo;
        }
    }

    free(vis);
    listaApagar(queue);
    return recomendacoes;
}