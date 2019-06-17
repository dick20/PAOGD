# HW5 简单的服务端

#### 任务介绍
- 为贪吃蛇游戏添加多人游戏功能：多个玩家可以分别控制贪吃蛇在游戏区域里驰骋，避免碰到自己、同类或障碍物，尽可能地吃更多的食物以生长！

#### 开发环境
- OpenGL3
- GLFW
- GLAD
- C++

#### DeadLine： 6月14日（周五）晚22点

##### 功能介绍：
- 必须连接到服务器才能开始游戏，成功上线服务器返回“WELCOME USERNAME”的欢迎信息
- 当没有新的玩家上线时，玩家可以进行单人游戏，当游戏已经开始时第二个玩家无法上线，服务器返回“BUSY”
- 当两个以上玩家同时上线时，可以进行多人游戏，客户端界面出现不同颜色的蛇，分别由不同玩家进行控制
- 当某一玩家关闭连接，该玩家游戏失败
- 除了单机贪吃蛇的基本规则外，当蛇头接触到同类时，该玩家游戏失败，生存时间长的玩家获胜，用你的身体挡住对手的路吧！


\*实际上这是简化的服务器，更像是“房间”的概念

#### Tips ：
1. 关于网络拓扑，上述提到的是简单的Client-Server模型，游戏运行在“权威”服务器下，客户端想进行一个操作时，会发送动作请求的数据包给服务器，服务器统一处理这些请求并进行状态更新，再把更新结果反馈给客户端。
![输入图片说明](https://images.gitee.com/uploads/images/2019/0602/114027_b2fd1f31_1194012.png "屏幕截图.png")
2. 关于传输层协议，采用UDP进行传输可以使数据包的传输更加灵活，UDP socket是无连接的，必须先调用Bind函数保留一个端口，再使用ReceiveFrom取回传入的数据；UDP socket是无状态的，需要自定义流量控制以及拥塞控制等规则，以确保所有数据能够按预期进行处理。

```
class UDPSocket {
public:

	~UDPSocket();

	int Bind( const SocketAddress& inToAddress );
	int SendTo( const void* inToSend, int inLength, const SocketAddress& inToAddress );
	int ReceiveFrom( void* inToReceive, int inMaxLength, SocketAddress& outFromAddress );

private:
	friend class SocketUtil;
	UDPSocket( SOCKET inSocket ) : mSocket( inSocket ) {}
	SOCKET mSocket;

};
```
3. 关于动作队列，客户端以一定的频率轮询键盘，保存到动作队列中，并将所有或者部分（最新若干个）动作发送到服务端，如何判断动作执行的先后顺序需要为动作增加时间戳，而模拟步长跟服务器、客户端的帧步长怎么设置也是需要关心的。

```
void RoboCatServer::Update() {
	// Code omitted

	ClientProxyPtr client = NetworkManagerServer::sInstance->GetClientProxy( GetPlayerId() );
	if( client )
	{
		MoveList& moveList = client->GetUnprocessedMoveList();
		for( const Move& unprocessedMove : moveList )
		{
			const InputState& currentState = unprocessedMove.GetInputState();
			float deltaTime = unprocessedMove.GetDeltaTime();
			ProcessInput( deltaTime, currentState );
			SimulateMovement( deltaTime );
		}

		moveList.Clear();
	}
        // Code omitted
}
```





#### 要求：
1. 完成多人贪吃蛇游戏的C-S框架搭建（50%）
2. 多客户端能够分别连接服务器并有正确的响应（连接成功、连接失败）（20%）
3. 服务器能够正确处理客户端的动作请求，客户端能够正确更新蛇的移动状态（25%）
4. 蛇的碰撞检测、奖励生成、胜负判断等内容的实现（10%+5% with bonus）

#### 参考资料
1.《网络多人游戏架构与编程》
