#include "Globals.h"

bool init();
void gameLoop();
void eventLoop();
void physicsLoop();
void renderLoop();
void shutdown();

int main(int argv, char** argc)
{
	if (!init())
	{
		atexit(SDL_Quit);

		return -1;
	}

	gameLoop();

	shutdown();

	return 0;
}

bool init()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		cerr << "Error initializing SDL: " << SDL_GetError() << endl;

		return false;
	}

	if (IMG_Init(IMG_INIT_PNG) < 0)
	{
		cerr << "Error initializing SDL_image: " << IMG_GetError() << endl;

		return false;
	}

	window = SDL_CreateWindow("Blackjack", 0, 100, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (!window)
	{
		cerr << "Error creating window: " << SDL_GetError() << endl;

		return false;
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

	if (!renderer)
	{
		cerr << "Error creating renderer: " << SDL_GetError() << endl;

		return false;
	}

	//INITIALIZE SPRITES
	bg = new Sprite("images/BG.png", renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 1);
	table = new Sprite("images/table.png", renderer, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 1, 1);

	play = new Button("images/buttons.png", renderer, (SCREEN_WIDTH / 2) - 124, (SCREEN_HEIGHT / 2) - 54, 247, 108, 1, 6);
	
	hit = new Button("images/buttons.png", renderer, 136, 20, 247, 108, 1, 6);
	stand = new Button("images/buttons.png", renderer, 519, 20, 247, 108, 1, 6);
	pass = new Button("images/buttons.png", renderer, 902, 20, 247, 108, 1, 6);
	doubleDown = new Button("images/buttons.png", renderer, 1285, 20, 247, 108, 1, 6);
	split = new Button("images/buttons.png", renderer, 1668, 20, 247, 108, 1, 6);

	if (!bg || !table || !play || !hit || !stand || !pass || !doubleDown || !split)
	{
		cerr << "Error loading sprites " << IMG_GetError() << endl;

		return false;
	}

	deck = new Deck(renderer);

	player = new Player();
	dealer = new Player();

	play->setCurrentButton(0);
	hit->setCurrentButton(1);
	stand->setCurrentButton(2);
	pass->setCurrentButton(3);
	doubleDown->setCurrentButton(4); 
	split->setCurrentButton(5);
}

void gameLoop()
{
	int totalFrames = 0;
	int startTime = 0;
	int endTime = 0;
	int deltaTime = 0;

	while (!done)
	{
		startTime = SDL_GetTicks();

		++totalFrames;

		eventLoop();
		physicsLoop();

		endTime = SDL_GetTicks();

		deltaTime = endTime - startTime;

		if (deltaTime < TICKSPERFRAME)
		{
			SDL_Delay(TICKSPERFRAME - deltaTime);
		}

		renderLoop();
	}
}

void eventLoop()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.key.repeat == 0)
		{
			switch (event.type)
			{
			case SDL_QUIT:
				done = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
				if (!play->isPressed() && play->isVisible() && event.button.x >= play->getXPos() && event.button.x <= play->getXPos() + play->getWidth() &&
					event.button.y >= play->getYPos() && event.button.y <= play->getYPos() + play->getHeight())
				{
					play->onPress();

					player->addCard(deck->getCard());

					Card* temp = deck->getCard();
					temp->setBack(true);
					dealer->addCard(temp);

					player->addCard(deck->getCard());
					dealer->addCard(deck->getCard());
				}
				else if (!hit->isPressed() && hit->isVisible() && event.button.x >= hit->getXPos() && event.button.x <= hit->getXPos() + hit->getWidth() &&
					event.button.y >= hit->getYPos() && event.button.y <= hit->getYPos() + hit->getHeight())
				{
					hit->onPress();

					player->addCard(deck->getCard());

					//ADD HIT LOGIC
					// highlight hit button?
				}
				else if (!stand->isPressed() && stand->isVisible() && event.button.x >= stand->getXPos() && event.button.x <= stand->getXPos() + stand->getWidth() &&
					event.button.y >= stand->getYPos() && event.button.y <= stand->getYPos() + stand->getHeight())
				{
					//ADD STAND LOGIC
					// highlight stand button?
				}
				else if (!pass->isPressed() && pass->isVisible() && event.button.x >= pass->getXPos() && event.button.x <= pass->getXPos() + pass->getWidth() &&
					event.button.y >= pass->getYPos() && event.button.y <= pass->getYPos() + pass->getHeight())
				{
					//ADD PASS LOGIC
					// highlight pass button?
				}
				else if (!doubleDown->isPressed() && doubleDown->isVisible() && event.button.x >= doubleDown->getXPos() && event.button.x <= doubleDown->getXPos() + doubleDown->getWidth() &&
					event.button.y >= doubleDown->getYPos() && event.button.y <= doubleDown->getYPos() + doubleDown->getHeight())
				{
					//ADD DOUBLE DOWN LOGIC
					// highlight double down button?
				}
				else if (!split->isPressed() && split->isVisible() && event.button.x >= split->getXPos() && event.button.x <= split->getXPos() + split->getWidth() &&
					event.button.y >= split->getYPos() && event.button.y <= split->getYPos() + split->getHeight())
				{
					//ADD SPLIT LOGIC
					// highlight split button?
				}

				break;
			case SDL_MOUSEBUTTONUP:
				if (play->isPressed() && play->isVisible() && event.button.x >= play->getXPos() && event.button.x <= play->getXPos() + play->getWidth() &&
					event.button.y >= play->getYPos() && event.button.y <= play->getYPos() + play->getHeight())
				{
					play->setPressed(false);
				}
				else if (!hit->isPressed() && hit->isVisible() && event.button.x >= hit->getXPos() && event.button.x <= hit->getXPos() + hit->getWidth() &&
					event.button.y >= hit->getYPos() && event.button.y <= hit->getYPos() + hit->getHeight())
				{
					//Player::getTotal(int handValue);

					//if (handValue > 21)
						// do some "you lose" actions (reduce credits by bet value, move to next hand if split, etc.)

					//ADD HIT LOGIC
				}
				else if (!stand->isPressed() && stand->isVisible() && event.button.x >= stand->getXPos() && event.button.x <= stand->getXPos() + stand->getWidth() &&
					event.button.y >= stand->getYPos() && event.button.y <= stand->getYPos() + stand->getHeight())
				{
					stand->setPressed(false);

					// if (split == true)
					// {
					//		resolve second player hand
					// }
					// else
					//		resolve dealer hand and compare for results (both hands if split)
					//ADD STAND LOGIC
				}
				else if (!pass->isPressed() && pass->isVisible() && event.button.x >= pass->getXPos() && event.button.x <= pass->getXPos() + pass->getWidth() &&
					event.button.y >= pass->getYPos() && event.button.y <= pass->getYPos() + pass->getHeight())
				{
					pass->setPressed(false);

					// check if pass is allowed, if yes then reduce credits by half of bet (rounded up), if not then do nothing and move on
					//ADD PASS LOGIC
				}
				else if (!doubleDown->isPressed() && doubleDown->isVisible() && event.button.x >= doubleDown->getXPos() && event.button.x <= doubleDown->getXPos() + doubleDown->getWidth() &&
					event.button.y >= doubleDown->getYPos() && event.button.y <= doubleDown->getYPos() + doubleDown->getHeight())
				{
					doubleDown->setPressed(false);

					//ADD DOUBLE DOWN LOGIC
					// check if double down is allowed (check only two cards dealt and check cards value must total 9, 10, or 11)
					// if yes then double bet
					// if no, then do nothing and move on
				}
				else if (!split->isPressed() && split->isVisible() && event.button.x >= split->getXPos() && event.button.x <= split->getXPos() + split->getWidth() &&
					event.button.y >= split->getYPos() && event.button.y <= split->getYPos() + split->getHeight())
				{
					split->setPressed(false);

					//ADD SPLIT LOGIC
					// check if split allowed (check only two cards dealt and cards are the same (not card value!))
					// if yes, move second card to first card of player second hand
					// if no, then do nothing and move on
				}
				break;
			}
		}
	}
}

