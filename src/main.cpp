#include "Game.h"
#include "Logs.h"

int main() {
    try{
        Game game;

        while(game.Run()){
            Logs::Print("Playing");
        };
    }
    catch(const std::exception& e){
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    Logs::Print("Game Closing");

    return 0;
}
