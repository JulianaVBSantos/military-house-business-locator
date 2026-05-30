#include "MainWindow.h"
#include <QAbstractItemView>
#include <QComboBox>
#include <cstring>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include <QPushButton>
#include <QLabel>
#include <QFrame>

#include <QTableWidget>
#include <QHeaderView>

#include <QLineEdit>

#include <QInputDialog>

#include <QMessageBox>

extern "C"
{
#include "../backend/lojas.h"
}

void MainWindow::atualizarTabela()
{
    tabela->setRowCount(totalLojas);

    for (int i = 0; i < totalLojas; i++)
    {
        tabela->setItem(i, 0,
                        new QTableWidgetItem(lojas[i].nome));

        tabela->setItem(i, 1,
                        new QTableWidgetItem(lojas[i].contato));

        tabela->setItem(i, 2,
                        new QTableWidgetItem(lojas[i].vendedora));

        tabela->setItem(i, 3,
                        new QTableWidgetItem(lojas[i].cidade));

        tabela->setItem(i, 4,
                        new QTableWidgetItem(lojas[i].estado));

        tabela->setItem(i, 5,
                        new QTableWidgetItem(lojas[i].cepmg));

        tabela->setItem(i, 6,
                        new QTableWidgetItem(lojas[i].cep));

        tabela->setItem(i, 7,
                        new QTableWidgetItem(lojas[i].endereco));
    }
}

void MainWindow::buscarLoja()
{
    QString texto = campoBusca->text();

    if (texto.isEmpty())
    {
        atualizarTabela();
        return;
    }

    Loja resultados[MAX_LOJAS];

    int encontrados = 0;

    QString tipo =
        tipoBusca->currentText();

    if (tipo == "Nome")
    {
        encontrados =
            buscarLojasPorNome(
                texto.toStdString().c_str(),
                resultados);
    }
    else if (tipo == "Colégio Militar")
    {
        encontrados =
            buscarLojasPorCEPMG(
                texto.toStdString().c_str(),
                resultados);
    }
    else if (tipo == "Local")
    {
        encontrados =
            buscarLojasPorLocal(
                texto.toStdString().c_str(),
                resultados);
    }

    tabela->setRowCount(encontrados);

    for (int i = 0; i < encontrados; i++)
    {
        tabela->setItem(i, 0,
                        new QTableWidgetItem(resultados[i].nome));

        tabela->setItem(i, 1,
                        new QTableWidgetItem(resultados[i].contato));

        tabela->setItem(i, 2,
                        new QTableWidgetItem(resultados[i].vendedora));

        tabela->setItem(i, 3,
                        new QTableWidgetItem(resultados[i].cidade));

        tabela->setItem(i, 4,
                        new QTableWidgetItem(resultados[i].estado));

        tabela->setItem(i, 5,
                        new QTableWidgetItem(resultados[i].cepmg));

        tabela->setItem(i, 6,
                        new QTableWidgetItem(resultados[i].cep));

        tabela->setItem(i, 7,
                        new QTableWidgetItem(resultados[i].endereco));
    }
}

void MainWindow::adicionarLojaQt()
{
    if (totalLojas >= MAX_LOJAS)
    {
        QMessageBox::warning(
            this,
            "Erro",
            "Limite de lojas atingido.");

        return;
    }

    Loja novaLoja;

    QString nome =
        QInputDialog::getText(
            this,
            "Adicionar Loja",
            "Nome da loja:");

    if (nome.isEmpty())
        return;

    QString contato =
        QInputDialog::getText(
            this,
            "Adicionar Loja",
            "Contato:");

    QString vendedora =
        QInputDialog::getText(
            this,
            "Adicionar Loja",
            "Vendedora:");

    QString endereco =
        QInputDialog::getText(
            this,
            "Adicionar Loja",
            "Endereço:");

    QString cidade =
        QInputDialog::getText(
            this,
            "Adicionar Loja",
            "Cidade:");

    QString estado =
        QInputDialog::getText(
            this,
            "Adicionar Loja",
            "Estado:");

    QString cepmg =
        QInputDialog::getText(
            this,
            "Adicionar Loja",
            "CEPMG:");

    QString cep =
        QInputDialog::getText(
            this,
            "Adicionar Loja",
            "CEP:");

    double lat =
        QInputDialog::getDouble(
            this,
            "Adicionar Loja",
            "Latitude:",
            0,
            -999999,
            999999,
            6);

    double lng =
        QInputDialog::getDouble(
            this,
            "Adicionar Loja",
            "Longitude:",
            0,
            -999999,
            999999,
            6);

    strncpy(
        novaLoja.nome,
        nome.toStdString().c_str(),
        sizeof(novaLoja.nome));

    strncpy(
        novaLoja.contato,
        contato.toStdString().c_str(),
        sizeof(novaLoja.contato));

    strncpy(
        novaLoja.vendedora,
        vendedora.toStdString().c_str(),
        sizeof(novaLoja.vendedora));

    strncpy(
        novaLoja.endereco,
        endereco.toStdString().c_str(),
        sizeof(novaLoja.endereco));

    strncpy(
        novaLoja.cidade,
        cidade.toStdString().c_str(),
        sizeof(novaLoja.cidade));

    strncpy(
        novaLoja.estado,
        estado.toStdString().c_str(),
        sizeof(novaLoja.estado));

    strncpy(
        novaLoja.cepmg,
        cepmg.toStdString().c_str(),
        sizeof(novaLoja.cepmg));

    strncpy(
        novaLoja.cep,
        cep.toStdString().c_str(),
        sizeof(novaLoja.cep));

    novaLoja.lat = lat;
    novaLoja.lng = lng;

    int resposta =
        QMessageBox::question(
            this,
            "Confirmar",
            "Deseja adicionar esta loja?");

    if (resposta != QMessageBox::Yes)
        return;

    lojas[totalLojas] = novaLoja;

    totalLojas++;

    salvarLojas("src/data/dados.json");

    atualizarTabela();

    QMessageBox::information(
        this,
        "Sucesso",
        "Loja adicionada com sucesso.");
}

