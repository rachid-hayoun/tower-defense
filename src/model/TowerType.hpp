#pragma once

enum class TowerType {BASIC,SNIPER,CANNON};

struct TowerStats {
    int baseDamage;
    float baseFireRate;
    float baseRange;
    float baseRotationSpeed;
    int baseCost;
    
    struct {
        int damage;
        float fireRate;
        float range;
        float rotationSpeed;
    } levelStats[3];
};

class TowerConfig {
public:
    static TowerStats getStatsForType(TowerType type) {
        switch (type) {
            case TowerType::BASIC:
                return {
                    20, 0.8f, 200.0f, 180.0f, 30, 
                    {
                        {20, 0.8f, 200.0f, 180.0f},  
                        {35, 1.2f, 250.0f, 240.0f},  
                        {60, 1.8f, 280.0f, 320.0f}   
                    }
                };
                
            case TowerType::SNIPER:
                return {
                    80, 0.3f, 400.0f, 120.0f, 60, 
                    {
                        {80, 0.3f, 400.0f, 120.0f},  
                        {120, 0.4f, 450.0f, 150.0f},  
                        {200, 0.5f, 500.0f, 180.0f}   
                    }
                };
                
            case TowerType::CANNON:
                return {
                    150, 0.2f, 150.0f, 90.0f, 80, 
                    {
                        {150, 0.2f, 150.0f, 90.0f},   
                        {250, 0.25f, 170.0f, 120.0f}, 
                        {400, 0.3f, 200.0f, 150.0f}  
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
                return "Tower1.png";
            case TowerType::SNIPER:
                return "TowerSniper.png";
            case TowerType::CANNON:
                return "TowerCannon.png";
            default:
                return "Tower1.png";
        }
    }
};
