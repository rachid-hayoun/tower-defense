#include "WaveManager.hpp"
#include "GameMap.hpp"
#include <iostream>

WaveManager::WaveManager(GameMap* map) : gameMap(map) {
    currentWaveIndex = 0;
    waveActive = false;
    waveInProgress = false;
    timeSinceWaveStart = 0.0f;
    timeUntilNextWave = 0.0f;
    currentEnemyIndex = 0;
    timeUntilNextSpawn = 0.0f;
    
    initializeWaves();
}

void WaveManager::initializeWaves() {
    waves.clear();
    
    Wave wave1;
    wave1.waveNumber = 1;
    wave1.delayAfterWave = 5.0f;
    for (int i = 0; i < 5; i++) {
        wave1.enemies.push_back({EnemyType::BASIC, i * 1.0f});
    }
    waves.push_back(wave1);
    
    Wave wave2;
    wave2.waveNumber = 2;
    wave2.delayAfterWave = 8.0f;
    for (int i = 0; i < 8; i++) {
        wave2.enemies.push_back({EnemyType::BASIC, i * 0.8f});
    }
    waves.push_back(wave2);
    
    Wave wave3;
    wave3.waveNumber = 3;
    wave3.delayAfterWave = 10.0f;
    for (int i = 0; i < 6; i++) {
        wave3.enemies.push_back({EnemyType::BASIC, i * 1.0f});
    }
    for (int i = 0; i < 3; i++) {
        wave3.enemies.push_back({EnemyType::FAST, 6.0f + i * 0.5f});
    }
    waves.push_back(wave3);
    
    Wave wave4;
    wave4.waveNumber = 4;
    wave4.delayAfterWave = 12.0f;
    for (int i = 0; i < 4; i++) {
        wave4.enemies.push_back({EnemyType::BASIC, i * 0.5f});
    }
    for (int i = 0; i < 2; i++) {
        wave4.enemies.push_back({EnemyType::TANK, 2.0f + i * 2.0f});
    }
    for (int i = 0; i < 4; i++) {
        wave4.enemies.push_back({EnemyType::FAST, 6.0f + i * 0.3f});
    }
    waves.push_back(wave4);
    
    Wave wave5;
    wave5.waveNumber = 5;
    wave5.delayAfterWave = 0.0f;
    for (int i = 0; i < 10; i++) {
        wave5.enemies.push_back({EnemyType::BASIC, i * 0.3f});
    }
    for (int i = 0; i < 5; i++) {
        wave5.enemies.push_back({EnemyType::FAST, 3.0f + i * 0.4f});
    }
    for (int i = 0; i < 3; i++) {
        wave5.enemies.push_back({EnemyType::TANK, 5.0f + i * 1.5f});
    }
    waves.push_back(wave5);
}

void WaveManager::update(float deltaTime) {
    if (waveActive) {
        updateWaveProgress(deltaTime);
        updateEnemySpawning(deltaTime);
    } else if (timeUntilNextWave > 0) {
        timeUntilNextWave -= deltaTime;
    }
    
    // Mettre à jour tous les ennemis actifs
    for (auto& enemy : activeEnemies) {
        if (enemy && !enemy->isDead() && !enemy->hasReachedEnd()) {
            enemy->update(deltaTime);
        }
    }
    
    // Note: On ne supprime plus automatiquement les ennemis morts ici
    // pour permettre au main.cpp de récupérer les récompenses d'abord
}

void WaveManager::updateWaveProgress(float deltaTime) {
    timeSinceWaveStart += deltaTime;
    
    if (currentEnemyIndex >= waves[currentWaveIndex].enemies.size()) {
        bool allEnemiesGone = true;
        for (const auto& enemy : activeEnemies) {
            if (enemy && !enemy->isDead() && !enemy->hasReachedEnd()) {
                allEnemiesGone = false;
                break;
            }
        }
        
        if (allEnemiesGone) {
            waveActive = false;
            waveInProgress = false;
            
            if (currentWaveIndex < waves.size() - 1) {
                timeUntilNextWave = waves[currentWaveIndex].delayAfterWave;
                currentWaveIndex++;
                std::cout << "Vague " << waves[currentWaveIndex-1].waveNumber 
                         << " terminée ! Prochaine vague dans " 
                         << timeUntilNextWave << " secondes." << std::endl;
            } else {
                std::cout << "Toutes les vagues terminées ! Victoire !" << std::endl;
            }
            
            // Nettoyer les ennemis morts/arrivés après la fin de vague
            removeDeadEnemies();
        }
    }
}

