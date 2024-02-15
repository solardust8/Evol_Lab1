#include <bits/stdc++.h>
#include <experimental/random>
#include "windows.h"
#include "psapi.h"

using namespace std;

int gen_binary()
{
    //srand(unsigned(time(nullptr)));
    return experimental::randint(0,1);
}

int main()
{
    vector<vector<int>> map;
    cout << "### A way to create map:\n1 - random\n2 - from file (map.txt)\n";
    int sel = 0;
    int a, b;

    cin >> sel;

    switch (sel % 2)
    {
        case 1:
        {
            cout << "### Specify dimensions:\n";
            cin >> a >> b;
            for (int i = 0; i < a; i++)
            {
                vector<int> row;
                for (int j = 0; j < b; j++)
                {
                    row.push_back(gen_binary());
                }
                map.push_back(row);
            }
            cout << "### Do you wish to save it?\n1 - yes\n2 - no\n";
            cin >> sel;
            if (sel % 2)
            {
                fstream fout;
                fout.open("map.txt");
                fout << a << " " << b << '\n';
                for (auto el: map)
                {
                    for (int& i: el)
                    {
                        fout << i << ' ';
                    }
                    fout << '\n';
                }
                fout.close();
            }
            else cout << "# not saved\n";
            
        }
        case 0:
        {
            fstream fin;
            fin.open("map.txt");
            fin >> a >> b;
            for (int i = 0; i < a; i++)
            {
                vector<int> row;
                for (int j = 0; j < b; j++)
                {
                    int x;
                    fin >> x;
                    row.push_back(x);
                }
                map.push_back(row);
            }
            fin.close();
        }
    }

    // Map consists of 1s and 0s: 1 - obstacle, 0 - free space

    // Enter start/end
    int x0, y0, x1, y1;
    while (true)
    {
        cout << "### Enter start point (x0 y0):\n";
        cin >> x0 >> y0;
        if (map[x0][y0] == 1) 
        {
            cout << "Obstacle\n";
            continue;
        }
        cout << "### Enter end point (x1 y1):\n";
        cin >> x1 >> y1;
        if (map[x1][y1] == 1)
        {
            cout << "Obstacle\n";
            continue;
        }
        break;
    }

    // Optimal variant of solution (STL Realization)

    unsigned int start_time = clock(), end_time;
    vector<vector<int>> distmap(a, vector<int>(b, 2e9));
    for (int i = 0; i < a; i++)
    {
        for (int j = 0; j < b; j++)
        {
            if (map[i][j])
            {
                distmap[i][j] = -1;
            }
        }
    }
    queue<pair<int, int>> que;

    pair<int, int> loc;
    distmap[x0][y0] = 0;
    que.push(make_pair(x0, y0));
    bool reached = false;
    int moves[] = {-1, 0, 1};
    vector<pair<int, int>> path;

    while (!reached && !que.empty())
    {
        loc = que.front();
        que.pop();
        int x = loc.first; 
        int y = loc.second;
        for (int& i: moves)
        {
            for (int& j: moves)
            {
                int xn, yn;
                if (i==0 && j==0) continue;
                if (i < 0)
                    xn = max(0, x+i);
                else
                    xn = min(a-1, x+i);
                if (j < 0)
                    yn = max(0, y+j);
                else
                    yn = min(b-1, y+j);
                
                if (map[xn][yn]) continue;
                if (distmap[xn][yn] == 2e9) 
                {
                    distmap[xn][yn] = distmap[x][y] + 1;
                    if (xn == x1 && yn == y1)
                    {
                        reached = true;
                        break;
                    }
                    else 
                        que.push(make_pair(xn, yn));
                }
            }
        }
    }
    //cout << "i am here" << endl;
    if (distmap[x1][y1] == 2e9)
    {
        end_time = clock();
        cout << "### Destination cannot be reached\n";
    }
    else
    {
        // Route fix
        reached = false;
        int xs = x1, ys = y1;
        path.push_back(make_pair(xs, ys));
        //cout << " (" << xs << ", "<< ys << ") ";
        while (!reached)
        {
            for (int& i: moves)
            {
                for (int& j: moves)
                {
                    int xn, yn;
                    if (i==0 && j==0) continue;
                    if (i < 0)
                        xn = max(0, xs+i);
                    else
                        xn = min(a, xs+i);
                    if (j < 0)
                        yn = max(0, ys+j);
                    else
                        yn = min(b, ys+j);
                    if ((distmap[xn][yn] == distmap[xs][ys] - 1) && distmap[xn][yn] >= 0)
                    {
                        xs = xn;
                        ys = yn;
                        path.push_back(make_pair(xs, ys));
                        //cout << "<---> (" << xs << ", "<< ys << ") ";
                    }
                    if (xs == x0 && ys == y0)
                    {
                        reached = true;
                        break;
                    }
                }
            }
        }
        end_time = clock();
        cout << "### Destination reached with {" << distmap[x1][y1] << "} hops\n### Path is: \n";
        for (auto &el: path)
        {
            cout << "<(" << el.first << ", "<< el.second << ")>";
        }
        cout << "\n### End of algo\n";
    }
    unsigned int search_time = end_time - start_time; 

    cout << "### Lee algorithm (STL Realization) runtime: " << search_time << " ms\n";

    PROCESS_MEMORY_COUNTERS_EX pmc;
    GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
    SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;

    SIZE_T physMemUsedByMe = pmc.WorkingSetSize;

    cout << "### VMem occupied by process: " << virtualMemUsedByMe/1024 << " Kbytes \n";
    cout << "### PhysMem occupied by process: " << physMemUsedByMe/1024 << " Kbytes \n";




    return 0;
}