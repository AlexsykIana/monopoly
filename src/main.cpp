#include <SFML/Graphics.hpp>

int main()
{
    // Створюємо вікно
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Works!");

    // Створюємо просту фігуру для відображення
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);
    shape.setPosition( (800.f - 2*100.f) / 2.f, (600.f - 2*100.f) / 2.f ); // Центруємо фігуру

    // Головний цикл програми, виконується, доки вікно відкрите
    while (window.isOpen())
    {
        // Обробка подій
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Якщо користувач закрив вікно
            if (event.type == sf::Event::Closed)
                window.close();

            // Якщо натиснута клавіша Escape
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                }
            }
        }

        // Очищення вікна (заливаємо чорним кольором)
        window.clear(sf::Color::Black);

        // Малюємо нашу фігуру
        window.draw(shape);

        // Відображаємо намальоване на екрані
        window.display();
    }

    return 0;
}