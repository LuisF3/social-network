#ifndef H_GRAFO
#define H_GRAFO

#define true 1
#define false 0
#define erro -1
#define minAmizade 50.0
typedef int bool;

typedef struct _grafo grafo;
typedef struct _usuario_no usuario;

/*
* Aloca dinamicamente um grafo, inicializando seus atributos. 
* @return grafo*: ponteiro para o grafo criado.
* Caso ocorra algum erro durante a alocação, o ponteiro terá valor nulo.
*/
grafo *grafoCriar();

/*
* Insere um vértice no fim da lista após a criação e atribuição dos dados ao novo elemento.
*/
void grafoInserirFim();

/*
* Desaloca o grafo da memória principal.
*/
void grafoApagar();

/*
* Lista todas as solicitações de amizade enviadas ao usuário, exibindo o remetente e o grau de afinidade e
* permitindo escolher quais pedidos serão aceitos.
* @param g: grafo das pessoas cadastradas no sistema (desnecessário)
*/
void grafoListarSolicitacoes(grafo *g);

/*
* Procura por todos os usuários cujo nome corresponda à pesquisa, possibilitando o envio de pedidos de amizade
* para todos os identificados. 
* @param g: grafo das pessoas cadastradas no sistema
* @param nome: chave para comparação durante a busca 
*/
void grafoBuscarTodosNomes(grafo *g, char *nome);

/*
* Busca nos registros o usuário com o mesmo nome do declarado como parâmetro.   
* @param g: grafo das pessoas cadastradas no sistema
* @param nome: valor sobre o qual será realizada a busca
* @return: -1 (nome não foi encontrado) / int (id da pessoa) 
*/
int grafoBuscarNome(grafo *g, char *nome);

/*
* Efetua o acesso ao sistema por meio do código de identificação.
* @param g: grafo das pessoas cadastradas no sistema
* @param id: código de identificação do usuário que está requisistando o acesso
*/
void grafoLogin(grafo *g, int id);

/*
* Sai do sistema.
*/
void grafoLogout();

/*
* Imprime a lista de amizades do usuário.
*/
void grafoListarAmizades();

/*
*
*/
void grafoRemoverAmizades();

/*
*
*/
void grafoAmizadesIndevidas();

/*
*
*/
void grafoRecomendacoes();

/*
*
* @param g:
*/
void grafoAdicionarTodos(grafo *g);


typedef struct LISTA lista;
typedef struct NOHLISTA nohLista;

/*
* Aloca dinamicamente e inicializa a lista.
* @return lista*: ponteiro da lista alocada dinamicamente com seus parâmetros inicializados.
* Caso ocorra algum erro durante a alocação, o ponteiro terá valor nulo.
*/
lista *listaCriar();

/*
* Insere ordenadamente na lista de acordo com o conteúdo.
* @param id: conteúdo do novo nó
* @param amigo: conteúdo do novo nó
* @param l: ponteiro da lista em que o nó será posicionado
* @return bool: false (ocorreu falha durante a operação) / true (operação foi realizado com êxito)
*/
bool listaInserirOrdenado(lista *l, int id, usuario *amigo);

/*
* Insere no fim da lista.
* @param id: conteúdo do novo nó
* @param amigo: conteúdo do novo nó
* @param l: ponteiro da lista em que o nó será posicionado
*/
bool listaInserirFim(lista *l, int id, usuario *amigo);

/*
* Remove o nó inicial da lista e retorna o seu conteúdo.
* @param l: ponteiro da lista que terá seu componente desalocado 
* @return usuario*: informações do nó removido
* Se a lista estiver vazia, o valor retornado será nulo.
*/
usuario *listaRemoverInicio(lista *l);

/*
* Remove o elemento cujas especificações sejam iguais aos valores de comparação
* @param l: lista em que será efetuada a remoção
* @param id: valor de comparação para remover
* @param amigo: valor de comparação para remover
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
bool listaIsNaLista(lista *l, int id);
int listaTamanho(lista *l); 

#endif