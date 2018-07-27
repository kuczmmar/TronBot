#include "map.h"
#include <iostream>
#include <utility>
#include <limits>
#include <cstdio>
using namespace std;

int main()
{
    //std::freopen("/home/marta/visual/trongame/map0", "r", stdin);
    Map mapa;
    mapa.readmap();
    mapa.findpos();

    if (mapa.walls_around())
        cout << '1' << endl;

    pair<int, int> enemy = mapa.findenemy();

    if (enemy.first != -1)
        mapa.wheretomove_minmax();
    else
        mapa.bestmove();
}
