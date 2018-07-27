#include "map.h"
#include <iostream>
#include <queue>
#include <utility>
#include <limits>
using namespace std;

char tab[5000000 * sizeof(state_minmax)];
int tab_index;

//MIN MAX ALPHA BETA

int Map::maxplayer(state_minmax *ptr, int alpha, int beta, int depthleft)
{
    bool wallsAround = true;

    for (int i = 0; i < 4; i++)
    {
        int p1 = ptr->myr + t1[i];
        int p2 = ptr->myc + t2[i];
        if (ptr->mapa[p1][p2] == ' ')
        {
            wallsAround = false;
            break;
        }
    }

    if (depthleft == 0 || wallsAround)
    {
        int vor = voronoi(ptr);
        cerr << "voronio max " << vor << endl;
        return vor;
    }

    ptr->mapa[ptr->myr][ptr->myc] = '#';

    for (int i = 0; i < 4; i++)
    {
        int p1 = ptr->myr + t1[i];
        int p2 = ptr->myc + t2[i];
        if (ptr->mapa[p1][p2] == ' ')
        {
            //state_minmax *child = new state_minmax;
            state_minmax *child = new (tab + tab_index) state_minmax;
            tab_index += sizeof(state_minmax);

            child->mapa = ptr->mapa;
            child->myr = p1;
            child->myc = p2;
            child->enr = ptr->enr;
            child->enc = ptr->enc;
            ptr->dzieci[i] = child;

            int score = minplayer(child, alpha, beta, depthleft - 1);
            child->wyn = score;
            if (score >= beta)
                return score + 1; //+1????????????
            if (score > alpha)
                alpha = score;
        }
    }
    return alpha; //////// alpha +1??????
}

int Map::minplayer(state_minmax *ptr, int alpha, int beta, int depthleft)
{
    bool wallsAround = true;

    for (int i = 0; i < 4; i++)
    {
        int p1 = ptr->enr + t1[i];
        int p2 = ptr->enc + t2[i];
        if (ptr->mapa[p1][p2] == ' ')
        {
            wallsAround = false;
            break;
        }
    }

    if (depthleft == 0 || wallsAround)
    {
        int vor = voronoi(ptr);
        cerr << "voronio min " << vor << endl;
        return vor;
    }

    ptr->mapa[ptr->enr][ptr->enc] = '#';
    for (int i = 0; i < 4; i++)
    {
        int p1 = ptr->enr + t1[i];
        int p2 = ptr->enc + t2[i];
        if (' ' == ptr->mapa[p1][p2])
        {
            //state_minmax *child = new state_minmax;
            state_minmax *child = new (tab + tab_index) state_minmax;
            tab_index += sizeof(state_minmax);

            child->mapa = ptr->mapa;
            child->myr = ptr->myr;
            child->myc = ptr->myc;
            child->enr = p1;
            child->enc = p2;
            ptr->dzieci[i] = child;

            int score = maxplayer(child, alpha, beta, depthleft - 1);
            child->wyn = score;
            if (score <= alpha)
                return score - 1;
            if (score < beta)
                beta = score;
        }
    }

    return beta;
}

void Map::wheretomove_minmax()
{
    int beta = numeric_limits<int>::max();
    int alpha = -beta;
    cerr << "alpha beta" << endl;

    state_minmax pocz;
    pocz.myr = myrow;
    pocz.myc = mycol;
    pocz.enr = enrow;
    pocz.enc = encol;
    pocz.mapa = vec;

    pocz.wyn = maxplayer(&pocz, alpha, beta, DEPTHMINMAX);
    tab_index = 0;

    int r, c;
    for (int i = 0; i < 4; i++)
    {
        if (pocz.dzieci[i] != 0)
        {
            cerr << "dzieci  " << pocz.dzieci[i]->wyn << "  ";
        }
        if (pocz.dzieci[i] != 0 && pocz.dzieci[i]->wyn == pocz.wyn)
        {
            r = pocz.dzieci[i]->myr;
            c = pocz.dzieci[i]->myc;
        }
    }
    cerr << endl;
    cerr << "wyn pocz   " << pocz.wyn << endl;
    cerr << "alpha beta moving  " << endl;
    if (r - myrow == -1)
        cout << '1' << endl;
    else if (r - myrow == 1)
        cout << '3' << endl;
    else if (c - mycol == -1)
        cout << '4' << endl;
    else if (c - mycol == 1)
        cout << '2' << endl;
}

// VORONOI

int Map::voronoi(state_minmax *ptr)
{
    vector<vector<int>> mydistance(height, vector<int>(width, numeric_limits<int>::max()));
    howfar(ptr->myr, ptr->myc, mydistance, ptr->mapa);
    vector<vector<int>> endistance(height, vector<int>(width, numeric_limits<int>::max()));
    howfar(ptr->enr, ptr->enc, endistance, ptr->mapa);
    int close = 0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (mydistance[i][j] < endistance[i][j])
                close++;
            else if (mydistance[i][j] > endistance[i][j])
                close--;
        }
    }
    return close;
}

void Map::howfar(int startrow, int startcol, vector<vector<int>> &distance, const std::vector<std::string> &mapa)
{
    queue<pair<int, int>> que;
    int r, c;
    vector<vector<bool>> visit(height, vector<bool>(width));
    que.push(make_pair(startrow, startcol));
    visit[startrow][startcol] = true;
    distance[startrow][startcol] = 0;

    while (!que.empty())
    {
        pair<int, int> u = que.front();
        que.pop();
        r = u.first;
        c = u.second;

        for (int i = 0; i < 4; i++)
        {
            int p1 = r + t1[i];
            int p2 = c + t2[i];
            if (!visit[p1][p2] && mapa[p1][p2] != '#')
            {
                distance[p1][p2] = distance[r][c] + 1;
                que.push(make_pair(p1, p2));
                visit[p1][p2] = true;
            }
        }
    }
}
