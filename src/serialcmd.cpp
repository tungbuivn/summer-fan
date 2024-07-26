#include "serialcmd.h"
#include "events.h"

SerialCmd::SerialCmd() : BaseThread()
{
    // cmdCount = 0;
    // act = NULL;
    // SerialUSB
};
void SerialCmd::processCommand()
{
    // if ((cmdCount > 0) && (act == NULL))
    // {
    //     cmdCount--;
    //     char buf[22];
    //     int p = 0;
    //     bool done = false;
    //     while (!done)
    //     {
    //         char c = data.front();
    //         data.pop();
    //         if ((c != 0xA) && (c != 0xD) && (p < 20))
    //         {
    //             buf[p++] = c;
    //             buf[p] = '\0';
    //         }

    //         done = c == 0xD;
    //     }
    //     std::string s(buf);
    //     if (p > 0)
    //     {

    //         // Serial.print(p);
    //         act = new CommandAction(s);
    //     }

    //     Serial.print("full command: ");
    //     Serial.println(buf);
    // }
}
void SerialCmd::readSerial()
{
    if (Serial.available())
    {
        char incomingByte = Serial.read();
        debug_printf("%c",incomingByte);
        if ((incomingByte == 0xD)||(incomingByte == 0xA)) {
            std::string str(data.begin(), data.end());
            data.clear();
            debug_printf("Full command: %s",str);
            // SerialData cmd;
            // cmd.str.assign(str);
            eventSystem.dispatchMessage(new SerialData(str));
        } else {
            data.push_back(incomingByte);
        }
       
    }
}
inline void SerialCmd::handleTask()
{
}
void SerialCmd::execute()
{
    static int cont = 0;
    // Serial.println("Looop =================");
    TBT_THC(5, 
        readSerial(),
        // processCommand(),
        // TBT_IF(act != NULL,
        //     TBT_BLOCK(cont = 1,
        //         TBT_WHILE( cont == 1, 
        //             TBT_BLOCK(
        //                 act->execute(), 
        //                 if (act->isFinished()) {
        //                     delete this->act;
        //                     act = NULL;
        //                     cont=0; 
        //                 }
        //             )
        //         )
        //     ), 
        // )
    )
}