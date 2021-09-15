#include <Arduino.h>
// C++ code
//
#define SenseBUT 12
#define PedalBUT 26
#define TurboBUT 32
#define TurboRelay 23
#define MotorRelayH1 22
#define MotorRelayH2 19
#define SteeringWheelRelayH1 5
#define SteeringWheelRelayH2 18
// define led Status ESP 32 Doit DEVKIT V1
#define LedStatus 2

int OLDSenseBUT = 0; //valor de registro de alteração

int SenseBUTtmp = 0; //valor temporário de auxilio
int SenseSW = 0;     //determina o sentido do motor do carrinho;
int OLDSenseSW = 0;  //valor de registro de alteração
int PedalSW = 0;     //determina o movimento do carrinho;
int OLDPedalSW = 0;  //valor de registro de alteração
int TurboSW = 0;     //determina o modo Turbo ligado ou desligado;
int OLDTurboSW = 0;  //valor de registro de alteração

int SteeringWheelSense = 0;    //determina o sentido de volante
int OLDSteeringWheelSense = 0; //valor de registro de alteração

String msgtxt; //criando varável para trafegar mensagens de texto;

String dataSerial; //local de armazenamento dos valos via comunicação serial

int RemoteControlON; // Define se o carro está sendo controlado remotamente

void msg(String txt)
{
    //função criada para ser responsável por qualquer tipo de mensagem;
    msgtxt = txt;
    Serial.println(msgtxt);
}

void StopCar()
{
    msg("Para o carro");
    OLDPedalSW = 0;
    digitalWrite(MotorRelayH1, 0);
    digitalWrite(MotorRelayH2, 0);
}

void SteeringWheelDirection()
{
    //verifica o modo de atuação da ponte H e determina o sentido do volante;
    if (true)
    {
        if (SteeringWheelSense == 1)
        {
            //Volante gira para a direita;
            msg("Volante gira para Direita");
            digitalWrite(SteeringWheelRelayH1, 1);
            digitalWrite(SteeringWheelRelayH2, 0);
        }
        if (SteeringWheelSense == -1)
        {
            //Volante gira para a esquerda;
            msg("Volante gira para Esquerda");
            digitalWrite(SteeringWheelRelayH1, 0);
            digitalWrite(SteeringWheelRelayH2, 1);
        }
        if (SteeringWheelSense != 1 && SteeringWheelSense != -1)
        {
            //volante não gira;
            msg("Volante nao gira");
            digitalWrite(SteeringWheelRelayH1, 0);
            digitalWrite(SteeringWheelRelayH2, 0);
        }
        if (SteeringWheelSense == 0)
        {
            //volante não gira;
            msg("Volante nao gira");
            digitalWrite(SteeringWheelRelayH1, 0);
            digitalWrite(SteeringWheelRelayH2, 0);
        }
    }
}

void Motion()
{
    if (PedalSW == 1)
    {
        if (SenseSW == 1)
        {
            //motor gira em uma direção;
            msg("Motor gira para frente");
            digitalWrite(MotorRelayH1, 1);
            digitalWrite(MotorRelayH2, 0);
        }
        if (SenseSW == -1)
        {
            //motor gira em uma direção oposta;
            msg("Motor gira para tras");
            digitalWrite(MotorRelayH1, 0);
            digitalWrite(MotorRelayH2, 1);
        }
        if (SenseSW != 1 && SenseSW != -1)
        {
            //motor não gira;
            msg("Motor nao gira 1");
            StopCar();
        }
        if (SenseSW == 0)
        {
            //motor não gira;
            msg("Motor nao gira 2");
            StopCar();
        }
    }
}

void SenseSWAUX()
{
    if (RemoteControlON == 0)
    {
        PedalSW = digitalRead(PedalBUT);
    }
    if (true)
    {
        if (PedalSW == 1)
        {
            Motion();
        }
        if (PedalSW == 0)
        {
            StopCar();
        }
    }
}

void SenseState()
{
    if (RemoteControlON == 1)
    {
    }
    if (RemoteControlON == 0)
    {
        SenseBUTtmp = digitalRead(SenseBUT);
    }
    if (SenseBUTtmp != OLDSenseBUT)
    {
        digitalWrite(LedStatus, 1);
        OLDSenseBUT = SenseBUTtmp;
        if (SenseBUTtmp == 1)
        {
            msg("mode go");
            SenseSW = 1;
            SenseSWAUX();
        }
        if (SenseBUTtmp == 0)
        {
            msg("mode back");
            SenseSW = -1;
            SenseSWAUX();
        }
        digitalWrite(LedStatus, 0);
    }
}

