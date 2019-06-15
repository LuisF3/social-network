#include <stdio.h>
#include <stdlib.h>
#include "grafo.h"

int debug = 0;
int id_usuario_logado = -1;

void arquivo_ler(grafo *g) {
    if(!g) return;
    FILE *fp = fopen("1.in", "r");

    int idade;
    char *nome;
    char *cidade;
    char *genero_filme;
    char *cor_favorita;
    char *time;

    while(!feof(fp)){
        fscanf(fp, "%d %m[^\n\r] %m[^\n\r] %m[^\n\r] %m[^\n\r] %m[^\n\r] %*c", &idade, &nome, &cidade, &genero_filme, &cor_favorita, &time);
        if(debug) printf("/%s/\n/%s/\n/%s/\n/%s/\n/%s/\n", nome, cidade, genero_filme, cor_favorita, time);    
        grafoInserirFim(g, idade, nome, cidade, genero_filme, cor_favorita, time);

        free(nome);
        free(cidade);
        free(genero_filme);
        free(cor_favorita);
        free(time);
    }

    fclose(fp);
}

void cadastrar(grafo *g) {
    if(!g) return;

    int idade;
    char *nome;
    char *cidade;
    char *genero_filme;
    char *cor_favorita;
    char *time;

    printf("Idade:\n>>");
    scanf("%d%*c", &idade);
    printf("Nome completo:\n>>");
    scanf("%m[^\n\r]%*c", &nome);
    printf("Cidade:\n>>");
    scanf("%m[^\n\r]%*c", &cidade);
    printf("Gênero de filme favorito:\n>>");
    scanf("%m[^\n\r]%*c", &genero_filme);
    printf("Cor favorita:\n>>");
    scanf("%m[^\n\r]%*c", &cor_favorita);
    printf("Time de futebol:\n>>");
    scanf("%m[^\n\r]%*c", &time);

    if(debug) printf("/%s/\n/%s/\n/%s/\n/%s/\n/%s/\n", nome, cidade, genero_filme, cor_favorita, time);    
    grafoInserirFim(g, idade, nome, cidade, genero_filme, cor_favorita, time);
    printf("Cadastrado com sucesso\n");

    free(nome);
    free(cidade);
    free(genero_filme);
    free(cor_favorita);
    free(time);
}

void login(grafo *g) {
    if(!g) return;

    char *nome;

    printf("Nome completo:\n>>");
    scanf("%m[^\r\n]%*c",&nome); 
    id_usuario_logado = grafoBuscarNome(g,nome);
    if(id_usuario_logado == -1) printf("Usuário inexistente\n");
    else printf("Usuário logado com sucesso\n\n");

    free(nome);
}

void logout(){
    id_usuario_logado = -1;
}

