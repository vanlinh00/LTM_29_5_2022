

//4.2 Vào ra sử dụng lập trình đa luồng
// client và server gửi và nhận dữ liệu đồng thời
/*
#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)

SOCKET clients[64];
int numClients = 0;
void RemoveClient(SOCKET client)
{
	// Tim vi tri cua client trong mang
	int i = 0;
	for (; i < numClients; i++)
		if (clients[i] == client) break;

	// Xoa client khoi mang
	if (i < numClients - 1)
		clients[i] = clients[numClients - 1];
	numClients--;
}
DWORD WINAPI ClientThread(LPVOID lpParam)
{
	SOCKET client = *(SOCKET*)lpParam;
	int ret;
	char buf[256];
	char cmd[32], id[32], tmp[32];

	// Xu ly dang nhap
	while (1)
	{
		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0)
			return 0;

		buf[ret] = 0;
		printf("Du lieu nhan duoc: %s\n", buf);
		ret = sscanf(buf, "%s %s %s", cmd, id, tmp);
		if (ret != 2)
		{
			const char* msg = "Sai cu phap. Hay nhap lai.\n";
			send(client, msg, strlen(msg), 0);
		}
		else
		{
			if (strcmp(cmd, "client_id:") != 0)
			{
				const char* msg = "Sai cu phap. Hay nhap lai.\n";
				send(client, msg, strlen(msg), 0);
			}
			else
			{
				const char* msg = "Dung cu phap. Nhap tin nhan de chuyen tiep.\n";
				send(client, msg, strlen(msg), 0);
				// Them vao mang
				clients[numClients] = client;
				numClients++;
				break;
			}
		}
	}

	// Chuyen tiep tin nhan
	while (1)
	{
		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0)
		{
			RemoveClient(client);
			return 0;
		}
		buf[ret] = 0;
		printf("Du lieu nhan duoc: %s\n", buf);
		char sbuf[256];
		sprintf(sbuf, "%s: %s", id, buf);
		for (int i = 0; i < numClients; i++)
			if (client != clients[i])
				send(clients[i], sbuf, strlen(sbuf), 0);
	}
	closesocket(client);
}
int main()
{
	// Khoi tao thu vien
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	// Tao socket
	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// Khai bao dia chi server
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(8000);
	// Gan cau truc dia chi voi socket
	bind(listener, (SOCKADDR*)&addr, sizeof(addr));
	// Chuyen sang trang thai cho ket noi
	listen(listener, 5);
	while (1)
	{
		SOCKET client = accept(listener, NULL, NULL);
		printf("Client moi ket noi: %d\n", client);
		CreateThread(0, 0, ClientThread, &client, 0, 0);
	}
}
*/

#include <winsock2.h>
#include <stdio.h>
#pragma comment(lib, "ws2_32")
#pragma warning(disable:4996)
DWORD WINAPI ClientThread(LPVOID lpParam)
{
	SOCKET client = *(SOCKET*)lpParam;
	int ret;
	char buf[256];
	char user[32], pass[32], tmp[32];
	// Xu ly dang nhap
	while (1)
	{
		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0)
			return 0;
		buf[ret] = 0;
		printf("Du lieu nhan duoc: %s\n", buf);
		ret = sscanf(buf, "%s %s %s", user, pass, tmp);
		if (ret != 2)
		{
			const char* msg = "Sai cu phap. Hay nhap lai.\n";
			send(client, msg, strlen(msg), 0);
		}
		else
		{
			FILE* f = fopen("C:\\test\\data.txt", "r");
			int found = 0;
			while (!feof(f))
			{
				char line[256];
				fgets(line, sizeof(line), f);
				char userf[32], passf[32];
				sscanf(line, "%s %s", userf, passf);
				if (strcmp(user, userf) == 0 && strcmp(pass, passf) == 0)
				{
					found = 1;
					break;
				}
			}
			fclose(f);
			if (found == 1)
			{
				const char* msg = "Dang nhap thanh cong. Hay nhap lenh.\n";
				send(client, msg, strlen(msg), 0);
				break;
			}
			else
			{
				const char* msg = "Khong tim thay tai khoan. Hay nhap lai.\n";
				send(client, msg, strlen(msg), 0);
			}
		}
	}
	// Thuc hien lenh
	while (1)
	{
		ret = recv(client, buf, sizeof(buf), 0);
		if (ret <= 0)
			return 0;
		buf[ret] = 0;
		printf("Du lieu nhan duoc: %s\n", buf);
		buf[ret - 1] = 0;
		char cmdbuf[256];
		sprintf(cmdbuf, "%s > c:\\test\\out.txt", buf);
		system(cmdbuf);
		FILE* f = fopen("c:\\test\\out.txt", "r");
		while (!feof(f))
		{
			char line[256];
			fgets(line, sizeof(line), f);
			send(client, line, strlen(line), 0);
		}
		fclose(f);
	}
	closesocket(client);
}
int main()
{
	// Khoi tao thu vien
	WSADATA wsa;
	WSAStartup(MAKEWORD(2, 2), &wsa);
	// Tao socket
	SOCKET listener = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	// Khai bao dia chi server
	SOCKADDR_IN addr;
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(8000);
	// Gan cau truc dia chi voi socket
	bind(listener, (SOCKADDR*)&addr, sizeof(addr));
	// Chuyen sang trang thai cho ket noi
	listen(listener, 5);
	while (1)
	{
		SOCKET client = accept(listener, NULL, NULL);
		printf("Client moi ket noi: %d\n", client);
		CreateThread(0, 0, ClientThread, &client, 0, 0);
	}
}