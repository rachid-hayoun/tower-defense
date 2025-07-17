#pragma once
#include <SFML/Graphics.hpp>

class GameState {
public:
    GameState();
    
    int getMoney() const;
    bool canAfford(int cost) const;
    bool spendMoney(int cost);
    void addMoney(int amount);
    
    int getLives() const;
    void loseLife();
    bool isGameOver() const;
    
IC_TOWER_COST = 50;
    static const int ADVANCED_TOWER_COST = 75;
    static const int HEAVY_TOWER_COST = 100;
    
    void drawUI(sf::RenderWindow& window, sf::Font& font);
    void setUIPosition(float x, float y);
    
private:
    int money;
    int lives;
    
    sf::Text moneyText;
    sf::Text livesText;
    sf::Text towerCostText;
    sf::Vector2f uiPosition;
    
    void updateUITexts();
};
