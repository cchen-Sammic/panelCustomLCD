#include "controlgui.h"
#include "ui_controlgui.h"
#include <QQuickItem>
#include <QDebug>
#include <QDateTime>
//#include <QGlobal.h>


controlGUI::controlGUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::controlGUI)
{
    ui->setupUi(this);
    estado = "apagado";
    QUrl source("qrc:widget/dial.qml");
    ui->dialQuickWidget->setSource(source);
    m_currentRootObject = ui->dialQuickWidget->rootObject();
    updateTimer0 = new QTimer(this); //actualización de inicio
    updateTimer1 = new QTimer(this); // actualización en ciclo secundario: enVACplusCiclo
    LCDparpadear = new QTimer(this);
    botonParpadear = new QTimer(this);
    cicloTimer = new QTimer(this);
    tipoProg="prog_normal";
    ///               p1    p2  p3   pVAC pPas pDryOil
    progEnvasadora<<"p1"<<"p2"<< "p3"<<"v"<<"s"<<"";
    ///prog vac vac+ gas sellar aire
    prog01<<93<<10<<15<<3.2<<5;
    prog02<<92<<2<<0<<3<<10;
    prog03<<40<<3<<30<<2.2<<3;
    prog04<<60<<0<<30<<4.1<<5;
    prog05<<84<<3<<0<<5<<6;
    progVAC<<94<<0<<0<<0<<0;
    progPaso<<97<<5<<20<<3.5<<6;

    progNum = 0;
    progNumNormal = 0;
    ui->valor_prog->setText(progEnvasadora[progNum]);
    circRojoExt = 0; //0
    circBlancoInt = 0; //0
    tiempoMuertoEncendiendo =0;
    enPausa = false;
    encendido = true;
    encendido2 = false;
    desdeOtroSET = false;
    printerON = true;
    configON=false;

    mostrarLCD("todo",false);
    mostrarBoton("todo",false);
    //    ui->icon_dryOil->setVisible(false);
    horasOil = (QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()).right(3)).toInt();
    modeloNum = (QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()).right(1)).toInt(); if(modeloNum==0){modeloNum=1;}
    int incrMbarInicial =(QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()).right(2)).toInt();
    mbarActual = 960+ incrMbarInicial;
    ui->valor_mbar->setNum(mbarActual);

    connections();
}
controlGUI::~controlGUI()
{
    delete ui;
}

void controlGUI::connections(){
    connect(ui->boton_interruptor,SIGNAL(clicked()),this,SLOT(alimentacionGeneral()));
    connect(updateTimer0,SIGNAL(timeout()),this,SLOT(estadoEncendiendo()));
    connect(updateTimer1,SIGNAL(timeout()),this,SLOT(enVACplusCiclo()));
    connect(botonParpadear,SIGNAL(timeout()),this,SLOT(updateParpadearBoton()));
    connect(ui->boton_envasadora,SIGNAL(clicked()),this,SLOT(start_ciclo()));
    connect(cicloTimer,SIGNAL(timeout()),this,SLOT(updateFuncCiclo()));
    connect(LCDparpadear,SIGNAL(timeout()),this,SLOT(updateParpadearLCD()));
    connect(ui->boton_conf,SIGNAL(clicked()),this,SLOT(set_config()));
    connect(ui->boton_prog,SIGNAL(clicked()),this,SLOT(set_prog()));
    connect(ui->boton_arriba,SIGNAL(clicked()),this,SLOT(set_botonArriba()));
    connect(ui->boton_abajo,SIGNAL(clicked()),this,SLOT(set_botonAbajo()));
    connect(ui->boton_ok,SIGNAL(clicked()),this,SLOT(set_ok()));
    connect(ui->boton_stop,SIGNAL(clicked()),this,SLOT(set_stop()));
    connect(ui->boton_vac,SIGNAL(clicked()),this,SLOT(set_VAC()));
    connect(ui->boton_vacPlus,SIGNAL(clicked()),this,SLOT(set_VACplus()));
    connect(ui->boton_gas,SIGNAL(clicked()),this,SLOT(set_gas()));
    connect(ui->boton_sellar,SIGNAL(clicked()),this,SLOT(set_sellar()));
    connect(ui->boton_aire,SIGNAL(clicked()),this,SLOT(set_aire()));

}

