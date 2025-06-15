#pragma once

enum class TowerType {
    BASIC,      // Tour de base - équilibrée
    SNIPER,     // Tour de sniper - longue portée, dégâts élevés, tir lent
    CANNON      // Tour canon - courte portée, dégâts explosifs, tir très lent
};

struct TowerStats {
    int baseDamage;
    float baseFireRate;
    float baseRange;
    float baseRotationSpeed;
    int baseCost;
    
    // Stats par niveau
    struct {
        int damage;
        float fireRate;
        float range;
        float rotationSpeed;
    } levelStats[3]; // Pour les 3 niveaux
};

class TowerConfig {
public:
    static TowerStats getStatsForType(TowerType type) {
        switch (type) {
            case TowerType::BASIC:
                return {
                    20, 0.8f, 200.0f, 180.0f, 30, // Stats de base
                    {
                        {20, 0.8f, 200.0f, 180.0f},  // Niveau 1
                        {35, 1.2f, 250.0f, 240.0f},  // Niveau 2
                        {60, 1.8f, 280.0f, 320.0f}   // Niveau 3
                    }
                };
                
            case TowerType::SNIPER:
                return {
                    80, 0.3f, 400.0f, 120.0f, 60, // Stats de base
                    {
                        {80, 0.3f, 400.0f, 120.0f},   // Niveau 1
                        {120, 0.4f, 450.0f, 150.0f},  // Niveau 2
                        {200, 0.5f, 500.0f, 180.0f}   // Niveau 3
                    }
                };
                
            case TowerType::CANNON:
                return {
                    150, 0.2f, 150.0f, 90.0f, 80, // Stats de base
                    {
                        {150, 0.2f, 150.0f, 90.0f},   // Niveau 1
                        {250, 0.25f, 170.0f, 120.0f}, // Niveau 2
                        {400, 0.3f, 200.0f, 150.0f}   // Niveau 3
                    }
                };
                
            default:
                return getStatsForType(TowerType::BASIC);
        }
    }
    
    static std::string getTypeString(TowerType type) {
        switch (type) {
            case TowerType::BASIC:
                return "Tour de Base";
            case TowerType::SNIPER:
                return "Tour Sniper";
            case TowerType::CANNON:
                return "Tour Canon";
            default:
                return "Inconnu";
        }
    }
    
    static std::string getTextureFileName(TowerType type) {
        switch (type) {
            case TowerType::BASIC:
                return "Assets/Towers/Tower1.png";
            case TowerType::SNIPER:
                return "Assets/Towers/TowerSniper.png";
            case TowerType::CANNON:
                return "Assets/Towers/TowerCannon.png";
            default:
                return "Assets/Towers/Tower1.png";
        }
    }
};