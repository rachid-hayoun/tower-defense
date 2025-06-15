#include "GameState.hpp"
#include <iostream>

GameState::GameState() : money(150), lives(20), uiPosition(10.0f, 10.0f) {
    // Configuration des textes UI
    moneyText.setCharacterSize(20);
    moneyText.setFillColor(sf::Color::Yellow);
    
    livesText.setCharacterSize(20);
    livesText.setFillColor(sf::Color::Red);
    
    towerCostText.setCharacterSize(16);
    towerCostText.setFillColor(sf::Color::White);
    
    updateUITexts();
}

int GameState::getMoney() const {
    return money;
}

bool GameState::canAfford(int cost) const {
    return money >= cost;
}

bool GameState::spendMoney(int cost) {
    if (canAfford(cost)) {
        money -= cost;
        updateUITexts();
        std::cout << "Dépensé " << cost << " pièces. Reste: " << money << std::endl;
        return true;
    }
    std::cout << "Pas assez d'argent ! Coût: " << cost << ", Disponible: " << money << std::endl;
    return false;
}

void GameState::addMoney(int amount) {
    money += amount;
    updateUITexts();
    std::cout << "Gagné " << amount << " pièces. Total: " << money << std::endl;
}

int GameState::getLives() const {
    return lives;
}

void GameState::loseLife() {
    if (lives > 0) {
        lives--;
        updateUITexts();
        std::cout << "Vie perdue ! Vies restantes: " << lives << std::endl;
    }
}

bool GameState::isGameOver() const {
    return lives <= 0;
}

void GameState::updateUITexts() {
    moneyText.setString("Argent: " + std::to_string(money));
    livesText.setString("Vies: " + std::to_string(lives));
    
    towerCostText.setString("Tours - Basique: " + std::to_string(BASIC_TOWER_COST) + 
                           " | Avancée: " + std::to_string(ADVANCED_TOWER_COST) + 
                           " | Lourde: " + std::to_string(HEAVY_TOWER_COST));
}

void GameState::drawUI(sf::RenderWindow& window, sf::Font& font) {
    // Définir la police pour tous les textes
    moneyText.setFont(font);
    livesText.setFont(font);
    towerCostText.setFont(font);
    
    // Positionner les éléments UI
    moneyText.setPosition(uiPosition.x, uiPosition.y);
    livesText.setPosition(uiPosition.x + 200.0f, uiPosition.y);
    towerCostText.setPosition(uiPosition.x, uiPosition.y + 30.0f);
    
    // Dessiner les éléments UI
    window.draw(moneyText);
    window.draw(livesText);
    window.draw(towerCostText);
}

void GameState::setUIPosition(float x, float y) {
    uiPosition = sf::Vector2f(x, y);
}