void PedalState()
{
    if (RemoteControlON == 1)
    {
        PedalSW = 1;
    }
    if (RemoteControlON == 0)
    {
        PedalSW = digitalRead(PedalBUT);
    }

    //chama a função para determinar o estado do motor;
    if (PedalSW != OLDPedalSW)
    {
        digitalWrite(LedStatus, 1);
        OLDPedalSW = PedalSW;
        if (PedalSW == 1)
        {
            //se o pedal for pressionado, chama a função para liga-lo;
            msg("pedal Pressionado");
            Motion();
        }
        if (PedalSW == 0)
        {
            //se o pedal não for pressionado, chama afunção para desligar o motor;
            msg("pedal livre");
            StopCar();
        }
        digitalWrite(LedStatus, 0);
    }
}

void TurboState()
{
    if (RemoteControlON == 1)
    {
        TurboSW = !TurboSW;
    }
    if (RemoteControlON == 0)
    {
        TurboSW = digitalRead(TurboBUT);
    }

    //define o modo turbo ligado ou desligado;
    if (TurboSW != OLDTurboSW)
    {
        digitalWrite(LedStatus, 1);
        OLDTurboSW = TurboSW;
        if (TurboSW == 1)
        {
            //se o modo turbo for ligado;
            digitalWrite(TurboRelay, 1);
            msg("modo turbo ativado");
            //Liga o rele do modo turbo;
        }
        if (TurboSW == 0)
        {
            //se o modo turbo for desligado;
            digitalWrite(TurboRelay, 0);
            msg("modo turbo desativado");
            //Desliga o rele do modo tubo;
        }
        digitalWrite(LedStatus, 0);
    }
}

void MonitorControl()
{
}

void setup()
{
    Serial.begin(9600); //Define velocidade de Comunicação Serial
    msg("Start Code");
    pinMode(TurboRelay, OUTPUT);
    pinMode(SteeringWheelRelayH1, OUTPUT);
    pinMode(SteeringWheelRelayH2, OUTPUT);
    pinMode(MotorRelayH1, OUTPUT);
    pinMode(MotorRelayH2, OUTPUT);
    pinMode(SenseBUT, INPUT);
    pinMode(PedalBUT, INPUT);
    pinMode(TurboBUT, INPUT);
    pinMode(LedStatus, OUTPUT);
    //-define 0 to all values
    digitalWrite(SteeringWheelRelayH1, 0);
    digitalWrite(SteeringWheelRelayH2, 0);
    digitalWrite(MotorRelayH1, 0);
    digitalWrite(MotorRelayH2, 0);
    digitalWrite(TurboRelay, 0);
    TurboSW = 0;
    SenseSW = 0;
    PedalSW = 0;
    digitalWrite(LedStatus, 1);
    delay(100);
    digitalWrite(LedStatus, 0);
}

void loop()
{
    TurboState();
    SenseState();
    PedalState();
    if (Serial.available())
    { // Verificar se há caracteres disponíveis
        dataSerial = Serial.readString();
        Serial.print(dataSerial);
        Serial.println("");
        if (dataSerial == "start" || dataSerial == "s")
        {
            RemoteControlON = 1;
            msg("Start Remote Control");
            msg("comandos possiveis:");
            msg("go");
            msg("back");
            msg("left");
            msg("right");
            msg("run");
            msg("stop");
            msg("turbo");
            msg("exit");
            msg("------------------");
        }
        while (RemoteControlON == 1)
        {
            if (Serial.available())
            {
                dataSerial = Serial.readString();
                Serial.print(dataSerial);
                Serial.println("");
                if (dataSerial == "exit" || dataSerial == "e")
                {
                    RemoteControlON = 0;
                    msg("Stop Remote Control");
                }
                if (dataSerial == "go" || dataSerial == "g")
                {
                    SenseBUTtmp = 1;
                    SenseState();
                    delay(1100);
                    StopCar();
                }
                if (dataSerial == "back" || dataSerial == "b")
                {
                    SenseBUTtmp = 0;
                    SenseState();
                    delay(1100);
                    StopCar();
                }
                if (dataSerial == "left" || dataSerial == "l")
                {
                    SteeringWheelSense = -1;
                    SteeringWheelDirection();
                    delay(500);
                    SteeringWheelSense = 0;
                    SteeringWheelDirection();
                }
                if (dataSerial == "right" || dataSerial == "r")
                {
                    SteeringWheelSense = 1;
                    SteeringWheelDirection();
                    delay(500);
                    SteeringWheelSense = 0;
                    SteeringWheelDirection();
                }
                if (dataSerial == "run")
                {
                    PedalState();
                    delay(1100);
                    StopCar();
                }
                if (dataSerial == "stop")
                {
                    StopCar();
                }
                if (dataSerial == "turbo" || dataSerial == "t")
                {
                    TurboState();
                }
            }
        }
    }
}