void controlGUI::mostrarLCD(QString tipo, bool mostrar){
    if(tipo == "todo"){
        ui->icon_caducidad->setVisible(mostrar);
        ui->icon_dryOil->setVisible(mostrar);
        ui->icon_h2o->setVisible(mostrar);
        ui->icon_mbar->setVisible(mostrar);
        ui->icon_oil->setVisible(mostrar);
        ui->icon_paso->setVisible(mostrar);
        ui->icon_pausa->setVisible(mostrar);
        ui->icon_pc1->setVisible(mostrar);
        ui->icon_pc2->setVisible(mostrar);
        ui->icon_pcCentral->setVisible(mostrar);
        ui->icon_printer->setVisible(mostrar);
        ui->icon_s1->setVisible(mostrar);
        ui->icon_s2->setVisible(mostrar);
        ui->icon_s3->setVisible(mostrar);
        ui->icon_sCentral->setVisible(mostrar);
        ui->icon_temperatura->setVisible(mostrar);
        ui->icon_ticket->setVisible(mostrar);
        ui->icon_vac->setVisible(mostrar);
        ui->valor_central->setVisible(mostrar);
        ui->valor_gas->setVisible(mostrar);
        ui->valor_mbar->setVisible(mostrar);
        ui->valor_nombre->setVisible(mostrar);
        ui->valor_prog->setVisible(mostrar);
        ui->valor_taire->setVisible(mostrar);
        ui->valor_tsellar->setVisible(mostrar);
        ui->valor_tvac->setVisible(mostrar);
        ui->valor_vac->setVisible(mostrar);

        ui->dialQuickWidget->setVisible(mostrar);
    }
    else if(tipo == "reinicio"){
        mostrar=!mostrar;
        ui->icon_caducidad->setVisible(mostrar);
        ui->icon_dryOil->setVisible(mostrar);
        ui->icon_h2o->setVisible(mostrar);
        ui->icon_mbar->setVisible(mostrar);
        ui->icon_oil->setVisible(mostrar);
        ui->icon_paso->setVisible(mostrar);
        ui->icon_pausa->setVisible(mostrar);
        ui->icon_pc1->setVisible(mostrar);
        ui->icon_pc2->setVisible(mostrar);
        ui->icon_pcCentral->setVisible(mostrar);
        ui->icon_printer->setVisible(mostrar);
        ui->icon_s1->setVisible(mostrar);
        ui->icon_s2->setVisible(mostrar);
        ui->icon_s3->setVisible(mostrar);
        ui->icon_sCentral->setVisible(mostrar);
        ui->icon_temperatura->setVisible(mostrar);
        ui->icon_ticket->setVisible(mostrar);
        ui->icon_vac->setVisible(mostrar);
        ui->valor_central->setVisible(mostrar);
        ui->valor_gas->setVisible(mostrar);
        ui->valor_mbar->setVisible(mostrar);
        ui->valor_nombre->setVisible(mostrar);
        ui->valor_prog->setVisible(mostrar);
        ui->valor_taire->setVisible(mostrar);
        ui->valor_tsellar->setVisible(mostrar);
        ui->valor_tvac->setVisible(mostrar);
        ui->valor_vac->setVisible(mostrar);
    }
    else if(tipo == "encendiendo"){
        //        if(horasOil<500){ui->icon_oil->setVisible(mostrar);}
        //        else ui->icon_dryOil->setVisible(!mostrar);
        ui->icon_oil->setVisible(mostrar);

        ui->valor_nombre->setVisible(mostrar);
        ui->dialQuickWidget->setVisible(mostrar);
        ui->valor_central->setVisible(mostrar);
    }
    else if(tipo == "setProg"){
        ui->icon_oil->setVisible(!mostrar);
        ui->icon_vac->setVisible(!mostrar);
        ui->icon_paso->setVisible(!mostrar);
        ui->icon_sCentral->setVisible(!mostrar);
        ui->icon_dryOil->setVisible(!mostrar);
        ui->icon_caducidad->setVisible(!mostrar);
        ui->icon_temperatura->setVisible(!mostrar);
        ui->icon_ticket->setVisible(!mostrar);

        ui->icon_pc1->setVisible(mostrar);
        ui->icon_pc2->setVisible(mostrar);
        ui->icon_s1->setVisible(mostrar);
        ui->icon_s2->setVisible(mostrar);
        ui->icon_s3->setVisible(mostrar);
        ui->icon_mbar->setVisible(mostrar);
        ui->valor_mbar->setVisible(mostrar);
        ui->valor_gas->setVisible(mostrar);
        ui->valor_prog->setVisible(mostrar);
        ui->valor_taire->setVisible(mostrar);
        ui->valor_tsellar->setVisible(mostrar);
        ui->valor_tvac->setVisible(mostrar);
        ui->valor_vac->setVisible(mostrar);
        ui->valor_central->setText(mostrarProgCentral(ui->valor_prog->text()));
        ui->valor_central->setVisible(mostrar);
        ui->valor_nombre->setText("sammic");
        ui->valor_nombre->setVisible(mostrar);
    }
    else if(tipo == "setProgVAC"){
        ui->icon_vac->setVisible(mostrar);
        ui->icon_pc1->setVisible(mostrar);
        ui->valor_prog->setVisible(mostrar);
        ui->icon_sCentral->setVisible(!mostrar);
        ui->icon_caducidad->setVisible(!mostrar);
        ui->icon_temperatura->setVisible(!mostrar);
        ui->icon_ticket->setVisible(!mostrar);

        ui->icon_paso->setVisible(!mostrar);
        ui->icon_pc2->setVisible(!mostrar);
        ui->icon_s1->setVisible(!mostrar);
        ui->icon_s2->setVisible(!mostrar);
        ui->icon_s3->setVisible(!mostrar);
        ui->icon_mbar->setVisible(mostrar);
        ui->valor_mbar->setVisible(mostrar);
        ui->valor_gas->setVisible(!mostrar);
        ui->valor_taire->setVisible(!mostrar);
        ui->valor_tsellar->setVisible(!mostrar);
        ui->valor_tvac->setVisible(!mostrar);
    }
    else if(tipo == "setProgPaso"){
        ui->icon_vac->setVisible(!mostrar);
        ui->icon_sCentral->setVisible(!mostrar);
        ui->icon_caducidad->setVisible(!mostrar);
        ui->icon_temperatura->setVisible(!mostrar);
        ui->icon_ticket->setVisible(!mostrar);

        ui->icon_paso->setVisible(mostrar);
        ui->icon_pc2->setVisible(mostrar);
        ui->icon_s1->setVisible(mostrar);
        ui->icon_s2->setVisible(mostrar);
        ui->icon_s3->setVisible(mostrar);
        ui->icon_mbar->setVisible(mostrar);
        ui->valor_mbar->setVisible(mostrar);
        ui->valor_gas->setVisible(mostrar);
        ui->valor_taire->setVisible(mostrar);
        ui->valor_tsellar->setVisible(mostrar);
        ui->valor_tvac->setVisible(mostrar);
    }
    else if(tipo =="setProgDryOil"){
        ui->icon_vac->setVisible(!mostrar);
        ui->icon_sCentral->setVisible(!mostrar);
        ui->icon_paso->setVisible(!mostrar);
        ui->icon_pc1->setVisible(!mostrar);
        ui->icon_pc2->setVisible(!mostrar);
        ui->icon_s1->setVisible(!mostrar);
        ui->icon_s2->setVisible(!mostrar);
        ui->icon_s3->setVisible(!mostrar);
        ui->icon_mbar->setVisible(!mostrar);
        ui->valor_vac->setVisible(!mostrar);
        ui->valor_mbar->setVisible(!mostrar);
        ui->valor_gas->setVisible(!mostrar);
        ui->valor_taire->setVisible(!mostrar);
        ui->valor_tsellar->setVisible(!mostrar);
        ui->valor_tvac->setVisible(!mostrar);

        ui->icon_dryOil->setVisible(mostrar);

    }
    else if(tipo =="setConfig"){
        ui->icon_oil->setVisible(!mostrar);
        ui->icon_vac->setVisible(!mostrar);
        ui->icon_paso->setVisible(!mostrar);
        ui->icon_sCentral->setVisible(!mostrar);
        ui->icon_dryOil->setVisible(!mostrar);
        ui->icon_pc1->setVisible(!mostrar);
        ui->icon_pc2->setVisible(!mostrar);
        ui->icon_s1->setVisible(!mostrar);
        ui->icon_s2->setVisible(!mostrar);
        ui->icon_s3->setVisible(!mostrar);
        ui->icon_mbar->setVisible(!mostrar);
        ui->valor_mbar->setVisible(!mostrar);
        ui->valor_gas->setVisible(!mostrar); 
        ui->valor_taire->setVisible(!mostrar);
        ui->valor_tsellar->setVisible(!mostrar);
        ui->valor_tvac->setVisible(!mostrar);
        ui->valor_vac->setVisible(!mostrar);
        ui->valor_central->setVisible(!mostrar);
//        ui->valor_nombre->setText("sammic");
        ui->valor_prog->setVisible(mostrar);
        ui->valor_nombre->setVisible(mostrar);
        ui->icon_printer->setVisible(mostrar);
        ui->icon_caducidad->setVisible(mostrar);
        ui->icon_temperatura->setVisible(mostrar);
        ui->icon_ticket->setVisible(mostrar);
    }
    else if(tipo == "enVAC"){
        ui->valor_vac->setVisible(mostrar);
        ui->icon_pc1->setVisible(mostrar);
        ui->valor_tvac->setVisible(!mostrar);
        ui->icon_s1->setVisible(!mostrar);
        ui->valor_gas->setVisible(!mostrar);
        ui->icon_pc2->setVisible(!mostrar);
        ui->valor_tsellar->setVisible(!mostrar);
        ui->icon_s2->setVisible(!mostrar);
        ui->valor_taire->setVisible(!mostrar);
        ui->icon_s3->setVisible(!mostrar);

        ui->icon_pcCentral->setVisible(mostrar);
        ui->icon_sCentral->setVisible(!mostrar);
        ui->valor_mbar->setVisible(mostrar);
        ui->icon_mbar->setVisible(mostrar);
    }
    else if(tipo == "enVACplus"){
        ui->valor_vac->setVisible(mostrar);
        ui->icon_pc1->setVisible(mostrar);
        ui->valor_tvac->setVisible(mostrar);
        ui->icon_s1->setVisible(mostrar);
        ui->valor_gas->setVisible(!mostrar);
        ui->icon_pc2->setVisible(!mostrar);
        ui->valor_tsellar->setVisible(!mostrar);
        ui->icon_s2->setVisible(!mostrar);
        ui->valor_taire->setVisible(!mostrar);
        ui->icon_s3->setVisible(!mostrar);

        ui->icon_pcCentral->setVisible(!mostrar);
        ui->icon_sCentral->setVisible(mostrar);
    }
    else if(tipo == "enGas"){
        ui->valor_vac->setVisible(mostrar);
        ui->icon_pc1->setVisible(mostrar);
        ui->valor_tvac->setVisible(mostrar);
        ui->icon_s1->setVisible(mostrar);
        ui->valor_gas->setVisible(mostrar);
        ui->icon_pc2->setVisible(mostrar);
        ui->valor_tsellar->setVisible(!mostrar);
        ui->icon_s2->setVisible(!mostrar);
        ui->valor_taire->setVisible(!mostrar);
        ui->icon_s3->setVisible(!mostrar);

        ui->icon_pcCentral->setVisible(mostrar);
        ui->icon_sCentral->setVisible(!mostrar);
    }
    else if(tipo == "enSellar"){
        ui->valor_vac->setVisible(mostrar);
        ui->icon_pc1->setVisible(mostrar);
        ui->valor_tvac->setVisible(mostrar);
        ui->icon_s1->setVisible(mostrar);
        ui->valor_gas->setVisible(mostrar);
        ui->icon_pc2->setVisible(mostrar);
        ui->valor_tsellar->setVisible(mostrar);
        ui->icon_s2->setVisible(mostrar);
        ui->valor_taire->setVisible(!mostrar);
        ui->icon_s3->setVisible(!mostrar);

        ui->icon_pcCentral->setVisible(!mostrar);
        ui->icon_sCentral->setVisible(mostrar);
    }
    else if(tipo == "enAire"){
        ui->valor_vac->setVisible(mostrar);
        ui->icon_pc1->setVisible(mostrar);
        ui->valor_tvac->setVisible(mostrar);
        ui->icon_s1->setVisible(mostrar);
        ui->valor_gas->setVisible(mostrar);
        ui->icon_pc2->setVisible(mostrar);
        ui->valor_tsellar->setVisible(mostrar);
        ui->icon_s2->setVisible(mostrar);
        ui->valor_taire->setVisible(mostrar);
        ui->icon_s3->setVisible(mostrar);

        ui->icon_pcCentral->setVisible(!mostrar);
        ui->icon_sCentral->setVisible(mostrar);
    }
}
void controlGUI::mostrarBoton(QString tipo, bool mostrar){
    if(tipo =="todo"){
        ui->boton_prog->setVisible(mostrar);
        ui->boton_conf->setVisible(mostrar);
        ui->boton_stop->setVisible(mostrar);
        ui->boton_arriba->setVisible(mostrar);
        ui->boton_ok->setVisible(mostrar);
        ui->boton_abajo->setVisible(mostrar);
        ui->boton_vac->setVisible(mostrar);
        ui->boton_vacPlus->setVisible(mostrar);
        ui->boton_gas->setVisible(mostrar);
        ui->boton_sellar->setVisible(mostrar);
        ui->boton_aire->setVisible(mostrar);
    }
    else if(tipo=="setProg"){
        ui->boton_prog->setVisible(mostrar);
        ui->boton_conf->setVisible(mostrar);
        ui->boton_stop->setVisible(mostrar);
        ui->boton_arriba->setVisible(mostrar);
        ui->boton_ok->setVisible(mostrar);
        ui->boton_abajo->setVisible(mostrar);
        ui->boton_vac->setVisible(mostrar);
        ui->boton_vacPlus->setVisible(mostrar);
        ui->boton_gas->setVisible(mostrar);
        ui->boton_sellar->setVisible(mostrar);
        ui->boton_aire->setVisible(mostrar);
    }
    else if(tipo=="setProgVAC"){
        ui->boton_prog->setVisible(mostrar);
        ui->boton_conf->setVisible(mostrar);
        ui->boton_stop->setVisible(mostrar);
        ui->boton_arriba->setVisible(mostrar);
        ui->boton_ok->setVisible(mostrar);
        ui->boton_abajo->setVisible(mostrar);
        ui->boton_vac->setVisible(mostrar);
        ui->boton_vacPlus->setVisible(!mostrar);
        ui->boton_gas->setVisible(!mostrar);
        ui->boton_sellar->setVisible(!mostrar);
        ui->boton_aire->setVisible(!mostrar);
    }
    else if(tipo=="setProgPaso"){
        ui->boton_prog->setVisible(mostrar);
        ui->boton_conf->setVisible(mostrar);
        ui->boton_stop->setVisible(mostrar);
        ui->boton_arriba->setVisible(mostrar);
        ui->boton_ok->setVisible(mostrar);
        ui->boton_abajo->setVisible(mostrar);
        ui->boton_vac->setVisible(mostrar);
        ui->boton_vacPlus->setVisible(mostrar);
        ui->boton_gas->setVisible(mostrar);
        ui->boton_sellar->setVisible(mostrar);
        ui->boton_aire->setVisible(mostrar);
    }
    else if(tipo=="enVAC"){
        ui->boton_vac->setVisible(mostrar);
        ui->boton_vacPlus->setVisible(!mostrar);
        ui->boton_gas->setVisible(!mostrar);
        ui->boton_sellar->setVisible(!mostrar);
        ui->boton_aire->setVisible(!mostrar);
    }
    else if(tipo=="enVACplus"){
        ui->boton_vac->setVisible(!mostrar);
        ui->boton_vacPlus->setVisible(mostrar);
        ui->boton_gas->setVisible(!mostrar);
        ui->boton_sellar->setVisible(!mostrar);
        ui->boton_aire->setVisible(!mostrar);
    }
    else if(tipo=="enGas"){
        ui->boton_vac->setVisible(!mostrar);
        ui->boton_vacPlus->setVisible(!mostrar);
        ui->boton_gas->setVisible(mostrar);
        ui->boton_sellar->setVisible(!mostrar);
        ui->boton_aire->setVisible(!mostrar);
    }
    else if(tipo=="enSellar"){
        ui->boton_vac->setVisible(!mostrar);
        ui->boton_vacPlus->setVisible(!mostrar);
        ui->boton_gas->setVisible(!mostrar);
        ui->boton_sellar->setVisible(mostrar);
        ui->boton_aire->setVisible(!mostrar);
    }
    else if(tipo=="enAire"){
        ui->boton_vac->setVisible(!mostrar);
        ui->boton_vacPlus->setVisible(!mostrar);
        ui->boton_gas->setVisible(!mostrar);
        ui->boton_sellar->setVisible(!mostrar);
        ui->boton_aire->setVisible(mostrar);
    }
    else if(tipo=="todoSET"){
        ui->boton_vac->setVisible(mostrar);
        ui->boton_vacPlus->setVisible(mostrar);
        ui->boton_gas->setVisible(mostrar);
        ui->boton_sellar->setVisible(mostrar);
        ui->boton_aire->setVisible(mostrar);
    }
}

