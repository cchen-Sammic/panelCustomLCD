#ifndef CONTROLGUI_H
#define CONTROLGUI_H

#include <QMainWindow>
#include <QQuickWidget>
#include <QTimer>
//#include <QQuickView>

namespace Ui {
class controlGUI;
}

class controlGUI : public QMainWindow
{
    Q_OBJECT

public:
    explicit controlGUI(QWidget *parent = 0);
    ~controlGUI();

private:
    Ui::controlGUI *ui;
    QObject *m_currentRootObject;
    QTimer *updateTimer0;
    QTimer *updateTimer1;
    QTimer *botonParpadear;
    QTimer *LCDparpadear;
    QTimer *cicloTimer;
    int circRojoExt;
    int circBlancoInt;
    int tiempoMuertoEncendiendo;
    int progNum;
    int modeloNum;
    QString estado;
    QString estadoAnterior;
    QString tipoProg;
    QString setPulsado_2;
    QList<QString> progEnvasadora;
    QList<float> prog01;
    QList<float> prog02;
    QList<float> prog03;
    QList<float> prog04;
    QList<float> prog05;
    QList<float> progVAC;
    QList<float> progPaso;
    bool encendido;
    bool encendido2;
    bool enPausa;
    bool enStop;
    int horasOil;
    int mbarActual;
    int mbarRegistro;
    int pc_mbarActual;
    int pc_mbarRegistro;
    int t_vacPlus;
    int pc_gasRef;
    int t_sellar;
    int t_aire;
    int progNumNormal;

    int incr_mbarAire;
    bool desdeOtroSET;
    bool printerON;
    bool configON;

private:
    void connections();
    void mostrarLCD(QString tipo,bool mostrar);
    void mostrarBoton(QString tipo,bool mostrar);
    QString mostrarProgCentral(QString valor);
    void updateGestionSistema(QString l_estado, QString accion);
    int calculoPresionPorcent(int mbares);
    void gestionEstadoCiclo(QString donde);



private slots:
    void updateFuncCiclo();
    void enVACplusCiclo();
    void updateParpadearBoton();
    void updateParpadearLCD();
    void alimentacionGeneral();
    void estadoEncendiendo();

    void set_config();
    void set_prog();
    void set_botonArriba();
    void set_botonAbajo();
    void set_ok();
    void set_stop();
    void set_VAC();
    void set_VACplus();
    void set_gas();
    void set_sellar();
    void set_aire();
    void start_ciclo();

signals:


};

#endif // CONTROLGUI_H
