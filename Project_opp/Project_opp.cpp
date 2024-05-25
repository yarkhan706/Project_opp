#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>

enum class AbilityType { COMBAT, HEAL, BUFF, FLEE, FEAR };
enum class SpecialEffect { NO_RETALIATION, FIRST_STRIKE, FLEE, FEAR };
enum class InvaderBehavior { VIOLENT, COWARD, ASSASSIN };
enum class DefenderBehavior { VIOLENT, COWARD, LEGENDARY };
enum class SupportBehavior { NO_SUPPORT, CLERIC, SHAMAN, FLEE_SUPPORT, FEAR_SUPPORT };

struct Position {
    int x, y;
};

class Ability {
private:
    std::string name;
    int basePower;
    int powerCoefficient;
    int lifeCoefficient;
    AbilityType type;
    std::vector<SpecialEffect> specialEffects;

public:
    Ability(std::string n, int bp, int pc, int lc, AbilityType t, std::vector<SpecialEffect> se = {})
        : name(n), basePower(bp), powerCoefficient(pc), lifeCoefficient(lc), type(t), specialEffects(se) {}

    int getEffectivePower(int creaturePower, int creatureHealth) const {
        int powerQuotient = std::floor(creaturePower / 100.0);
        int healthQuotient = std::floor(creatureHealth / 100.0);
        return basePower + (powerCoefficient * powerQuotient) + (lifeCoefficient * healthQuotient);
    }

    AbilityType getType() const { return type; }
    std::string getName() const { return name; }
    const std::vector<SpecialEffect>& getSpecialEffects() const { return specialEffects; }
};

class Monster {
private:
    std::string name;
    std::string species;
    int attackPower;
    int lifePoints;
    Position position;
    InvaderBehavior invaderBehavior;
    DefenderBehavior defenderBehavior;
    SupportBehavior supportBehavior;
    std::vector<Ability> abilities;
    std::vector<SpecialEffect> specialEffects;

public:
    Monster(std::string n, std::string s, int ap, int lp, Position pos, InvaderBehavior ib, DefenderBehavior db, SupportBehavior sb)
        : name(n), species(s), attackPower(ap), lifePoints(lp), position(pos), invaderBehavior(ib), defenderBehavior(db), supportBehavior(sb) {}

    Position getPosition() const {
        return position;
    }

    void move(char direction, const std::vector<std::vector<char>>& maze) {
        int newX = position.x;
        int newY = position.y;
        switch (direction) {
        case 'N': newX--; break;
        case 'E': newY++; break;
        case 'S': newX++; break;
        case 'W': newY--; break;
        }
        if (newX > 0 && newX <= maze.size() && newY > 0 && newY <= maze[0].size() && maze[newX - 1][newY - 1] != '#') {
            position.x = newX;
            position.y = newY;
        }
        std::cout << name << " moved to (" << position.x << ", " << position.y << ")\n";
    }

    bool isAlive() const {
        return lifePoints > 0;
    }

    std::string getName() const {
        return name;
    }

    std::string getSpecies() const {
        return species;
    }

    int getAttackPower() const {
        return attackPower;
    }

    int getLifePoints() const {
        return lifePoints;
    }

    void interact(Monster& other, const std::vector<std::vector<char>>& maze);

    bool hasSpecialEffect(SpecialEffect effect) const;

    void addAbility(const Ability& ability);

    Ability getStrongestAbility(AbilityType type) const;

    void executeAbility(const Ability& ability, Monster& target, const std::vector<std::vector<char>>& maze);

    void combatInteraction(Monster& other, const std::vector<std::vector<char>>& maze);

    void supportInteraction(Monster& other, const std::vector<std::vector<char>>& maze);

    void applyDamage(int damage) {
        lifePoints -= damage;
        std::cout << name << " received " << damage << " damage, remaining life points: " << lifePoints << "\n";
    }

    void heal(int amount) {
        lifePoints += amount;
        std::cout << name << " healed " << amount << " life points, new life points: " << lifePoints << "\n";
    }

    void buff(int amount) {
        attackPower += amount;
        std::cout << name << " buffed " << amount << " attack power, new attack power: " << attackPower << "\n";
    }

    bool attemptRetreat(const std::vector<std::vector<char>>& maze);
};

bool Monster::attemptRetreat(const std::vector<std::vector<char>>& maze) {
    std::vector<Position> directions = { {position.x - 1, position.y}, {position.x, position.y + 1}, {position.x + 1, position.y}, {position.x, position.y - 1} };
    for (const auto& dir : directions) {
        if (dir.x > 0 && dir.x <= maze.size() && dir.y > 0 && dir.y <= maze[0].size() && maze[dir.x - 1][dir.y - 1] != '#') {
            position = dir;
            std::cout << name << " retreated to (" << position.x << ", " << position.y << ")\n";
            return true;
        }
    }
    return false;
}