void controlGUI::set_config(){
//    qDebug()<<"en config-   parametro estado:"<<estado;
    if(estado=="setProg" || estado=="setProgVAC" || estado=="setProgPaso"){
        if(ui->boton_conf->isChecked()){
            estadoAnterior = estado;
            estado="setConfig";
            qDebug()<<"boton pulsado config     estoy en estado:"<<estado;
            configON=true;
            mostrarBoton("todoSET",false);
            mostrarLCD(estado,true);
            ui->valor_nombre->setText("25.02.2015");
//            botonParpadear->start(800);
            if(!printerON){ //si está apagada
                qDebug()<<"configuracion de impresora activada:"<<printerON;
                estado="setPrinter";
                LCDparpadear->start(400);
                ui->icon_caducidad->setVisible(printerON);
                ui->icon_temperatura->setVisible(printerON);
                ui->icon_ticket->setVisible(printerON);
            }
            else if(printerON){
                qDebug()<<"configuracion de impresora activada:"<<printerON;
                estado="setCaducidad";
                LCDparpadear->start(400);
//                ui->icon_caducidad->setVisible(printerON);
//                ui->icon_temperatura->setVisible(printerON);
//                ui->icon_ticket->setVisible(printerON);
            }
        }
    }
    else if(configON){ //salir modo configuracion
        estado = estadoAnterior;
        mostrarBoton(estado,true);
        mostrarLCD(estado,true);
        configON=false;
        LCDparpadear->stop();
        botonParpadear->stop();
        ui->boton_conf->setVisible(true);
    }
}
void controlGUI::set_prog(){
    if(tipoProg=="prog_normal"){
        tipoProg="prog_vac";
        estado="setProgVAC";
        ui->valor_nombre->setText("sammic");
        progNum=3;
    }
    else if(tipoProg=="prog_vac"){
        tipoProg="prog_paso";
        estado="setProgPaso";
        ui->valor_nombre->setText("sammic");
        progNum=4;
    }
    else if(tipoProg=="prog_paso"){
        tipoProg="prog_dryOil";
        estado="setProgDryOil";
        ui->valor_nombre->setText(QString("%1 h").arg(horasOil));
        progNum=5;
    }
    else if(tipoProg=="prog_dryOil"){
        tipoProg="prog_normal";
        estado="setProg";
        ui->valor_nombre->setText("sammic");
        progNum=progNumNormal;
    }
    qDebug()<<"tipo de progrma:"<<tipoProg<<"   progNum:"<<progNum;
    ui->valor_prog->setText(progEnvasadora[progNum]);
    ui->valor_central->setText(mostrarProgCentral(progEnvasadora[progNum]));
    if(true){
        if(progNum==0){
            //            estado="setProg";
            mostrarLCD(estado,true);
            mostrarBoton(estado,true);
            ui->valor_vac->setNum(prog01[0]);
            ui->valor_tvac->setNum(prog01[1]);
            ui->valor_gas->setNum(prog01[2]);
            ui->valor_tsellar->setNum(prog01[3]);
            ui->valor_taire->setNum(prog01[4]);
        }
        else if(progNum==1){
            //            estado="setProg";
            mostrarLCD(estado,true);
            mostrarBoton(estado,true);
            ui->valor_vac->setNum(prog02[0]);
            ui->valor_tvac->setNum(prog02[1]);
            ui->valor_gas->setNum(prog02[2]);
            ui->valor_tsellar->setNum(prog02[3]);
            ui->valor_taire->setNum(prog02[4]);
        }
        else if(progNum==2){
            //            estado="setProg";
            mostrarLCD(estado,true);
            mostrarBoton(estado,true);
            ui->valor_vac->setNum(prog03[0]);
            ui->valor_tvac->setNum(prog03[1]);
            ui->valor_gas->setNum(prog03[2]);
            ui->valor_tsellar->setNum(prog03[3]);
            ui->valor_taire->setNum(prog03[4]);
        }
        else if(progNum==3){
            //            estado="setProgVAC";
            mostrarLCD(estado,true);
            mostrarBoton(estado,true);
            ui->valor_vac->setNum(progVAC[0]);
            ui->valor_tvac->setNum(progVAC[1]);
            ui->valor_gas->setNum(progVAC[2]);
            ui->valor_tsellar->setNum(progVAC[3]);
            ui->valor_taire->setNum(progVAC[4]);
        }
        else if(progNum==4){
            //            estado="setProgPaso";
            mostrarLCD(estado,true);
            mostrarBoton(estado,true);
            ui->valor_vac->setNum(progPaso[0]);
            ui->valor_tvac->setNum(progPaso[1]);
            ui->valor_gas->setNum(progPaso[2]);
            ui->valor_tsellar->setNum(progPaso[3]);
            ui->valor_taire->setNum(progPaso[4]);
        }
        else if(progNum==5){
            //            estado="setProgPaso";
            mostrarLCD("setProgDryOil",true);
            mostrarBoton("todoSET",false);
            ui->valor_vac->setNum(progPaso[0]);
            ui->valor_tvac->setNum(progPaso[1]);
            ui->valor_gas->setNum(progPaso[2]);
            ui->valor_tsellar->setNum(progPaso[3]);
            ui->valor_taire->setNum(progPaso[4]);
        }
    }
}
void controlGUI::set_botonAbajo(){
    qDebug()<<estado<<"boton pulsado MENOS";
    updateGestionSistema(estado,"menos");

}
void controlGUI::set_botonArriba(){
    qDebug()<<estado<<"boton pulsado MAS";
    updateGestionSistema(estado,"mas");
}
void controlGUI::set_ok(){
    qDebug()<<"boton pulsado: OK   estado actual:"<<estado;
    ui->valor_nombre->setText("sammic");
    if(estado=="setValorVAC"){
        botonParpadear->stop();
        ui->boton_vac->setVisible(true);
        ui->icon_pcCentral->setVisible(false);
        ui->valor_vac->setNum(ui->valor_central->text().toInt());
    }
    else if(estado=="setValorVACplus"){
        botonParpadear->stop();
        ui->boton_vacPlus->setVisible(true);
        ui->icon_sCentral->setVisible(false);
        ui->valor_tvac->setText(ui->valor_central->text());
    }
    else if(estado=="setValorGas"){
        botonParpadear->stop();
        ui->boton_gas->setVisible(true);
        ui->icon_pcCentral->setVisible(false);
        ui->valor_gas->setNum(ui->valor_central->text().toInt());
    }
    else if(estado=="setValorSellar"){
        botonParpadear->stop();
        ui->boton_sellar->setVisible(true);
        ui->icon_sCentral->setVisible(false);
        ui->valor_tsellar->setText(ui->valor_central->text());
    }
    else if(estado=="setValorAire"){
        botonParpadear->stop();
        ui->boton_aire->setVisible(true);
        ui->icon_sCentral->setVisible(false);
        ui->valor_taire->setText(ui->valor_central->text());
    }

    if(desdeOtroSET==false){
        updateGestionSistema(estado,"ok");
    }
    else if(desdeOtroSET==true){
        qDebug()<<"\t->Ok por boton set";
        desdeOtroSET=false;
        if(setPulsado_2=="botonVAC"){
            ui->boton_vac->click();
        }
        else if(setPulsado_2=="botonVACplus"){
            ui->boton_vacPlus->click();
        }
        else if(setPulsado_2=="botonGas"){
            ui->boton_gas->click();
        }
        else if(setPulsado_2=="botonSellar"){
            ui->boton_sellar->click();
        }
        else if(setPulsado_2=="botonAire"){
            ui->boton_aire->click();
        }
        else if(setPulsado_2=="botonBajarTapa"){
            ui->boton_envasadora->setChecked(false);
            ui->boton_envasadora->click();
        }
    }
    if(estado=="enVAC"){
        enPausa = !enPausa; //el primer enPausa convertir a true
        qDebug()<<"envasadora en:"<<estado<<"  pausa pulsado:"<<enPausa;
        if(enPausa== true){
            ui->icon_pausa->setVisible(true);
            LCDparpadear->start(400);
            updateGestionSistema(estado,"pausa");
        }
        else if(enPausa == false){
            ui->icon_pausa->setVisible(false);
            ui->valor_central->setVisible(true);
            LCDparpadear->stop();
            updateGestionSistema(estado,"marcha");
        }
        else{
            if(progNum==0){estado = "setProg";}
            else if(progNum==1){estado = "setProg";}
            else if(progNum==2){estado = "setProg";}
            else if(progNum==3){estado = "setProgVAC";}
            else if(progNum==4){estado = "setProgPaso";}
        }
    }
}
void controlGUI::set_stop(){
    if(estado =="setValorVAC" || estado =="setValorVACplus" || estado =="setValorGas" || estado == "setValorSellar" || estado =="setValorAire" ||estado=="setProg" || estado=="setProgVAC" || estado=="setProgPaso"){
        qDebug()<<"no responderá STOP";
    }
    else {
        qDebug()<<"boton pulsado STOP, parar ciclo";
        ui->icon_pausa->setVisible(false);
        updateGestionSistema(estado,"stop");
    }

}
void controlGUI::set_VAC(){
    qDebug()<<"boton pulsado VAC    estoy en"<<estado;
    if(estado=="setProg" || estado=="setProgVAC" || estado=="setProgPaso"){
        ui->valor_central->setNum(ui->valor_vac->text().toInt());
        ui->icon_pcCentral->setVisible(true);
        mostrarBoton("todoSET",true);
        ui->valor_nombre->setText("vac");
        botonParpadear->start(500);

        estado = "setValorVAC";
        updateGestionSistema(estado,"VAC");
    }
    else if(estado =="setValorVACplus" || estado =="setValorGas" || estado == "setValorSellar" || estado =="setValorAire"){
        desdeOtroSET = true;
        setPulsado_2 = "botonVAC";
        ui->boton_ok->click();
    }
}
void controlGUI::set_VACplus(){
    qDebug()<<"boton pulsado VACplus    estoy en"<<estado;
    if(estado=="setProg" || estado=="setProgVAC" || estado=="setProgPaso"){
        ui->valor_central->setNum(ui->valor_tvac->text().toInt());
        ui->icon_sCentral->setVisible(true);
        mostrarBoton("todoSET",true);
        ui->valor_nombre->setText("vac plus");
        botonParpadear->start(500);

        estado = "setValorVACplus";
        updateGestionSistema(estado,"VACplus");
    }
    else if(estado =="setValorVAC" || estado =="setValorGas" || estado == "setValorSellar" || estado =="setValorAire"){
        desdeOtroSET = true;
        setPulsado_2 = "botonVACplus";
        ui->boton_ok->click();
    }
}
void controlGUI::set_gas(){
    qDebug()<<"boton pulsado GAS    estoy en"<<estado;
    if(estado=="setProg" || estado=="setProgVAC" || estado=="setProgPaso"){
        ui->valor_central->setNum(ui->valor_gas->text().toInt());
        ui->icon_pcCentral->setVisible(true);
        mostrarBoton("todoSET",true);
        ui->valor_nombre->setText("gas");
        botonParpadear->start(500);

        estado = "setValorGas";
        updateGestionSistema(estado,"gas");
    }
    else if(estado =="setValorVAC" || estado =="setValorVACplus" || estado == "sestValorSellar" || estado =="setValorAire"){
        desdeOtroSET = true;
        setPulsado_2 = "botonGas";
        ui->boton_ok->click();
    }
}
void controlGUI::set_sellar(){
    qDebug()<<"boton pulsado SELLAR    estoy en"<<estado;
    if(estado=="setProg" || estado=="setProgVAC" || estado=="setProgPaso"){
        ui->valor_central->setText(ui->valor_tsellar->text());
        ui->icon_sCentral->setVisible(true);
        mostrarBoton("todoSET",true);
        ui->valor_nombre->setText("sellar");
        botonParpadear->start(500);

        estado = "setValorSellar";
        updateGestionSistema(estado,"sellar");
    }
    else if(estado =="setValorVAC" || estado =="setValorGas" || estado == "setValorVACplus" || estado =="setValorAire"){
        desdeOtroSET = true;
        setPulsado_2 = "botonSellar";
        ui->boton_ok->click();
    }
}
void controlGUI::set_aire(){
    qDebug()<<"boton pulsado AIRE    estoy en"<<estado;
    if(estado=="setProg" || estado=="setProgVAC" || estado=="setProgPaso"){
        ui->valor_central->setText(ui->valor_taire->text());
        ui->icon_sCentral->setVisible(true);
        mostrarBoton("todoSET",true);
        ui->valor_nombre->setText("aire");
        botonParpadear->start(500);

        estado = "setValorAire";
        updateGestionSistema(estado,"aire");
    }
    else if(estado =="setValorVAC" || estado =="setValorGas" || estado == "setValorSellar" || estado =="setValorVACplus"){
        desdeOtroSET = true;
        setPulsado_2 = "botonAire";
        ui->boton_ok->click();
    }

}

