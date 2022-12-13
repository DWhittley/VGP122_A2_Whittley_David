#include "Globals.h"

bool init();
void gameLoop();
void eventLoop();
void resolve(bool);
void dealerPlay();
void physicsLoop();
void renderLoop();
void shutdown();
void reset();

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
	splitPlayer = new Player();
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

bool hasSplit = false;
bool player1Done = false;

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

					//player->addCard(deck->getCard());
					player->addCard(player->getCard(0));//Split test logic
					dealer->addCard(deck->getCard());
				}
				else if (!hit->isPressed() && hit->isVisible() && event.button.x >= hit->getXPos() && event.button.x <= hit->getXPos() + hit->getWidth() &&
					event.button.y >= hit->getYPos() && event.button.y <= hit->getYPos() + hit->getHeight())
				{
					if (!player1Done) {
						hit->onPress();
						player->addCard(deck->getCard());
						if (player->getTotal() > 21 && hasSplit) {
							player1Done = true;
							cout << "Player busted hand one.";
							player->setBusted();
							//This is a bust for hand1
						}
						else if (player->getTotal() > 21 && !hasSplit) {
							cout << "Player busted hand one with ";
							cout << player->getTotal() << endl;
							player->setBusted();
							renderLoop();
							SDL_Delay(1500);
							reset();
						}
					} else if (hasSplit && player1Done) {
						hit->onPress();
						splitPlayer->addCard(deck->getCard());
						if (splitPlayer->getTotal() > 21 && player->getTotal() > 21) {
							cout << "Player busted hand one and two." << endl;
							player->setBusted();
							splitPlayer->setBusted();
							renderLoop();
							SDL_Delay(1500);
							reset();
						}
						else if (splitPlayer->getTotal() > 21 && player->getTotal() < 22) {
							cout << "Player busted hand two." << endl;
							splitPlayer->setBusted();
							dealerPlay();
							resolve(hasSplit);
						}
					}
				}
				else if (!stand->isPressed() && stand->isVisible() && event.button.x >= stand->getXPos() && event.button.x <= stand->getXPos() + stand->getWidth() &&
					event.button.y >= stand->getYPos() && event.button.y <= stand->getYPos() + stand->getHeight())
				{
					if (player1Done && hasSplit) {
						cout << "Player splithand stands with: " << splitPlayer->getTotal() << endl;
						dealerPlay();
						resolve(hasSplit);
					}
					if (!player1Done) {
						cout << "Player hand stands with: " << splitPlayer->getTotal() << endl;
						player1Done = true;
					}
					if (!hasSplit) {
						dealerPlay();
						resolve(hasSplit);
					}
				}
				else if (!pass->isPressed() && pass->isVisible() && event.button.x >= pass->getXPos() && event.button.x <= pass->getXPos() + pass->getWidth() &&
					event.button.y >= pass->getYPos() && event.button.y <= pass->getYPos() + pass->getHeight())
				{
					if (!hasSplit)
					{
						cout << "Player passed" << endl;

						renderLoop();
						SDL_Delay(1500);
						reset();
					}
					else if (hasSplit && !player1Done) {
						cout << "Player passed" << endl;
						player1Done = true;
					}

					else if (hasSplit && player1Done)
					{
						cout << "Player split hand passed" << endl;

						renderLoop();
						SDL_Delay(1500);
						reset();
					}
				}
				
				else if (!doubleDown->isPressed() && doubleDown->isVisible() && event.button.x >= doubleDown->getXPos() && event.button.x <= doubleDown->getXPos() + doubleDown->getWidth() &&
					event.button.y >= doubleDown->getYPos() && event.button.y <= doubleDown->getYPos() + doubleDown->getHeight())
				{
					if (player->getCardCount() == 2)
						player->addCard(deck->getCard());
					else
						cout << "Can only double down on initial hand." << endl;
				}
				else if (!split->isPressed() && split->isVisible() && event.button.x >= split->getXPos() && event.button.x <= split->getXPos() + split->getWidth() &&
					event.button.y >= split->getYPos() && event.button.y <= split->getYPos() + split->getHeight())
				{
					if (player->getCardCount() == 2 && (player->getCard(0)->getFace() == player->getCard(1)->getFace())) {
						splitPlayer->addCard(player->popCard()); //Create new player using player's second card
						player->addCard(deck->getCard()); // add a card to player hand
						splitPlayer->addCard(deck->getCard());  // add a card to player split hand
						renderLoop(); // re-render 
						hasSplit = true;
					}
					else
						cout << "Player tried to split without having only 2 cards that are the same" << endl;
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

					//ADD HIT LOGIC
				}
				else if (!stand->isPressed() && stand->isVisible() && event.button.x >= stand->getXPos() && event.button.x <= stand->getXPos() + stand->getWidth() &&
					event.button.y >= stand->getYPos() && event.button.y <= stand->getYPos() + stand->getHeight())
				{
					stand->setPressed(false);

					//ADD STAND LOGIC
				}
				else if (!pass->isPressed() && pass->isVisible() && event.button.x >= pass->getXPos() && event.button.x <= pass->getXPos() + pass->getWidth() &&
					event.button.y >= pass->getYPos() && event.button.y <= pass->getYPos() + pass->getHeight())
				{
					pass->setPressed(false);

					//ADD PASS LOGIC
				}
				else if (!doubleDown->isPressed() && doubleDown->isVisible() && event.button.x >= doubleDown->getXPos() && event.button.x <= doubleDown->getXPos() + doubleDown->getWidth() &&
					event.button.y >= doubleDown->getYPos() && event.button.y <= doubleDown->getYPos() + doubleDown->getHeight())
				{
					doubleDown->setPressed(false);

					//ADD DOUBLE DOWN LOGIC
					
				}
				else if (!split->isPressed() && split->isVisible() && event.button.x >= split->getXPos() && event.button.x <= split->getXPos() + split->getWidth() &&
					event.button.y >= split->getYPos() && event.button.y <= split->getYPos() + split->getHeight())
				{
					split->setPressed(false);

					//ADD SPLIT LOGIC

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
			deck->setYPos(480);

			deck->drawCard(player->getCard(i));
		}
	}
	
	if (splitPlayer->getCardCount() > 0)
	{
		for (int i = 0; i < splitPlayer->getCardCount(); ++i)
		{
			deck->setXPos(200 + (i * 269));
			deck->setYPos(840);

			deck->drawCard(splitPlayer->getCard(i));
		}
	}

	if (dealer->getCardCount() > 0)
	{
		for (int i = 0; i < dealer->getCardCount(); ++i)
		{
			deck->setXPos(200 + (i * 269));
			deck->setYPos(140);

			deck->drawCard(dealer->getCard(i));
		}
	}

	SDL_RenderPresent(renderer);
}

