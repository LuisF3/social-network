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
* @param g: grafo das pessoas cadastradas no sistema
* @param idade: valor para o campo 'idade' do usuário
* @param nome: valor para o campo 'nome' do usuário
* @param nome_cidade: valor para o campo 'nome' de 'cidade' do usuário
* @param estado: valor para o campo 'estado' de 'cidade' do usuário
* @param latitude: valor para o campo 'latitude' de 'cidade' do usuário
* @param longitude: valor para o campo 'longitude' de 'cidade' do usuário
* @param genero_filme: valor para o campo 'genero_filme' do usuário
* @param cor: valor para o campo 'nome' de 'cor' do usuário
* @param red: valor para o campo 'red' de 'cor' do usuário
* @param green: valor para o campo 'green' de 'cor' do usuário
* @param blue: valor para o campo 'blue' de cor do usuário
* @param time: valor para o campo 'time' do usuário 
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
* Identifica todos os amigos cuja afinidade seja inferior ao valor estabelecido, permitindo ao usuário cadastrado
* removê-los de sua lista de amizades.
*/
void grafoAmizadesIndevidas();

/*
* Realiza busca no sistema e lista as pessoas com perfil mais próximo, embora possam ser exibidos usuários com afinidade
* inferior à porcentagem mínima. Para que a funcionalidade opere corretamente, o perfil logado deve apresentar ao menos
* um amigo adicionado.
*/
void grafoRecomendacoes();

/*
* Estabelece relações entre os usuários aleatoriamente.
* @param g: grafo das pessoas cadastradas no sistema
*/
void grafoAdicionarTodos(grafo *g);

/*
* Aplica busca binária sobre o conjunto de cidades presentes no sistema para listar as mais próximas da chave declarada como parâmetro.
* Caso não existam cidades com nome parecido de acordo com o método empregado, imprime na tela o valor da última execução. Caso contrário,
* possibilita inserir os dados da cidade selecionada nas demais variáveis de parâmetro.
* @param g: grafo das pessoas cadastradas no sistema
* @param nome: string utilizada como comparação
* @param estado: string para armazenamento do estado da cidade selecionada, caso ocorra a seleção de uma cidade
* @param latitude: ponteiro para armazenamento da latitude da cidade selecionada, caso ocorra a seleção de uma cidade
* @param longitude: ponteiro para armazenamento da latitude da cidade selecionada, caso ocorra a seleção de uma cidade
* @return bool: false (nenhuma cidade identificada) / true (cidade selecionada) 
*/
bool busca_binaria_cidade(grafo *g, char *nome, char *estado, float *latitude, float *longitude);

/*
* Aplica busca binária sobre o conjunto de cores presentes no sistema para listar as mais próximas da chave declarada como parâmetro.
* Caso não existam cores com nome parecido de acordo com o método empregado, imprime na tela o valor da última execução. Caso contrário,
* possibilita inserir os dados da cor selecionada nas demais variáveis de parâmetro. 
* @param g: grafo das pessoas cadastradas no sistema
* @param cor: string utilizada como comparação                                                     
* @param red: ponteiro para armazenamento da quantidade de vermelho, caso ocorra a seleção de uma cor
* @param green: ponteiro para armazenamento da quantidade de verde, caso ocorra a seleção de uma cor
* @param blue: ponteiro para armazenamento da quantidade de azul, caso ocorra a seleção de uma cor
* @return bool: false (nenhuma cor identificada) / true (cor selecionada)
*/
bool busca_binaria_cor(grafo *g, char *cor, float *red, float *green, float *blue);

/*
* Percorre a lista de nomes dos usuários para checar a existência da chave de busca no conjunto.
* @param nome: nome para checagem
* @param g: grafo das pessoas cadastradas no sistema
* @return bool: false (o nome não consta no sistema) / true (nome já registrado)
*/
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
* Produz uma lista com todos os amigos cuja amizade estiver abaixo do mínimo.
* @param l: lista de amizades do usuário
* @return lista*: ponteiro com a lista resultante
*/
lista *listaBuscaAmizadesFracas(lista *l);

/*
* Procura pelo usuário de mesmo identificador do valor de parâmtero.
* @param l: lista sobre a qual será realizada a operação
* @param id: identificador do usuário procurado
* @return usuario*: ponteiro para o nó da lista na qual se encontra o valor desejado
* Caso não se encontre a chave entre os valores existentes, a função retornará nulo.
*/
usuario *listaBusca_id(lista *l, int id);

/*
* Remove o elemento da lista cujo atributo 'id' seja equivalente ao declarado.
* @param l: lista sobre a qual será realizada a operação
* @param id: identificador do usuário procurado
*/
void listaRemoverBusca_id(lista *l, int id);

/*
* Procura por um usuário com o id especificado na lista.
* @param l: lista sobre a qual será realizada a operação
* @param id: identificador do usuário procurado
* @return bool: false (usuário não encontrado) / true (usuário encontrado)
*/
bool listaIsNaLista(lista *l, int id);

/*
* Retorna o número de contatos presentes na lista de amizades.
* @param l: lista sobre a qual será realizada a operação
* @return int: número de elementos da lista
*/
int listaTamanho(lista *l); 

#endif