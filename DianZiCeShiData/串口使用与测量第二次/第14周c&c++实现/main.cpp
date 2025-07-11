#include <cstdio>
#include <iostream>
#include <string>
#include "serial.h"

std::string format(const std::vector<unsigned char> &data) {
    std::string str(2 * data.size() + 1, '\x00');
    for (int i = 0; i < data.size(); i++) {
        sprintf(&str[i * 2], "%02X", data[i]);
    }
    return str;
}

typedef unsigned char uchar;

int main() {
    serial pipe("/dev/ttyUSB0", B1200);
    std::vector<unsigned char> data;
    std::vector<uchar> prefix = {0xaa, 0x55};
    std::vector<uchar> id = {0x02, 0x00, 0x02, 0x00, 0x00, 0x04, 0x00, 0x06, 0x01, 0x04, 0x00, 0x09};
    std::vector<uchar> secret;
    
    id.insert(id.begin(),  prefix.begin(), prefix.end());
    // pipe.myWrite(data);
    // pipe.myRead(6)
	pipe.myWrite(id);
    while (1)
    { 
        secret = pipe.myRead(7);
        int i = 0;
        for (i = 0; i < secret.size()-1; i++) {
        	if (secret[i]==0xaa && secret[i+1] == 0x55) break;
        }
        i += 3;
        std::vector<uchar> sent;
        for (int j = 0; j < 4; ++j, i++) {
        	sent.push_back(secret[i%secret.size()]);
        }
        sent.insert(sent.begin(), prefix.begin(), prefix.end());
        std::cout << format(secret) << std::endl;
        pipe.myWrite(sent);
        std::cout << format(sent) << std::endl;

    }

    
    
    

    return 0;
}
