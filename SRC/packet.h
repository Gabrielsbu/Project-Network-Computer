#include "packets.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <time.h>


void atribuiCabecalho(Pacote *pkt, uint32_t payload_len, uint32_t psecret, uint16_t step){ 
    Cabecalho *cabecalho = malloc(sizeof(Cabecalho));

    cabecalho->payload_len = payload_len;
    cabecalho->psecret = psecret;
    cabecalho->step = step;
    cabecalho->student_id = 643;

    pkt->cabecalho = cabecalho;
}

void alocaCabecalho(Pacote *pkt){
    Cabecalho *cabecalho = malloc(sizeof(Cabecalho));
    pkt->cabecalho = cabecalho;
}

void atribuiPayload1(Pacote *pkt){
	Payload_1* payload1 = malloc(sizeof(Payload_1));
    payload1->msg = malloc(HELLO_SIZE);

    memcpy(payload1->msg, HELLO_MSG, HELLO_SIZE);
    pkt->payload = payload1;
}

void alocaPayload1(Pacote *pkt){
	Payload_1* payload = malloc(sizeof(Payload_1));
    payload->msg = malloc(HELLO_SIZE);

    pkt->payload = payload;
}

void liberaPayload1(Pacote *pkt){
    free(((Payload_1 *)pkt->payload)->msg);
    free((Payload_1 *)pkt->payload);
}

void atribuiPayload2(Pacote *pkt, uint32_t num, uint32_t udp_port, uint32_t secretA){
    Payload_2 *payload2 = malloc(sizeof(Payload_2));

    memset(payload2, 0, sizeof(Payload_2));
    
    payload2->num = num;
    payload2->len = 16;
    payload2->udp_port = udp_port;
    payload2->secretA = secretA;

    pkt->payload = payload2;
}

void alocaPayload2(Pacote *pkt){
	Payload_2* payload2 = malloc(sizeof(Payload_2));
    memset(payload2, 0, sizeof(Payload_2));

    pkt->payload = payload2;
}

void liberaPayload2(Pacote *pkt){
	free((Payload_2 *)pkt->payload);
}

void atribuiPayload3(Pacote *pkt, uint32_t packet_id){
    Payload_3 *payload3 = malloc(sizeof(Payload_3));

    memset(payload3, 0, sizeof(Payload_3));
    
    payload3->packet_id = packet_id;

    pkt->payload = payload3;
}

void alocaPayload3(Pacote *pkt){
	Payload_3* payload3 = malloc(sizeof(Payload_3));
    memset(payload3, 0, sizeof(Payload_3));

    pkt->payload = payload3;
}

void liberaPayload3(Pacote *pkt){
	free((Payload_3 *)pkt->payload);
}

void atribuiPayload4(Pacote *pkt, uint32_t acked_packet_id){
    Payload_4 *payload4 = malloc(sizeof(Payload_4));

    memset(payload4, 0, sizeof(Payload_4));
    
    payload4->acked_packet_id = acked_packet_id;

    pkt->payload = payload4;
}

void alocaPayload4(Pacote *pkt){
	Payload_4* payload4 = malloc(sizeof(Payload_4));
    memset(payload4, 0, sizeof(Payload_4));

    pkt->payload = payload4;
}

void liberaPayload4(Pacote *pkt){
	free((Payload_4 *)pkt->payload);
}

void atribuiPayload5(Pacote *pkt, uint32_t tcp_port, uint32_t secretB){
    Payload_5 *payload5 = malloc(sizeof(Payload_5));

    memset(payload5, 0, sizeof(Payload_5));
    
    payload5->tcp_port = tcp_port;
    payload5->secretB = secretB;

    pkt->payload = payload5;
}

void alocaPayload5(Pacote *pkt){
	Payload_5* payload5 = malloc(sizeof(Payload_5));
    memset(payload5, 0, sizeof(Payload_5));

    pkt->payload = payload5;
}

void liberaPayload5(Pacote *pkt){
	free((Payload_5 *)pkt->payload);
}

void atribuiPayload6(Pacote *pkt, uint32_t num2, uint32_t secretC, char c){
	Payload_6 *payload6 = malloc(sizeof(Payload_6));

    memset(payload6, 0, sizeof(Payload_6));
    
    payload6->num2 = num2;
    payload6->len2 = 16;
    payload6->secretC = secretC;
    payload6->c = c;

    pkt->payload = payload6;
}

void alocaPayload6(Pacote *pkt){
	Payload_6* payload6 = malloc(sizeof(Payload_6));
    memset(payload6, 0, sizeof(Payload_6));

    pkt->payload = payload6;
}

