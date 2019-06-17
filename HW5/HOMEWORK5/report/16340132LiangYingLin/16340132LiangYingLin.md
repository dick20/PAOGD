# 中山大学数据科学与计算机学院本科生实验报告

## （2019年春季学期）

| 课程名称 |    PAOGD    |   任课老师   |           郑贵锋           |
| :------: | :---------: | :----------: | :------------------------: |
|   年级   |     16      | 专业（方向） | 软件工程（计算机应用方向） |
|   学号   |  16340132   |     姓名     |           梁颖霖           |
|   电话   | 13680473185 |    Email     |        dic0k@qq.com        |
| 开始日期 |  2019/6/8   |   完成日期   |         2019/5/15          |

------

## 一、实验题目

**HW5 简单的服务端**

## 二、实现内容

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

## 三、实验结果

#### 1. 客户端

定义一个UDPSocket需要bind绑定一个端口，并在这个端口根据socket的地址来收取sendTo或者发送ReceiveFrom消息。该类还包含了一个友元工具类SocketUtil来辅助获取socket地址，获取时间戳等。

```cpp
class UDPSocket
{
public:

	~UDPSocket();
	
	int Bind( const SocketAddress& inToAddress );
	int SendTo( const void* inToSend, int inLength, const SocketAddress& inToAddress );
	int ReceiveFrom( void* inToReceive, int inMaxLength, SocketAddress& outFromAddress );

	int SetNonBlockingMode( bool inShouldBeNonBlocking );

private:
	friend class SocketUtil;
	UDPSocket( SOCKET inSocket ) : mSocket( inSocket ) {}
	SOCKET mSocket;

};
```

下面基于`	#include "WinSock2.h"`的库函数来实现三个重要的方法，用以后面传递信息以及客户端与服务器的建立连接。

实现Bind方法：根据绑定的ip地址来绑定端口，并返回错误。

```cpp
int UDPSocket::Bind( const SocketAddress& inBindAddress )
{
	int error = bind( mSocket, &inBindAddress.mSockAddr, inBindAddress.GetSize() );
	if( error != 0 )
	{
		SocketUtil::ReportError( "UDPSocket::Bind" );
		return SocketUtil::GetLastError();
	}
	
	return NO_ERROR;
}
```

实现SendTo方法，发送的参数包括ip地址，发送长度，具体的内容。

```cpp
int UDPSocket::SendTo( const void* inToSend, int inLength, const SocketAddress& inToAddress )
{
	int byteSentCount = sendto( mSocket,
							   static_cast< const char* >( inToSend ),
							   inLength,
							   0, &inToAddress.mSockAddr, inToAddress.GetSize() );
	if( byteSentCount <= 0 )
	{
		//we'll return error as negative number to indicate less than requested amount of bytes sent...
		SocketUtil::ReportError( "UDPSocket::SendTo" );
		return -SocketUtil::GetLastError();
	}
	else
	{
		return byteSentCount;
	}
}
```

实现ReceiveFrom方法，接受的参数包括ip地址，发送长度，具体的内容。

```cpp
int UDPSocket::ReceiveFrom( void* inToReceive, int inMaxLength, SocketAddress& outFromAddress )
{
	socklen_t fromLength = outFromAddress.GetSize();
	
	int readByteCount = recvfrom( mSocket,
								 static_cast< char* >( inToReceive ),
								 inMaxLength,
								 0, &outFromAddress.mSockAddr, &fromLength );
	if( readByteCount >= 0 )
	{
		return readByteCount;
	}
	else
	{
		int error = SocketUtil::GetLastError();
		if( error == WSAEWOULDBLOCK )
		{
			return 0;
		}
		else if( error == WSAECONNRESET )
		{
			LOG( "Connection reset from %s", outFromAddress.ToString().c_str() );
			return -WSAECONNRESET;
		}
		else
		{
			SocketUtil::ReportError( "UDPSocket::ReceiveFrom" );
			return -error;
		}
	}
}
```

**工具类：**

```cpp
enum SocketAddressFamily
{
	INET = AF_INET,
	INET6 = AF_INET6
};

class SocketUtil
{
public:
	static bool StaticInit();
	static void	CleanUp();
	
    // 返回接口的报错信息
	static void	ReportError( const char* inOperationDesc );
	static int GetLastError();

	static int Select( const vector< TCPSocketPtr >* inReadSet,
                      vector< TCPSocketPtr >* outReadSet,
                      const vector< TCPSocketPtr >* inWriteSet,
                      vector< TCPSocketPtr >* outWriteSet,
                      const vector< TCPSocketPtr >* inExceptSet,
                      vector< TCPSocketPtr >* outExceptSet );

	static UDPSocketPtr	CreateUDPSocket( SocketAddressFamily inFamily );
};
```

该工具类是模仿RoboCat中的，但是我们的贪吃蛇不需要用到TCP，且可以简单的默认网络为IPV4，并除去报错之类的辅助函数，于是只保留了上述几个接口。

#### 2. 服务端

服务端需要处理的业务包括

+ 判断客户端的socket地址来决定用户的id
+ 为用户创造一个游戏实例
+ 处理客户端发过来的**动作请求**
+ 返回更新的情况

定义一个Server类，处理用户的连接请求。

