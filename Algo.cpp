#include "Algo.h"


using namespace std;
struct AStarNode1 {
    int id;
    double f, g, h;
    int parent;
};

struct AStarNode2 {
    int x, y;
    double g, h, f;
    int pX, pY;
    string dirName;
};

struct AStarNode3 {
    int id;
    double g, h, f;
    int parent;
    string dirName;
};

static struct Direction {
    int dx, dy;
    double cost;
    string name;
} dirs[8] = {
    {-1,  0, 1.0, "Up"},
    { 1,  0, 1.0, "Down"},
    { 0,  1, 1.0, "Right"},
    { 0, -1, 1.0, "Left"},
    { 1,  1, 1.5, "Down-Right"},
    { 1, -1, 1.5, "Down-Left"},
    {-1,  1, 1.5, "Up-Right"},
    {-1, -1, 1.5, "Up-Left"}
};
 PathNode* buildPath(const vector<int> &parent, const vector<double> &gCost, const vector<double> &hCost, int start, int goal){
    PathNode *head = nullptr;
    int curr = goal;
    while (curr != -1){
        PathNode *newNode = new PathNode();
        newNode->name = to_string(curr);
        newNode->g = gCost[curr];
        newNode->h = hCost[curr];
        newNode->f = gCost[curr] + hCost[curr];
        newNode->next = head;
        head = newNode;
        if (curr == start) break;
        curr = parent[curr];
    }
    return head;
}
vector<int> calcHeuristic(double adjMatrix[100][100], int goal, int n) {
    vector<int> dist(n, -1);
    vector<int> bfsQueue;
    
    dist[goal] = 0;
    bfsQueue.push_back(goal);
    
    int idx = 0;
    while (idx < (int)bfsQueue.size()) {
        int curr = bfsQueue[idx++];
        for (int next = 0; next < n; next++) {
            
            if (adjMatrix[next][curr] != 0 && dist[next] == -1) {
                dist[next] = dist[curr] + 1;
                bfsQueue.push_back(next);
            }
        }
    }
    return dist;
}
//Task 1
PathNode* findSocialPath(double adjMatrix[100][100], int startPerson, int goalPerson){
    int n = 100;
    vector<double> gCost(n,1e9);
    vector<double> hCost(n,0);
    vector<bool> closed(n, false);
    vector<int> parent(n, -1);
    vector<AStarNode1> openList;

    vector<int> hDist = calcHeuristic(adjMatrix, goalPerson, n);

    double hStart = (hDist[startPerson] == -1) ? 0 : hDist[startPerson];
    gCost[startPerson] = 0;
    hCost[startPerson] = hStart;
    openList.push_back({startPerson, hStart, 0, hStart, -1});
    while (!openList.empty()){
        int bestIdx = 0;
        for (int i = 1; i < openList.size(); ++i){
            if (openList[i].f < openList[bestIdx].f){
                bestIdx = i;
            }
        }
        AStarNode1 curr = openList[bestIdx];
        openList.erase(openList.begin() + bestIdx);

        if (curr.id == goalPerson) return buildPath(parent, gCost, hCost, startPerson, goalPerson);
        closed[curr.id] = true;
        for (int next = 0; next < n; ++next){
            if (adjMatrix[curr.id][next] == 0 || closed[next]) continue;
            double newG = gCost[curr.id] + 1;
            if (newG < gCost[next]){
                gCost[next] = newG;
                parent[next] = curr.id;
                 double hVal = (hDist[next] == -1) ? 0 : hDist[next];
                hCost[next] = hVal;
                openList.push_back({next, newG+hVal, newG, hVal, curr.id});
            }
        }
    }
    return nullptr;
}
//Task2


double getHeuristic(int mode, int x1, int x2, int y1, int y2){
    if (mode == 1){return abs(x1 - x2) + abs(y1 - y2);}
    if (mode == 2){return sqrt((x1 - x2)*(x1 - x2) + (y1 - y2)*(y1 - y2));}
    if (mode == 3){return max(abs(x1 - x2), abs(y1 - y2));}
    return 0;
}

