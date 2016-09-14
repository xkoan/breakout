#include "game.h"
#include "mainmenu.h"
#include "gameobjectmanager.h"
#include "gameball.h"
#include "aipaddle.h"

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

void Game::start()
{
    if(_gameState != Uninit)
        return;

    _window.create(sf::VideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32), "pong");

    PlayerPaddle *player1 = new PlayerPaddle();
    player1->setPosition(1150, 550);

    AIPaddle * player2 = new AIPaddle();
    player2->setPosition(50, 550);

    GameBall *ball = new GameBall();
    ball->setPosition((SCREEN_WIDTH/2),(SCREEN_HEIGHT/2)-15);

    _gameObjectManager.add("Paddle1", player1);
    _gameObjectManager.add("Paddle2", player2);
    _gameObjectManager.add("Ball", ball);
    _gameState = Game::ShowingMenu;

    while(!isExiting())
        gameLoop();

    _window.close();
}

const GameObjectManager& Game::getGameObjectManager()
{
    return Game::_gameObjectManager;
}

bool Game::isExiting()
{
    if(_gameState == Game::Exiting)
        return true;
    else
        return false;
}

void Game::gameLoop()
{
    switch(_gameState)
    {
        case Game::ShowingMenu:
        {
            showMenu();
            break;
        }
        case Game::Playing:
        {
            sf::Event event;
            while(_window.pollEvent(event))
            {
                if(event.type == sf::Event::Closed)
                    _gameState = Game::Exiting;

                if(event.type == sf::Event::KeyPressed)
                    if(event.key.code == sf::Keyboard::Escape) showMenu();
            }

            _window.clear(sf::Color(0,0,0));

            _gameObjectManager.updateAll();
            _gameObjectManager.drawAll(_window);

            _window.display();
            break;
        }
    }
}

void Game::showMenu()
{
    MainMenu mainMenu;
    MainMenu::MenuResult result = mainMenu.show(_window);
    switch(result)
    {
        case MainMenu::Exit:
            _gameState = Game::Exiting;
            break;
        case MainMenu::Play:
            _gameState = Game::Playing;
            break;
        case MainMenu::Options:
            _gameState = Game::Options;
            break;
        case MainMenu::Scoreboard:
            _gameState = Game::Scoreboard;
            break;
    }
}

Game::GameState Game::_gameState = Uninit;
sf::RenderWindow Game::_window;
GameObjectManager Game::_gameObjectManager;