void liberaPayload6(Pacote *pkt){
	free((Payload_6 *)pkt->payload);
}

void atribuiPayload7(Pacote *pkt, uint32_t len2, char c){
	Payload_7* payload7 = malloc(sizeof(Payload_7));
    payload7->payloadDeTamLen2 = malloc(len2);

    memset(payload7->payloadDeTamLen2,c,len2);

    pkt->payload = payload7;
}

void alocaPayload7(Pacote *pkt, uint32_t len2){
	Payload_7* payload7 = malloc(sizeof(Payload_7));
    payload7->payloadDeTamLen2 = malloc(len2);

    pkt->payload = payload7;
}

void liberaPayload7(Pacote *pkt){
    free(((Payload_7 *)pkt->payload)->payloadDeTamLen2);
    free((Payload_7 *)pkt->payload);
}

void atribuiPayload8(Pacote *pkt, uint32_t secretD){
    Payload_8 *payload8 = malloc(sizeof(Payload_8));

    memset(payload8, 0, sizeof(Payload_8));
    
    payload8->secretD = secretD;

    pkt->payload = payload8;
}

void alocaPayload8(Pacote *pkt){
	Payload_8* payload8 = malloc(sizeof(Payload_8));
    memset(payload8, 0, sizeof(Payload_8));

    pkt->payload = payload8;
}

void liberaPayload8(Pacote *pkt){
	free((Payload_8 *)pkt->payload);
}

size_t Encode(Pacote *pkt, uint8_t *outbuf, size_t bufSize, int stepCliente){
	size_t size = 0; 
    uint16_t u16 = 0; 
    uint32_t u32 = 0;
    char u8 = 0;
    
	if(12 + pkt->cabecalho->payload_len > bufSize){	//pouco espaco
    	printf("Buffer Nao Suporta o Pacote Enviado No Passo %d\n",stepCliente);
        return -1;
	}

    memset(outbuf, 0, bufSize);

    u32 = htonl(pkt->cabecalho->payload_len);
    memcpy(outbuf + PAYLOAD_LEN_SHIFT, &u32, UI32SIZE);
    size += UI32SIZE;
    
    u32 = htonl(pkt->cabecalho->psecret);
    memcpy(outbuf + PSECRET_SHIFT, &u32, UI32SIZE);
    size += UI32SIZE;
    
    u16 = htons(pkt->cabecalho->step);
    memcpy(outbuf + STEP_SHIFT, &u16, UI16SIZE);
    size += UI16SIZE;
    
    u16 = htons(pkt->cabecalho->student_id);
    memcpy(outbuf + STUDENT_ID_SHIFT, &u16, UI16SIZE);
    size += UI16SIZE;
    

    
    if (stepCliente == 1){
        memcpy(outbuf + size, ((Payload_1 *)pkt->payload)->msg, 12);
        size += 12;

    }else if(stepCliente == 2){
    	u32 = htonl(((Payload_2 *)pkt->payload)->num);
        memcpy(outbuf + size, &u32, 4);
        size += UI32SIZE;
        u32 = htonl(((Payload_2*)pkt->payload)->len);
        memcpy(outbuf+size, &u32, 4);
        size+= UI32SIZE;
        u32 = htonl(((Payload_2*)pkt->payload)->udp_port);
        memcpy(outbuf+size, &u32, 4);
        size+= UI32SIZE;
        u32 = htonl(((Payload_2*)pkt->payload)->secretA);
        memcpy(outbuf+size, &u32, 4);
        size+= UI32SIZE;

    }else if(stepCliente == 3){
    	u32 = htonl(((Payload_3 *)pkt->payload)->packet_id);
        memcpy(outbuf + size, &u32, 4);
        size+= UI32SIZE;
    	
    }else if(stepCliente == 4){
    	u32 = htonl(((Payload_4 *)pkt->payload)->acked_packet_id);
        memcpy(outbuf + size, &u32, 4);
        size+= UI32SIZE;
    	
    }else if(stepCliente == 5){
    	u32 = htonl(((Payload_5 *)pkt->payload)->tcp_port);
        memcpy(outbuf + size, &u32, 4);
        size += UI32SIZE;
        u32 = htonl(((Payload_5 *)pkt->payload)->secretB);
        memcpy(outbuf+size, &u32, 4);
        size+= UI32SIZE;
    	
    }else if(stepCliente == 6){
    	u32 = htonl(((Payload_6 *)pkt->payload)->num2);
        memcpy(outbuf + size, &u32, 4);
        size += UI32SIZE;
        u32 = htonl(((Payload_6 *)pkt->payload)->len2);
        memcpy(outbuf+size, &u32, 4);
        size+= UI32SIZE;
        u32 = htonl(((Payload_6 *)pkt->payload)->secretC);
        memcpy(outbuf + size, &u32, 4);
        size += UI32SIZE;
        u8 = ((Payload_6 *)pkt->payload)->c;
        memcpy(outbuf+size, &u8, 1);
        size+= UI8SIZE;
    	
    }else if(stepCliente == 7){
    	int len2 = htonl(pkt->cabecalho->payload_len);
    	strcpy(outbuf + size, ((Payload_7 *)pkt->payload)->payloadDeTamLen2);
    	size += len2;
    	
    }else if(stepCliente == 8){
    	u32 = htonl(((Payload_8 *)pkt->payload)->secretD);
        memcpy(outbuf + size, &u32, 4);
        size += UI32SIZE;

    }

    return size;
}

