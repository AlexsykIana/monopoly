//
// Created by Legion on 18.06.2025.
//

#ifndef GOTOJAILTILE_H
#define GOTOJAILTILE_H

#include "BoardTile.h"

class Player;
class GameBoard;

class GoToJailTile: public BoardTile {
    public:
    explicit GoToJailTile(sf::Vector2f visualPos);

    void onPlayerLanded(Player &player, GameBoard &board, std::vector<Player *> &allPlayers) override;
};

#endif //GOTOJAILTILE_H
