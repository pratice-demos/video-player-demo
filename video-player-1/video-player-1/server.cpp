/**********
*  tcp服务端
***********/

#include <iostream>
#include <string>
#include <winsock2.h>

using namespace std;

// 定义消息长度
#define MAX_MSG_SIZE 1024

#pragma comment (lib,"Ws2_32.lib")

// 初始化套接字库
void initWsaData() {
    WSADATA wsaData;
    WORD vcode = MAKEWORD(2, 2);
    int err = WSAStartup(vcode, &wsaData);
    if (err == -1) {
        cout << "初始化套接字库失败" << endl;
    }
    else {
        cout << "初始化套接字库成功" << endl;
    }
    //检测版本号
    if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wHighVersion) != 2) {
        cout << "套接字版本号不符" << endl;
        exit(-1);
    }
    else {
        cout << "检验套接字版本号通过" << endl;
    }
}

// 传输文件
void sendFile(SOCKET sock) {
    // 先检查文件是否存在
    const char* filename = "E:/code/C/video/video-player-1/video-player-1/test.mp4";  //文件名
    FILE* fp = fopen(filename, "rb");  //以二进制方式打开文件
    if (fp == NULL) {
        cout << "打开文件失败" << endl;
        exit(-1);
    }
    else {
        cout << "打开文件成功" << endl;
    }

    // 缓存数组
    char sendBuf[MAX_MSG_SIZE];
    memset(sendBuf, 0, MAX_MSG_SIZE);
    int nCount;
    while ((nCount = fread(sendBuf, 1, MAX_MSG_SIZE, fp)) > 0) {
        send(sock, sendBuf, nCount, 0);
    }

    shutdown(sock, SD_SEND);  //文件读取完毕，断开输出流，

    cout << "传输完毕" << endl;

    recv(sock, sendBuf, MAX_MSG_SIZE, 0);  //阻塞，等待客户端接收完毕

    fclose(fp);
}

// socket 通信
void mainTcp() {
    initWsaData();

    //创建套接字
    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    //创建地址簇对象
    sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_port = htons(9888);
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
    //绑定套接字
    int bindStatus = bind(sock, (struct sockaddr*)&sin, sizeof(sin));
    if (bindStatus == -1) {
        cout << "套接字绑定失败" << endl;
        exit(-1);
    }
    else {
        cout << "套接字绑定成功" << endl;
    }

    //将套接字设为监听模式，等待客户端连接
    int listenStatus = listen(sock, 128);
    if (listenStatus == -1) {
        cout << "监听失败" << endl;
        exit(-1);
    }
    else {
        cout << "设置监听成功,服务端正在监听中..." << endl;
    }

    // 收到请求后，接收连接请求，返回一个对应此次连接的新套接字，接受连接
    sockaddr_in sinAccept;
    int len = sizeof(sin);
    SOCKET newSock = accept(sock, (struct sockaddr*)&sinAccept, &len);
    if (newSock == SOCKET_ERROR) {
        cout << "连接失败" << endl;
        exit(-1);
    }
    else {
        cout << "连接建立成功，准备接收数据" << endl;
    }

    // 用新建立的套接字传输文件给客户端
    sendFile(newSock);

    //关闭套接字及套接字库
    closesocket(sock);
    closesocket(newSock);
    WSACleanup();
}

int main() {
    mainTcp();
    return 0;
}