int Decode(Pacote *pkt, uint8_t *inbuf, size_t bufSize, int stepServidor){
    uint8_t u8 = 0; 
    uint16_t u16 = 0;
    uint32_t u32 = 0;

    alocaCabecalho(pkt);

    memcpy(&u32, inbuf + PAYLOAD_LEN_SHIFT, UI32SIZE);	
    pkt->cabecalho->payload_len = ntohl(u32);

    memcpy(&u32, inbuf + PSECRET_SHIFT, UI32SIZE);		
    pkt->cabecalho->psecret = ntohl(u32);

    memcpy(&u16, inbuf + STEP_SHIFT, UI16SIZE);	
    pkt->cabecalho->step = ntohs(u16);

    memcpy(&u16, inbuf + STUDENT_ID_SHIFT, UI16SIZE);	
    pkt->cabecalho->student_id = ntohs(u16);


	if (stepServidor == 1){
        alocaPayload1(pkt);

        memcpy(((Payload_1 *)pkt->payload)->msg, inbuf + TAM_CABECALHO, 12);

    }else if(stepServidor == 2){
    	alocaPayload2(pkt);
       
        memcpy(&u32, inbuf + TAM_CABECALHO, 4);
        ((Payload_2 *)pkt->payload)->num = ntohl(u32);

        memcpy(&u32, inbuf + TAM_LEN, 4);
        ((Payload_2 *)pkt->payload)->len = ntohl(u32);

        memcpy(&u32, inbuf + TAM_UDP, 4);
        ((Payload_2 *)pkt->payload)->udp_port = ntohl(u32);

        memcpy(&u32, inbuf + TAM_SECRETA, 4);
        ((Payload_2 *)pkt->payload)->secretA = ntohl(u32);

    }else if(stepServidor == 3){
    	alocaPayload3(pkt);

    	memcpy(&u32, inbuf + TAM_CABECALHO, 4);
        ((Payload_3 *)pkt->payload)->packet_id = ntohl(u32);
    	
    }else if(stepServidor == 4){
    	alocaPayload4(pkt);

    	memcpy(&u32, inbuf + TAM_CABECALHO, 4);
        ((Payload_4 *)pkt->payload)->acked_packet_id = ntohl(u32);
    	
    }else if(stepServidor == 5){
    	alocaPayload5(pkt);
       
        memcpy(&u32, inbuf + TAM_CABECALHO, 4);
        ((Payload_5 *)pkt->payload)->tcp_port = ntohl(u32);

        memcpy(&u32, inbuf + TAM_LEN, 4);
        ((Payload_5 *)pkt->payload)->secretB = ntohl(u32);
    	
    }else if(stepServidor == 6){
    	alocaPayload6(pkt);
       
        memcpy(&u32, inbuf + TAM_CABECALHO, 4);
        ((Payload_6 *)pkt->payload)->num2 = ntohl(u32);

        memcpy(&u32, inbuf + TAM_LEN, 4);
        ((Payload_6 *)pkt->payload)->len2 = ntohl(u32);

        memcpy(&u32, inbuf + TAM_UDP, 4);
        ((Payload_6 *)pkt->payload)->secretC = ntohl(u32);

        memcpy(&u8, inbuf + TAM_SECRETA, 1);
        ((Payload_6 *)pkt->payload)->c = u8;
   	
    }else if(stepServidor == 7){

    	
    	alocaPayload7(pkt, pkt->cabecalho->payload_len);

    	
    	strcpy(((Payload_7 *)pkt->payload)->payloadDeTamLen2, inbuf + TAM_CABECALHO);
    

    }else if(stepServidor == 8){
    	alocaPayload8(pkt);
       
        memcpy(&u32, inbuf + TAM_CABECALHO, 4);
        ((Payload_8 *)pkt->payload)->secretD = ntohl(u32);
    }    

    return 1;
}
