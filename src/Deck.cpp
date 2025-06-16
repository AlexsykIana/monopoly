#include "Deck.h"
#include "Player.h"
#include <iostream>
#include <chrono>


Deck::Deck(DeckType deckType) : type(deckType) {
    if (type == DeckType::CHANCE) {
        initializeChanceDeck();
    } else {
        initializeCommunityChestDeck();
    }
    shuffle(); // Перетасовуємо колоду після ініціалізації
}

// Приватні методи для ініціалізації карток
void Deck::initializeChanceDeck() {
    // Тут ти створюєш всі картки "Шанс"
    // Card("Опис", ActionType, value1, value2, isGetOutOfJail)
    cards.emplace_back("Advance to Go (Collect $200)", Card::ActionType::MOVE_TO_POSITION, 0 /*Позиція GO*/);
    cards.emplace_back("Go to Jail. Go directly to Jail. Do not pass Go, do not collect $200", Card::ActionType::GO_TO_JAIL);
    cards.emplace_back("Bank pays you dividend of $50", Card::ActionType::RECEIVE_MONEY_FROM_BANK, 50);
    cards.emplace_back("Get out of Jail Free. This card may be kept until needed or sold.", Card::ActionType::GET_OUT_OF_JAIL_FREE, 0, 0, true);
    cards.emplace_back("Pay poor tax of $15", Card::ActionType::PAY_MONEY_TO_BANK, 15);
    // ... і так далі для всіх 16 карток "Шанс" ...
    std::cout << "Chance deck initialized with " << cards.size() << " cards." << std::endl;
}

void Deck::initializeCommunityChestDeck() {
    // Тут ти створюєш всі картки "Громадська казна"
    cards.emplace_back("Advance to Go (Collect $200)", Card::ActionType::MOVE_TO_POSITION, 0);
    cards.emplace_back("Bank error in your favor. Collect $200", Card::ActionType::RECEIVE_MONEY_FROM_BANK, 200);
    cards.emplace_back("Doctor's fees. Pay $50", Card::ActionType::PAY_MONEY_TO_BANK, 50);
    cards.emplace_back("Get out of Jail Free. This card may be kept until needed or sold.", Card::ActionType::GET_OUT_OF_JAIL_FREE, 0, 0, true);
    // ... і так далі для всіх 16 карток "Громадська казна" ...
    std::cout << "Community Chest deck initialized with " << cards.size() << " cards." << std::endl;
}

// Реалізація shuffle
void Deck::shuffle() {
    // Використовуємо більш сучасний генератор випадкових чисел
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine rng(seed);
    std::shuffle(cards.begin(), cards.end(), rng);
    std::cout << (type == DeckType::CHANCE ? "Chance" : "Community Chest") << " deck shuffled." << std::endl;
}

// Реалізація drawCard
Card Deck::drawCard(Player& player) {
    if (cards.empty()) { // Якщо основна колода порожня
        if (discardPile.empty()) {
            // Це не повинно трапитися, якщо є картки "Вийти з в'язниці",
            // які не повертаються, або якщо всі картки були "Вийти з в'язниці".
            // Або якщо гра дуже довга і всі картки були зіграні кілька разів.
            std::cerr << "Error: Both deck and discard pile are empty for "
                      << (type == DeckType::CHANCE ? "Chance" : "Community Chest") << "!" << std::endl;
            // Повернути якусь "порожню" або спеціальну картку помилки
            return Card("Error: No cards left!", Card::ActionType::PAY_MONEY_TO_BANK, 0); // Приклад
        }
        // Перемішуємо використані картки і робимо їх новою колодою
        cards.swap(discardPile); // Міняємо місцями вміст cards та discardPile (discardPile стає порожнім)
        shuffle();
        std::cout << (type == DeckType::CHANCE ? "Chance" : "Community Chest")
                  << " discard pile reshuffled into deck." << std::endl;
    }

    Card drawnCard = cards.back(); // Беремо верхню картку (останню у векторі)
    cards.pop_back();              // Видаляємо її з основної колоди

    if (drawnCard.isGetOutOfJailFree()) {
        // Гравець зберігає цю картку. Вона не йде в discardPile зараз.
        // player.addGetOutOfJailCard(); // Потрібен метод в Player
        std::cout << player.getName() << " received 'Get Out of Jail Free' card to keep." << std::endl;
    } else {
        discardPile.push_back(drawnCard); // Інші картки йдуть у discardPile
    }
    return drawnCard;
}

// Реалізація returnGetOutOfJailCard
void Deck::returnGetOutOfJailCard(const Card& card) {
    if (card.isGetOutOfJailFree()) {
        discardPile.push_back(card); // Повертаємо в discardPile (або в кінець основної колоди)
        std::cout << "'Get Out of Jail Free' card returned to "
                  << (type == DeckType::CHANCE ? "Chance" : "Community Chest") << " discard pile." << std::endl;
    }
}

// Реалізація isEmpty
bool Deck::isEmpty() const {
    return cards.empty() && discardPile.empty(); // Повністю порожня, якщо обидві стопки порожні
}