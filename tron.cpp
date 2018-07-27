#include "map.h"
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <algorithm>
using namespace std;

void Map::readmap()
{
    string val;
    cin >> width >> height;
    getline(cin, val);
    while (getline(cin, val))
    {
        vec.push_back(val);
    }
}

void Map::findpos()
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (vec[i][j] == '1')
            {
                myrow = i;
                mycol = j;
            }
            if (vec[i][j] == '2')
            {
                enrow = i;
                encol = j;
            }
        }
    }
}

pair<int, int> Map::findenemy()
{
    queue<pair<int, int>> que;
    int r, c;
    vector<vector<bool>> visit(height, vector<bool>(width));
    que.push(make_pair(myrow, mycol));
    visit[myrow][mycol] = true;

    while (!que.empty())
    {
        pair<int, int> u = que.front();
        que.pop();
        r = u.first;
        c = u.second;

        if (vec[r][c] == '2')
            return make_pair(r, c);

        for (int i = 0; i < 4; i++)
        {
            int p1 = r + t1[i];
            int p2 = c + t2[i];
            if (!visit[p1][p2] && vec[p1][p2] != '#')
            {
                pair<int, int> pole(p1, p2);
                que.push(pole);
                visit[p1][p2] = true;
            }
        }
    }
    return make_pair(-1, -1);
}

// NO ENEMY
void Map::bestmove()
{
    queue<state *> que;
    state pocz;
    pocz.c = mycol;
    pocz.r = myrow;
    pocz.mapa = vec;
    pocz.parent = NULL;
    pocz.depth = 1;
    que.push(&pocz);
    while (!que.empty())
    {
        state *ptr = que.front();
        que.pop();
        ptr->wyn = 0;
        ptr->mapa[ptr->r][ptr->c] = '#';
        bool wallsAround = true;

        for (int i = 0; i < 4; i++)
        {
            int p1 = ptr->r + t1[i];
            int p2 = ptr->c + t2[i];
            if (ptr->mapa[p1][p2] == ' ')
                wallsAround = false;
        }

        if (ptr->depth == MAX_DEPTH || wallsAround)
            ptr->wyn = howmuchspace(ptr);

        else
        {
            for (int i = 0; i < 4; i++)
            {
                int p1 = ptr->r + t1[i];
                int p2 = ptr->c + t2[i];
                if (ptr->mapa[p1][p2] == ' ')
                {
                    //state* child=new state;
                    state *child = new (tab + tab_index) state;
                    tab_index += sizeof(state);

                    child->parent = ptr;
                    child->mapa = ptr->mapa;
                    child->r = p1;
                    child->c = p2;
                    child->depth = ptr->depth + 1;
                    ptr->dzieci[i] = child;
                    que.push(child);
                }
            }
        }
    }

    findmax(&pocz);
    int r, c;
    pocz.wyn = 0;
    for (int i = 0; i < 4; i++)
    {
        if (pocz.dzieci[i] != 0 && pocz.wyn < pocz.dzieci[i]->wyn)
        {
            pocz.wyn = pocz.dzieci[i]->wyn;
            r = pocz.dzieci[i]->r;
            c = pocz.dzieci[i]->c;
        }
    }
    if (r - myrow == -1)
        cout << '1' << endl;
    else if (r - myrow == 1)
        cout << '3' << endl;
    else if (c - mycol == -1)
        cout << '4' << endl;
    else if (c - mycol == 1)
        cout << '2' << endl;
}

int Map::howmuchspace(state *ptr)
{
    queue<pair<int, int>> que;
    que.push(make_pair(ptr->r, ptr->c));
    vector<vector<bool>> visit(height, vector<bool>(width));
    visit[ptr->r][ptr->c] = true;
    int licznik = 0;

    while (!que.empty())
    {
        pair<int, int> u = que.front();
        que.pop();
        licznik++;
        int r = u.first;
        int c = u.second;

        for (int i = 0; i < 4; i++)
        {
            int p1 = r + t1[i];
            int p2 = c + t2[i];
            if (!visit[p1][p2] && ptr->mapa[p1][p2] != '#')
            {
                que.push(make_pair(p1, p2));
                visit[p1][p2] = true;
            }
        }
    }
    return licznik;
}

void Map::findmax(state *ptr)
{
    // if (4==std::count(ptr->dzieci, ptr->dzieci+4, NULL)){return ;}
    for (int i = 0; i < 4; i++)
    {
        if (ptr->dzieci[i] != 0)
        {
            findmax(ptr->dzieci[i]);
            if ((ptr->wyn) < (ptr->dzieci[i]->wyn))
            {
                ptr->wyn = ptr->dzieci[i]->wyn + 1;
            }
        }
    }
    return;
}

bool Map::walls_around()
{
    for (int i = 0; i < 4; i++)
    {
        int p1 = myrow + t1[i];
        int p2 = mycol + t2[i];
        if (vec[p1][p2] == ' ')
        {
            return false;
        }
    }
    return true;
}