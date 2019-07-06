#include "GameObject.h"


GameObject::GameObject()
	: Position(0, 0, 0), Size(1, 1, 1), Velocity(0.0f), Color(1.0f), Rotation(0.0f), Sprite(), IsSolid(false), Destroyed(false) { }

GameObject::GameObject(glm::vec3 pos, glm::vec3 size, Texture2D sprite, glm::vec3 color, glm::vec3 velocity)
	: Position(pos), Size(size), Velocity(velocity), Color(color), Rotation(0.0f), Sprite(sprite), IsSolid(false), Destroyed(false) { }

void GameObject::Draw(SpriteRenderer &renderer)
{
	renderer.DrawSprite(this->Sprite, this->Position, this->Size, this->Rotation, this->Color);
}