void Monster::interact(Monster& other, const std::vector<std::vector<char>>& maze) {
    if (!isAlive() || !other.isAlive()) {
        return; // No interaction if either monster is dead
    }

    if (species == other.species) {
        supportInteraction(other, maze);
    }
    else {
        combatInteraction(other, maze);
    }
}

bool Monster::hasSpecialEffect(SpecialEffect effect) const {
    return std::find(specialEffects.begin(), specialEffects.end(), effect) != specialEffects.end();
}

void Monster::addAbility(const Ability& ability) {
    abilities.push_back(ability);
}

Ability Monster::getStrongestAbility(AbilityType type) const {
    Ability strongestAbility = abilities[0];
    int maxPower = 0;
    for (const Ability& ability : abilities) {
        if (ability.getType() == type) {
            int effectivePower = ability.getEffectivePower(attackPower, lifePoints);
            if (effectivePower > maxPower) {
                maxPower = effectivePower;
                strongestAbility = ability;
            }
        }
    }
    return strongestAbility;
}

void Monster::executeAbility(const Ability& ability, Monster& target, const std::vector<std::vector<char>>& maze) {
    int effectivePower = ability.getEffectivePower(attackPower, lifePoints);
    switch (ability.getType()) {
    case AbilityType::COMBAT:
        target.applyDamage(effectivePower);
        break;
    case AbilityType::HEAL:
        heal(effectivePower);
        break;
    case AbilityType::BUFF:
        buff(effectivePower);
        break;
    case AbilityType::FLEE:
        attemptRetreat(maze); // Attempt to retreat when using flee ability
        break;
    case AbilityType::FEAR:
        if (target.getLifePoints() < effectivePower) {
            target.attemptRetreat(maze); // Target attempts to retreat when feared
        }
        break;
    }
    std::cout << name << " uses " << ability.getName() << " on " << target.getName() << " with effective power " << effectivePower << std::endl;
}

void Monster::combatInteraction(Monster& other, const std::vector<std::vector<char>>& maze) {
    std::cout << getName() << " engages in combat with " << other.getName() << "\n";

    // New condition: cowardly invader or defender attempting to flee if health is low
    if ((invaderBehavior == InvaderBehavior::COWARD || defenderBehavior == DefenderBehavior::COWARD) && getLifePoints() < other.getAttackPower()) {
        if (attemptRetreat(maze)) {
            std::cout << getName() << " retreated from " << other.getName() << " due to low health\n";
            return;
        }
    }

    // Check if the defender fears the invader
    if ((defenderBehavior == DefenderBehavior::COWARD) && other.getLifePoints() < getAttackPower()) {
        if (other.attemptRetreat(maze)) {
            std::cout << other.getName() << " retreated from " << getName() << " due to fear\n";
            return;
        }
    }

    // Check if the invader fears the defender
    if ((invaderBehavior == InvaderBehavior::COWARD) && getLifePoints() < other.getAttackPower()) {
        if (attemptRetreat(maze)) {
            std::cout << getName() << " retreated from " << other.getName() << " due to fear\n";
            return;
        }
    }

    Ability invaderAbility = getStrongestAbility(AbilityType::COMBAT);
    Ability defenderAbility = other.getStrongestAbility(AbilityType::COMBAT);

    bool invaderFirstStrike = hasSpecialEffect(SpecialEffect::FIRST_STRIKE);
    bool defenderFirstStrike = other.hasSpecialEffect(SpecialEffect::FIRST_STRIKE);

    if (invaderFirstStrike) {
        executeAbility(invaderAbility, other, maze);
    }

    if (defenderFirstStrike) {
        other.executeAbility(defenderAbility, *this, maze);
    }

    if (!invaderFirstStrike) {
        executeAbility(invaderAbility, other, maze);
    }

    if (!defenderFirstStrike) {
        other.executeAbility(defenderAbility, *this, maze);
    }

    // No retaliation
    if (hasSpecialEffect(SpecialEffect::NO_RETALIATION) && other.isAlive()) {
        std::cout << other.getName() << " cannot retaliate against " << getName() << " due to No Retaliation effect\n";
    }
}

