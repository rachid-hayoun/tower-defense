#pragma once
#include <SFML/Graphics.hpp>

class GameState {
public:
    GameState();
    
    // Gestion de l'argent
    int getMoney() const;
    bool canAfford(int cost) const;
    bool spendMoney(int cost);
    void addMoney(int amount);
    
    // Gestion des vies
    int getLives() const;
    void loseLife();
    bool isGameOver() const;
    
    // Coûts des tours
    static const int BASIC_TOWER_COST = 50;
    static const int ADVANCED_TOWER_COST = 75;
    static const int HEAVY_TOWER_COST = 100;
    
    // Interface utilisateur
    void drawUI(sf::RenderWindow& window, sf::Font& font);
    void setUIPosition(float x, float y);
    
private:
    int money;
    int lives;
    
    // Éléments UI
    sf::Text moneyText;
    sf::Text livesText;
    sf::Text towerCostText;
    sf::Vector2f uiPosition;
    
    void updateUITexts();
};