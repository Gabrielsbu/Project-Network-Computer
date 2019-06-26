//cd /home/flavio/Redes/Trabalho\ 1/Códigos
// curl ifconfig.me

#ifndef PACKETS_H_
#define PACKETS_H_

//Inclusões de dependências
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
 
//Macros, enum e constantes globais
#define HELLO_MSG "Hello World"
#define HELLO_SIZE 12
#define UI32SIZE 4
#define UI16SIZE 2
#define UI8SIZE 1

enum {
    PAYLOAD_LEN_SHIFT = 0,
    PSECRET_SHIFT = 4,
    STEP_SHIFT = 8,
    STUDENT_ID_SHIFT = 10,
    TAM_CABECALHO = 12,
    TAM_LEN = 16,
    TAM_UDP = 20,
    TAM_SECRETA = 24,
    TAM_PAYLOAD_1 = 12,
    DEFAULT = 12235,
};


struct header{
    uint32_t payload_len;
    uint32_t psecret; 
    uint16_t step; 
    uint16_t student_id; 
};
typedef struct header Cabecalho;


struct payload_1{
    char *msg;
}payload_1;
typedef struct payload_1 Payload_1;


typedef struct payload_2{
    uint32_t num;
    uint32_t len;
    uint32_t udp_port;
    uint32_t secretA;    
}payload_2;
typedef struct payload_2 Payload_2;


typedef struct payload_3{
    uint32_t packet_id;
}payload_3;
typedef struct payload_3 Payload_3;


typedef struct payload_4{ ///ACK SERVIDOR
    uint32_t acked_packet_id;
}payload_4;
typedef struct payload_4 Payload_4;


typedef struct payload_5{
    uint32_t tcp_port;
    uint32_t secretB;
}payload_5;
typedef struct payload_5 Payload_5;


typedef struct payload_6{
    uint32_t num2;
    uint32_t len2;
    uint32_t secretC;
    char c;    
}payload_6;
typedef struct payload_6 Payload_6;


typedef struct payload_7{
     char *payloadDeTamLen2;    
}payload_7;
typedef struct payload_7 Payload_7;


typedef struct payload_8{
    uint32_t secretD;
}payload_8;
typedef struct payload_8 Payload_8;


struct packet{
    Cabecalho* cabecalho;
    void *payload;
}packet;
typedef struct packet Pacote;


#endif