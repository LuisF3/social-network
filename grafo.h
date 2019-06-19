#ifndef H_GRAFO
#define H_GRAFO

#define true 1
#define false 0
#define erro -1
#define minAmizade 50.0 //valor mínino de afinidade para considerar uma amizade como verdadeira
typedef int bool;

typedef struct _grafo grafo;
typedef struct _usuario_no usuario;
typedef struct _cidade cidade;
typedef struct _cor cor;

void grafoEncontrarNamorado();

/*
* Aloca dinamicamente um grafo, inicializando seus atributos. 
* @return grafo*: ponteiro para o grafo criado.
* Caso ocorra algum erro durante a alocação, o ponteiro terá valor nulo.
*/
grafo *grafoCriar();

/*
* Insere um vértice no fim da lista após a criação e atribuição dos dados ao novo elemento.
* @param g:
* @param idade:
* @param nome:
* @param nome_cidade:
* @param estado:
* @param latitude:
* @param longitude:
* @param genero_filme:
* @param cor:
* @param red:
* @param green:
* @param blue:
* @param filme: 
*/
void grafoInserirFim(grafo *g, int idade, char *nome, char *nome_cidade, char *estado, float latitude, float longitude, char *genero_filme, char *cor, float red, float green, float blue,char *time);

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
* Remove uma ou mais pessoas da lista de amizades.
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
* Estabelece relações entre os usuários aleatoriamente.
* @param g: grafo das pessoas cadastradas no sistema
*/
void grafoAdicionarTodos(grafo *g);

/*
*
* @param g:
* @param cidade_info:
* @return bool:
*/
bool busca_binaria_cidade(grafo *g, char *nome, char *estado, float *latitude, float *longitude);
bool busca_binaria_cor(grafo *g, char *cor, float *red, float *green, float *blue);
bool busca_nome(char *nome, grafo *g);

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

/*
*
* @param l:
* @return lista*:
*/
lista *listaBuscaAmizadesFracas(lista *l);

/*
*
* @param l:
* @param id:
* @return usuario*:
*/
usuario *listaBusca_id(lista *l, int id);

/*
*
* @param l:
* @param id:
*/
void listaRemoverBusca_id(lista *l, int id);

/*
*
* @param l:
* @param id:
* @return bool:
*/
bool listaIsNaLista(lista *l, int id);

/*
* Retorna o número de contatos presentes na lista de amizades.
* @param l: lista sobre a qual será realizada a operação
* @return int: número de elementos da lista
*/
int listaTamanho(lista *l); 

#endif