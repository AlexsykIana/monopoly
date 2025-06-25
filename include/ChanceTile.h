//
// Created by Legion on 17.06.2025.
//

#ifndef CHANCETILE_H
#define CHANCETILE_H

#include "BoardTile.h"

class Player;
class GameBoard;

class ChanceTile : public BoardTile {
public:
    explicit  ChanceTile(sf::Vector2f visualPos);

    void onPlayerLanded(Player& player, GameBoard& board, std::vector<Player*>& allPlayers) override;
};

#endif //CHANCETILE_H
