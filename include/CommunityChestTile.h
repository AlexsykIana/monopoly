//
// Created by Legion on 18.06.2025.
//

#ifndef COMMUNITYCHESTTILE_H
#define COMMUNITYCHESTTILE_H
#include "BoardTile.h"
#include "CommunityChestTile.h"

class Player;
class Gameboard;

class CommunityChestTile : public BoardTile {
    public:
    explicit  CommunityChestTile(sf::Vector2f visualPos);

    void onPlayerLanded(Player &player, GameBoard &board, std::vector<Player *> &allPlayers) override;
};
#endif //COMMUNITYCHESTTILE_H
