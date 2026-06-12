#ifndef LOJAS_H
#define LOJAS_H

#define MAX_LOJAS 100

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        char nome[100];
        char contato[30];
        char vendedora[50];
        char endereco[150];
        char cidade[60];
        char estado[3];
        char cepmg[100];
        char cep[10];
        double lat;
        double lng;
    } Loja;

    typedef struct
    {
        char nome[100];
        char uf[3];
        double latitude;
        double longitude;
    } Municipio;

    int buscarLojasPorNome(
        const char *nomeBusca,
        Loja resultados[]);

    int buscarLojasPorCEPMG(
        const char *cepmgBusca,
        Loja resultados[]);

    int buscarLojasPorLocal(
        const char *localBusca,
        Loja resultados[]);

    int buscarLojaMaisProximaPorMunicipio(
        const char *cidade,
        Loja resultado[]);

    void carregarMunicipios();
    
    int getTotalMunicipios();
    const Municipio *getMunicipio(
        int indice);

    extern Loja lojas[MAX_LOJAS];
    extern int totalLojas;

    void carregarLojas(const char *arquivo);
    void salvarLojas(const char *arquivo);
    void paraMinusculo(char *str);
    void removerAcentos(char *str);
    void buscarPorNome(const char *nomeBusca);
    void buscarPorCEPMG(const char *cepmgBusca);
    void listarLojas();
    void buscarPorLocal(const char *localBusca);
    void adicionarLoja();
    void removerLoja();
    void editarLoja();
    void lerCampoObrigatorio(char *campo, int tamanho, const char *mensagem);
    void limparBuffer();
    void gerarMapa();
    static void copiarStringSeguro(char *destino, const char *origem, size_t tamanho);

#ifdef __cplusplus
}
#endif

#endif