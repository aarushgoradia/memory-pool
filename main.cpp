#include <iostream>
#include "memory_pool.hpp"

struct Enemy {
    int hp;
    float x, y;

    Enemy(int h, float xpos, float ypos) : hp(h), x(xpos), y(ypos) {
        std::cout << "Enemy constructed: HP=" << hp << " at (" << x << ", " << y << ")\n";
    }

    ~Enemy() {
        std::cout << "Enemy destroyed\n";
    }
};

int main() {
    size_t size = sizeof(MemoryBlock) + sizeof(Enemy);
    MemoryPool pool(size, 10);

    Enemy* e1 = pool.allocate<Enemy>(100, 1.0f, 2.0f);
    Enemy* e2 = pool.allocate<Enemy>(75, 3.0f, 4.0f);
    PoolPointer<Enemy> e3(pool.allocate<Enemy>(60, 5.0f, 6.0f), &pool);

    std::cout << "e1 HP:" << e1->hp << "\n";
    e2->hp -= 10;
    std::cout << "e2 HP:" << e2->hp << "\n";

    e3->x += 3.0f;
    std::cout << "e3 xpos:" << e3->x << "\n";


    pool.deallocate(e1);
    pool.deallocate(e2);

    return 0;
}