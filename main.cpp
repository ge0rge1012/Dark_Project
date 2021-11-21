/*
Class Game is made by using a stupid realisation of sigleton pattern
This class provides only main logic of the game, all actions are determinated
Program can be started by using static function Game::start_game()
It seems strange and illogical to use so significant abstraction, but i find it an appropriate base for future
(in my sick mind)

project is needed to be code restyled!!!! Especially names of vars:D

little note for me about game stages:
1) loading all textures to TextureHolder, player can see progress bar
Also we'll need AudioHolder, I think
use sf::Image for big picture on the background of main menu
Moreover, sf::Texture/sf::Image can work with .gif!

2) Main Menu, configuraion of the game, information, may be training (why not)
here should be abilities to save and load progress

3) game process
when game is paused, process of game is stopped, but not closed (of course)

TextureHolder modifiing:
1) make it look not so palliative (may be make an hierarchy)
2) textures loading should be a separate step - not at the beginnig of Game::start_game()

*/

#include "EngineClasses.h"

int main()
{
	Game::start_game(); // start game

	return 0;
}