void physicsLoop()
{
}

void renderLoop()
{
	bg->draw();
	table->draw();

	if (!play->isPressed() && play->isVisible())
	{
		play->draw();
	}
	else if (hit->isPressed() && hit->isVisible())
	{
		hit->setPressed(false);
	}
	else if (stand->isPressed() && stand->isVisible())
	{

	}
	else if (pass->isPressed() && pass->isVisible())
	{

	}
	else if (doubleDown->isPressed() && doubleDown->isVisible())
	{

	}
	else if (split->isPressed() && split->isVisible())
	{

	}
	else 
	{
		play->setVisible(false);

		hit->draw();		
		stand->draw();

		if(pass->isVisible())
			pass->draw();
		if (doubleDown->isVisible())
			doubleDown->draw();
		if (split->isVisible())
			split->draw();
	}

	if (player->getCardCount() > 0)
	{
		for (int i = 0; i < player->getCardCount(); ++i)
		{
			deck->setXPos(200 + (i * 269));
			deck->setYPos(700);

			deck->drawCard(player->getCard(i));
		}
	}

	if (dealer->getCardCount() > 0)
	{
		for (int i = 0; i < dealer->getCardCount(); ++i)
		{
			deck->setXPos(200 + (i * 269));
			deck->setYPos(200);

			deck->drawCard(dealer->getCard(i));
		}
	}

	SDL_RenderPresent(renderer);
}

void shutdown()
{
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	IMG_Quit();
	SDL_Quit();

	delete bg;
	delete table;
	delete deck;
	delete play;
}