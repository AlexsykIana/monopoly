#include "Deck.h"
#include "Player.h"
#include <iostream>
#include <chrono>
#include <random>


Deck::Deck(DeckType deckType) : type(deckType) {
    if (type == DeckType::CHANCE) {
        initializeChanceDeck();
    } else {
        initializeCommunityChestDeck();
    }
    shuffle();
}

void Deck::initializeChanceDeck() {
    cards.emplace_back("Advance to Go (Collect $200)", Card::ActionType::MOVE_TO_POSITION, 0 /*Позиція GO*/);
    cards.emplace_back("Go to Jail. Go directly to Jail. Do not pass Go, do not collect $200", Card::ActionType::GO_TO_JAIL);
    cards.emplace_back("Bank pays you dividend of $50", Card::ActionType::RECEIVE_MONEY_FROM_BANK, 50);
    cards.emplace_back("Get out of Jail Free. This card may be kept until needed or sold.", Card::ActionType::GET_OUT_OF_JAIL_FREE, 0, 0, true);
    cards.emplace_back("Pay poor tax of $15", Card::ActionType::PAY_MONEY_TO_BANK, 15);
    std::cout << "Chance deck initialized with " << cards.size() << " cards." << std::endl;
}

void Deck::initializeCommunityChestDeck() {
    cards.emplace_back("Advance to Go (Collect $200)", Card::ActionType::MOVE_TO_POSITION, 0);
    cards.emplace_back("Bank error in your favor. Collect $200", Card::ActionType::RECEIVE_MONEY_FROM_BANK, 200);
    cards.emplace_back("Doctor's fees. Pay $50", Card::ActionType::PAY_MONEY_TO_BANK, 50);
    cards.emplace_back("Get out of Jail Free. This card may be kept until needed or sold.", Card::ActionType::GET_OUT_OF_JAIL_FREE, 0, 0, true);
    std::cout << "Community Chest deck initialized with " << cards.size() << " cards." << std::endl;
}

// Реалізація shuffle
void Deck::shuffle() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);
    std::shuffle(cards.begin(), cards.end(), rng);
    std::cout << (type == DeckType::CHANCE ? "Chance" : "Community Chest") << " deck shuffled." << std::endl;
}

// Реалізація drawCard
Card Deck::drawCard(const Player& player) {
    if (cards.empty()) {
        if (discardPile.empty()) {
            std::cerr << "Error: Both deck and discard pile are empty for "
                      << (type == DeckType::CHANCE ? "Chance" : "Community Chest") << "!" << std::endl;
            return {"Error: No cards left!", Card::ActionType::PAY_MONEY_TO_BANK, 0};
        }
        cards.swap(discardPile);
        shuffle();
        std::cout << (type == DeckType::CHANCE ? "Chance" : "Community Chest")
                  << " discard pile reshuffled into deck." << std::endl;
    }

    Card drawnCard = cards.back();
    cards.pop_back();

    if (drawnCard.isGetOutOfJailFree()) {
        std::cout << player.getName() << " received 'Get Out of Jail Free' card to keep." << std::endl;
    } else {
        discardPile.push_back(drawnCard);
    }
    return drawnCard;
}

void Deck::returnGetOutOfJailCard(const Card& card) {
    if (card.isGetOutOfJailFree()) {
        discardPile.push_back(card);
        std::cout << "'Get Out of Jail Free' card returned to "
                  << (type == DeckType::CHANCE ? "Chance" : "Community Chest") << " discard pile." << std::endl;
    }
}

bool Deck::isEmpty() const {
    return cards.empty() && discardPile.empty();
}

bool Deck::discardPileIsEmpty() const {
    return discardPile.empty();
}