void controlGUI::alimentacionGeneral(){
    if(ui->boton_interruptor->isChecked()){ ///ENCENDER MAQUINA
        ui->valor_nombre->setText(QString("%1 h").arg(horasOil));
        ui->boton_envasadora->setIcon(QIcon(":/boton/imagen/envasadora abiertaON.png"));
        ui->valor_central->setNum(modeloNum);
        circBlancoInt = 0;
        circRojoExt = 100;
        m_currentRootObject->setProperty("valorCirInterior", circBlancoInt);
        m_currentRootObject->setProperty("valorCirExterior", circRojoExt);
        estado = "encendiendo";
        mostrarBoton("todo",true);
        mostrarLCD(estado,true);

        ///////////////////////////////
        bool animacionEncendiendo = true;
        if(animacionEncendiendo){
            updateTimer0->start(100);
        }
        else if(!animacionEncendiendo){
            estado="setProg";
            mostrarLCD(estado,true);
            mostrarBoton(estado,true);
            if(true){ //pantalla principal
                if(progNum==0){
                    estado="setProg";
                    mostrarLCD(estado,true);
                    mostrarBoton(estado,true);
                    ui->valor_vac->setNum(prog01[0]);
                    ui->valor_tvac->setNum(prog01[1]);
                    ui->valor_gas->setNum(prog01[2]);
                    ui->valor_tsellar->setText(QString::number(prog01[3]));
                    ui->valor_taire->setNum(prog01[4]);
                }
                else if(progNum==1){
                    estado="setProg";
                    mostrarLCD(estado,true);
                    mostrarBoton(estado,true);
                    ui->valor_vac->setNum(prog02[0]);
                    ui->valor_tvac->setNum(prog02[1]);
                    ui->valor_gas->setNum(prog02[2]);
                    ui->valor_tsellar->setText(QString::number(prog02[3]));
                    ui->valor_taire->setNum(prog02[4]);
                }
                else if(progNum==2){
                    estado="setProg";
                    mostrarLCD(estado,true);
                    mostrarBoton(estado,true);
                    ui->valor_vac->setNum(prog03[0]);
                    ui->valor_tvac->setNum(prog03[1]);
                    ui->valor_gas->setNum(prog03[2]);
                    ui->valor_tsellar->setText(QString::number(prog03[3]));
                    ui->valor_taire->setNum(prog03[4]);
                }
                else if(progNum==3){
                    estado="setProgVAC";
                    mostrarLCD(estado,true);
                    mostrarBoton(estado,true);
                    ui->valor_vac->setNum(progVAC[0]);
                    ui->valor_tvac->setNum(progVAC[1]);
                    ui->valor_gas->setNum(progVAC[2]);
                    ui->valor_tsellar->setText(QString::number(progVAC[3]));
                    ui->valor_taire->setNum(progVAC[4]);
                }
                else if(progNum==4){
                    estado="setProgPaso";
                    mostrarLCD(estado,true);
                    mostrarBoton(estado,true);
                    ui->valor_vac->setNum(progPaso[0]);
                    ui->valor_tvac->setNum(progPaso[1]);
                    ui->valor_gas->setNum(progPaso[2]);
                    ui->valor_tsellar->setText(QString::number(progPaso[3]));
                    ui->valor_taire->setNum(progPaso[4]);
                }
            }
        }
        ///////////////////////////////
    }
    else if(!ui->boton_interruptor->isChecked()){ ///APAGAR MAQUINA
        //        qApp->exit();
        ui->boton_envasadora->setIcon(QIcon(":/boton/imagen/envasadora abiertaOFF.png"));
        mostrarBoton("todo",false);
        mostrarLCD("reinicio",true);
        m_currentRootObject->setProperty("valorCirExterior", 100);
        m_currentRootObject->setProperty("valorCirInterior", 0);
        horasOil = (QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()).right(3)).toInt();
        int incrMbarInicial =(QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()).right(2)).toInt();
        mbarActual = 910+ incrMbarInicial;

    }
}

