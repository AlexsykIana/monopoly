//
// Created by Legion on 18.06.2025.
//

#ifndef FREEPARKINGTILE_H
#define FREEPARKINGTILE_H

#include "BoardTile.h"

class FreeParkingTile : public BoardTile {
    public:
    explicit FreeParkingTile(sf::Vector2f visualPos);

    void onPlayerLanded(Player &player, GameBoard &board, std::vector<Player *> &allPlayers) override;
};
#endif //FREEPARKINGTILE_H