void reset() {
	delete player;
	delete dealer;
	delete splitPlayer;
	delete deck;

	hasSplit = false;
	player1Done = false;

	deck = new Deck(renderer);
	player = new Player();
	splitPlayer = new Player();
	dealer = new Player();

	player->addCard(deck->getCard());

	Card* temp = deck->getCard();
	temp->setBack(true);
	dealer->addCard(temp);

	player->addCard(deck->getCard());
	dealer->addCard(deck->getCard());
}

void dealerPlay() {
	dealer->getCard(0)->setBack(false);
	while (dealer->getTotal() < 17) {
		dealer->addCard(deck->getCard());
	}
	if (dealer->getTotal() > 21) {
		cout << "Dealer busted, so Player wins hand(s) that didn't bust/pass" << endl;
	}
}

void resolve(bool splitvar) {
	if (!splitvar) {
		if (dealer->getTotal() > 21) {
			cout << "Dealer: " << dealer->getTotal() << endl;
			cout << "Player: " << player->getTotal() << endl;
			cout << "Dealer busted so Player hand wins" << endl;
		}
		if (dealer->getTotal() < player->getTotal())
		{
			cout << "Dealer: " << dealer->getTotal() << endl;
			cout << "Player: " << player->getTotal() << endl;
			cout << "Player hand wins" << endl;
		}
		else if (dealer->getTotal() == player->getTotal())
		{
			cout << "Dealer: " << dealer->getTotal() << endl;
			cout << "Player: " << player->getTotal() << endl;
			cout << "Player hand is a push" << endl;
		}
		else
		{
			cout << "Dealer: " << dealer->getTotal() << endl;
			cout << "Player: " << player->getTotal() << endl;
			cout << "Dealer hand loses" << endl;
		}
	}
	else {
		if (player->hasBust()) {
			if (dealer->getTotal() > 21) {
				cout << "Dealer: " << dealer->getTotal() << endl;
				cout << "Player: " << player->getTotal() << endl;
				cout << "Dealer busted so Player hand wins" << endl;
			}
			if (dealer->getTotal() < player->getTotal())
			{
				cout << "Dealer: " << dealer->getTotal() << endl;
				cout << "Player: " << player->getTotal() << endl;
				cout << "Player hand wins" << endl;
			}
			else if (dealer->getTotal() == player->getTotal())
			{
				cout << "Dealer: " << dealer->getTotal() << endl;
				cout << "Player: " << player->getTotal() << endl;
				cout << "Player hand is a push" << endl;
			}
			else
			{
				cout << "Dealer: " << dealer->getTotal() << endl;
				cout << "Player: " << player->getTotal() << endl;
				cout << "Dealer hand loses" << endl;
			}
		}
		if (splitPlayer->hasBust()) {
			if (dealer->getTotal() < splitPlayer->getTotal()) {
				cout << "Dealer: " << dealer->getTotal() << endl;
				cout << "Player split: " << splitPlayer->getTotal() << endl;
				cout << "Player split hand wins" << endl;
			}
			else if (dealer->getTotal() == splitPlayer->getTotal()) {
				cout << "Dealer: " << dealer->getTotal() << endl;
				cout << "Player split: " << splitPlayer->getTotal() << endl;
				cout << "Player split hand is a push" << endl;
			}
			else {
				cout << "Dealer: " << dealer->getTotal() << endl;
				cout << "Player split: " << splitPlayer->getTotal() << endl;
				cout << "Dealer split hand loses" << endl;
			}
		}
	}
	
	renderLoop();
	SDL_Delay(1500);
	reset();
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