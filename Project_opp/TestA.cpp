//#include <iostream>
//#include <vector>
//#include <string>
//#include <algorithm>
//#include <cmath>
//
//enum class AbilityType { COMBAT, HEAL, BUFF };
//enum class InvaderBehavior { VIOLENT, COWARD };
//enum class DefenderBehavior { VIOLENT, LEGENDARY, COWARD };
//enum class SupportBehavior { NO_SUPPORT, SHAMAN, CLERIC };
//
//struct Position {
//    int x, y;
//};
//
//class Ability {
//public:
//    std::string name;
//    int damage, heal, buff;
//    AbilityType type;
//
//    Ability(std::string name, int damage, int heal, int buff, AbilityType type)
//        : name(name), damage(damage), heal(heal), buff(buff), type(type) {}
//};
//
//class Monster {
//private:
//    std::string name, type;
//    int attackPower, lifePoints, range;
//    Position position;
//    InvaderBehavior invaderBehavior;
//    DefenderBehavior defenderBehavior;
//    SupportBehavior supportBehavior;
//    std::vector<Ability> abilities;
//
//public:
//    Monster(std::string name, std::string type, int attackPower, int lifePoints, Position position, InvaderBehavior invaderBehavior, DefenderBehavior defenderBehavior, SupportBehavior supportBehavior)
//        : name(name), type(type), attackPower(attackPower), lifePoints(lifePoints), position(position), invaderBehavior(invaderBehavior), defenderBehavior(defenderBehavior), supportBehavior(supportBehavior) {}
//
//    void addAbility(Ability ability) {
//        abilities.push_back(ability);
//    }
//
//    std::string getName() const { return name; }
//    int getLifePoints() const { return lifePoints; }
//    Position getPosition() const { return position; }
//    bool isAlive() const { return lifePoints > 0; }
//
//    void move(char direction, const std::vector<std::vector<char>>& maze) {
//        Position newPosition = position;
//        switch (direction) {
//        case 'N': newPosition.x--; break;
//        case 'S': newPosition.x++; break;
//        case 'E': newPosition.y++; break;
//        case 'W': newPosition.y--; break;
//        }
//        if (newPosition.x >= 0 && newPosition.x < maze.size() && newPosition.y >= 0 && newPosition.y < maze[0].size() && maze[newPosition.x][newPosition.y] != '#') {
//            position = newPosition;
//        }
//    }
//
//    void interact(Monster& other, const std::vector<std::vector<char>>& maze) {
//        for (const auto& ability : abilities) {
//            if (ability.type == AbilityType::COMBAT) {
//                other.lifePoints -= ability.damage;
//            }
//            else if (ability.type == AbilityType::HEAL) {
//                lifePoints += ability.heal;
//            }
//            else if (ability.type == AbilityType::BUFF) {
//                attackPower += ability.buff;
//            }
//        }
//    }
//};
//
//void threeRoundFight() {
//    std::vector<std::vector<char>> maze = {
//        {' ', ' ', ' ', '#', ' '},
//        {' ', '#', ' ', '#', ' '},
//        {' ', '#', ' ', ' ', ' '},
//        {' ', ' ', '#', ' ', ' '},
//        {'#', ' ', ' ', ' ', ' '}
//    };
//
//    Monster dragon("NORMAL_DRAGON_1", "DRAGON", 100, 1000, { 1, 1 }, InvaderBehavior::VIOLENT, DefenderBehavior::VIOLENT, SupportBehavior::SHAMAN);
//    Monster spider("NORMAL_SPIDER_1", "SPIDER", 70, 400, { 2, 1 }, InvaderBehavior::VIOLENT, DefenderBehavior::VIOLENT, SupportBehavior::CLERIC);
//
//    Ability dragonStrike("DRAGON_STRIKE", 40, 0, 10, AbilityType::COMBAT);
//    Ability dragonHeal("DRAGON_HEAL", 0, 20, 0, AbilityType::HEAL);
//    Ability dragonBuff("DRAGON_BUFF", 0, 0, 10, AbilityType::BUFF);
//
//    Ability pounce("POUNCE", 10, 0, 5, AbilityType::COMBAT);
//    Ability webAttack("WEB_ATTACK", 20, 0, 2, AbilityType::COMBAT);
//    Ability cocoonHeal("COCOON_HEAL", 0, 10, 10, AbilityType::HEAL);
//
//    dragon.addAbility(dragonStrike);
//    dragon.addAbility(dragonHeal);
//    dragon.addAbility(dragonBuff);
//
//    spider.addAbility(pounce);
//    spider.addAbility(webAttack);
//    spider.addAbility(cocoonHeal);
//
//    for (int round = 1; round <= 3; ++round) {
//        std::cout << "Round " << round << ":\n";
//        dragon.interact(spider, maze);
//        if (!spider.isAlive()) {
//            std::cout << spider.getName() << " has been defeated!\n";
//            return;
//        }
//        spider.interact(dragon, maze);
//        if (!dragon.isAlive()) {
//            std::cout << dragon.getName() << " has been defeated!\n";
//            return;
//        }
//    }
//
//    std::cout << "After 3 rounds:\n";
//    std::cout << dragon.getName() << " DRAGON " << dragon.getLifePoints() << " " << dragon.getPosition().x << " " << dragon.getPosition().y << "\n";
//    std::cout << spider.getName() << " SPIDER " << spider.getLifePoints() << " " << spider.getPosition().x << " " << spider.getPosition().y << "\n";
//}
//
//int main() {
//    threeRoundFight();
//    return 0;
//}
