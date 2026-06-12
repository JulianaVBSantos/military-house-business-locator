#include "MainWindow.h"
#include <QPainter>
#include <QPixmap>
#include <QDialog>
#include <QRadioButton>
#include <QDialogButtonBox>
#include <QDesktopServices>
#include <QUrl>
#include <QAbstractItemView>
#include <QComboBox>
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
#include <cstring>
#include <QGraphicsOpacityEffect>
#include <QDir>
#include <QCoreApplication>
#include <QMenu>
#include <QAction>
#include <QStandardPaths>
#include <QDir>

extern "C"
{
#include "../backend/lojas.h"
}

#include <QStandardPaths>
#include <QDir>

static QString caminhoDados()
{
    QString pasta =
        QStandardPaths::writableLocation(
            QStandardPaths::AppDataLocation);

    QDir().mkpath(pasta);

    return pasta + "/dados.json";
}

static QString caminhoBackup()
{
    return QCoreApplication::applicationDirPath() + "/data/dados_backup.json";
}

QString normalizarTexto(QString texto)
{ // tratativa de acento
    texto = texto.toLower();

    texto.replace("á", "a");
    texto.replace("à", "a");
    texto.replace("â", "a");
    texto.replace("ã", "a");

    texto.replace("é", "e");
    texto.replace("ê", "e");

    texto.replace("í", "i");

    texto.replace("ó", "o");
    texto.replace("ô", "o");
    texto.replace("õ", "o");

    texto.replace("ú", "u");

    texto.replace("ç", "c");

    return texto;
}

void MainWindow::onTabelaContextMenu(const QPoint &pos)
{
    int row = tabela->rowAt(pos.y());
    if (row < 0)
        return;

    tabela->selectRow(row);

    QTableWidgetItem *item = tabela->itemAt(pos);
    if (!item)
        return;

    QMenu menu(this);

    QAction *acaoEditar = menu.addAction("✏️ Editar loja");
    QAction *acaoRemover = menu.addAction("🗑️ Remover loja");

    // controle de permissão (depois de criar as ações)
    acaoEditar->setEnabled(administrador);
    acaoRemover->setEnabled(administrador);

    QAction *selecionado =
        menu.exec(tabela->viewport()->mapToGlobal(pos));

    if (selecionado == acaoEditar)
    {
        editarLojaQt();
    }
    else if (selecionado == acaoRemover)
    {
        removerLojaQt();
    }
}

void MainWindow::restaurarDados()
{
    QMessageBox msg(this);
    msg.setWindowTitle("Restaurar Dados");
    msg.setText("Deseja restaurar todos os dados originais?\n\nTodas as alterações serão perdidas.");

    QPushButton *btnSim = msg.addButton("Sim", QMessageBox::YesRole);
    QPushButton *btnNao = msg.addButton("Não", QMessageBox::NoRole);

    msg.exec();

    if (msg.clickedButton() != btnSim)
        return;

    carregarLojas(
        caminhoBackup().toStdString().c_str());
    ::carregarMunicipios();

    salvarLojas(
        caminhoDados().toStdString().c_str());

    atualizarTabela();

    QMessageBox::information(
        this,
        "Sucesso",
        "Dados restaurados com sucesso.");
}

void MainWindow::abrirMapa()
{
    gerarMapa();

    QDesktopServices::openUrl(
        QUrl::fromLocalFile("mapa.html"));
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

        QTableWidgetItem *itemEndereco =
            new QTableWidgetItem(lojas[i].endereco);

        itemEndereco->setToolTip(
            lojas[i].endereco);

        tabela->setItem(i, 7,
                        itemEndereco);
    }

    tabela->clearSelection();
    tabela->setCurrentCell(-1, -1);
}

