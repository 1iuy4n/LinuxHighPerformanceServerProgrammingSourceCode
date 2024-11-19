#include <sys/socket.h> // 包含socket相关函数的头文件
#include <arpa/inet.h>  // 包含网络地址转换函数的头文件
#include <assert.h>     // 包含断言函数的头文件，用于调试
#include <stdio.h>      // 包含标准输入输出函数的头文件
#include <unistd.h>     // 包含POSIX操作系统API的头文件，如close函数
#include <string.h>     // 包含字符串操作函数的头文件，如memset和bzero
#include <stdlib.h>     // 包含标准库函数的头文件，如atoi和exit

#define BUFFER_SIZE 512 // 定义缓冲区大小为512字节

int main( int argc, char* argv[] ) // 主函数，argc是参数个数，argv是参数数组
{
    // 检查命令行参数个数是否正确
    if( argc <= 3 )
    {
        printf( "usage: %s ip_address port_number send_bufer_size\n", basename( argv[0] ) );
        return 1; // 如果参数个数不正确，打印使用方法并退出
    }
    const char* ip = argv[1]; // 第一个命令行参数是服务器的IP地址
    int port = atoi( argv[2] ); // 第二个命令行参数是服务器的端口号，使用atoi函数转换为整数

    // 定义服务器地址结构体
    struct sockaddr_in server_address;
    bzero( &server_address, sizeof( server_address ) ); // 将服务器地址结构体清零
    server_address.sin_family = AF_INET; // 设置地址族为IPv4
    inet_pton( AF_INET, ip, &server_address.sin_addr ); // 将IP地址转换为网络字节序并设置到服务器地址结构体
    server_address.sin_port = htons( port ); // 将端口号转换为网络字节序并设置到服务器地址结构体

    // 创建一个TCP socket
    int sock = socket( PF_INET, SOCK_STREAM, 0 );
    assert( sock >= 0 ); // 断言socket创建成功

    // 获取第三个命令行参数，设置TCP发送缓冲区大小
    int sendbuf = atoi( argv[3] );
    int len = sizeof( sendbuf );
    /* 先设置TCP发送缓冲区的大小，然后立即读取之 */
    setsockopt( sock, SOL_SOCKET, SO_SNDBUF, &sendbuf, sizeof( sendbuf ) ); // 设置socket选项
    getsockopt( sock, SOL_SOCKET, SO_SNDBUF, &sendbuf, ( socklen_t* )&len ); // 获取设置后的TCP发送缓冲区大小
    printf( "the tcp send buffer size after setting is %d\n", sendbuf ); // 打印TCP发送缓冲区大小

    // 尝试连接到服务器
    if ( connect( sock, ( struct sockaddr* )&server_address, sizeof( server_address ) ) != -1 )
    {
        char buffer[ BUFFER_SIZE ]; // 定义一个缓冲区
        memset( buffer, 'a', BUFFER_SIZE ); // 将缓冲区填充为'a'
        send( sock, buffer, BUFFER_SIZE, 0 ); // 发送缓冲区数据到服务器
    }

    close( sock ); // 关闭socket
    return 0; // 程序正常退出
}