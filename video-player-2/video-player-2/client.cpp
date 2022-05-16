/**********
* tcp客户端
**********/

#include <iostream>
#include <string>
#include <winsock2.h>
#include "player.hpp"

using namespace std;

//定义消息长度
#define MAX_MSG_SIZE 1024

#pragma comment (lib,"Ws2_32.lib")

//初始化套接字库
void initWsaData() {
    WSADATA wsaData;
    WORD vCode = MAKEWORD(2, 2);
    int err = WSAStartup(vCode, &wsaData);
    if (err == -1) {
        cout << "初始化套接字库失败" << endl;
        exit(-1);
    }
    else {
        cout << "初始化套接库成功" << endl;
    }
    //检查套接字库版本号
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wHighVersion) != 2) {
        cout << "套接字库版本号不符" << endl;
        exit(-1);
    }
    else {
        cout << "套接字库版本检验通过" << endl;
    }
}

// 接受文件
void acceptFile(SOCKET sock) {
    //先输入文件名，看文件是否能创建成功
    const char* filename = "E:/code/C/video/video-player-2/video-player-2/demo.mp4";  //文件名
    FILE* fp = fopen(filename, "wb");  //以二进制方式打开（创建）文件
    if (fp == NULL) {
        cout << "创建文件失败" << endl;
        exit(-1);
    }
    else {
        cout << "创建文件成功" << endl;
    }

    //循环接收数据，直到文件传输完毕
    char recvBuf[MAX_MSG_SIZE];
    memset(recvBuf, 0, MAX_MSG_SIZE);
    int nCount;
    while ((nCount = recv(sock, recvBuf, MAX_MSG_SIZE, 0)) > 0) {
        fwrite(recvBuf, nCount, 1, fp);
    }

    cout << "文件接受完毕" << endl;

    fclose(fp);
}

//socket通信
void mainTcp() {
    initWsaData();

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == SOCKET_ERROR) {
        cout << "创建套接字失败" << endl;
        exit(-1);
    }
    else {
        cout << "创建套接字成功" << endl;
    }

    //设置地址
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(9888);
    sin.sin_addr.s_addr = inet_addr("127.0.0.1");

    //连接服务器
    int connStatus = connect(sock, (struct sockaddr*)&sin, sizeof(sin));
    if (connStatus == -1) {
        cout << "连接服务器失败" << endl;
        exit(-1);
    }
    else {
        cout << "连接成功" << endl;
    }

    // 从服务端接受文件
    acceptFile(sock);

    // 打开视频播放器
    openPlayer();

    //关闭套接字及套接字库
    closesocket(sock);
    WSACleanup();
}

int main() {
    mainTcp();
    return 0;
}