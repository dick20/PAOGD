#include "Snake.h"
#include <iostream>

Snake::Snake(float startX, float startZ, int blockCount) {
	for (float i = 0; i < blockCount; i++) {
		Block* temp = new Block(startX + i, 0, startZ);
		if (i == 0)
			temp->setColor(0, 1, 0); //snake head is set to green color
		blocks.push_back(temp);
	}
	isUpdated = false;
	currentDirection = D_LEFT;
	score = 0;
}

Snake::~Snake(){

}

//loops through block array and calls their draw functions
void Snake::draw(int type) { 
	for (int i = 0; i <= blocks.size() - 1; i++) {
		Block* temp = blocks.at(i);
		temp->draw(type);
	}
}

//all snake movement handled in this and pushSnake() function below
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

//doesn't change snake head position but advances all other blocks
void Snake::pushSnake() { 
	for (int i = blocks.size() - 1; i >= 1; i--) {
		Block* temp = blocks.at(i);
		Block* prevBlock = blocks.at(i - 1);
		temp->setX(prevBlock->getX());
		temp->setZ(prevBlock->getZ());
	}
}

//update function called at every frame
void Snake::update(Fruit* fruit, Stone* stones[]) {
	//improper fps-capping, will update later
	std::this_thread::sleep_for(std::chrono::milliseconds(100)); 
	//if movement wasn't already handled on keypress event, then handle it now
	if (!isUpdated) 
		move(currentDirection);
	isUpdated = false;
	collisionDetection(fruit, stones);
}

//checks if snake head hit fruit or it's own body
void Snake::collisionDetection(Fruit* fruit, Stone* stones[]) {
	float x = blocks.at(0)->getX();
	float z = blocks.at(0)->getZ();
	//check for fruit collision
	if (fruit->getX() == x && fruit->getZ() == z) { 
		score++;
		printf("Score = %i\n", score);
		if (score > 4) {
			std::cout << "Player 2 disconnect! You Win!\n";
		}
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

	Block* snakeHead = blocks.at(0);
	if (-6 >= x && currentDirection == D_LEFT) {
		pushSnake();
		snakeHead->setX(6.0f);
		currentDirection = D_LEFT;
		isUpdated = true;
	}
	else if (x >= 6 && currentDirection == D_RIGHT) {
		pushSnake();
		snakeHead->setX(-6.0f);
		currentDirection = D_RIGHT;
		isUpdated = true;
	}
	else if (z >= 6 && currentDirection == D_UP) {
		pushSnake();
		snakeHead->setZ(-6.0f);
		currentDirection = D_UP;
		isUpdated = true;
	}
	else if (z <= -6 && currentDirection == D_DOWN) {
		pushSnake();
		snakeHead->setZ(6.0f);
		currentDirection = D_DOWN;
		isUpdated = true;
	}
	//check for collision with own body
	for (int i = 0; i < 1; i++) {
		if (stones[i]->getX() == x && stones[i]->getZ() == z) {
			//so far no game over screen so this is the only thing that happens...
			printf("Stone collision. You loss!\n");
			exit(0); 
			//glutHideWindow();
		}
	}
	//check for collision with own body
	for (int i = 1; i <= blocks.size() - 1; i++) { 
		if (blocks.at(i)->getX() == blocks.at(0)->getX() && blocks.at(i)->getZ() == blocks.at(0)->getZ()) {
			printf("Body collision. You loss!\n"); 
			//... and collided block turns red.
			glutHideWindow();
			blocks.at(i)->setColor(1, 0, 0); 
		}
	}
};

//adds a new block to the snake
void Snake::addBlock() { 
	Block* newBlock = new Block(-100, 0, -100);
	blocks.push_back(newBlock);
};

Direction Snake::getDirection() {
	return currentDirection; 
}