```cpp
class Server : public Engine
{
public:
	static bool StaticInit();
	virtual void DoFrame() override;
	virtual int Run();
	// 处理一个新的用户建立连接
	void HandleNewClient( ClientProxyPtr inClientProxy );
	// 处理用户关闭连接的情况
	void HandleLostClient( ClientProxyPtr inClientProxy );
	// 为用户创建一个贪吃蛇的实例
	Snake	GetSnakeForPlayer( int inPlayerId );
	void	SpawnSnakeForPlayer( int inPlayerId );
private:
	Server();
	bool	InitNetworkManager();
	void	SetupWorld();
};
```

这里主要讲述如何建立连接，以及处理断开连接的情况

```cpp
void Server::HandleNewClient( ClientProxyPtr inClientProxy )
{
    // 获取用户的id
	int playerId = inClientProxy->GetPlayerId();
    // 添加为用户添加一个用户实例
	ScoreBoardManager::sInstance->AddEntry( playerId, inClientProxy->GetName() );
	SpawnCatForPlayer( playerId );
    // 返回创建信息给客户端，告诉客户端
    SendMessage("WELCOME Player", playerId, inClientProxy);
}
```

断开连接的时候

```cpp
void Server::HandleLostClient( ClientProxyPtr inClientProxy )
{
	// 获取用户id
	int playerId = inClientProxy->GetPlayerId();
    // 移除游戏实例
	ScoreBoardManager::sInstance->RemoveEntry( playerId );
	SnakePtr snake = GetSnakeForPlayer( playerId );
    // 杀掉蛇
	if( snake.status )
	{
		snake->SetDoesWantToDie( true );
	}
    // 返回创建信息给客户端，告诉客户端
    SendMessage("YOU LOSS", playerId, inClientProxy);
}
```

#### 3. 服务端处理客户端动作请求

客户端发送的数据，由于贪吃蛇的控制只有上下左右四个方向，故用户发送的数据只包括WASD四个字母，并且要附带**当前的时间戳**，以一个moveList的方式来发送。

服务器处理的数据

```cpp
class SnakeServer : public Snake
{
public:
	// 判断碰撞死亡
	void HandleDying();
	// 判断蛇头位置，来更新位置
	void UpdatePosition();
	// 判断是否吃到食物，增长身体
	void EatFood( int inDamagingPlayerId );
	
	const SocketAddress& GetSocketAddress()	const {return mSocketAddress;}
	int GetPlayerId() const	{ return mPlayerId; }
	const string& GetName() const { return mName; }

	// 获取未处理的用户移动
	const MoveList&	GetUnprocessedMoveList() const 
		{ return mUnprocessedMoveList; }
	MoveList& GetUnprocessedMoveList() { return mUnprocessedMoveList; }

protected:
	SnakeServer();

private:
	SocketAddress mSocketAddress;
	string mName;
	int mPlayerId;
	MoveList mUnprocessedMoveList;
};
```

关于碰撞检测：

```cpp
void Snake::collisionDetection(Fruit* fruit, Stone* stones[]) {
	float x = blocks.at(0)->getX();
	float z = blocks.at(0)->getZ();
	//check for fruit collision
	if (fruit->getX() == x && fruit->getZ() == z) { 
		score++;
		printf("Score = %i\n", score);
		bool repeat = false;
		// makes sure that fruit doesn't spawn underneath body
		do { 
			repeat = false;
			fruit->setX(rand() % 11 - 5);
			fruit->setZ(rand() % 11 - 5);
			for (int i = 0; i <= blocks.size() - 1;i++) {
				x = blocks.at(i)->getX();
				z = blocks.at(i)->getZ();
				if (fruit->getX() == x && fruit->getZ() == z)
					repeat = true;
			}
		} while (repeat);
		addBlock();
	}
```

蛇头的变化，根据输入

```cpp
void Snake::move(Direction direction) { 
	Block* snakeHead = blocks.at(0);
	if (direction == D_LEFT && currentDirection != D_RIGHT) {
		pushSnake();
		snakeHead->setX(snakeHead->getX() - 1.0f);
		currentDirection = direction;
		isUpdated = true;
	}
	else if (direction == D_RIGHT && currentDirection != D_LEFT) {
		pushSnake();
		snakeHead->setX(snakeHead->getX() + 1.0f);
		currentDirection = direction;
		isUpdated = true;
	}
	else if (direction == D_UP && currentDirection != D_DOWN) {
		pushSnake();
		snakeHead->setZ(snakeHead->getZ() + 1.0f);
		currentDirection = direction;
		isUpdated = true;
	}
	else if (direction == D_DOWN && currentDirection != D_UP) {
		pushSnake();
		snakeHead->setZ(snakeHead->getZ() - 1.0f);
		currentDirection = direction;
		isUpdated = true;
	}
};
```

#### 4. 实验截图

实验截图如下：

![1](C:\Users\asus\Desktop\HOMEWORK5\report\16340132LiangYingLin\images\1.jpg)

玩家1加入游戏

![2](C:\Users\asus\Desktop\HOMEWORK5\report\16340132LiangYingLin\images\2.png)

玩家2退出游戏，玩家1获得游戏胜利

![demo](C:\Users\asus\Desktop\HOMEWORK5\report\16340132LiangYingLin\images\demo.gif)

游戏动图

