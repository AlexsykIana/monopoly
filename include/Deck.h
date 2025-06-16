// Deck.h
#ifndef DECK_H
#define DECK_H

#include "Card.h" // Потрібен Card
#include <vector>
#include <string> // Для типу колоди
#include <random> // Для перетасування
#include <algorithm> // Для std::shuffle

class Deck {
public:
    enum class DeckType {
        CHANCE,
        COMMUNITY_CHEST
    };

private:
    std::vector<Card> cards;          // Картки, що зараз у колоді
    std::vector<Card> discardPile;    // Використані картки
    DeckType type;                    // Тип колоди (Шанс або Громадська казна)

    void initializeChanceDeck();      // Приватний метод для створення карток "Шанс"
    void initializeCommunityChestDeck(); // Приватний метод для створення карток "Громадська казна"

public:
    // Конструктор
    explicit Deck(DeckType deckType); // explicit, щоб уникнути неявних перетворень

    // Перетасувати основну колоду карток
    void shuffle();

    // Витягнути картку. Повертає картку.
    // Якщо картка "Вийти з в'язниці", вона не йде в discardPile відразу.
    Card drawCard(Player& player); // Передаємо гравця, якщо картка "Вийти з в'язниці" має бути йому віддана

    // Повернути картку "Вийти з в'язниці" в колоду (зазвичай у discardPile)
    void returnGetOutOfJailCard(const Card& card);

    // Чи порожня основна колода
    [[nodiscard]] bool isEmpty() const;
};

#endif //DECK_H