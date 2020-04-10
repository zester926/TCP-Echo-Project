#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

void ShowErrorMessage(string message)
{
	cout << "[오류 발생]: " << message << '\n';
	system("pause");
	exit(1);
} // 네트워크 프로그래밍에서는 오류가 발생할 가능성이 높으므로 에러 핸들링에 관한
  // 함수를 작성해주어야 한다.

int main()
{
	WSADATA wsaData;
	SOCKET serverSocket, clientSocket; // 서버 프로그램에서는 서버 소켓 뿐만 아니라
	// 한명의 클라이언트에게 메세지를 보내기 위한 클라이언트 소켓 또한 필요하다.
	SOCKADDR_IN serverAddress, clientAddress;

	int serverPort = 9876;
	char received[256];

	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // Winsock을 초기화 한다.
		// 이때, MAKEWORD(2, 2) 는 2.2 버전의 구조체 작성을 의미하며, Winsock의 설정
		// 정보는 wsaData 변수에 저장된다.
		ShowErrorMessage("WSAStartup()");

	serverSocket = socket(PF_INET, SOCK_STREAM, 0); // TCP 소켓을 생성한다.
		// 이때, PF_INET 은 AF_INET 과 동일하다.

	if (serverSocket == INVALID_SOCKET)
		ShowErrorMessage("socket()");

	memset(&serverAddress, 0, sizeof(serverAddress));
	serverAddress.sin_family = AF_INET; // IPv4 형식 사용
	serverAddress.sin_addr.s_addr = htonl(INADDR_ANY);
	// 4 바이트 정수 네트워크 바이트 형식으로. 이때, l은 long을 의미
	serverAddress.sin_port = htons(serverPort);
	// 2 바이트 정수 네크워크 바이트 형식으로. 이때, s는 short를 의미
	if (bind(serverSocket, (SOCKADDR*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR)
		ShowErrorMessage("bind()");
	cout << "[현재 상태] bind()\n";

	if (listen(serverSocket, 5) == SOCKET_ERROR)
		ShowErrorMessage("listen()");
	cout << "[현재 상태] listen()\n";

	int sizeClientAddress = sizeof(clientAddress);

		clientSocket = accept(serverSocket, (SOCKADDR*)&clientAddress, &sizeClientAddress);
		cout << "[현재 상태] accept()\n";

	if (clientSocket == INVALID_SOCKET)
		ShowErrorMessage("accept()");
	while (1) // 클라이언트의 메세지를 받아서 그대로 다시 전달한다.
	{
		int length = recv(clientSocket, received, sizeof(received), 0);
		received[length] = NULL;
		cout << "[클라이언트 메세지]: " << received << '\n';
		cout << "[메세지 전송]: " << received << '\n';
		if (strcmp(received, "[exit]") == 0)
		{
			send(clientSocket, received, sizeof(received) - 1, 0);
			cout << "[서버 종료]\n";
			break;
		}
		send(clientSocket, received, sizeof(received) - 1, 0);
	}
}