void MainWindow::buscarLoja()
{
    QString texto =
        normalizarTexto(
            campoBusca->text());

    if (texto.isEmpty())
    {
        atualizarTabela();
        return;
    }

    Loja resultados[MAX_LOJAS];

    int encontrados = 0;

    if (tipoSelecionado == "Nome")
    {
        encontrados =
            buscarLojasPorNome(
                texto.toStdString().c_str(),
                resultados);
    }
    else if (tipoSelecionado == "Colégio")
    {
        encontrados =
            buscarLojasPorCEPMG(
                texto.toStdString().c_str(),
                resultados);
    }
    else if (tipoSelecionado == "Local")
    {
        encontrados =
            buscarLojasPorLocal(
                texto.toStdString().c_str(),
                resultados);

        if (encontrados == 0)
        {
            encontrados =
                buscarLojaMaisProximaPorMunicipio(
                    texto.toStdString().c_str(),
                    resultados);
        }
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

        QTableWidgetItem *itemEndereco =
            new QTableWidgetItem(resultados[i].endereco);

        itemEndereco->setToolTip(
            resultados[i].endereco);

        tabela->setItem(i, 7,
                        itemEndereco);

        tabela->clearSelection();
        tabela->setCurrentCell(-1, -1);
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

    QMessageBox msg(this);
    msg.setWindowTitle("Confirmar");
    msg.setText("Deseja adicionar esta loja?");

    QPushButton *btnSim = msg.addButton("Sim", QMessageBox::YesRole);
    msg.addButton("Não", QMessageBox::NoRole);

    msg.exec();

    if (msg.clickedButton() != btnSim)
        return;

    lojas[totalLojas] = novaLoja;

    totalLojas++;

    salvarLojas(
        caminhoDados().toStdString().c_str());
    atualizarTabela();

    QMessageBox::information(
        this,
        "Sucesso",
        "Loja adicionada com sucesso.");
}

void MainWindow::removerLojaQt()
{
    QList<QTableWidgetSelectionRange> selecao =
        tabela->selectedRanges();

    if (selecao.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Erro",
            "Selecione uma loja.");

        return;
    }

    int linha = selecao.first().topRow();

    QMessageBox msg(this);
    msg.setWindowTitle("Remover");
    msg.setText("Deseja remover esta loja?");

    QPushButton *btnSim = msg.addButton("Sim", QMessageBox::YesRole);
    msg.addButton("Não", QMessageBox::NoRole);

    msg.exec();

    if (msg.clickedButton() != btnSim)
        return;

    for (int i = linha; i < totalLojas - 1; i++)
    {
        lojas[i] = lojas[i + 1];
    }

    totalLojas--;

    salvarLojas(
        caminhoDados().toStdString().c_str());

    atualizarTabela();

    tabela->clearSelection();
    tabela->setCurrentCell(-1, -1);

    QMessageBox::information(
        this,
        "Sucesso",
        "Loja removida com sucesso.");
}

