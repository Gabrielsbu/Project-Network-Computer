#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/socket.h>

#include <string.h>
#include <sys/types.h>
#include <netdb.h>

#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "packets.h"
#include "packet.h"
#include "practical.h"

int main(int argc, char **argv) {

	int porta = atoi(argv[1]);
	//Criando conexão do tipo UDP

	int socketCliente;
	struct sockaddr_in serverAddr;
	uint8_t outbuf[BUFSIZE];

	srand(time(NULL));

	socketCliente = socket(AF_INET, SOCK_DGRAM, 0);

	if(socketCliente < 0){
		DieWithSystemMessage("Fail\n");
		return 1;
	}
	//Configurando a estrutura UDP
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;  // Familia Ipv4..
	serverAddr.sin_port = htons(porta); //Porta gerada lá em cima
	serverAddr.sin_addr.s_addr = inet_addr("0"); //Endereço padrão 0.0.0.0

	Pacote *pacote = malloc(sizeof(Pacote)); //Aloco dinamicamente / Estancio, o pacote;
	uint32_t psecret; // variavel que vai me auxiliar ao mandar os segredos
	//Criando header para A1...
	alocaCabecalho(pacote);
	pacote->cabecalho->payload_len = 16;
	pacote->cabecalho->psecret = 7878;
	pacote->cabecalho->step = 1;
	pacote->cabecalho->student_id = 353;

	//Atribuindo payload 1.
	atribuiPayload1(pacote);

	//Realizando Encode para enviar o pacote Net/host
	Encode(pacote, outbuf, BUFSIZE, 1);
	//Configurandod envio.
	ssize_t numBytesSnd = sendto(socketCliente, outbuf, BUFSIZE, 0, 
	(struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(numBytesSnd < 0){
		DieWithSystemMessage("Fail\n");
	}
	printf("|------------------------------------|\n");
    printf("|*      Enviando Hello World...     *|\n");
	
	
	liberaPayload1(pacote);
	

	memset(&outbuf,'\0',BUFSIZE);
	socklen_t lenSockS = sizeof(serverAddr);
	ssize_t numBytesRcvd = recvfrom(socketCliente, outbuf, BUFSIZE, 0,
	 (struct sockaddr*)&serverAddr, &lenSockS);
	if(numBytesRcvd < 0){
		DieWithSystemMessage("Fail\n");
	}

	memset(pacote,0,sizeof(pacote));
	Decode(pacote, outbuf, BUFSIZE, 2);

	printf("|-------------------------------------------------------------|\n");
	printf("|O Servidor -> (Endereço -> %s/ Port -> %d) enviou  |\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));
	printf("|-------------------------------------------------------------|\n\n");

	printf("|------------------------------------|\n");
	printf("|           Recebendo A2             |\n");

	printf("|        Num : %d                     |\n",((Payload_2*)pacote->payload)->num);
    printf("|        Len : %d                    |\n",((Payload_2*)pacote->payload)->len);
    printf("|        UDP Port : %d            |\n",((Payload_2*)pacote->payload)->udp_port);
    printf("|        SecretA = %d               |\n\n",((Payload_2*)pacote->payload)->secretA);
	printf("|------------------------------------|\n\n");

	int lenPacote = ((Payload_2*)pacote->payload)->len;
	int numeroPacotes = ((Payload_2*)pacote->payload)->num; 
	
	printf("|---------------------------------|\n");
	printf("|    Enviar %d Pacotes             |\n", numeroPacotes);
	printf("|---------------------------------|\n\n");
	liberaPayload2(pacote);

	for(int i = 0; i < numeroPacotes; i++){

	
		atribuiCabecalho(pacote,lenPacote,psecret,1);
		atribuiPayload3(pacote,i);

		Encode(pacote, outbuf, BUFSIZE, 3);

		ssize_t numBytesSnd = sendto(socketCliente, outbuf, BUFSIZE, 0, 
		(struct sockaddr*)&serverAddr, sizeof(serverAddr));
		if(numBytesSnd < 0){
			DieWithSystemMessage("Fail\n");
		}
		printf("|-----------------------------------------------------------------------------|\n");
		printf("|Enviando pacote %d para o servidor no (Endereço -> %s / Port-> %d): %s|\n", i, inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port), outbuf);
		printf("|-----------------------------------------------------------------------------|\n");
		memset(&outbuf,'\0',BUFSIZE);
		numBytesRcvd = recvfrom(socketCliente, outbuf, BUFSIZE, 0, 
		(struct sockaddr*)&serverAddr, &lenSockS);
		if(numBytesRcvd < 0){
			DieWithSystemMessage("Fail\n");
		}

		memset(pacote,0,sizeof(pacote));
		Decode(pacote, outbuf, BUFSIZE, 4);
	
		printf("|O Servidor enviou o ACK do pacote = %d                                        |\n",((Payload_4*)pacote->payload)->acked_packet_id);
		printf("|-----------------------------------------------------------------------------|\n\n");
	}
	printf("|-------------------------------------------------------|\n");
	printf("|Todos os  %d pacotes foram enviados com sucesso V       |\n",numeroPacotes);
	printf("|-------------------------------------------------------|\n\n");

	liberaPayload4(pacote);
	
	
	numBytesRcvd = recvfrom(socketCliente, outbuf, BUFSIZE, 0, 
	(struct sockaddr*)&serverAddr, &lenSockS);
	if(numBytesRcvd < 0){
		DieWithSystemMessage("Fail\n");
	}
	printf("|---------------------------------------------------------|\n");
	printf("|O servidor no (Endereço -> %s /Port %d) enviou |\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));
	printf("|---------------------------------------------------------|\n\n");
	
	printf("|-----------------------------------------|\n");
	printf("|              Recebendo B2               |\n");
	

	memset(pacote,0,sizeof(pacote));
	Decode(pacote, outbuf, BUFSIZE, 5);

	
	printf("|         Tcp Port : %d                |\n",((Payload_5*)pacote->payload)->tcp_port);
    printf("|         Secret B : %d                  |\n",((Payload_5*)pacote->payload)->secretB);
	printf("|-----------------------------------------|\n\n");
	
	
	close(socketCliente);

	int socketClienteTCP = socket(AF_INET, SOCK_STREAM, 0);
  	if(socketClienteTCP < 0){
		  DieWithSystemMessage("Fail\n");
		   return 1;
	}

	int portaTCPAleatoria = ((Payload_5*)pacote->payload)->tcp_port;
	serverAddr.sin_port = htons(portaTCPAleatoria);

	if(connect(socketClienteTCP, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
		DieWithSystemMessage("Fail\n"); 
		return 1;
	}

	
	recv(socketClienteTCP, outbuf, BUFSIZE, 0);
	printf("|---------------------------------------------------------------------------------|\n");
	printf("|O servidor enviou o Pacote B2 para o cliente (Endereço -> %s/ Port -> %d)| \n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));
	printf("|---------------------------------------------------------------------------------|\n\n");
	memset(pacote,0,sizeof(pacote));
	Decode(pacote, outbuf, BUFSIZE, 6);
	
	printf("|------------------------------------|\n");
	printf("|           Recebendo C2             |\n");

	printf("|        Num2 : %d                    |\n",((Payload_6*)pacote->payload)->num2);
    printf("|        Len2: %d                    |\n",((Payload_6*)pacote->payload)->len2);
    printf("|        Secret C : %d              |\n",((Payload_6*)pacote->payload)->secretC);
    printf("|        Character = %c               |\n",((Payload_6*)pacote->payload)->c);
	printf("|------------------------------------|\n\n");
	

	
	
	numeroPacotes = ((Payload_6*)pacote->payload)->num2;
	int len2 = ((Payload_6*)pacote->payload)->len2; 
	char caractere = ((Payload_6*)pacote->payload)->c;

	for(int i = 0; i < numeroPacotes; i++){
		atribuiCabecalho(pacote,len2,8000+i,1); 
		atribuiPayload7(pacote,len2,caractere);

		memset(&outbuf,'\0',BUFSIZE);
		Encode(pacote, outbuf, BUFSIZE, 7);
		send(socketClienteTCP,outbuf,BUFSIZE, 0);
			
	}
	
	printf("|------------------------------------------------------------|\n");
	printf("|Todos os  %d pacotes TCPS foram enviados com sucesso V       |\n",numeroPacotes);
	printf("|------------------------------------------------------------|\n\n");
	

	memset(&outbuf,'\0',BUFSIZE);
	recv(socketClienteTCP, outbuf, BUFSIZE, 0);
	
	printf("|---------------------------------------------------------|\n");
	printf("|O servidor no (Endereço -> %s /Port %d) enviou |\n", inet_ntoa(serverAddr.sin_addr), ntohs(serverAddr.sin_port));
	printf("|---------------------------------------------------------|\n\n");
	
	printf("|-----------------------------------------|\n");
	printf("|               Passo final               |\n");
	memset(pacote,0,sizeof(pacote));
	Decode(pacote, outbuf, BUFSIZE, 8);

	printf("|             Secret D : %d              |\n",((Payload_8*)pacote->payload)->secretD);
	printf("|-----------------------------------------|\n\n");
	
	
	while(1);
		
	close(socketClienteTCP);
	return 1;	
}
