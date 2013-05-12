#include <iostream>
#include <malloc.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <cstdlib>
#include <signal.h>
#include <stdio.h>
#include "RtMidi.h"

using std::cout;
using std::endl;

int main()
{
    RtMidiIn *midiin = new RtMidiIn();
    std::vector<unsigned char> message;
    int bytes, port;
    int serialport;
    serialport = open("/dev/ttyACM0", O_RDWR | O_NOCTTY | O_NDELAY);
    bool note_array[108];
    for (int i = 0; i < 108; i++)
    {
        note_array[i] = false;
    }

    double stamp;

    unsigned int ports = midiin->getPortCount();

    for (int i = 0; i < ports; i++)
    {
        cout << i << ": " <<  midiin->getPortName(i) << endl;
    }

    std::cin >> port;

    midiin->openPort(port);

    midiin->ignoreTypes(false, false, false);

    cout << "Receiving on " << midiin->getPortName(port) << endl;

    unsigned char buffer[11] = {0};
    while (true)
    {
        stamp = midiin->getMessage( &message );
        bytes = message.size();
        for (int i = 0; i < bytes; i++)
        {
            cout << "Byte " << i << ": ";
           printf("0x%x\n", (int)message[i]);
            if ((message[i + 0] & 0xA0) == 0xA0)
                continue;
            else if ((message[i + 0] & 0xF0) == 0xF0)
                continue;
            else if ((message[i + 0] & 0xE0) == 0xE0)
                continue;
            else if ((message[i + 0] & 0xD0) == 0xD0)
                continue;
            else if ((message[i + 0] & 0xC0) == 0xC0)
                continue;
            else if ((message[i + 0] & 0xB0) == 0xB0)
                continue;
            else if ((message[i + 0] & 0xA0) == 0xA0)
                continue;
            else if ((message[i + 0] & 0x90) == 0x90)
            {
                cout << "Note ON: " << (int)message[i+1] << endl;
                note_array[message[i + 1]] = true;
            }
            else if ((message[i + 0] & 0x80) == 0x80)
            {
                cout << "Note OFF: " << (int)message[i+1] << endl;
                note_array[message[i + 1]] = false;
            }
        }
        if (message.size() > 0)
        {
            int note_number = 21;
	        for (int j = 0; j < 11; j++)
	        {
                buffer[j] = 0;
                if (note_array[note_number])
                    buffer[j]++;
                note_number++;
                for (int k = 0; k < 7; k++)
                {
                    buffer[j] = buffer[j]<<1;
                    if (note_array[note_number])
                        buffer[j]++;
                    note_number++;
                }
                cout << (int) buffer[j];
            }
            cout << endl;
            for (int i = 21; i < 108; i++)
            {
                if (note_array[i])
                    cout << 1;
                else
                    cout << 0;
            }
            cout << endl;
            write(serialport, buffer, 11);
        }        
    }
}

