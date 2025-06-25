// Deck.h
#ifndef DECK_H
#define DECK_H

#include "Card.h" // Потрібен Card
#include <vector>

class Deck {
public:
    enum class DeckType {
        CHANCE,
        COMMUNITY_CHEST
    };

private:
    std::vector<Card> cards;
    std::vector<Card> discardPile;
    DeckType type;

    void initializeChanceDeck();
    void initializeCommunityChestDeck();

public:
    explicit Deck(DeckType deckType);

    void shuffle();

    Card drawCard(const Player& player);

    void returnGetOutOfJailCard(const Card& card);

    [[nodiscard]] bool isEmpty() const;
    [[nodiscard]] bool discardPileIsEmpty() const;
};

#endif //DECK_H