void titulo() {
    // Logo escrito em ascii. Disponível no site http://www.network-science.de/ascii/, fonte "smisome1"
    printf("   ___       ___       ___       ___       ___       ___            ___       ___                           \n");
    printf("  /\\  \\     /\\__\\     /\\  \\     /\\  \\     /\\  \\     /\\  \\          /\\__\\     /\\__\\          \n");
    printf(" _\\:\\  \\   /:| _|_   /::\\  \\   _\\:\\  \\   /::\\  \\   /::\\  \\        /:/ _/_   /::L_L_             \n");
    printf("/\\/::\\__\\ /::|/\\__\\ /\\:\\:\\__\\ /\\/::\\__\\ /::\\:\\__\\ /::\\:\\__\\      /:/_/\\__\\ /:/L:\\__\\  \n");
    printf("\\::/\\/__/ \\/|::/  / \\:\\:\\/__/ \\::/\\/__/ \\;:::/  / \\/\\::/  /      \\:\\/:/  / \\/_/:/  /          \n");
    printf(" \\:\\__\\     |:/  /   \\::/  /   \\:\\__\\    |:\\/__/    /:/  /        \\::/  /    /:/  /                \n");
    printf("  \\/__/     \\/__/     \\/__/     \\/__/     \\|__|     \\/__/          \\/__/     \\/__/                  \n");
    printf("                          ___       ___       ___       ___                                                 \n");
    printf("                         /\\__\\     /\\  \\     /\\__\\     /\\  \\                                        \n");
    printf("                        /:| _|_   /::\\  \\   /::L_L_   /::\\  \\                                           \n");
    printf("                       /::|/\\__\\ /:/\\:\\__\\ /:/L:\\__\\ /::\\:\\__\\                                    \n");
    printf("                       \\/|::/  / \\:\\/:/  / \\/_/:/  / \\:\\:\\/  /                                       \n");
    printf("                         |:/  /   \\::/  /    /:/  /   \\:\\/  /                                            \n");
    printf("                         \\/__/     \\/__/     \\/__/     \\/__/                                            \n");
    printf("      ___       ___       ___       ___       ___       ___       ___       ___                             \n");
    printf("     /\\  \\     /\\  \\     /\\  \\     /\\  \\     /\\  \\     /\\  \\     /\\__\\     /\\  \\            \n");
    printf("    /::\\  \\   /::\\  \\   _\\:\\  \\   /::\\  \\    \\:\\  \\   _\\:\\  \\   /:/ _/_   /::\\  \\          \n");
    printf("   /:/\\:\\__\\ /::\\:\\__\\ /\\/::\\__\\ /::\\:\\__\\   /::\\__\\ /\\/::\\__\\ |::L/\\__\\ /:/\\:\\__\\    \n");
    printf("   \\:\\ \\/__/ \\;:::/  / \\::/\\/__/ \\/\\::/  /  /:/\\/__/ \\::/\\/__/ |::::/  / \\:\\/:/  /             \n");
    printf("    \\:\\__\\    |:\\/__/   \\:\\__\\     /:/  /   \\/__/     \\:\\__\\    L;;/__/   \\::/  /               \n");
    printf("     \\/__/     \\|__|     \\/__/     \\/__/               \\/__/               \\/__/                      \n\n");
    printf("====================================================================================\n\n");
}



void usuario_home_page(grafo *g) {
    if(!g) return;
    char opcao, *nome;
    while(true) {
        printf("1) Solicitações de amizade\n2) Pesquisar usuário\n3) Listar amizades\n4) Remover amigo\n5) Amizades fracas\n6) Recomendações de amigos\n7) Deslogar\n");
        printf(">>");
        scanf("%c%*c", &opcao);
        switch(opcao) {
            case '1':
                grafoListarSolicitacoes(g);
            break;
            case '2':
                printf(">>");
                scanf("%m[^\r\n]%*c", &nome);
                grafoBuscarTodosNomes(g, nome);
            break;
            case '3':
                grafoListarAmizades(g);
            break;
            case '4':
                grafoRemoverAmizades(g);
            break;
            case '5':
                grafoAmizadesIndevidas();
            break;
            case '6':
                grafoRecomendacoes();
            break;
            case '7':
                logout();
                grafoLogout();
                printf("Usuário deslogado com sucesso.\n");
                return;
            break;
            default:
                printf("Opção inválida\n");
        }
    }
}

void home_page(grafo *g) {
    if(!g) return;

    char opcao;
    while(true) {
        printf("1) Cadastrar\n2) Login\n3) Sair\n");
        printf(">>");
        scanf("%c%*c", &opcao);
        switch(opcao) {
            case '1':
                cadastrar(g);
            break;
            case '2':
                login(g);
                grafoLogin(g, id_usuario_logado);
                if(id_usuario_logado != -1)
                    usuario_home_page(g);
            break;
            case '3':
                system("tput reset");
                return;
            break;
            default:
                printf("Opção inválida\n");
        }
    }
}

int main(int argc, char const *argv[]) {
    titulo();
    grafo *g = grafoCriar();
    arquivo_ler(g);
    home_page(g);
    grafoApagar(g);

    return 0;
}