PathNode* buildPath2(const vector<int> &parents, const vector<double> &gCost, const vector<double> &hCost, int start, int goal, int coords[100][2]){
       PathNode *head = nullptr;
       int curr = goal;
       while (curr != -1){
        PathNode *newNode = new PathNode();
        newNode->name = "(" + to_string(coords[curr][0]) +","+to_string(coords[curr][1])+")";
        newNode->g = gCost[curr];
        newNode->h = hCost[curr];
        newNode->f = gCost[curr] + hCost[curr];
        newNode->next = nullptr;

        newNode->next = head;
        head = newNode;
        if (curr == start)break;
        curr = parents[curr];
        
       }
       return head;
}
PathNode* findDronePath(double adjMatrix[100][100], int coords[100][2], int startPoint, int goalPoint, int mode){
    int n = 100;
    vector<int> parent(n, -1);
    vector<double> gCost(n, 1e9);
    vector<double> hCost(n, 0);
    vector<AStarNode1> openList;
    vector<bool> closed(n, false);
    double hStart = getHeuristic(mode, coords[startPoint][0],coords[goalPoint][0],coords[startPoint][1],coords[goalPoint][1]);
    hCost[startPoint] = hStart;
    gCost[startPoint] = 0;
    openList.push_back({startPoint, hStart, 0, hStart, -1});
    while (!openList.empty()){
        int bestIdx = 0;
for (int i = 1; i < openList.size(); ++i) {
    if (openList[i].f < openList[bestIdx].f) {
        bestIdx = i;
    }
    
    else if (openList[i].f == openList[bestIdx].f &&
             openList[i].g > openList[bestIdx].g) {
        bestIdx = i;
    }
}
        AStarNode1 curr = openList[bestIdx];
        openList.erase(openList.begin() + bestIdx);
        if (closed[curr.id]) continue;
        if (curr.id ==  goalPoint) return buildPath2(parent, gCost, hCost, startPoint, goalPoint, coords);
        closed[curr.id] = true;
        for (int next = 0; next < n; ++next){
            if (adjMatrix[curr.id][next] == 0 || closed[next]) continue;
            double newG = adjMatrix[curr.id][next] + gCost[curr.id];
            if (newG < gCost[next]){
                gCost[next] = newG;
                parent[next] = curr.id;
                double hVal = getHeuristic(mode, coords[next][0], coords[goalPoint][0], coords[next][1], coords[goalPoint][1]);
                hCost[next] = hVal;
                openList.push_back({next, newG+hVal, newG, hVal, curr.id});

            }
        }
    }

    return nullptr;

}

// Task 3

double getHeuristicMaze(int mode, int x1, int x2, int y1, int y2){
    if (mode == 1) return abs(x1-x2) + abs(y1-y2);        
    if (mode == 2) return max(abs(x1-x2), abs(y1-y2));    
    return 0;
}


