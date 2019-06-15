#ifndef H_GRAFO
#define H_GRAFO

#define true 1
#define false 0
#define erro -1
#define minAmizade 50.0
typedef int bool;

typedef struct _grafo grafo;
typedef struct _usuario_no usuario;

grafo *grafoCriar();
void grafoInserirFim();
void grafoApagar();
void grafoListarSolicitacoes(grafo *g);
void grafoBuscarTodosNomes(grafo *g, char *nome);
int grafoBuscarNome(grafo *g, char *nome);
void grafoLogin(grafo *g, int id);
void grafoLogout();
void grafoListarAmizades();
void grafoRemoverAmizades();
void grafoAmizadesIndevidas();
void grafoRecomendacoes();
void grafoAdicionarTodos(grafo *g);

typedef struct LISTA lista;
typedef struct NOHLISTA nohLista;

/*
* Aloca dinamicamente e inicializa a lista.
* @return l: ponteiro da lista alocada dinamicamente com seus parâmetros inicializados.
* Caso ocorra algum erro durante a alocação, o ponteiro terá valor nulo.
*/
lista *listaCriar();

/*
* Insere ordenadamente na lista de acordo com o conteúdo.
* @param id: conteúdo do novo nó
* @param usuario: conteúdo do novo nó
* @param l: ponteiro da lista em que o nó será posicionado
*/
bool listaInserirOrdenado(lista *l, int id, usuario *amigo);

/*
* Insere no fim da lista.
* @param id: conteúdo do novo nó
* @param usuario: conteúdo do novo nó
* @param l: ponteiro da lista em que o nó será posicionado
*/
bool listaInserirFim(lista *l, int id, usuario *amigo);

/*
* Remove o nó inicial da lista e retorna o seu conteúdo.
* @param l: ponteiro da lista que terá seu componente desalocado 
*/
usuario *listaRemoverInicio(lista *l);

/*
* Remove o elemento cujas especificações sejam iguais aos valores de comparação
* @param l: lista em que será efetuada a remoção
* @param chaveveBusca: valor de comparação para remover
* @param usuario: valor de comparação para remover
*/
void listaRemoverBusca(lista *l, int id, usuario *amigo);

/*
* Indica se a lista possui algum elemento nela.
* @param l: ponteiro da lista de verificação
* @return int: 0 (lista não vazia) - 1 (lista vazia)
*/
int listaVazia(lista *l);

/*
* Exclui todos os elementos da lista, bem como desaloca a lista e o nó cabeça.
* @param l: ponteiro da lista que será apagada
*/
void listaApagar(lista *l);
lista *listaBuscaAmizadesFracas(lista *l);
usuario *listaBusca_id (lista *l, int id);
void listaRemoverBusca_id (lista *l, int id);

#endif