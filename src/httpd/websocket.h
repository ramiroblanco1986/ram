#ifndef _WEBSOCKET_H_

#define _WEBSOCKET_H_

#define BUFFER 4


// Opcode Meaning Reference 
// 0 Continuation Frame [RFC6455]
// 1 Text Frame [RFC6455]
// 2 Binary Frame [RFC6455]
// 3-7 Unassigned
// 8 Connection Close Frame [RFC6455]
// 9 Ping Frame [RFC6455]
// 10 Pong Frame [RFC6455]
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


#endif //_WEBSOCKET_H_

