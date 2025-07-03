//
// Created by Legion on 17.06.2025.
//

#ifndef GOTILE_H
#define GOTILE_H
#include "BoardTile.h"

class GoTile : public BoardTile {
    public:
    explicit GoTile(sf::Vector2f position);

    LandedAction onPlayerLanded(Player &player, GameBoard &board, std::vector<Player *> &allPlayers) override;
};
#endif //GOTILE_H
