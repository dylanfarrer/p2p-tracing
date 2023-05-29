#include <iostream>
#include <string>
#include <vector>
#include <bitset>

#define PORT 65335
#define IP_1 127
#define IP_2 0
#define IP_3 0
#define IP_4 1

int findRandom()
{
    // Generate the random number
    int num = ((int)rand() % 2);
 
    // Return the generated number
    return num;
}

std::string generateBinaryString(int N)
{
    std::string S = "";

    for (int i = 0; i < N; i++) {
        int x = findRandom();
 
        // Append it to the string
        S += std::to_string(x);
    }

    return S;
}

std::bitset<88> encode(std::string data_object_string) {
    /*
     * append read bit (1), ip (32), port (16)
     * to binary
     * form:
     * (read bit)(ip)(port)(object)
     * 1 - 32 - 16 - 32
     * = 81
     * 
     * 
     * 
     * Update:
     * read bit, number of peers? * peer_ips, ip, port, data etc
     */
    std::bitset<8> read("0");
    std::bitset<8> ip_first(IP_1);
    std::bitset<8> ip_second(IP_2);
    std::bitset<8> ip_third(IP_3);
    std::bitset<8> ip_fourth(IP_4);
    std::bitset<16> port(PORT);

    std::bitset<32> data_object(data_object_string);
    std::bitset<88> full_object(read.to_string() + \
                                ip_first.to_string() + \
                                ip_second.to_string() + \
                                ip_third.to_string() + \
                                ip_fourth.to_string() + \
                                port.to_string() + \
                                data_object.to_string());
    return full_object;
}

std::vector<std::bitset<88> > encodeSet(std::vector<std::string> data_objects) {

    std::vector<std::bitset<88>  > full_objects;

    for (std::vector<std::string>::iterator iter = data_objects.begin();
        iter < data_objects.end();
        ++iter) {
        std::bitset<88> full_object = encode(*iter);
        full_objects.push_back(full_object);
        std::cout << std::endl;
        std::cout << full_object << std::endl;
    }

    return full_objects;
}

void checkReadStatus(std::bitset<88> qualified_object) {
    std::string object_string = qualified_object.to_string();

    std::bitset<8> read_bytestr(object_string.substr(0,8));
    std::bitset<32> data_bytestr(object_string.substr(56, 32));

    int read_status = (int)read_bytestr.to_ulong();

    std::cout << read_status << " ";
    if (read_status == 0) {
        std::cout << "Read status: False ";
    } else {
        std::cout << "Read status: True ";
    }
    std::cout << data_bytestr << std::endl << std::endl;
}

void checkReadStatusForSet(std::vector<std::bitset<88> > full_objects) {
    for (std::vector<std::bitset<88> >::iterator iter = full_objects.begin();
        iter < full_objects.end();
        ++iter) {
            checkReadStatus(*iter);
        }
}

void decode(std::bitset<88> qualified_object) {
    std::string object_string = qualified_object.to_string();

    std::bitset<8> read_bytestr(object_string.substr(0,8));
    std::bitset<8> ip_1_bytest(object_string.substr(8,8));
    std::bitset<8> ip_2_bytestr(object_string.substr(16,8));
    std::bitset<8> ip_3_bytestr(object_string.substr(24,8));
    std::bitset<8> ip_4_bytestr(object_string.substr(32,8));
    std::bitset<16> port_bytesstr(object_string.substr(40,16));
    std::bitset<32> data_bytestr(object_string.substr(56, 32));

    std::cout << (int)read_bytestr.to_ulong() << " ";
    std::cout << (int)ip_1_bytest.to_ulong() << " ";
    std::cout << (int)ip_2_bytestr.to_ulong() << " ";
    std::cout << (int)ip_3_bytestr.to_ulong() << " ";
    std::cout << (int)ip_4_bytestr.to_ulong() << " ";
    std::cout << (int)port_bytesstr.to_ulong() << " ";
    std::cout << data_bytestr << std::endl << std::endl;
}

void decodeSet(std::vector<std::bitset<88> > full_objects) {
    for (std::vector<std::bitset<88> >::iterator iter = full_objects.begin();
        iter < full_objects.end();
        ++iter) {
           decode(*iter);
        }
}

int main() {
    srand(time(NULL));
    std::cout << PORT << " " << IP_1 << " " << IP_2 << " " << IP_3 << " " << IP_4 << std::endl;

    std::vector<std::string> data_objects;

    // generate 10 random 32 bit binaries (in strings)
    for (int i = 0; i < 10; ++i) {
        data_objects.push_back(generateBinaryString(32));
    }

    // display the binaries
    for (std::vector<std::string>::iterator iter = data_objects.begin();
        iter < data_objects.end();
        ++iter) {
        std::cout << *iter << std::endl;
    }

    // Encode 
    std::vector<std::bitset<88> > full_objects = encodeSet(data_objects);

    // check if read
    checkReadStatusForSet(full_objects);

    // full decode
    decodeSet(full_objects);
}