void controlGUI::estadoEncendiendo(){
    //updateTimer0
    int incBlanco = 7;
    int incRojo = 5;
    //    qDebug()<<"blanco:"<<circBlancoInt<<" rojo:"<<circRojoExt;
    if(circBlancoInt<40 && circRojoExt==100){
        circBlancoInt = circBlancoInt + incBlanco;
        m_currentRootObject->setProperty("valorCirInterior", circBlancoInt);
    }
    else if(circBlancoInt>=40 && circBlancoInt<100 && circRojoExt<=100 && circRojoExt>60){
        circBlancoInt = circBlancoInt + incBlanco;
        m_currentRootObject->setProperty("valorCirInterior", circBlancoInt);
        circRojoExt = circRojoExt - incRojo;
        m_currentRootObject->setProperty("valorCirExterior", circRojoExt);
    }
    else if(circBlancoInt<=100 && circBlancoInt>0 && circRojoExt<=60 && circRojoExt >=0){
        circBlancoInt = circBlancoInt - incBlanco;
        m_currentRootObject->setProperty("valorCirInterior", circBlancoInt);
        if(circRojoExt>0){
            circRojoExt = circRojoExt - incRojo;}
        else if(circRojoExt<=0){
            circRojoExt=0;
        }
        m_currentRootObject->setProperty("valorCirExterior", circRojoExt);
    }
    else {
        tiempoMuertoEncendiendo = tiempoMuertoEncendiendo +1;
        if(tiempoMuertoEncendiendo ==6){
            estado="setProg";
            mostrarLCD(estado,true);
            mostrarBoton(estado,true);
            tiempoMuertoEncendiendo=0;
            updateTimer0->stop();
            if(true){ //pantalla principal
                ui->valor_mbar->setNum(mbarActual);
                if(progNum==0){
                    estado="setProg";
                    mostrarLCD(estado,true);
                    mostrarBoton(estado,true);
                    ui->valor_vac->setNum(prog01[0]);
                    ui->valor_tvac->setNum(prog01[1]);
                    ui->valor_gas->setNum(prog01[2]);
                    ui->valor_tsellar->setText(QString::number(prog01[3]));
                    //                    ui->valor_tsellar->setNum(prog01[3]);
                    ui->valor_taire->setNum(prog01[4]);
                }
                else if(progNum==1){
                    estado="setProg";
                    mostrarLCD(estado,true);
                    mostrarBoton(estado,true);
                    ui->valor_vac->setNum(prog02[0]);
                    ui->valor_tvac->setNum(prog02[1]);
                    ui->valor_gas->setNum(prog02[2]);
                    ui->valor_tsellar->setText(QString::number(prog02[3]));
                    //                    ui->valor_tsellar->setNum(prog02[3]);
                    ui->valor_taire->setNum(prog02[4]);
                }
                else if(progNum==2){
                    estado="setProg";
                    mostrarLCD(estado,true);
                    mostrarBoton(estado,true);
                    ui->valor_vac->setNum(prog03[0]);
                    ui->valor_tvac->setNum(prog03[1]);
                    ui->valor_gas->setNum(prog03[2]);
                    ui->valor_tsellar->setText(QString::number(prog03[3]));
                    //ui->valor_tsellar->setNum(prog03[3]);
                    ui->valor_taire->setNum(prog03[4]);
                }
                else if(progNum==3){
                    estado="setProgVAC";
                    mostrarLCD(estado,true);
                    mostrarBoton(estado,true);
                    ui->valor_vac->setNum(progVAC[0]);
                    ui->valor_tvac->setNum(progVAC[1]);
                    ui->valor_gas->setNum(progVAC[2]);
                    ui->valor_tsellar->setText(QString::number(progVAC[3]));
                    // ui->valor_tsellar->setNum(progVAC[3]);
                    ui->valor_taire->setNum(progVAC[4]);
                }
                else if(progNum==4){
                    estado="setProgPaso";
                    mostrarLCD(estado,true);
                    mostrarBoton(estado,true);
                    ui->valor_vac->setNum(progPaso[0]);
                    ui->valor_tvac->setNum(progPaso[1]);
                    ui->valor_gas->setNum(progPaso[2]);
                    ui->valor_tsellar->setText(QString::number(progPaso[3]));
                    //                    ui->valor_tsellar->setNum(progPaso[3]);
                    ui->valor_taire->setNum(progPaso[4]);
                }
            }
        }
    }
}

