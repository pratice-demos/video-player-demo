/**********
*  tcp�����
***********/

#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

// ������Ϣ����
#define MAX_MSG_SIZE 1024

#pragma comment (lib,"Ws2_32.lib")

// ��ʼ���׽��ֿ�
void initWsaData() {
    WSADATA wsaData;
    WORD vcode = MAKEWORD(2, 2);
    int err = WSAStartup(vcode, &wsaData);
    if (err == -1) {
        cout << "��ʼ���׽��ֿ�ʧ��" << endl;
    }
    else {
        cout << "��ʼ���׽��ֿ�ɹ�" << endl;
    }
    //���汾��
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wHighVersion) != 2) {
        cout << "�׽��ְ汾�Ų���" << endl;
        exit(-1);
    }
    else {
        cout << "�����׽��ְ汾��ͨ��" << endl;
    }
}

// �����ļ�
void sendFile(SOCKET sock) {
    // �ȼ���ļ��Ƿ����
    const char* filename = "E:/code/C/video/video-player-1/video-player-1/test.mp4";  //�ļ���
    FILE* fp = fopen(filename, "rb");  //�Զ����Ʒ�ʽ���ļ�
    if (fp == NULL) {
        cout << "���ļ�ʧ��" << endl;
        exit(-1);
    }
    else {
        cout << "���ļ��ɹ�" << endl;
    }

    // ��������
    char sendBuf[MAX_MSG_SIZE];
    memset(sendBuf, 0, MAX_MSG_SIZE);
    int nCount;
    while ((nCount = fread(sendBuf, 1, MAX_MSG_SIZE, fp)) > 0) {
        send(sock, sendBuf, nCount, 0);
    }

    shutdown(sock, SD_SEND);  //�ļ���ȡ��ϣ��Ͽ��������

    cout << "�������" << endl;

    recv(sock, sendBuf, MAX_MSG_SIZE, 0);  //�������ȴ��ͻ��˽������

    fclose(fp);
}

// socket ͨ��
void mainTcp() {
    initWsaData();

    //�����׽���
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    //������ַ�ض���
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(9888);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    //���׽���
    int bindStatus = bind(sock, (struct sockaddr*)&sin, sizeof(sin));
    if (bindStatus == -1) {
        cout << "�׽��ְ�ʧ��" << endl;
        exit(-1);
    }
    else {
        cout << "�׽��ְ󶨳ɹ�" << endl;
    }

    //���׽�����Ϊ����ģʽ���ȴ��ͻ�������
    int listenStatus = listen(sock, 128);
    if (listenStatus == -1) {
        cout << "����ʧ��" << endl;
        exit(-1);
    }
    else {
        cout << "���ü����ɹ�,��������ڼ�����..." << endl;
    }

    // �յ�����󣬽����������󣬷���һ����Ӧ�˴����ӵ����׽��֣���������
    sockaddr_in sinAccept;
    int len = sizeof(sin);
    SOCKET newSock = accept(sock, (struct sockaddr*)&sinAccept, &len);
    if (newSock == SOCKET_ERROR) {
        cout << "����ʧ��" << endl;
        exit(-1);
    }
    else {
        cout << "���ӽ����ɹ���׼����������" << endl;
    }

    // ���½������׽��ִ����ļ����ͻ���
    sendFile(newSock);

    //�ر��׽��ּ��׽��ֿ�
    closesocket(sock);
    closesocket(newSock);
    WSACleanup();
}

int main() {
    mainTcp();
    return 0;
}