// Реализация NetworkClient для Windows
#include <string>
#include <iostream>
#include <stdexcept>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "NetworkClient.h"

#pragma comment(lib, "ws2_32.lib")

NetworkClient::NetworkClient(const std::string& ip, int port)
	: server_ip(ip), port(port), sock(INVALID_SOCKET) {}

NetworkClient::~NetworkClient() {
	if (sock != INVALID_SOCKET) {
		closesocket(sock);
		std::cerr << "Соединение с сервером закрыто" << std::endl;
	}
	WSACleanup();
}

void NetworkClient::connecting() {
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2,2), &wsaData) != 0)
		throw std::runtime_error("Ошибка инициализации WinSock");

	sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sock == INVALID_SOCKET)
		throw std::runtime_error("Не удалось создать сокет");

	sockaddr_in serv_addr;
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	if (inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr) <= 0) {
		closesocket(sock);
		throw std::runtime_error("Некорректный адрес сервера");
	}

	if (connect(sock, (sockaddr*)&serv_addr, sizeof(serv_addr)) == SOCKET_ERROR) {
		closesocket(sock);
		throw std::runtime_error("Ошибка подключения");
	}
}

void NetworkClient::sendMess(const std::string& message) {
	if (send(sock, message.c_str(), message.size(), 0) == SOCKET_ERROR)
		throw std::runtime_error("Ошибка отправки");
}

std::string NetworkClient::getMess() {
	char buffer[1024] = {0};
	int bytes_read = recv(sock, buffer, sizeof(buffer), 0);

	if (bytes_read == SOCKET_ERROR) throw std::runtime_error("Ошибка чтения");
	if (bytes_read == 0) throw std::runtime_error("Сервер закрыл соединение");

	return std::string(buffer, bytes_read);
}
