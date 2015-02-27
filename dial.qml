import QtQuick 2.1
import QtQuick.Controls 1.1

Rectangle {
    width: 171
    height: 131
    color: "black"

    id: page
    property int valorCentral:0
    property int valorCirInterior:85
    property int valorCirExterior:85


    Canvas {
        id:mycanvas
        anchors.rightMargin: 0
        anchors.bottomMargin: 1
        anchors.leftMargin: 0
        anchors.topMargin: -15
        anchors.fill: parent
        antialiasing: true

        onPaint: {
            var ctx = getContext("2d");
            ctx.reset();
            var centreX = width / 2;
            var centreY = height / 2 +10;
            var cirExtAnchura = 15;
            var cirIntAnchura = 13;
            var radioExterior = width/2.8;
            var radioInterior = radioExterior - cirIntAnchura -3;
            var anguloZero = Math.PI/2+ Math.PI/180*40;
            var anguloFinal = anguloZero + Math.PI/180*285;

            var anguloPorcentaje = valorCentral/100;
            var anguloPorcentInterior = valorCirInterior/100;
            var anguloPorcentExterior = valorCirExterior/100;

            var anguloRecorridoExt = Math.PI/180*285 * (1-anguloPorcentExterior);
            var anguloRecorridoInt = Math.PI/180*285 * anguloPorcentInterior;
            var cirInteriorAng_raya = Math.PI/180*4;
            var cirInteior_numRayas = anguloRecorridoInt/(2*cirInteriorAng_raya);

            if(true){// perimetro circunferencia exterior
                ctx.beginPath();
                ctx.strokeStyle = '#9C1F1F';//'rgb(255, 20, 44)' #9C1F1F
                ctx.lineWidth = cirExtAnchura;
                //arc(real x, real y, real radius, real startAngle, real endAngle, bool anticlockwise)
                ctx.arc(centreX, centreY, radioExterior, anguloZero, anguloFinal, false);
                ctx.stroke();
                ctx.beginPath();
                ctx.strokeStyle = 'black';
                ctx.lineWidth = cirExtAnchura-2;
                //arc(real x, real y, real radius, real startAngle, real endAngle, bool anticlockwise)
                ctx.arc(centreX, centreY, radioExterior, anguloZero, anguloFinal, false);
                ctx.stroke();
                // raya final
                ctx.beginPath();
                ctx.strokeStyle = '#9C1F1F';//'rgb(255, 20, 44)' #9C1F1F
                ctx.lineWidth = cirExtAnchura;
                //arc(real x, real y, real radius, real startAngle, real endAngle, bool anticlockwise)
                ctx.arc(centreX, centreY, radioExterior, anguloFinal - Math.PI/180*1, anguloFinal, false);
                ctx.stroke();
                // raya inicial
                ctx.beginPath();
                ctx.strokeStyle = '#9C1F1F';//'rgb(255, 20, 44)' #9C1F1F
                ctx.lineWidth = cirExtAnchura;
                //arc(real x, real y, real radius, real startAngle, real endAngle, bool anticlockwise)
                ctx.arc(centreX, centreY, radioExterior, anguloZero - Math.PI/180*1, anguloZero, false);
                ctx.stroke();
            }

            if(true){// circunferencia exterior
                ctx.beginPath();
                ctx.strokeStyle = '#9C1F1F';//'rgb(255, 20, 44)' #9C1F1F
                ctx.lineWidth = cirExtAnchura;
                //arc(real x, real y, real radius, real startAngle, real endAngle, bool anticlockwise)
                ctx.arc(centreX, centreY, radioExterior, anguloZero, anguloZero+anguloRecorridoExt, false);
                ctx.stroke();
            }

            if(true){// circunferencia interior
                //                console.log("angulo recorrido:"+180*( anguloRecorridoInt)/Math.PI+"º   num rayas:"+cirInteior_numRayas)
                for(var i=0;i<cirInteior_numRayas;i++){//irInteior_numRayas
                    var anguloAjuste = Math.PI/180*4.5;
                    ctx.beginPath();
                    ctx.strokeStyle= 'white';
                    ctx.lineWidth = cirIntAnchura;
                    //arc(real x, real y, real radius, real startAngle, real endAngle, bool anticlockwise)
                    ctx.arc(centreX, centreY,radioInterior,anguloFinal - (cirInteriorAng_raya)*(2*i-1) -anguloAjuste,anguloFinal - cirInteriorAng_raya*2*i-anguloAjuste, true);
                    ctx.stroke();
                }
            }

        }
        Timer {
            interval: 100;
            repeat: true;
            running: true;
            onTriggered: {
                mycanvas.requestPaint ();
            }
        }
    }
}


