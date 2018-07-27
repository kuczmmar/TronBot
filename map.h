#ifndef MAP_H
#define MAP_H

#include <string>
#include <vector>
#include <queue>
#include <utility>

struct state
{
    std::vector<std::string> mapa;
    int r, c, wyn, depth;
    state *parent;
    state *dzieci[4] = {0, 0, 0, 0};
};

struct state_minmax
{
    std::vector<std::string> mapa;
    int myr, myc, enr, enc, wyn;
    state_minmax *dzieci[4] = {0, 0, 0, 0};
};

extern char tab[5000000 * sizeof(state)];
extern int tab_index;

class Map
{
  public:
    bool walls_around();
    void readmap();
    void findpos();
    std::pair<int, int> findenemy();
    void move(int a, int b);
    void bestmove();
    void findmax(state *p);
    int howmuchspace(state *);

    void wheretomove_minmax();
    int maxplayer(state_minmax *, int alpha, int beta, int depthleft);
    int minplayer(state_minmax *, int alpha, int beta, int depthleft);
    //int getmyrow() const {return myrow;}
    //int getmycol() const {return mycol;}

    int voronoi(state_minmax *);
    void howfar(int startrow, int startcol, std::vector<std::vector<int>> &distance, const std::vector<std::string> &mapa);

  private:
    std::vector<std::string> vec;
    int width, height;
    int myrow, mycol, enrow, encol;
    std::vector<std::vector<std::pair<int, int>>> parent;

    int t1[5] = {-1, 0, 1, 0};
    int t2[5] = {0, 1, 0, -1};
    int enemydis;
    int childmax[4] = {0, 0, 0, 0};
    const int MAX_DEPTH = 15;
    static const int DEPTHMINMAX = 10;
};
#endif