void MainWindow::editarLojaQt()
{
    QList<QTableWidgetSelectionRange> selecao =
        tabela->selectedRanges();

    if (selecao.isEmpty())
    {
        QMessageBox::warning(
            this,
            "Erro",
            "Selecione uma loja.");

        return;
    }

    int linhaTabela =
        selecao.first().topRow();

    QString nomeSelecionado =
        tabela->item(linhaTabela, 0)->text();

    int linha = -1;

    for (int i = 0; i < totalLojas; i++)
    {
        if (nomeSelecionado ==
            lojas[i].nome)
        {
            linha = i;
            break;
        }
    }

    if (linha == -1)
    {
        QMessageBox::warning(
            this,
            "Erro",
            "Loja não encontrada.");

        return;
    }

    QStringList opcoes;

    opcoes << "Nome"
           << "Contato"
           << "Vendedora"
           << "Cidade"
           << "Estado"
           << "CEPMG"
           << "CEP"
           << "Endereço"
           << "Latitude"
           << "Longitude";

    bool ok;

    QString campo =
        QInputDialog::getItem(
            this,
            "Editar Campo",
            "Selecione o campo:",
            opcoes,
            0,
            false,
            &ok);

    if (!ok || campo.isEmpty())
        return;

    QString valorAtual;

    if (campo == "Nome")
        valorAtual = lojas[linha].nome;

    else if (campo == "Contato")
        valorAtual = lojas[linha].contato;

    else if (campo == "Vendedora")
        valorAtual = lojas[linha].vendedora;

    else if (campo == "Cidade")
        valorAtual = lojas[linha].cidade;

    else if (campo == "Estado")
        valorAtual = lojas[linha].estado;

    else if (campo == "CEPMG")
        valorAtual = lojas[linha].cepmg;

    else if (campo == "CEP")
        valorAtual = lojas[linha].cep;

    else if (campo == "Endereço")
        valorAtual = lojas[linha].endereco;

    QString novoValor;

    if (campo == "Latitude")
    {
        double valor =
            QInputDialog::getDouble(
                this,
                "Editar Latitude",
                "Nova latitude:",
                lojas[linha].lat,
                -999999,
                999999,
                6,
                &ok);

        if (!ok)
            return;

        QMessageBox msg(this);
        msg.setWindowTitle("Confirmar");
        msg.setText("Deseja alterar a latitude?");

        QPushButton *btnSim = msg.addButton("Sim", QMessageBox::YesRole);
        msg.addButton("Não", QMessageBox::NoRole);

        msg.exec();

        if (msg.clickedButton() != btnSim)
            return;

        lojas[linha].lat = valor;
    }
    else if (campo == "Longitude")
    {
        double valor =
            QInputDialog::getDouble(
                this,
                "Editar Longitude",
                "Nova longitude:",
                lojas[linha].lng,
                -999999,
                999999,
                6,
                &ok);

        if (!ok)
            return;

        QMessageBox msg(this);
        msg.setWindowTitle("Confirmar");
        msg.setText("Deseja alterar a longitude?");

        QPushButton *btnSim = msg.addButton("Sim", QMessageBox::YesRole);
        msg.addButton("Não", QMessageBox::NoRole);

        msg.exec();

        if (msg.clickedButton() != btnSim)
            return;

        lojas[linha].lng = valor;
    }
    else
    {
        novoValor =
            QInputDialog::getText(
                this,
                "Editar Campo",
                "Novo valor:",
                QLineEdit::Normal,
                valorAtual,
                &ok);

        if (!ok || novoValor.isEmpty())
            return;

        QMessageBox msg(this);
        msg.setWindowTitle("Confirmar");
        msg.setText("Deseja salvar a alteração?");

        QPushButton *btnSim = msg.addButton("Sim", QMessageBox::YesRole);
        msg.addButton("Não", QMessageBox::NoRole);

        msg.exec();

        if (msg.clickedButton() != btnSim)
            return;

        if (campo == "Nome")
            strncpy(lojas[linha].nome,
                    novoValor.toStdString().c_str(),
                    sizeof(lojas[linha].nome));

        else if (campo == "Contato")
            strncpy(lojas[linha].contato,
                    novoValor.toStdString().c_str(),
                    sizeof(lojas[linha].contato));

        else if (campo == "Vendedora")
            strncpy(lojas[linha].vendedora,
                    novoValor.toStdString().c_str(),
                    sizeof(lojas[linha].vendedora));

        else if (campo == "Cidade")
            strncpy(lojas[linha].cidade,
                    novoValor.toStdString().c_str(),
                    sizeof(lojas[linha].cidade));

        else if (campo == "Estado")
            strncpy(lojas[linha].estado,
                    novoValor.toStdString().c_str(),
                    sizeof(lojas[linha].estado));

        else if (campo == "CEPMG")
            strncpy(lojas[linha].cepmg,
                    novoValor.toStdString().c_str(),
                    sizeof(lojas[linha].cepmg));

        else if (campo == "CEP")
            strncpy(lojas[linha].cep,
                    novoValor.toStdString().c_str(),
                    sizeof(lojas[linha].cep));

        else if (campo == "Endereço")
            strncpy(lojas[linha].endereco,
                    novoValor.toStdString().c_str(),
                    sizeof(lojas[linha].endereco));
    }

    salvarLojas(
        caminhoDados().toStdString().c_str());

    atualizarTabela();

    tabela->clearSelection();
    tabela->setCurrentCell(-1, -1);

    QMessageBox::information(
        this,
        "Sucesso",
        "Dados atualizados.");
}