void WaveManager::updateEnemySpawning(float deltaTime) {
    if (currentEnemyIndex >= waves[currentWaveIndex].enemies.size()) {
        return;
    }
    
    timeUntilNextSpawn -= deltaTime;
    
    if (timeUntilNextSpawn <= 0) {
        WaveEnemy& enemyToSpawn = waves[currentWaveIndex].enemies[currentEnemyIndex];
        spawnEnemy(enemyToSpawn.type);
        currentEnemyIndex++;
        
        if (currentEnemyIndex < waves[currentWaveIndex].enemies.size()) {
            timeUntilNextSpawn = waves[currentWaveIndex].enemies[currentEnemyIndex].spawnDelay - 
                                waves[currentWaveIndex].enemies[currentEnemyIndex-1].spawnDelay;
        }
    }
}

void WaveManager::spawnEnemy(EnemyType type) {
    auto enemy = createEnemy(type);
    if (enemy) {
        activeEnemies.push_back(std::move(enemy));
    }
}

std::unique_ptr<Enemy> WaveManager::createEnemy(EnemyType type) {
    float startX = 32.0f;
    float startY = 160.0f;
    
    auto enemy = std::make_unique<Enemy>(startX, startY, gameMap, type);
    
    switch (type) {
        case EnemyType::BASIC:
            // Valeurs par défaut : 100 HP, 150 speed, 10 reward
            break;
            
        case EnemyType::FAST:
            enemy->setSpeed(250.0f);
            enemy->setHealth(60);
            enemy->setReward(15);
            break;
            
        case EnemyType::TANK:
            enemy->setSpeed(80.0f);
            enemy->setHealth(250);
            enemy->setReward(25);
            break;
    }
    
    if (!enemy->loadTexture()) {
        std::cout << "Erreur lors du chargement de la texture pour l'ennemi type " << (int)type << std::endl;
        return nullptr;
    }
    
    return enemy;
}

void WaveManager::startNextWave() {
    if (currentWaveIndex < waves.size() && !waveActive) {
        waveActive = true;
        waveInProgress = true;
        timeSinceWaveStart = 0.0f;
        currentEnemyIndex = 0;
        timeUntilNextSpawn = waves[currentWaveIndex].enemies[0].spawnDelay;
        timeUntilNextWave = 0.0f;
        
        std::cout << "Vague " << waves[currentWaveIndex].waveNumber 
                 << " commencée ! (" << waves[currentWaveIndex].enemies.size() 
                 << " ennemis)" << std::endl;
    }
}

void WaveManager::removeDeadEnemies() {
    activeEnemies.erase(
        std::remove_if(activeEnemies.begin(), activeEnemies.end(),
            [](const std::unique_ptr<Enemy>& enemy) {
                return !enemy || enemy->isDead() || enemy->hasReachedEnd();
            }),
        activeEnemies.end()
    );
}

// Nouvelle méthode pour forcer le nettoyage depuis main.cpp
void WaveManager::cleanupProcessedEnemies() {
    removeDeadEnemies();
}

bool WaveManager::isWaveActive() const {
    return waveActive;
}

bool WaveManager::allWavesCompleted() const {
    return currentWaveIndex >= waves.size() && !waveActive;
}

const std::vector<std::unique_ptr<Enemy>>& WaveManager::getEnemies() const {
    return activeEnemies;
}

int WaveManager::getCurrentWaveNumber() const {
    if (currentWaveIndex < waves.size()) {
        return waves[currentWaveIndex].waveNumber;
    }
    return waves.size();
}

int WaveManager::getTotalWaves() const {
    return waves.size();
}

float WaveManager::getTimeUntilNextWave() const {
    return timeUntilNextWave;
}

bool WaveManager::isWaveInProgress() const {
    return waveInProgress;
}