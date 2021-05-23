#ifndef _WEBSOCKET_H_

#define _WEBSOCKET_H_

#define BUFFER 100


// Opcode Meaning Reference 
// 0 Continuation Frame [RFC6455]
#define OPC_CONT O
// 1 Text Frame [RFC6455]
#define OPC_TEXT 1
// 2 Binary Frame [RFC6455]
#define OPC_BIN 2
// 3-7 Unassigned
// 8 Connection Close Frame [RFC6455]
#define OPC_CLOSE 8
// 9 Ping Frame [RFC6455]
#define OPC_PING 9
// 10 Pong Frame [RFC6455]
#define OPC_PONG 10
// 11-15 Unassigned


struct
{
	unsigned int fin : 1;
	unsigned int rsv1 : 1;
	unsigned int rsv2 : 1;
	unsigned int rsv3 : 1;
	unsigned int mask : 1;
	unsigned int opcode : 4;
	unsigned int paylen : 7;
} ws_frame_16;

int read_websocket(int accept_fd);

#endif //_WEBSOCKET_H_