void MainWindow::removerLojaQt()
{
    int linha = tabela->currentRow();

    if (linha < 0)
    {
        QMessageBox::warning(
            this,
            "Erro",
            "Selecione uma loja.");

        return;
    }

    int resposta =
        QMessageBox::question(
            this,
            "Remover",
            "Deseja remover esta loja?");

    if (resposta != QMessageBox::Yes)
        return;

    for (int i = linha; i < totalLojas - 1; i++)
    {
        lojas[i] = lojas[i + 1];
    }

    totalLojas--;

    salvarLojas("src/data/dados.json");

    atualizarTabela();
}

void MainWindow::editarLojaQt()
{
    int linha = tabela->currentRow();

    if (linha < 0)
    {
        QMessageBox::warning(
            this,
            "Erro",
            "Selecione uma loja.");

        return;
    }

    QString novoNome =
        QInputDialog::getText(
            this,
            "Editar Loja",
            "Novo nome:",
            QLineEdit::Normal,
            lojas[linha].nome);

    if (novoNome.isEmpty())
        return;

    strncpy(
        lojas[linha].nome,
        novoNome.toStdString().c_str(),
        sizeof(lojas[linha].nome));

    salvarLojas("src/data/dados.json");

    atualizarTabela();
}

MainWindow::MainWindow()
{
    resize(1200, 700);

    setWindowTitle(
        "Military House Business Locator");

    QWidget *central =
        new QWidget();

    setCentralWidget(central);

    QHBoxLayout *mainLayout =
        new QHBoxLayout();

    central->setLayout(mainLayout);

    // =========================
    // Menu lateral
    // =========================

    QFrame *menuLateral =
        new QFrame();

    menuLateral->setFixedWidth(220);

    QVBoxLayout *menuLayout =
        new QVBoxLayout();

    menuLateral->setLayout(menuLayout);

    QPushButton *btnBuscar =
        new QPushButton("Buscar Loja");

    QPushButton *btnAdicionar =
        new QPushButton("Adicionar Loja");

    QPushButton *btnEditar =
        new QPushButton("Editar Loja");

    QPushButton *btnRemover =
        new QPushButton("Remover Loja");

    QPushButton *btnMapa =
        new QPushButton("Abrir Mapa");

    menuLayout->addWidget(btnBuscar);

    menuLayout->addWidget(btnAdicionar);

    menuLayout->addWidget(btnEditar);

    menuLayout->addWidget(btnRemover);

    menuLayout->addWidget(btnMapa);

    menuLayout->addStretch();

    // =========================
    // Área conteúdo
    // =========================

    QFrame *areaConteudo =
        new QFrame();

    QVBoxLayout *conteudoLayout =
        new QVBoxLayout();

    areaConteudo->setLayout(
        conteudoLayout);

    QLabel *titulo =
        new QLabel(
            "Lojas cadastradas");

    conteudoLayout->addWidget(
        titulo);

    // =========================
    // Busca
    // =========================

    tipoBusca = new QComboBox();

    tipoBusca->addItem("Nome");
    tipoBusca->addItem("Colégio Militar");
    tipoBusca->addItem("Local");

    conteudoLayout->addWidget(
        tipoBusca);

    campoBusca =
        new QLineEdit();

    campoBusca->setPlaceholderText(
        "Digite sua busca...");

    conteudoLayout->addWidget(
        campoBusca);

    // =========================
    // Tabela
    // =========================

    // =========================
    // Tabela
    // =========================

    tabela =
        new QTableWidget();

    // impedir edição direta
    tabela->setEditTriggers(
        QAbstractItemView::NoEditTriggers);

    tabela->setColumnCount(8);

    QStringList colunas;

    colunas << "Nome"
            << "Contato"
            << "Vendedora"
            << "Cidade"
            << "Estado"
            << "CEPMG"
            << "CEP"
            << "Endereço";

    tabela->setHorizontalHeaderLabels(
        colunas);

    tabela->horizontalHeader()
        ->setSectionResizeMode(
            QHeaderView::Stretch);

    conteudoLayout->addWidget(
        tabela);

    // =========================
    // Carregar dados
    // =========================

    carregarLojas(
        "src/data/dados.json");

    atualizarTabela();

    // =========================
    // Conexões
    // =========================

    connect(btnBuscar,
            &QPushButton::clicked,
            this,
            &MainWindow::buscarLoja);

    connect(campoBusca,
            &QLineEdit::textChanged,
            this,
            &MainWindow::buscarLoja);

    connect(btnAdicionar,
            &QPushButton::clicked,
            this,
            &MainWindow::adicionarLojaQt);

    connect(btnRemover,
            &QPushButton::clicked,
            this,
            &MainWindow::removerLojaQt);

    connect(btnEditar,
            &QPushButton::clicked,
            this,
            &MainWindow::editarLojaQt);

    // =========================
    // Layout principal
    // =========================

    mainLayout->addWidget(
        menuLateral);

    mainLayout->addWidget(
        areaConteudo);
}