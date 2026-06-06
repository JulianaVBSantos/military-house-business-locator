#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QLineEdit>
#include <QComboBox>

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow();

private slots:
    void buscarLoja();
    void adicionarLojaQt();
    void removerLojaQt();
    void editarLojaQt();
    void abrirMapa();
    void restaurarDados();

private:
    QTableWidget *tabela;
    QLineEdit *campoBusca;
    QComboBox *tipoBusca;
    QString tipoSelecionado = "Nome";

    void atualizarTabela();
};

#endif