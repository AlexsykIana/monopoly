//
// Created by Legion on 16.06.2025.
//
#include "StreetInitializer.h"

#include "Street.h"

struct TempStreetData {
    std::string name;
    int price;
    Street::StreetColorGroup group;
    float posX;
    float posY;
};

const TempStreetData propertyRawData[] = {
    //Brown group
    {"Mediterranean Avenue", 60, Street::StreetColorGroup::BROWN, 725.f, 825.f},
    {"Baltic Avenue", 60, Street::StreetColorGroup::BROWN, 580.f, 825.f},
    //Light_Blue group
    {"Oriental Avenue", 100, Street::StreetColorGroup::LIGHT_BLUE, 355.f, 825.f},
    {"Vermont Avenue", 100, Street::StreetColorGroup::LIGHT_BLUE, 210.f, 825.f},
    {"Connecticut Avenue", 120, Street::StreetColorGroup::LIGHT_BLUE, 137.f, 825.f},
    //Pink group
    {"St. Charles Place", 140, Street::StreetColorGroup::PINK, 35.f, 725.f},
    {"States Avenue", 140, Street::StreetColorGroup::PINK, 35.f, 580.f},
    {"Virginia Avenue", 160, Street::StreetColorGroup::PINK, 35.f, 505.f},
    //Orange group
    {"St. james Place", 180, Street::StreetColorGroup::ORANGE, 35.f, 360.f},
    {"Tennesse Avenue", 180, Street::StreetColorGroup::ORANGE, 35.f, 210.f},
    {"New York Avenue", 200, Street::StreetColorGroup::ORANGE, 35.f, 135.f},
    //Red group
    {"Kentucky Avenue", 220, Street::StreetColorGroup::RED, 137.f, 35.f},
    {"Indiana Avenue", 220, Street::StreetColorGroup::RED, 285.f, 35.f},
    {"Illinois Avenue", 240, Street::StreetColorGroup::RED, 355.f, 35.f},
    //Yellow group
    {"Atlantic Avenue", 260, Street::StreetColorGroup::YELLOW, 505.f, 35.f},
    {"Ventnor Avenue", 260, Street::StreetColorGroup::YELLOW, 580.f, 35.f},
    {"Marvin Gardens", 280, Street::StreetColorGroup::YELLOW, 725.f, 35.f},
    //Green group
    {"Pacific Avenue", 300, Street::StreetColorGroup::GREEN, 825.f, 135.f},
    {"North Carolina Avenue", 300, Street::StreetColorGroup::GREEN, 825.f, 210.f},
    {"Pennsylvania Avenue", 320, Street::StreetColorGroup::GREEN, 825.f, 360.f},
    //Blue group
    {"Park Place", 350, Street::StreetColorGroup::BLUE, 825.f, 580.f},
    {"Boardwalk", 400, Street::StreetColorGroup::BLUE, 825.f, 725.f},
    //Train group
    {"Reading Railroad", 200, Street::StreetColorGroup::TRAIN, 430.f, 825.f},
    {"Pennsylvania Railroad", 200, Street::StreetColorGroup::TRAIN, 35.f, 430},
    {"B. & O. Railroad", 200, Street::StreetColorGroup::TRAIN, 430.f, 35.f},
    {"Short Line", 200, Street::StreetColorGroup::TRAIN, 825.f, 430.f},
    //Communal group
    {"Electric Company", 150, Street::StreetColorGroup::COMMUNAL, 35.f, 650.f},
    {"Water Works", 150, Street::StreetColorGroup::COMMUNAL, 650.f, 35.f}
};

std::vector<Street> initializeAllProperties() {
    std::vector<Street> propertiesVector;
    propertiesVector.reserve(std::size(propertyRawData)); // Опціонально: резервуємо місце

    for (const auto& data : propertyRawData) {
        propertiesVector.emplace_back(data.name, data.price, data.group, sf::Vector2f(data.posX, data.posY));
    }

    return propertiesVector;
}

