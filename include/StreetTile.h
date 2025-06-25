//
// Created by Legion on 17.06.2025.
//

#ifndef STREETTILE_H
#define STREETTILE_H

#include "BoardTile.h"
#include "Street.h"

class Player;
class GameBoard;

class StreetTile : public BoardTile {
private:
    Street propertyData;

public:
    explicit StreetTile(const Street& initialPropertyData);

    void onPlayerLanded(Player& player, GameBoard& board, std::vector<Player*>& allPlayers) override;

    [[nodiscard]] Street& getPropertyData();
    [[nodiscard]] const Street& getPropertyData() const;

};

#endif //STREETTILE_H