void controlGUI::start_ciclo(){
    qDebug()<<"iniciar ciclo"<<ui->boton_envasadora->isChecked();
    if(ui->boton_envasadora->isChecked()){
        if(estado=="setProg" || estado=="setProgVAC" || estado=="setProgPaso"){
            qDebug()<<"Ciclo para el programa:"<<estado;
            ui->boton_envasadora->setIcon(QIcon(":/boton/imagen/envasadora cerrado.png"));
            estado="enVAC";
            gestionEstadoCiclo(estado);
        }
        else {
            if(estado =="setValorVAC" || estado =="setValorGas" || estado == "setValorSellar" || estado =="setValorVACplus"){
                desdeOtroSET = true;
                setPulsado_2 = "botonBajarTapa";
                ui->boton_ok->click();
            }
        }
    }
    else if(!ui->boton_envasadora->isChecked()){
        if(estado =="finCiclo"){
            ui->boton_envasadora->setIcon(QIcon(":/boton/imagen/envasadora abiertaON.png"));
            ui->valor_mbar->setNum(mbarActual);
            if(true){ //pantalla principal
                if(progNum==0){
                    estado="setProg";
                    mostrarLCD(estado,true);
                    mostrarBoton(estado,true);
                    ui->valor_vac->setNum(prog01[0]);
                    ui->valor_tvac->setNum(prog01[1]);
                    ui->valor_gas->setNum(prog01[2]);
                    ui->valor_tsellar->setText(QString::number(prog01[3]));
                    //                    ui->valor_tsellar->setNum(prog01[3]);
                    ui->valor_taire->setNum(prog01[4]);
                }
                else if(progNum==1){
                    estado="setProg";
                    mostrarLCD(estado,true);
                    mostrarBoton(estado,true);
                    ui->valor_vac->setNum(prog02[0]);
                    ui->valor_tvac->setNum(prog02[1]);
                    ui->valor_gas->setNum(prog02[2]);
                    ui->valor_tsellar->setText(QString::number(prog02[3]));
                    ui->valor_taire->setNum(prog02[4]);
                }
                else if(progNum==2){
                    estado="setProg";
                    mostrarLCD(estado,true);
                    mostrarBoton(estado,true);
                    ui->valor_vac->setNum(prog03[0]);
                    ui->valor_tvac->setNum(prog03[1]);
                    ui->valor_gas->setNum(prog03[2]);
                    ui->valor_tsellar->setText(QString::number(prog03[3]));
                    ui->valor_taire->setNum(prog03[4]);
                }
                else if(progNum==3){
                    estado="setProgVAC";
                    mostrarLCD(estado,true);
                    mostrarBoton(estado,true);
                    ui->valor_vac->setNum(progVAC[0]);
                    ui->valor_tvac->setNum(progVAC[1]);
                    ui->valor_gas->setNum(progVAC[2]);
                    ui->valor_tsellar->setText(QString::number(progVAC[3]));
                    ui->valor_taire->setNum(progVAC[4]);
                }
                else if(progNum==4){
                    estado="setProgPaso";
                    mostrarLCD(estado,true);
                    mostrarBoton(estado,true);
                    ui->valor_vac->setNum(progPaso[0]);
                    ui->valor_tvac->setNum(progPaso[1]);
                    ui->valor_gas->setNum(progPaso[2]);
                    ui->valor_tsellar->setText(QString::number(progPaso[3]));
                    ui->valor_taire->setNum(progPaso[4]);
                }
            }
        }
        qDebug()<<"mi esatado actual"<<estado;
    }
}

void controlGUI::gestionEstadoCiclo(QString donde){
    qDebug()<<"[* *] gestionEstadoCiclo:"<<ui->boton_envasadora->isChecked()<<" donde:"<<donde;
    if(donde=="enVAC"){
        ui->valor_nombre->setText("vac");
        mostrarBoton(donde,true);
        mostrarLCD(donde,true);
        cicloTimer->start(15);
    }
    else if(donde=="enVACplus"){
        mostrarBoton(donde,true);
        mostrarLCD(donde,true);
        ui->valor_nombre->setText("vac plus");
        t_vacPlus= ui->valor_tvac->text().toInt();
        cicloTimer->start(1000);
        updateTimer1->start(400);
    }
    else if(donde == "enGas"){
        mostrarBoton(donde,true);
        mostrarLCD(donde,true);
        ui->valor_nombre->setText("gas");
        pc_mbarRegistro = calculoPresionPorcent(mbarActual);
        pc_gasRef= ui->valor_gas->text().toInt();
        //        qDebug()<<"presion registro:"<<pc_mbarRegistro<<"mbar   %gas ref:"<<pc_gasRef;
        cicloTimer->start(20);
    }
    else if(donde=="enSellar"){
        mostrarBoton(donde,true);
        mostrarLCD(donde,true);
        ui->valor_nombre->setText("sellar");
        QString string_tsellar =ui->valor_tsellar->text();
        if(string_tsellar.contains(",")){
            string_tsellar.replace(QString(","),QString("."));
            t_sellar=string_tsellar.toFloat()*10;
        }
        else t_sellar = string_tsellar.toFloat()*10;
        cicloTimer->start(100);
    }
    else if(donde=="enAire"){
        estado = "enAire";
        mostrarBoton(donde,true);
        mostrarLCD(donde,true);
        ui->valor_nombre->setText("aire");
        if(enStop==true){
            enStop=false;
            ui->valor_central->setVisible(false);
            ui->icon_sCentral->setVisible(false);
        }
        t_aire = ui->valor_taire->text().toInt();
        incr_mbarAire = ((1300-mbarActual*1)/t_aire)*0.8;
        qDebug()<<"seg aire:"<<t_aire<<" incr:"<<incr_mbarAire;
        if(incr_mbarAire<=0){
            incr_mbarAire=5;
        }
        pc_mbarActual = calculoPresionPorcent(mbarActual);
        cicloTimer->start(1000);
    }
    else if(donde=="finCiclo"){
        int incrMbarInicial =(QString::number(QDateTime::currentDateTime().toMSecsSinceEpoch()).right(2)).toInt();
        mbarActual = 960+ incrMbarInicial;
        ui->boton_envasadora->click();
    }
}

void controlGUI::updateFuncCiclo(){
    if(estado=="enVAC"){
        int incr_mbar = 1;
        int mbar_pc = calculoPresionPorcent(mbarActual);
        ui->valor_central->setNum(mbar_pc);
        ui->valor_mbar->setNum(mbarActual);

        circBlancoInt = mbar_pc;
        circRojoExt = mbar_pc;
        m_currentRootObject->setProperty("valorCirInterior", circBlancoInt);
        m_currentRootObject->setProperty("valorCirExterior", circRojoExt);

        if(mbar_pc==ui->valor_vac->text().toInt()){
            cicloTimer->stop();
            if(progNum!=3){ // si no esta en programa VAC exterior ...
                estado="enVACplus";
                gestionEstadoCiclo(estado);
            }else qDebug()<<"VAC exterior presion alcanzada";
        }
        if(mbarActual<15){
            cicloTimer->stop();
            if(progNum!=3){ // si no esta en programa VAC exterior ...
                estado="enVACplus";
                gestionEstadoCiclo(estado);
            }else qDebug()<<"VAC exterior presion alcanzada";
        }
        mbarActual = mbarActual - incr_mbar;

    }
    else if(estado=="enVACplus"){
        ui->valor_central->setNum(t_vacPlus);
        if(t_vacPlus==0){
            cicloTimer->stop();
            updateTimer1->stop();
            estado="enGas";
            gestionEstadoCiclo(estado);
        }
        int incr = 1;
        t_vacPlus = t_vacPlus - incr;
    }
    else if(estado=="enGas"){
        int mbar_pc = calculoPresionPorcent(mbarActual);
        int pc_gasActual = pc_mbarRegistro - mbar_pc;
        ui->valor_central->setNum(pc_gasActual);
        ui->valor_mbar->setNum(mbarActual);
        m_currentRootObject->setProperty("valorCirInterior", mbar_pc);
        m_currentRootObject->setProperty("valorCirExterior", mbar_pc);

        if(pc_gasActual>=pc_gasRef){
            cicloTimer->stop();
            estado="enSellar";
            gestionEstadoCiclo(estado);
        }
        int incr = 1;
        mbarActual = mbarActual + incr;
    }
    else if(estado=="enSellar"){
        QString nf = QString::number(t_sellar);
        if(nf.length()==2){
            nf=nf.insert(1,",");
        }
        else if(nf.length()==1){
            nf= nf.insert(0,"0,");
        }
        if(t_sellar>0){
            ui->valor_central->setText(nf);

        }
        else if(t_sellar<=0){
            t_sellar=0;
            ui->valor_central->setText(nf);
            cicloTimer->stop();
            estado="enAire";
            gestionEstadoCiclo(estado);
        }
        float incr = 1;
        t_sellar= t_sellar -incr;
    }
    else if(estado=="enAire"){
        int incr = 1;
        t_aire = t_aire - incr;
        if(t_aire>0){
            ui->valor_central->setNum(t_aire);
            ui->valor_mbar->setNum(mbarActual);
            if(pc_mbarActual<100){
                //               qDebug()<<"mbar actual:"<<mbarActual<<"+ incr:"<<incr_mbarAire<<"="<<mbarActual + incr_mbarAire;
                mbarActual = mbarActual + incr_mbarAire;
                pc_mbarActual= calculoPresionPorcent(mbarActual);
                //                qDebug()<<"->\t\t\t% mbar:"<<pc_mbarActual;
                if(pc_mbarActual<=0){
                    pc_mbarActual=0;
                }
                m_currentRootObject->setProperty("valorCirExterior", pc_mbarActual);
                m_currentRootObject->setProperty("valorCirInterior", pc_mbarActual);
            }
        }
        if(t_aire<=0 || pc_mbarActual==0){
            m_currentRootObject->setProperty("valorCirExterior", 0);
            m_currentRootObject->setProperty("valorCirInterior", 0);
            ui->valor_central->setNum(t_aire);
            cicloTimer->stop();
            estado="finCiclo";
            m_currentRootObject->setProperty("valorCirExterior", 0);
            gestionEstadoCiclo(estado);
        }
    }
}

