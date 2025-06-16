// GameBoard.cpp
#include "GameBoard.h"
#include <iostream>
#include "StreetInitializer.h"


GameBoard::GameBoard() {
    initializeAllTiles();
}


void GameBoard::initializeAllTiles() {
    tiles.reserve(TOTAL_SQUARES);

    tiles.emplace_back(std::make_unique<GoTile>(sf::Vector2f(825.f, 825.f))); // Приклад координат

    Street mediterranean_data("Mediterranean Avenue", 60, Street::StreetColorGroup::BROWN, sf::Vector2f(740.f, 825.f));
    tiles.emplace_back(std::make_unique<StreetTile>(mediterranean_data));

    // Клітинка 2: Community Chest
    tiles.emplace_back(std::make_unique<CommunityChestTile>(sf::Vector2f(665.f, 825.f)));

    // Клітинка 3: Baltic Avenue
    Street baltic_data("Baltic Avenue", 60, Street::StreetColorGroup::BROWN, sf::Vector2f(590.f, 825.f));
    tiles.emplace_back(std::make_unique<StreetTile>(baltic_data));

    // Клітинка 4: Income Tax
    tiles.emplace_back(std::make_unique<IncomeTaxTile>(sf::Vector2f(515.f, 825.f), 200));

    // ... і так далі для всіх 40 клітинок ...
    // Тобі потрібно буде створити об'єкти StreetTile, ChanceTile, TaxTile, GoToJailTile і т.д.
    // для кожної клітинки з її унікальними параметрами (назва, ціна, група, позиція).

    // Приклад заповнення до кінця (дуже спрощено, лише для демонстрації структури)
    // Тобі потрібно буде замінити це на реальні дані та типи клітинок
    for (int i = tiles.size(); i < TOTAL_SQUARES; ++i) {
        // Заглушка для решти клітинок
        tiles.emplace_back(std::make_unique<BoardTile>(
            "Square " + std::to_string(i),
            BoardTile::TileType::UNKNOWN, // Потрібно буде визначити правильний тип
            sf::Vector2f(100.f + (i * 20.f), 100.f) // Дуже приблизні координати
        ));
    }

    if (tiles.size() != TOTAL_SQUARES) {
        std::cerr << "CRITICAL ERROR: GameBoard was not initialized with the correct number of tiles!" << std::endl;
        // Тут можна кинути виняток або завершити програму, бо це серйозна помилка
    }
     std::cout << "GameBoard initialized with " << tiles.size() << " tiles." << std::endl;
}

// Метод для отримання візуальної позиції клітинки за її індексом
sf::Vector2f GameBoard::getVisualPositionForTile(int tileIndex) const {
    if (tileIndex >= 0 && tileIndex < tiles.size() && tiles[tileIndex]) {
        return tiles[tileIndex]->getVisualPosition();
    }
    std::cerr << "Warning: Tried to get visual position for invalid tile index: " << tileIndex << std::endl;
    return sf::Vector2f(0, 0); // Повертаємо нульовий вектор у разі помилки
}

// Метод для отримання об'єкта клітинки за її індексом
const BoardTile* GameBoard::getTile(int tileIndex) const {
    if (tileIndex >= 0 && tileIndex < tiles.size() && tiles[tileIndex]) {
        return tiles[tileIndex].get(); // .get() для отримання сирого вказівника з unique_ptr
    }
    std::cerr << "Warning: Tried to get tile at invalid index: " << tileIndex << std::endl;
    return nullptr; // Повертаємо nullptr у разі помилки
}

// Метод для отримання загальної кількості клітинок
int GameBoard::getTotalSquares() const {
    return TOTAL_SQUARES;
}