PathNode *buildPath3(int pX[100][100], int pY[100][100], string dirName[100][100], double gCost[100][100], double hCost[100][100],int startX, int startY, int goalX, int goalY){
     PathNode* head = nullptr;
    int gx = goalX, gy = goalY;

    while (true) {
        
        if (gx == startX && gy == startY) break;

        PathNode* newNode = new PathNode();
        newNode->name     = dirName[gx][gy];
        newNode->g        = gCost[gx][gy];
        newNode->h        = hCost[gx][gy];
        newNode->f        = gCost[gx][gy] + hCost[gx][gy];
        newNode->next     = head;
        head              = newNode;

        int px = pX[gx][gy], py = pY[gx][gy];
        gx = px;
        gy = py;
    }
    return head;
}
PathNode* findWarehousePath(int warehouse[100][100], int m, int n, int startX, int startY, int goalX, int goalY, int mode){
    int pX[100][100], pY[100][100];
    bool closed[100][100];
    double gCost[100][100];
    double hCost[100][100];
    string dirName[100][100];
    for (int i = 0; i < m ; ++i){
        for (int j = 0; j < n; ++j){
            pX[i][j] = -1;
            pY[i][j] = -1;
            closed[i][j] = false;
            gCost[i][j] = 1e9;
            hCost[i][j] = 0;
            dirName[i][j] = "";
        }
    }
    vector<AStarNode2> openList;
    double hStart =  getHeuristicMaze(mode, startX, goalX,startY, goalY);
    hCost[startX][startY] = hStart;
    gCost[startX][startY] = 0;
    openList.push_back({startX, startY, 0, hStart, hStart, -1,-1,"Start"});
    while (!openList.empty()){
        int bestIdx = 0;
        for (int i = 1; i < openList.size(); ++i){
            if (openList[i].f < openList[bestIdx].f){
                bestIdx = i;
            }
            
        }
        AStarNode2 curr = openList[bestIdx];
        openList.erase(openList.begin() + bestIdx);
        if (closed[curr.x][curr.y]) continue;
        if (curr.x == goalX && curr.y == goalY) return buildPath3(pX, pY,dirName, gCost, hCost, startX, startY, goalX, goalY);
        closed[curr.x][curr.y] = true;
        for (auto &dir : dirs){
            int nX = curr.x + dir.dx;
            int nY = curr.y + dir.dy;
            if (nX < 0 || nX >= m || nY < 0 || nY >= n || warehouse[nX][nY] == 1 || closed[nX][nY]) continue;
            double newG = gCost[curr.x][curr.y] + dir.cost;
            if (newG < gCost[nX][nY]){
                gCost[nX][nY] = newG;
                pX[nX][nY] = curr.x;
                pY[nX][nY] = curr.y;
                dirName[nX][nY] = dir.name;

                double hVal =  getHeuristicMaze(mode, nX, goalX, nY, goalY);
                hCost[nX][nY] = hVal;
                openList.push_back({nX, nY, newG, hVal, hVal + newG, curr.x, curr.y, dir.name});
            }
            

        }
    }
    return nullptr;
}
//Task 4
PathNode* buildPath4(const vector<int> &parent, const vector<double> &gCost, const vector<double> &hCost, vector<string> &dir, int start, int goal){
    PathNode *head = nullptr;
    int curr = goal;
    while (curr != -1){
        if (curr == start) break;
        PathNode *newNode = new PathNode();
        newNode->name = dir[curr];
        newNode->g = gCost[curr];
        newNode->h = hCost[curr];
        newNode->f = gCost[curr] + hCost[curr];
        
        
        newNode->next = head;
        head = newNode;
        if (curr == start) break;
            curr = parent[curr];
        
    }
    return head;
}
void buildWeightedMatrix(int floorPlan[100][100], int m, int n, double weightMatrix[100][100]){
      for (int i = 0; i < m*n; ++i){
        for (int j = 0; j < m * n; ++j){
            weightMatrix[i][j] = 0;
        }
      }
      for (int i = 0; i < m; ++i){
        for (int j = 0; j < n; ++j){
            if (floorPlan[i][j] == 1) continue;
            int fromIdx = i * n + j;
            for (auto &dir : dirs){
                int nx = i + dir.dx;
                int ny = j + dir.dy;
                if (nx < 0 || nx >= m) continue;
                if (ny < 0 || ny >= n) continue;
                if (floorPlan[nx][ny] == 1) continue;
                int toIdx = nx * n + ny;
                weightMatrix[fromIdx][toIdx] = dir.cost;
            }
        }
      }
    
}
PathNode* findEvacuationPath(int floorPlan[100][100], int m, int n,int startX, int startY,int exitX, int exitY, double weightMatrix[100][100], int mode){
     int start = startX * n + startY;
     int goal = exitX * n + exitY;
     int total = m * n;
     buildWeightedMatrix(floorPlan, m, n, weightMatrix);
     vector<int> parent(total, -1);
     vector<double> gCost(total, 1e9);
     vector<double> hCost(total, 0);
     vector<bool> closed(total, false);
     vector<string> dirMap(total,"");
     vector<AStarNode3> openList;

     double hVal = getHeuristicMaze(mode, startX, exitX, startY, exitY);
     gCost[start] = 0;
     hCost[start] = hVal;

     openList.push_back({start,0,hVal, hVal, -1, ""});
     while (!openList.empty()){
     int bestIdx = 0;
     for (int i = 1; i < openList.size(); ++i){
        if (openList[i].f < openList[bestIdx].f){
            bestIdx = i;
        }
     }
     AStarNode3 curr = openList[bestIdx];
     openList.erase(openList.begin() + bestIdx);
     if (closed[curr.id]) continue;
     if (curr.id == goal) return buildPath4(parent, gCost, hCost, dirMap, start, goal);
     closed[curr.id] = true;
     for (int next = 0; next < total; ++next){
        if (weightMatrix[curr.id][next] == 0 || closed[next]) continue;
        double newG = weightMatrix[curr.id][next] + gCost[curr.id];
        if (newG < gCost[next]){
            gCost[next] = newG;
            parent[next] = curr.id;

            int nx = next / n, ny = next % n;
            double hVal = getHeuristicMaze(mode, nx, exitX, ny, exitY);
            hCost[next] = hVal;
            string dirN ="";
            int cx = curr.id/n, cy = curr.id%n;
            for (auto &dir : dirs){
                if (nx == cx + dir.dx && ny == cy + dir.dy){
                    dirN = dir.name;
                }
            
            }
            dirMap[next] = dirN;
            openList.push_back({next, newG, hVal, newG + hVal, curr.id, dirN});
        } 
     }
    }
     return nullptr;
}