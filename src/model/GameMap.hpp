#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "Tower.hpp"
#include "TowerType.hpp"

class Enemy;

const int TILE_SIZE = 64;
const int MAP_WIDTH = 20;
const int MAP_HEIGHT = 15;
const int GRASS = 0;
const int PATH = 1;
const int PLAS = 2;
const int GRASSUP = 3;
const int GRASSDOWN = 4;
const int GRASSRIGHT = 5;
const int GRASSLEFT = 6;
const int GRASSLD = 7;
const int GRASSAR = 8;
const int GRASSAL = 9;
const int GRASSBL = 10;
const int GRASSBR = 11;
const int GRASSHR = 12;
const int GRASSHL = 13;
const int GRASSMO = 14;

class GameMap {
public:
    GameMap();
    bool loadTextures();
    void updateHover(int mouseX, int mouseY);
    void updateTowers(float deltaTime, const std::vector<Enemy*>& enemies);
    void draw(sf::RenderWindow& window);
    void drawTowerRanges(sf::RenderWindow& window);
    void drawTowerInfo(sf::RenderWindow& window, sf::Font& font);
    int getTileSize() const;
    int getMapWidth() const;
    int getMapHeight() const;
    bool isPath(int x, int y) const;
    bool isPlace(int x, int y) const;
    bool isGrass(int x, int y) const;

    void addTower(int tileX, int tileY, TowerType type = TowerType::BASIC);
    bool loadTowerTextures();
    
    Tower* getSelectedTower();
    void selectTower(int mouseX, int mouseY);
    void deselectTower();
    bool hasTowerAt(int tileX, int tileY) const;
    
    // Système d'amélioration
    bool upgradeTower(int mouseX, int mouseY);
    Tower* getTowerAt(int mouseX, int mouseY);

    std::vector<int> collectAllTowerRewards();

private:
    std::vector<std::vector<int>> mapData;
    sf::Texture grassTexture;
    sf::Texture pathTexture;
    sf::Texture placementTexture;
    sf::Texture grass2Texture;
    sf::Texture grass3Texture;
    sf::Texture grass4Texture;
    sf::Texture grass5Texture;
    sf::Texture grass6Texture;
    sf::Texture grass7Texture;
    sf::Texture grass8Texture;
    sf::Texture grass9Texture;
    sf::Texture grass10Texture;
    sf::Texture grass11Texture;
    sf::Texture grass12Texture;
    sf::Texture grass13Texture;
    
    // Textures pour chaque type de tour
    std::map<TowerType, sf::Texture> towerTextures;

    sf::Sprite grassSprite;
    sf::Sprite pathSprite;
    sf::Sprite placementSprite;
    sf::Sprite grass2Sprite;
    sf::Sprite grass3Sprite;
    sf::Sprite grass4Sprite;
    sf::Sprite grass5Sprite;
    sf::Sprite grass6Sprite;
    sf::Sprite grass7Sprite;
    sf::Sprite grass8Sprite;
    sf::Sprite grass9Sprite;
    sf::Sprite grass10Sprite;
    sf::Sprite grass11Sprite;
    sf::Sprite grass12Sprite;
    sf::Sprite grass13Sprite;

    std::vector<Tower> towers;
    std::vector<std::vector<bool>> towerPlacement; 
    Tower* selectedTower;

    sf::RectangleShape hoverIndicator;
    int hoveredTileX;
    int hoveredTileY;
    bool isValidHover;

    bool loadTexturesFromFiles();
    float calculateTowerRotation(int tileX, int tileY);
};