void Monster::supportInteraction(Monster& other, const std::vector<std::vector<char>>& maze) {
    std::cout << getName() << " supports " << other.getName() << "\n";
    if (supportBehavior == SupportBehavior::CLERIC) {
        Ability healingAbility = getStrongestAbility(AbilityType::HEAL);
        executeAbility(healingAbility, other, maze);
    }
    else if (supportBehavior == SupportBehavior::SHAMAN) {
        Ability buffAbility = getStrongestAbility(AbilityType::BUFF);
        executeAbility(buffAbility, other, maze);
    }
    else if (supportBehavior == SupportBehavior::FLEE_SUPPORT) {
        Ability fleeAbility = getStrongestAbility(AbilityType::FLEE);
        executeAbility(fleeAbility, other, maze);
    }
    else if (supportBehavior == SupportBehavior::FEAR_SUPPORT) {
        Ability fearAbility = getStrongestAbility(AbilityType::FEAR);
        executeAbility(fearAbility, other, maze);
    }
}

void runTest(int N, int M, int D, int K, const std::vector<std::string>& mazeLayout, std::vector<Monster>& monsters, const std::vector<std::vector<char>>& actionsPerDay) {
    std::vector<std::vector<char>> maze(N, std::vector<char>(M));

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < M; ++j) {
            maze[i][j] = mazeLayout[i][j];
        }
    }

    for (int day = 0; day < D; ++day) {
        std::cout << "Day " << day + 1 << ":\n";

        for (int i = 0; i < K; ++i) {
            monsters[i].move(actionsPerDay[day][i], maze);
        }

        for (int i = 0; i < K; ++i) {
            for (int j = i + 1; j < K; ++j) {
                if (monsters[i].isAlive() && monsters[j].isAlive() &&
                    monsters[i].getPosition().x == monsters[j].getPosition().x &&
                    monsters[i].getPosition().y == monsters[j].getPosition().y) {
                    monsters[i].interact(monsters[j], maze);
                }
            }
        }

        monsters.erase(std::remove_if(monsters.begin(), monsters.end(), [](const Monster& m) { return !m.isAlive(); }), monsters.end());
    }

    std::cout << "Remaining monsters:\n";
    for (const Monster& monster : monsters) {
        std::cout << monster.getName() << " " << monster.getSpecies() << " " << monster.getAttackPower() << " " << monster.getLifePoints() << " " << monster.getPosition().x << " " << monster.getPosition().y << std::endl;
    }
}



int main() {
    std::vector<std::string> mazeLayout = {
        "_####",
        "_#__#",
        "_#_##",
        "___##"
    };

    std::vector<Monster> monsters = {
        Monster("NORMAL_DRAGON_1", "DRAGON", 100, 1000, {2, 1}, InvaderBehavior::VIOLENT, DefenderBehavior::VIOLENT, SupportBehavior::CLERIC),
        Monster("LEGENDARY_DRAGON_ELROE", "DRAGON", 420, 4200, {2, 4}, InvaderBehavior::VIOLENT, DefenderBehavior::VIOLENT, SupportBehavior::SHAMAN),
        Monster("NORMAL_SPIDER_1", "SPIDER", 70, 400, {3, 1}, InvaderBehavior::VIOLENT, DefenderBehavior::VIOLENT, SupportBehavior::CLERIC)
    };

    monsters[0].addAbility(Ability("DRAGON_STRIKE", 20, 10, 5, AbilityType::COMBAT));
    monsters[0].addAbility(Ability("DRAGON_HEAL", 10, 5, 5, AbilityType::HEAL));
    monsters[0].addAbility(Ability("DRAGON_BUFF", 10, 5, 5, AbilityType::BUFF));
    monsters[0].addAbility(Ability("DRAGON_FLIGHT", 100, 10, 5, AbilityType::COMBAT, { SpecialEffect::FIRST_STRIKE }));

    monsters[1].addAbility(Ability("ELROE_FLIGHT", 2, 1, 1, AbilityType::COMBAT));
    monsters[1].addAbility(Ability("ELROE_STRIKE", 20, 10, 5, AbilityType::COMBAT));
    monsters[1].addAbility(Ability("ELROE_BLESSING", 10, 5, 5, AbilityType::BUFF));

    monsters[2].addAbility(Ability("COCOON_HEAL", 30, 10, 5, AbilityType::HEAL));
    monsters[2].addAbility(Ability("POUNCE", 30, 10, 5, AbilityType::COMBAT));
    monsters[2].addAbility(Ability("WEB_ATTACK", 100, 20, 10, AbilityType::COMBAT, { SpecialEffect::FIRST_STRIKE }));

    std::vector<std::vector<char>> actionsPerDay = {
        {'S', 'E', 'E'},
        {'S', 'E', 'E'},
        {'S', 'E', 'E'}
    };

    runTest(4, 5, 3, 3, mazeLayout, monsters, actionsPerDay);

    return 0;
}