void controlGUI::enVACplusCiclo(){
    ui->valor_mbar->setNum(mbarActual);
    int mbar_pc = calculoPresionPorcent(mbarActual);
    circBlancoInt = mbar_pc;
    circRojoExt = mbar_pc;
    m_currentRootObject->setProperty("valorCirInterior", circBlancoInt);
    m_currentRootObject->setProperty("valorCirExterior", circRojoExt);

    if(mbarActual==0){
        cicloTimer->stop();
        updateTimer1->stop();
        estado="enGas";
        gestionEstadoCiclo(estado);
    }
    int incr = 1;
    mbarActual = mbarActual - incr;

}

void controlGUI::updateGestionSistema(QString l_estado, QString accion){
    qDebug()<<"[*] updateGestionSitema - tipoProg:"<<tipoProg<<"   l_estado:"<<l_estado<<" accion:"<<accion;
   ///Acción STOP
    if(accion=="stop"){
        enStop= true;
        ui->valor_taire->setNum(3);
        gestionEstadoCiclo("enAire");
    }
    else if(l_estado =="enVAC" && accion=="pausa"){
        cicloTimer->stop();
    }
    else if(l_estado =="enVAC" && accion=="marcha"){
        gestionEstadoCiclo("enVAC");
    }
    ///Acción Función de programas
    if((l_estado=="setProg" || l_estado=="setProgVAC" || l_estado=="setProgPaso") && (accion=="mas" || accion=="menos")){
        if(tipoProg=="prog_normal"){
            if(accion=="mas"){
                progNum = progNum +1;
                if (progNum>=3){
                    progNum=0;
                }
                qDebug()<<"\t\tprog"<<progNum;
                ui->valor_prog->setText(progEnvasadora[progNum]);
                ui->valor_central->setText(mostrarProgCentral(progEnvasadora[progNum]));
            }
            else if(accion=="menos"){
                progNum = progNum -1;
                if (progNum<0){
                    progNum=2;
                }
                qDebug()<<"\t\tprog"<<progNum;
                ui->valor_prog->setText(progEnvasadora[progNum]);
                ui->valor_central->setText(mostrarProgCentral(progEnvasadora[progNum]));
            }
            progNumNormal = progNum;
        }
        else if(tipoProg=="prog_vac"){
            //            if(accion=="mas"){
            //                progNum = progNum +1;
            //                if (progNum>=3){
            //                    progNum=0;
            //                }
            //                progNum = 4;
            //                ui->valor_prog->setText(progEnvasadora[progNum]);
            //                ui->valor_central->setText(mostrarProgCentral(progEnvasadora[progNum]));
            //            }
            //            else if(accion=="menos"){
            //                progNum = progNum -1;
            //                if (progNum<0){
            //                    progNum=2;
            //                }
            progNum = 3;
            qDebug()<<"\t\tprog"<<progNum;
            ui->valor_prog->setText(progEnvasadora[progNum]);
            ui->valor_central->setText(mostrarProgCentral(progEnvasadora[progNum]));
        }
        else if(tipoProg=="prog_paso"){
            //            if(accion=="mas"){
            //                progNum = progNum +1;
            //                if (progNum>=3){
            //                    progNum=0;
            //                }
            //                progNum = 4;
            //                ui->valor_prog->setText(progEnvasadora[progNum]);
            //                ui->valor_central->setText(mostrarProgCentral(progEnvasadora[progNum]));
            //            }
            //            else if(accion=="menos"){
            //                progNum = progNum -1;
            //                if (progNum<0){
            //                    progNum=2;
            //                }
            progNum = 4;
            qDebug()<<"\t\tprog"<<progNum;
            ui->valor_prog->setText(progEnvasadora[progNum]);
            ui->valor_central->setText(mostrarProgCentral(progEnvasadora[progNum]));
        }
        if(true){
            if(progNum==0){
                l_estado="setProg";
                mostrarLCD(l_estado,true);
                mostrarBoton(l_estado,true);
                ui->valor_vac->setNum(prog01[0]);
                ui->valor_tvac->setNum(prog01[1]);
                ui->valor_gas->setNum(prog01[2]);
                ui->valor_tsellar->setNum(prog01[3]);
                ui->valor_taire->setNum(prog01[4]);
            }
            else if(progNum==1){
                l_estado="setProg";
                mostrarLCD(l_estado,true);
                mostrarBoton(l_estado,true);
                ui->valor_vac->setNum(prog02[0]);
                ui->valor_tvac->setNum(prog02[1]);
                ui->valor_gas->setNum(prog02[2]);
                ui->valor_tsellar->setNum(prog02[3]);
                ui->valor_taire->setNum(prog02[4]);
            }
            else if(progNum==2){
                l_estado="setProg";
                mostrarLCD(l_estado,true);
                mostrarBoton(l_estado,true);
                ui->valor_vac->setNum(prog03[0]);
                ui->valor_tvac->setNum(prog03[1]);
                ui->valor_gas->setNum(prog03[2]);
                ui->valor_tsellar->setNum(prog03[3]);
                ui->valor_taire->setNum(prog03[4]);
            }
            else if(progNum==3){
                l_estado="setProgVAC";
                mostrarLCD(l_estado,true);
                mostrarBoton(l_estado,true);
                ui->valor_vac->setNum(progVAC[0]);
                ui->valor_tvac->setNum(progVAC[1]);
                ui->valor_gas->setNum(progVAC[2]);
                ui->valor_tsellar->setNum(progVAC[3]);
                ui->valor_taire->setNum(progVAC[4]);
            }
            else if(progNum==4){
                l_estado="setProgPaso";
                mostrarLCD(l_estado,true);
                mostrarBoton(l_estado,true);
                ui->valor_vac->setNum(progPaso[0]);
                ui->valor_tvac->setNum(progPaso[1]);
                ui->valor_gas->setNum(progPaso[2]);
                ui->valor_tsellar->setNum(progPaso[3]);
                ui->valor_taire->setNum(progPaso[4]);
            }
        }
    }
    else if((l_estado=="setValorVAC") &&(accion=="mas" || accion=="menos")){
        int incr = 1;
        int valorActual = 0;
        if(progNum==0){valorActual = prog01[0];}
        else if(progNum==1){valorActual = prog02[0];}
        else if(progNum==2){valorActual = prog03[0];}
        else if(progNum==3){valorActual = progVAC[0];}
        else if(progNum==4){valorActual = progPaso[0];}

        qDebug()<<"estoy en setValorVAC progNum:"<<progNum<<" valor actual:"<<valorActual<< "accion:"<<accion;
        if(accion=="mas"){
            if(valorActual>0 && valorActual<99){
                valorActual = valorActual + incr;
                qDebug()<<valorActual;
            }
        }
        else if(accion=="menos"){
            if(valorActual>0 && valorActual<100){
                valorActual = valorActual - incr;
                qDebug()<<valorActual;
            }
        }
        ui->valor_central->setNum(valorActual);

        if(progNum==0){prog01[0]=valorActual;}
        else if(progNum==1){prog02[0]=valorActual;}
        else if(progNum==2){prog03[0]=valorActual;}
        else if(progNum==3){progVAC[0]=valorActual;}
        else if(progNum==4){progPaso[0]=valorActual;}
    }
    else if((l_estado=="setValorVACplus") &&(accion=="mas" || accion=="menos")){
        float incr = 1;
        float valorActual = 0;
        if(progNum==0){valorActual = prog01[1];}
        else if(progNum==1){valorActual = prog02[1];}
        else if(progNum==2){valorActual = prog03[1];}
        else if(progNum==3){valorActual = progVAC[1];}
        else if(progNum==4){valorActual = progPaso[1];}
        qDebug()<<"estoy en setValorVACplus progNum:"<<progNum<<" valor actual:"<<valorActual<< "accion:"<<accion;

        if(accion=="mas"){
            if(valorActual>=0 && valorActual<=15){
                valorActual = valorActual + incr;
            }
        }
        else if(accion=="menos"){
            if(valorActual>=0 && valorActual<=15){
                valorActual = valorActual - incr;
            }
        }
        if(valorActual<=0){valorActual=0;}
        else if(valorActual>=15){valorActual=15;}
        ui->valor_central->setText(QString::number(valorActual));

        if(progNum==0){prog01[1]=valorActual;}
        else if(progNum==1){prog02[1]=valorActual;}
        else if(progNum==2){prog03[1]=valorActual;}
        else if(progNum==3){progVAC[1]=valorActual;}
        else if(progNum==4){progPaso[1]=valorActual;}
    }
    else if((l_estado=="setValorGas") &&(accion=="mas" || accion=="menos")){
        int incr = 1;
        int valorActual = 0;
        if(progNum==0){valorActual = prog01[2];}
        else if(progNum==1){valorActual = prog02[2];}
        else if(progNum==2){valorActual = prog03[2];}
        else if(progNum==3){valorActual = progVAC[2];}
        else if(progNum==4){valorActual = progPaso[2];}
        qDebug()<<"estoy en setValorVACplus progNum:"<<progNum<<" valor actual:"<<valorActual<< "accion:"<<accion;

        if(accion=="mas"){
            if(valorActual>=0 && valorActual<=99){
                valorActual = valorActual + incr;
            }
        }
        else if(accion=="menos"){
            if(valorActual>=0 && valorActual<=99){
                valorActual = valorActual - incr;
            }
        }
        if(valorActual<=0){valorActual=0;}
        else if(valorActual>=99){valorActual=99;}
        ui->valor_central->setNum(valorActual);

        if(progNum==0){prog01[2]=valorActual;}
        else if(progNum==1){prog02[2]=valorActual;}
        else if(progNum==2){prog03[2]=valorActual;}
        else if(progNum==3){progVAC[2]=valorActual;}
        else if(progNum==4){progPaso[2]=valorActual;}
    }
    else if((l_estado=="setValorSellar") &&(accion=="mas" || accion=="menos")){
        float incr = 0.1;
        float valorActual = 0;
        if(progNum==0){valorActual = prog01[3];}
        else if(progNum==1){valorActual = prog02[3];}
        else if(progNum==2){valorActual = prog03[3];}
        else if(progNum==3){valorActual = progVAC[3];}
        else if(progNum==4){valorActual = progPaso[3];}
        qDebug()<<"estoy en setValorVACplus progNum:"<<progNum<<" valor actual:"<<valorActual<< "accion:"<<accion;

        if(accion=="mas"){
            if(valorActual>=0 && valorActual<=7){
                valorActual = valorActual + incr;
            }
        }
        else if(accion=="menos"){
            if(valorActual>=0 && valorActual<=7){
                valorActual = valorActual - incr;
            }
        }
        if(valorActual<=0){valorActual=0;}
        else if(valorActual>=7){valorActual=7;}
        ui->valor_central->setText(QString::number(valorActual));

        if(progNum==0){prog01[3]=valorActual;}
        else if(progNum==1){prog02[3]=valorActual;}
        else if(progNum==2){prog03[3]=valorActual;}
        else if(progNum==3){progVAC[3]=valorActual;}
        else if(progNum==4){progPaso[3]=valorActual;}
    }
    else if((l_estado=="setValorAire") &&(accion=="mas" || accion=="menos")){
        float incr = 1;
        float valorActual = 0;
        if(progNum==0){valorActual = prog01[4];}
        else if(progNum==1){valorActual = prog02[4];}
        else if(progNum==2){valorActual = prog03[4];}
        else if(progNum==3){valorActual = progVAC[4];}
        else if(progNum==4){valorActual = progPaso[4];}
        qDebug()<<"estoy en setValorVACplus progNum:"<<progNum<<" valor actual:"<<valorActual<< "accion:"<<accion;

        if(accion=="mas"){
            if(valorActual>=0 && valorActual<=10){
                valorActual = valorActual + incr;
            }
        }
        else if(accion=="menos"){
            if(valorActual>=0 && valorActual<=10){
                valorActual = valorActual - incr;
            }
        }
        if(valorActual<=0){valorActual=0;}
        else if(valorActual>=10){valorActual=10;}
        ui->valor_central->setText(QString::number(valorActual));

        if(progNum==0){prog01[4]=valorActual;}
        else if(progNum==1){prog02[4]=valorActual;}
        else if(progNum==2){prog03[4]=valorActual;}
        else if(progNum==3){progVAC[4]=valorActual;}
        else if(progNum==4){progPaso[4]=valorActual;}
    }
    else if((l_estado=="setProg" || l_estado=="setProgVAC" || l_estado=="setProgPaso") &&(accion=="ok")){
        ui->valor_prog->setText(progEnvasadora[progNum]);
        ui->valor_central->setText(mostrarProgCentral(progEnvasadora[progNum]));
    }
    else if((configON) && (accion=="mas"|| accion=="menos")){
        //l_estado=="setCaducidad" || l_estado=="setTicket" || l_estado=="setTemperatura"
        if(accion=="menos"){
            if(l_estado=="setCaducidad"){
                ui->icon_caducidad->setVisible(true);
                estado="setTikect";
            }
            else if(l_estado=="setTicket"){
                ui->icon_ticket->setVisible(true);
                estado="setTemperatura";
            }
            else if(l_estado=="setTemperatura"){
                    ui->icon_temperatura->setVisible(true);
                    estado="setPrinter";
            }
            else if(l_estado=="setPrinter"){
                ui->icon_printer->setVisible(true);
                estado="setCaducidad";
            }
        }
        else if(accion=="mas"){
            if(l_estado=="setCaducidad"){
                ui->icon_caducidad->setVisible(true);
                estado="setPrinter";
            }
            else if(l_estado=="setPrinter"){
                ui->icon_printer->setVisible(true);
                estado="setTemperatura";
            }
            else if(l_estado=="setTemperatura"){
                    ui->icon_temperatura->setVisible(true);
                    estado="setTicket";
            }
            else if(l_estado=="setTicket"){
                ui->icon_ticket->setVisible(true);
                estado="setCaducidad";
            }
        }
        qDebug()<<"cambiar de:"<<l_estado<<" a:"<<estado;
    }
}

