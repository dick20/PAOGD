class SnakeClient : public Snake
{
public:
	static	GameObjectPtr	StaticCreate()		{ return GameObjectPtr( new SnakeClient() ); }

	virtual void Update();
	virtual void	HandleDying() override;

	virtual void	Read( InputMemoryBitStream& inInputStream ) override;

protected:
	SnakeClient();


private:
	float				mTimeLocationBecameOutOfSync;
	float				mTimeVelocityBecameOutOfSync;
	
	SpriteComponentPtr	mSpriteComponent;
};