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

	int porta = DEFAULT;

	int socketServer;
	struct sockaddr_in serverAddr, sockCliente;
	uint8_t inbuf[BUFSIZE];

	srand(time(NULL));
	pid_t filho;
	int contFilho = 1;

					    
	socketServer = socket(AF_INET, SOCK_DGRAM, 0);
	if(socketServer < 0){
		printf("Erro Criacao Socket UDP Servidor\n");
		 return 1;
	}
	
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET; 
	serverAddr.sin_port = htons(porta);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  

	Pacote *pacote = malloc(sizeof(Pacote)); 
	uint32_t psecret, num, len;
	uint32_t gerarPorta;

	if(bind(socketServer, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
		DieWithSystemMessage("Fail\n"); 
		return 1;
	}
	socklen_t lenSockCliente = sizeof(sockCliente);
		printf("|------------------------------------|\n");
		printf("|*            Bem-Vindo o/          *|\n");
		printf("|*        Aguardando cliente        *|\n");
    	printf("|------------------------------------|\n");
	while(1){
		ssize_t numBytesRcvd = recvfrom(socketServer, inbuf, BUFSIZE, 0,
		 (struct sockaddr*)&sockCliente, &lenSockCliente);
		if(numBytesRcvd < 0){
			DieWithSystemMessage("Fail\n");
		}

		memset(pacote,0,sizeof(pacote));
		Decode(pacote, inbuf, BUFSIZE, 1);
		printf("|----------------------------------------------------|\n");
		printf("|O Cliente no (Endereço -> %s/ Port -> %d) |\n", inet_ntoa(sockCliente.sin_addr), ntohs(sockCliente.sin_port));
		printf("|----------------------------------------------------|\n\n");

		
		printf("|---------------------------|\n");
     	printf("|        Recebendo A1       |\n");
     	printf("|---------------------------|\n");
      	printf("|        %s        |\n", ((Payload_1 *)pacote->payload)->msg);
      	printf("|---------------------------|\n\n");

		// realizando comparação da string com o que foi passado, só pra continuar...
		if(strcmp(((Payload_1*)pacote->payload)->msg,"Hello World") == 0){

  			if((filho = fork()) == 0){

			
				int socketServerUDP = socket(AF_INET, SOCK_DGRAM, 0); 
				if(socketServerUDP < 0){
					printf("Processo %d) Fail\n",contFilho); 
					return 1;
				}
		
				gerarPorta = randx(10000, 20000);
				
				serverAddr.sin_port = htons(gerarPorta); 

				if(bind(socketServerUDP, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
					DieWithSystemMessage("Fail\n"); 
					return 1;
				}

				srand(time(NULL));
				psecret = randx(1, 9999); 
				printf("|-------------------------------------------|\n");
				printf("|Cliente %d)  Numero Aleatorio SecretA = %d|\n", contFilho, psecret);
				printf("|-------------------------------------------|\n\n");

				srand(time(NULL));
				num = randx(1, 4); 

				atribuiCabecalho(pacote,16,psecret,2);
				atribuiPayload2(pacote, num, gerarPorta, psecret);
				Encode(pacote, inbuf, BUFSIZE, 2);

				ssize_t numBytesSnd = sendto(socketServerUDP, inbuf, sizeof(inbuf), 0, (struct sockaddr *) &sockCliente, lenSockCliente);
	  			if (numBytesSnd < 0){ 
					  DieWithSystemMessage("Fail\n");
				}    
	  			else if (numBytesSnd != sizeof(inbuf)){
					   printf("Enviou mais ou menos bytes\n"); 
				}
				printf("|---------------------------------------------------------------------------------------|\n");
	  			printf("|Cliente %d) Servidor enviou A2 para o cliente no (Endereço -> %s / Port -> %d)|\n", contFilho, inet_ntoa(sockCliente.sin_addr), ntohs(sockCliente.sin_port));
				printf("|---------------------------------------------------------------------------------------|\n\n");
	  			liberaPayload2(pacote);

  				
  					for(int i = 0; i < num; i++){
  						memset(&inbuf, '\0', BUFSIZE);
  						numBytesRcvd = recvfrom(socketServerUDP, inbuf, BUFSIZE, 0, 
						  (struct sockaddr*)&sockCliente, &lenSockCliente);
						if(numBytesRcvd < 0){
							DieWithSystemMessage("Fail\n");
							return 1;
						}

						memset(pacote,0,sizeof(pacote));
						Decode(pacote, inbuf, BUFSIZE, 3);

						printf("|---------------------------------------------------------------------------------------|\n");
						printf("|Cliente %d) O servidor recebeu o Pacote %d do (Endereço -> %s / Port -> %d)    |\n", contFilho, i, inet_ntoa(sockCliente.sin_addr), ntohs(sockCliente.sin_port));
						
						atribuiCabecalho(pacote,4,psecret,2);
						atribuiPayload4(pacote, i);
						Encode(pacote, inbuf, BUFSIZE, 4);

						numBytesSnd = sendto(socketServerUDP, inbuf, BUFSIZE, 0, (struct sockaddr *) &sockCliente, lenSockCliente);
  						if (numBytesSnd < 0){
							 DieWithSystemMessage("Fail\n");
						}    
  						else if (numBytesSnd != sizeof(inbuf)){
							 DieWithSystemMessage("Fail\n");
						}
  						printf("|Cliente %d) O servidor enviou o ACK %d para o (Endereço -> %s / Port-> %d)     |\n",  contFilho, i, inet_ntoa(sockCliente.sin_addr), ntohs(sockCliente.sin_port));
						printf("|---------------------------------------------------------------------------------------|\n\n");
  					}
					printf("|-------------------------------------|\n");
  					printf("|Cliente %d) Recebeu todos os pacotes  |\n", contFilho);
  					printf("|-------------------------------------|\n\n");
					
					printf("|--------------------------|\n");
  					printf("|       Enviando B2        |\n");
  					printf("|--------------------------|\n\n");

  					srand(time(NULL));
  					int socketServerTCP = socket(AF_INET, SOCK_STREAM, 0);
  					if(socketServerTCP < 0){
						 DieWithSystemMessage("Fail criação de socket TCP\n");
						  return 1;
					}

  					int portaTCPAleatoria = randx(1000, 9000);
					serverAddr.sin_port = htons(portaTCPAleatoria);

					if(bind(socketServerTCP, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
							DieWithSystemMessage("Fail\n");
						 return 1;
					}
					if(listen(socketServerTCP,10) < 0){
						DieWithSystemMessage("Fail\n");
						 return 1;
					}

					uint32_t secretB = randx(1, 1000);

					printf("|-------------------------------------------|\n");
					printf("|Cliente %d)  Numero Aleatorio SecretB = %d |\n", contFilho, secretB);
					printf("|-------------------------------------------|\n\n");

					atribuiCabecalho(pacote,8,psecret,2);
					atribuiPayload5(pacote, portaTCPAleatoria, secretB);
					Encode(pacote, inbuf, BUFSIZE, 5);

					numBytesSnd = sendto(socketServerUDP, inbuf, BUFSIZE, 0, (struct sockaddr *) &sockCliente, lenSockCliente);
  					if (numBytesSnd < 0){ 
						  DieWithSystemMessage("Fail\n");
				 	}    
  					else if (numBytesSnd != sizeof(inbuf)){
						   DieWithSystemMessage("Fail\n");
					}
  					
					printf("|---------------------------------------------------------------------------------------|\n");
	  				printf("|Cliente %d) Servidor enviou B2 para o cliente no (Endereço -> %s / Port -> %d)|\n", contFilho, inet_ntoa(sockCliente.sin_addr), ntohs(sockCliente.sin_port));
					printf("|---------------------------------------------------------------------------------------|\n\n");
	  			
					close(socketServer);
					
					printf("|----------------------------------------|\n");
					printf("|Iniciando a conexão TCP em 3 segundos...|\n");
					printf("|----------------------------------------|\n\n");
					sleep(3);

				
					struct sockaddr_in novoAddr; 
					socklen_t lenNovoAddr = sizeof(novoAddr);
					int novoSocket = accept(socketServerTCP, 
					(struct sockaddr *) &novoAddr, &lenNovoAddr); 

					psecret = randx(1, 1000); 
					printf("|-------------------------------------------|\n");
					printf("|Cliente %d)  Numero Aleatorio SecretC = %d |\n", contFilho, psecret);
					
					atribuiCabecalho(pacote,13+3,psecret,2);						

					num = randx(1, 4); 

					char c = randxL();
					printf("|Cliente %d) Letra Aleatoria C = (%c)         |\n", contFilho, c);
					printf("|-------------------------------------------|\n\n");

					atribuiPayload6(pacote, num, psecret, c);
					memset(&inbuf, '\0', BUFSIZE);
					Encode(pacote, inbuf, BUFSIZE, 6);										

					send(novoSocket, inbuf, sizeof(inbuf), 0);

					
					for(int i = 0; i < num; i++){
						
						recv(novoSocket, inbuf, BUFSIZE, 0);
						Decode(pacote, inbuf, BUFSIZE, 7);
					
						printf("|------------------------------------------------------------------------|\n");
						printf("|Completar com a letra Aleatória até o tamanho de Len = %s |\n",((Payload_7*)pacote->payload)->payloadDeTamLen2);
						printf("|------------------------------------------------------------------------|\n\n");
					}		
					sockCliente.sin_port = ((Payload_5*)pacote->payload)->tcp_port;
					printf("|------------------------------------------------------------------------------------------|\n");
					printf("|Cliente %d) O servidor recebeu todos os %d Pacotes do (Endereço -> %s / Port -> %d)|\n", contFilho, num, inet_ntoa(sockCliente.sin_addr), ntohs(sockCliente.sin_port));
					printf("|------------------------------------------------------------------------------------------|\n\n");

				
					printf("|---------------------------------------------------------------------------------------|\n");
	  				printf("|Cliente %d) Servidor enviou D2 para o cliente no (Endereço -> %s / Port -> %d)|\n", contFilho, inet_ntoa(sockCliente.sin_addr), ntohs(sockCliente.sin_port));
					printf("|---------------------------------------------------------------------------------------|\n\n");

					psecret = randx(1, 1000); 
					printf("|-------------------------------------------|\n");
					printf("|Cliente %d)  Numero Aleatorio SecretD = %d |\n", contFilho, psecret);
					printf("|-------------------------------------------|\n\n");

					atribuiCabecalho(pacote, 4, 8001, 2);
					atribuiPayload8(pacote, psecret);
					memset(&inbuf, '\0', BUFSIZE);
					Encode(pacote, inbuf, BUFSIZE, 8);										

					send(novoSocket, inbuf, sizeof(inbuf), 0);

					close(novoSocket);
					close(socketServerTCP);

					printf("Protocolo finalizado com sucesso...\n");
					
  					return 1; 
				
  			}else{
  				contFilho++;
  			}
  		}else{
  			DieWithSystemMessage("Esperado Hello World, envie novamente \n");
  		}
	}
	return 1;
}