MainWindow::MainWindow() // construtor
{
    QString pastaApp = QCoreApplication::applicationDirPath();

    administrador = false;

    while (true)
    {
        QDialog perfilDialog(this);

        perfilDialog.setWindowTitle("Selecionar Perfil");
        perfilDialog.resize(350, 220);

        QVBoxLayout *perfilLayout =
            new QVBoxLayout(&perfilDialog);

        QLabel *tituloPerfil =
            new QLabel("Selecione o perfil de acesso");

        tituloPerfil->setAlignment(Qt::AlignCenter);

        tituloPerfil->setStyleSheet(
            "font-size: 18px;"
            "font-weight: bold;"
            "color: #7f1d1d;"
            "margin-bottom: 15px;");

        QRadioButton *radioAdmin =
            new QRadioButton("Perfil Gestor");

        QRadioButton *radioUsuario =
            new QRadioButton("Perfil Consulta");

        radioUsuario->setChecked(true);

        perfilLayout->addStretch();

        perfilLayout->addWidget(
            tituloPerfil,
            0,
            Qt::AlignCenter);

        perfilLayout->addSpacing(20);

        perfilLayout->addWidget(
            radioAdmin,
            0,
            Qt::AlignCenter);

        perfilLayout->addWidget(
            radioUsuario,
            0,
            Qt::AlignCenter);

        perfilLayout->addStretch();

        QDialogButtonBox *botoes =
            new QDialogButtonBox(
                QDialogButtonBox::Ok |
                QDialogButtonBox::Cancel);

        perfilLayout->addWidget(botoes);

        connect(botoes,
                &QDialogButtonBox::accepted,
                &perfilDialog,
                &QDialog::accept);

        connect(botoes,
                &QDialogButtonBox::rejected,
                &perfilDialog,
                &QDialog::reject);

        if (perfilDialog.exec() != QDialog::Accepted)
        {
            exit(0);
        }

        if (radioUsuario->isChecked())
        {
            administrador = false;
            break;
        }

        bool ok;

        QString senha =
            QInputDialog::getText(
                this,
                "Senha Administrador",
                "Digite a senha:",
                QLineEdit::Password,
                "",
                &ok);

        if (!ok)
        {
            continue;
        }

        if (senha == "admin123")
        {
            administrador = true;
            break;
        }

        QMessageBox::warning(
            this,
            "Acesso negado",
            "Senha incorreta.");
    }

    resize(1450, 800);

    setWindowTitle(
        "Localizador Comercial Casa Do Militar");

    QWidget *central =
        new QWidget();

    setCentralWidget(central);

    QHBoxLayout *mainLayout =
        new QHBoxLayout();

    mainLayout->setContentsMargins(0, 0, 0, 0);

    central->setLayout(mainLayout);

    setStyleSheet(

        "QMainWindow {"
        "background-color: #e4e1e1;"
        "}"

        "QFrame#menu {"
        "background-color: #e4e1e1;"
        "}"

        "QPushButton {"
        "background-color: #B30000;"
        "color: #FFFFFF;"
        "border: none;"
        "border-radius: 10px;"
        "padding: 12px;"
        "font-size: 14px;"
        "font-weight: bold;"
        "}"

        "QPushButton:hover {"
        "background-color: #CC0000;"
        "}"

        "QPushButton:pressed {"
        "background-color: #800020;"
        "}"

        "QPushButton:disabled {"
        "background-color: #555555;"
        "color: #AAAAAA;"
        "}"

        "QLabel {"
        "font-size: 15px;"
        "font-weight: bold;"
        "color: #111827;"
        "}"

        "QMessageBox QLabel {"
        "color: #111827;"
        "}"

        "QInputDialog QLabel {"
        "color: #111827;"
        "}"

        "#tituloEmpresa {"
        "color: #800020;"
        "font-size: 18px;"
        "font-weight: bold;"
        "padding-bottom: 10px;"
        "}");

    QFrame *menuLateral =
        new QFrame();

    menuLateral->setObjectName("menu");

    menuLateral->setFixedWidth(260);

    QVBoxLayout *menuLayout =
        new QVBoxLayout();

    menuLayout->setSpacing(10);

    menuLateral->setLayout(menuLayout);

    QLabel *logoImagem = new QLabel();

    logoImagem->setPixmap(
        QPixmap(":/images/imagens/logo.png") // caminho correto no Qt Resource System
            .scaled(
                140,
                140,
                Qt::KeepAspectRatio,
                Qt::SmoothTransformation));

    logoImagem->setAlignment(Qt::AlignCenter);

    menuLayout->addWidget(logoImagem);

    QLabel *tituloEmpresa =
        new QLabel("CASA DO MILITAR");

    tituloEmpresa->setAlignment(Qt::AlignCenter);

    tituloEmpresa->setObjectName("tituloEmpresa");

    menuLayout->addWidget(tituloEmpresa);

    QPushButton *btnAdicionar =
        new QPushButton("➕ Adicionar Loja");

    QPushButton *btnEditar =
        new QPushButton("✏️ Editar Loja");

    QPushButton *btnRemover =
        new QPushButton("🗑️ Remover Loja");

    QPushButton *btnMapa =
        new QPushButton("🗺️ Abrir Mapa");

    QPushButton *btnRestaurar =
        new QPushButton("⟳ Restaurar Dados");

    if (!administrador)
    {
        btnAdicionar->setEnabled(false);
        btnEditar->setEnabled(false);
        btnRemover->setEnabled(false);
        btnRestaurar->setEnabled(false);
    }

    menuLayout->addWidget(btnAdicionar);
    menuLayout->addWidget(btnEditar);
    menuLayout->addWidget(btnRemover);
    menuLayout->addWidget(btnMapa);
    menuLayout->addWidget(btnRestaurar);

    menuLayout->addStretch();

    QLabel *perfilAtual =
        new QLabel(
            administrador
                ? "Perfil: Gestor"
                : "Perfil: Consulta");

    perfilAtual->setStyleSheet(
        "color: #800020;"
        "padding: 15px;"
        "font-size: 15px;"
        "font-weight: bold;");

    menuLayout->addWidget(
        perfilAtual,
        0,
        Qt::AlignCenter);

    QFrame *areaConteudo =
        new QFrame();

    QVBoxLayout *conteudoLayout =
        new QVBoxLayout();

    conteudoLayout->setContentsMargins(
        20, 20, 20, 20);

    conteudoLayout->setSpacing(15);

    areaConteudo->setLayout(
        conteudoLayout);

    QLabel *titulo =
        new QLabel(
            "LOJAS CADASTRADAS");

    QLabel *assinatura =
        new QLabel(
            "Developed by Juliana V. B. Santos\n     All rights reserved © 2026");

    perfilAtual->setAlignment(Qt::AlignCenter);

    assinatura->setAlignment(Qt::AlignCenter);

    assinatura->setStyleSheet(
        "color: #800020;"
        "padding-bottom: 5px;"
        "font-size: 11px;");

    menuLayout->addWidget(assinatura);

    titulo->setStyleSheet(
        "font-size: 26px;"
        "font-weight: bold;"
        "color: #7f1d1d;");

    conteudoLayout->addWidget(
        titulo);

    QHBoxLayout *layoutBuscaTipo =
        new QHBoxLayout();

    auto makeButton = [](const QString &text, const QString &iconPath)
    {
        QPushButton *btn = new QPushButton(text);

        QIcon icon(iconPath);

        btn->setIcon(icon);
        btn->setIconSize(QSize(32, 32));
        btn->setMinimumHeight(55);
        btn->setCheckable(true);

        return btn;
    };

    QPushButton *btnNome =
        makeButton("Nome da Loja",
                   ":/icons/icons/search.png");

    QPushButton *btnColegio =
        makeButton("Colégio Militar",
                   ":/icons/icons/school.png");

    QPushButton *btnLocal =
        makeButton("Local",
                   ":/icons/icons/pin.png");

    btnNome->setChecked(true);

    QString estiloBusca =

        "QPushButton {"
        "background-color: white;"
        "color: #7f1d1d;"
        "border: 2px solid #991b1b;"
        "border-radius: 14px;"
        "font-size: 17px;"
        "font-weight: bold;"
        "padding: 14px;"
        "}"

        "QPushButton:hover {"
        "background-color: #fef2f2;"
        "}"

        "QPushButton:checked {"
        "background-color: #991b1b;"
        "color: white;"
        "}";

    btnNome->setStyleSheet(estiloBusca);
    btnColegio->setStyleSheet(estiloBusca);
    btnLocal->setStyleSheet(estiloBusca);

    layoutBuscaTipo->addWidget(btnNome);
    layoutBuscaTipo->addWidget(btnColegio);
    layoutBuscaTipo->addWidget(btnLocal);

    conteudoLayout->addLayout(
        layoutBuscaTipo);

    campoBusca =
        new QLineEdit();

    campoBusca->setPlaceholderText(
        "Digite uma cidade, loja ou CEPMG...");

    campoBusca->setMinimumHeight(45);

    campoBusca->setStyleSheet(

        "QLineEdit {"
        "background-color: white;"
        "border: 2px solid #991b1b;"
        "border-radius: 12px;"
        "padding-left: 15px;"
        "font-size: 15px;"
        "color: #111827;"
        "}"

        "QLineEdit:focus {"
        "border: 2px solid #dc2626;"
        "}");

    conteudoLayout->addWidget(
        campoBusca);

    tabela =
        new QTableWidget();

    tabela->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(tabela, &QTableWidget::customContextMenuRequested,
            this, &MainWindow::onTabelaContextMenu);

    tabela->setEditTriggers(
        QAbstractItemView::NoEditTriggers);

    tabela->setAlternatingRowColors(true);

    tabela->setShowGrid(false);

    tabela->setWordWrap(true);

    tabela->setSelectionBehavior(
        QAbstractItemView::SelectRows);
    tabela->setSelectionMode(
        QAbstractItemView::SingleSelection);

    tabela->setStyleSheet(

        "QTableWidget {"
        "background: white;"
        "selection-background-color: #991b1b;"
        "selection-color: white;"
        "}"

        "QTableWidget::item:selected {"
        "background: #991b1b;"
        "color: white;"
        "}"

        "QTableWidget::item:hover {"
        "background: #991b1b;"
        "color: white;"
        "}");

    tabela->verticalHeader()
        ->setVisible(false);

    tabela->verticalHeader()
        ->setDefaultSectionSize(38);

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

    tabela->horizontalHeader()->setStyleSheet(
        "QHeaderView::section {"
        "background-color: #800020;"
        "color: white;"
        "font-weight: 900;"
        "font-size: 15px;"
        "padding: 10px;"
        "border: 1px solid #991b1b;"
        "}");

    tabela->horizontalHeader()
        ->setSectionResizeMode(
            QHeaderView::Stretch);

    tabela->horizontalHeader()
        ->setSectionResizeMode(
            7,
            QHeaderView::ResizeToContents);

    conteudoLayout->addWidget(
        tabela);

    connect(
        tabela,
        &QTableWidget::cellDoubleClicked,
        this,
        [this](int row, int)
        {
            QString texto;

            texto += "Nome: " + tabela->item(row, 0)->text() + "\n\n";

            texto += "Contato: " + tabela->item(row, 1)->text() + "\n\n";

            texto += "Vendedora: " + tabela->item(row, 2)->text() + "\n\n";

            texto += "Cidade: " + tabela->item(row, 3)->text() + "\n\n";

            texto += "Estado: " + tabela->item(row, 4)->text() + "\n\n";

            texto += "CEPMG: " + tabela->item(row, 5)->text() + "\n\n";

            texto += "CEP: " + tabela->item(row, 6)->text() + "\n\n";

            texto += "Endereço: " + tabela->item(row, 7)->text();

            QMessageBox msg(this);

            msg.setWindowTitle(
                "Detalhes da Loja");

            msg.setText(texto);

            msg.exec();
        });

    QString arquivoDados = caminhoDados();

    if (!QFile::exists(arquivoDados))
    {
        QFile::copy(
            caminhoBackup(),
            arquivoDados);
    }

    carregarLojas(
        arquivoDados.toStdString().c_str());

    ::carregarMunicipios();

    atualizarTabela();

    tabela->clearSelection();

    atualizarTabela();

    tabela->clearSelection();

    ::carregarMunicipios();

    atualizarTabela();

    connect(campoBusca,
            &QLineEdit::textChanged,
            this,
            &MainWindow::buscarLoja);

    connect(btnAdicionar,
            &QPushButton::clicked,
            this,
            &MainWindow::adicionarLojaQt);

    connect(btnEditar,
            &QPushButton::clicked,
            this,
            &MainWindow::editarLojaQt);

    connect(btnRemover,
            &QPushButton::clicked,
            this,
            &MainWindow::removerLojaQt);

    connect(btnMapa,
            &QPushButton::clicked,
            this,
            &MainWindow::abrirMapa);

    connect(btnRestaurar,
            &QPushButton::clicked,
            this,
            &MainWindow::restaurarDados);

    connect(btnNome,
            &QPushButton::clicked,
            this,
            [this, btnNome, btnColegio, btnLocal]()
            {
                tipoSelecionado = "Nome";

                btnNome->setChecked(true);
                btnColegio->setChecked(false);
                btnLocal->setChecked(false);

                buscarLoja();
            });

    connect(btnColegio,
            &QPushButton::clicked,
            this,
            [this, btnNome, btnColegio, btnLocal]()
            {
                tipoSelecionado = "Colégio";

                btnNome->setChecked(false);
                btnColegio->setChecked(true);
                btnLocal->setChecked(false);

                buscarLoja();
            });

    connect(btnLocal,
            &QPushButton::clicked,
            this,
            [this, btnNome, btnColegio, btnLocal]()
            {
                tipoSelecionado = "Local";

                btnNome->setChecked(false);
                btnColegio->setChecked(false);
                btnLocal->setChecked(true);

                buscarLoja();
            });

    mainLayout->addWidget(
        menuLateral);

    mainLayout->addWidget(
        areaConteudo);
}