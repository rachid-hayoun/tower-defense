#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Enemy.hpp"

class GameMap;

// Remove the duplicate EnemyType enum - it's already defined in Enemy.hpp

struct WaveEnemy {
    EnemyType type;
    float spawnDelay; 
};

struct Wave {
    std::vector<WaveEnemy> enemies;
    float delayAfterWave;
    int waveNumber;
};

class WaveManager {
public:
    WaveManager(GameMap* map);
    
    void update(float deltaTime);
    void startNextWave();
    bool isWaveActive() const;
    bool allWavesCompleted() const;
    
    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const;
    int getCurrentWaveNumber() const;
    int getTotalWaves() const;
    float getTimeUntilNextWave() const;
    bool isWaveInProgress() const;
    
    void removeDeadEnemies();
    void cleanupProcessedEnemies(); // Add this declaration
    
private:
    GameMap* gameMap;
    
    std::vector<Wave> waves;
    int currentWaveIndex;
    bool waveActive;
    bool waveInProgress;
    
    float timeSinceWaveStart;
    float timeUntilNextWave;
    
    std::vector<std::unique_ptr<Enemy>> activeEnemies;
    
    int currentEnemyIndex;
    float timeUntilNextSpawn;
    
    void initializeWaves();
    void spawnEnemy(EnemyType type);
    std::unique_ptr<Enemy> createEnemy(EnemyType type);
    void updateWaveProgress(float deltaTime);
    void updateEnemySpawning(float deltaTime);
};