void controlGUI::updateParpadearBoton(){
    encendido = !encendido;
    if(estado=="setValorVAC"){ui->boton_vac->setVisible(encendido);}
    else if(estado=="setValorVACplus"){ui->boton_vacPlus->setVisible(encendido);}
    else if(estado=="setValorGas"){ui->boton_gas->setVisible(encendido);}
    else if(estado=="setValorSellar"){ui->boton_sellar->setVisible(encendido);}
    else if(estado=="setValorAire"){ui->boton_aire->setVisible(encendido);}
    else if(configON){ui->boton_conf->setVisible(encendido);}
}
void controlGUI::updateParpadearLCD(){
    encendido2 =!encendido2;
    if(estado=="enVAC"){ui->valor_central->setVisible(encendido2);}
    else if(estado=="setPrinter"){ui->icon_printer->setVisible(encendido2);}
    else if(estado=="setCaducidad"){ui->icon_caducidad->setVisible(encendido2);}
    else if(estado=="setTicket"){ui->icon_ticket->setVisible(encendido2);}
    else if(estado=="setTemperatura"){ui->icon_temperatura->setVisible(encendido2);}
}

QString controlGUI ::mostrarProgCentral(QString valor){
    QString valorMostrar;
    //    if(valor.contains("p")){
    //        valorMostrar= valor.right(2);
    //    }
    //    else valorMostrar = valor;
    valorMostrar = valor;
    return valorMostrar;
}
int controlGUI::calculoPresionPorcent(int mbares){
    float m =-0.098713;
    return int(m*(mbares-1013));
}
