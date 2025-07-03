//
// Created by Legion on 16.06.2025.
//

#ifndef BOARDTILE_H
#define BOARDTILE_H

#include <string>
#include <SFML/Graphics.hpp>
#include <vector>


class Player;
class GameBoard;

class BoardTile {
public:
    enum class LandedAction {
        NOTHING,
        TURN_ENDS_IMMEDIATELY
    };
    enum class TileType {
        PROPERTY,
        GO,
        CHANCE,
        COMMUNITY_CHEST,
        INCOME_TAX,
        LUXURY_TAX,
        JAIL_OR_JUST_VISITING,
        GO_TO_JAIL,
        FREE_PARKING
    };

protected:
    std::string name;
    TileType type;
    sf::Vector2f visualPosition;

public:
    BoardTile(std::string tileName, TileType tileType, sf::Vector2f visualPos);

    // Віртуальний деструктор: важливий для правильного видалення об'єктів похідних класів
    // через вказівник на базовий клас.
    virtual ~BoardTile() = default; // = default означає, що компілятор згенерує стандартний деструктор

    [[nodiscard]] const std::string& getName() const;
    [[nodiscard]] TileType getType() const;
    [[nodiscard]] sf::Vector2f getVisualPosition() const;


    virtual LandedAction onPlayerLanded(Player& player, GameBoard& board, std::vector<Player*>& allPlayers);

};
#endif //BOARDTILE_H
