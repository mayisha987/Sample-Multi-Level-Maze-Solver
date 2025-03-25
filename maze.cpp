#include <iostream>
#include <vector>
#include <queue>
#include <deque>
#include <cmath>
#include <algorithm>

using namespace std;

// Node structure for storing positions in the maze
struct Node {
    int x, y;
    int level;  // Current level in the maze
    int cost;   // Cost from start
    Node* parent;  // To reconstruct the path

    // Default constructor
    Node() : x(-1), y(-1), level(-1), cost(0), parent(nullptr) {}

    // Parameterized constructor
    Node(int x, int y, int level, int cost = 0, Node* parent = nullptr)
        : x(x), y(y), level(level), cost(cost), parent(parent) {}
};

// Directions for movement (up, down, left, right)
int dx[] = {-1, 1, 0, 0};
int dy[] = {0, 0, -1, 1};

// BFS Implementation for solving a level
vector<Node> bfs(const vector<vector<vector<int>>>& maze, Node start, Node end) {
    int levels = maze.size();
    int rows = maze[0].size();
    int cols = maze[0][0].size();
    vector<vector<vector<bool>>> visited(levels, vector<vector<bool>>(rows, vector<bool>(cols, false)));
    queue<Node> q;
    vector<vector<vector<Node>>> parent(levels, vector<vector<Node>>(rows, vector<Node>(cols, Node())));

    q.push(start);
    visited[start.level][start.x][start.y] = true;

    while (!q.empty()) {
        Node current = q.front();
        q.pop();

        // If we reach the goal on the current level, reconstruct the path
        if (current.x == end.x && current.y == end.y && current.level == end.level) {
            vector<Node> path;
            Node* node = &current;
            while (node != nullptr) {
                path.push_back(*node);
                node = node->parent;
            }
            reverse(path.begin(), path.end());
            return path;
        }

        // Explore neighbors on the same level
        for (int i = 0; i < 4; i++) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];

            if (newX >= 0 && newX < rows && newY >= 0 && newY < cols &&
                !visited[current.level][newX][newY] && maze[current.level][newX][newY] != 1) {
                visited[current.level][newX][newY] = true;
                Node next(newX, newY, current.level, current.cost + 1, &parent[current.level][newX][newY]);
                q.push(next);
                parent[current.level][newX][newY] = current;  // Set the parent for backtracking
            }
        }

        // Check if we need to transition to the next level (i.e., the staircase '2')
        if (maze[current.level][current.x][current.y] == 2 && current.level + 1 < levels) {
            Node next(current.x, current.y, current.level + 1, current.cost + 1, &parent[current.level][current.x][current.y]);
            visited[current.level + 1][current.x][current.y] = true;
            q.push(next);
            parent[current.level + 1][current.x][current.y] = current;  // Set the parent for the next level
        }
    }

    return {}; // No path found
}

// Function to print the maze and path
void printMaze(const vector<vector<vector<int>>>& maze, const vector<Node>& path) {
    int levels = maze.size();
    int rows = maze[0].size();
    int cols = maze[0][0].size();

    for (int level = 0; level < levels; level++) {
        cout << "Level " << level + 1 << ":\n";
        vector<vector<char>> mazeCopy(rows, vector<char>(cols, ' '));

        // Copy maze into mazeCopy
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (maze[level][i][j] == 1) {
                    mazeCopy[i][j] = '#';  // Wall
                } else if (maze[level][i][j] == 2) {
                    mazeCopy[i][j] = 'S';  // Staircase
                }
            }
        }

        // Mark the path
        for (const auto& node : path) {
            if (node.level == level) {
                mazeCopy[node.x][node.y] = '.';  // Path
            }
        }

        // Print the maze with the path
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                cout << mazeCopy[i][j] << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
}

int main() {
    // Sample Input: Multi-level maze representation (0: free, 1: wall, 2: staircase)
    vector<vector<vector<int>>> maze = {
        {
            {0, 1, 0, 0, 0},
            {0, 1, 0, 1, 0},
            {0, 1, 0, 1, 0},
            {0, 0, 0, 0, 2},  // Staircase at the bottom right corner
            {0, 1, 1, 1, 0}
        },
        {
            {0, 1, 0, 0, 0},
            {0, 0, 0, 1, 0},
            {0, 0, 0, 0, 0},
            {0, 1, 1, 1, 0},
            {0, 0, 0, 0, 0}
        }
    };

    Node start(0, 0, 0);  // Start position at (0, 0) on level 0
    Node end(4, 4, 1);    // End position at (4, 4) on level 1 (second level)

    vector<Node> path = bfs(maze, start, end);
    if (!path.empty()) {
        printMaze(maze, path);
    } else {
        cout << "No path found!" << endl;
    }

    return 0;
}
