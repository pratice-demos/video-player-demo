/**********
* tcp�ͻ���
**********/

#include <iostream>
#include <string>
#include <winsock2.h>
#include "player.hpp"

using namespace std;

//������Ϣ����
#define MAX_MSG_SIZE 1024

#pragma comment (lib,"Ws2_32.lib")

//��ʼ���׽��ֿ�
void initWsaData() {
    WSADATA wsaData;
    WORD vCode = MAKEWORD(2, 2);
    int err = WSAStartup(vCode, &wsaData);
    if (err == -1) {
        cout << "��ʼ���׽��ֿ�ʧ��" << endl;
        exit(-1);
    }
    else {
        cout << "��ʼ���׽ӿ�ɹ�" << endl;
    }
    //����׽��ֿ�汾��
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wHighVersion) != 2) {
        cout << "�׽��ֿ�汾�Ų���" << endl;
        exit(-1);
    }
    else {
        cout << "�׽��ֿ�汾����ͨ��" << endl;
    }
}

// �����ļ�
void acceptFile(SOCKET sock) {
    //�������ļ��������ļ��Ƿ��ܴ����ɹ�
    const char* filename = "E:/code/C/video/video-player-2/video-player-2/demo.mp4";  //�ļ���
    FILE* fp = fopen(filename, "wb");  //�Զ����Ʒ�ʽ�򿪣��������ļ�
    if (fp == NULL) {
        cout << "�����ļ�ʧ��" << endl;
        exit(-1);
    }
    else {
        cout << "�����ļ��ɹ�" << endl;
    }

    //ѭ���������ݣ�ֱ���ļ��������
    char recvBuf[MAX_MSG_SIZE];
    memset(recvBuf, 0, MAX_MSG_SIZE);
    int nCount;
    while ((nCount = recv(sock, recvBuf, MAX_MSG_SIZE, 0)) > 0) {
        fwrite(recvBuf, nCount, 1, fp);
    }

    cout << "�ļ��������" << endl;

    fclose(fp);
}

//socketͨ��
void mainTcp() {
    initWsaData();

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == SOCKET_ERROR) {
        cout << "�����׽���ʧ��" << endl;
        exit(-1);
    }
    else {
        cout << "�����׽��ֳɹ�" << endl;
    }

    //���õ�ַ
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(9888);
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");

    //���ӷ�����
    int connStatus = connect(sock, (struct sockaddr*)&sin, sizeof(sin));
    if (connStatus == -1) {
        cout << "���ӷ�����ʧ��" << endl;
        exit(-1);
    }
    else {
        cout << "���ӳɹ�" << endl;
    }

    // �ӷ���˽����ļ�
    acceptFile(sock);

    // ����Ƶ������
    openPlayer();

    //�ر��׽��ּ��׽��ֿ�
    closesocket(sock);
    WSACleanup();
}

int main() {
    mainTcp();
    return 0;
}