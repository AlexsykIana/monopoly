#ifndef JAILORJUSTVISITINGTILE_H
#define JAILORJUSTVISITINGTILE_H

#include "BoardTile.h"

class Player;

class JailOrJustVisitingTile : public BoardTile {
    public:
    explicit JailOrJustVisitingTile(sf::Vector2f visualPos);

    void onPlayerLanded(Player &player, GameBoard &board, std::vector<Player *> &allPlayers) override;
};
#endif //JAILORJUSTVISITINGTILE_H
