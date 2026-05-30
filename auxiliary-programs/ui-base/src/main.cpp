#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QMainWindow janela;

    janela.resize(800, 600);
    janela.show();

    return app.exec();
}