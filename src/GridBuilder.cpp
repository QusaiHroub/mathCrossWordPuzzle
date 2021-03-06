/* mathCrossWordPuzzle
* Copyright 2019 Qusai Hroub.
*
* This file is part of the mathCrossWordPuzzle.
*
* Authors:
* Qusai Hroub <qusaihroub.r@gmail.com>
*
* This program is free software; you can redistribute it and/or modify it
* under the terms and conditions of the GNU General Public License,
* version 3, as published by the Free Software Foundation.
*
* This program is distributed in the hope it will be useful, but WITHOUT ANY
* WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
* FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
* more details.
*/

#include "GridBuilder.hpp"
#include <vector>
#include <queue>
#include <time.h>
#include <stdlib.h>

using namespace std;

GridBuilder::GridBuilder() {}

void GridBuilder::freeMem(vector < vector < CoreCell *> > *adjM) {
    for (int i = 0; i < (*adjM).size(); i++) {
        for (int j = 0; j < (*adjM)[i].size(); j++) {
            delete (*adjM)[i][j];
            (*adjM)[i][j] = nullptr;
        }
    }
}


void GridBuilder::init (int height, int width) {
    delete m_adjList;
    delete m_nodeList;
    m_adjList = new vector< vector < int > >();
    m_nodeList = new vector < node > ();
    if (m_adjMatrix == nullptr) {
        m_adjMatrix = new  vector< vector < CoreCell *> >();
    } else {
        freeMem(m_adjMatrix);
    }
    m_adjMatrix->assign(height, vector<CoreCell * > (width));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            (*m_adjMatrix)[i][j] = new BlockedCell();
        }
    }
}

void GridBuilder::creatAdjMatrix(vector< vector < CoreCell *> > **adjMatrix, vector< vector < int > > **adjList,
                                 vector<node> **nodeList, int height, int width, int startY, int startX, bool isV, double per) {

    maxC = (int)((height * width / 5) * (per / 10.0));
    if (m_adjList == nullptr) {
        m_adjList = new vector< vector < int > >();
    } else {
        delete m_adjList;
        m_adjList = new vector< vector < int > >();
    }
    if (m_nodeList == nullptr) {
        m_nodeList = new vector < node > ();
    } else {
        delete m_nodeList;
        m_nodeList = new vector < node > ();
    }
    while (m_adjList->size() < maxC) {
        creatPuzzle(height, width, startY, startX, isV);
    }
    *adjMatrix = m_adjMatrix;
    *adjList = m_adjList;
    *nodeList = m_nodeList;
}

 void GridBuilder::creatPuzzle (int height, int width, int startY, int startX, bool isV) {
    init(height, width);
    queue<lNode> q;
    lNode list;
    list.isV = isV;
    list.y = startY;
    list.x = startX;
    list.isMid = false;
    list.dir = true;
    q.push(list);
    while (!q.empty()) {
        lNode head = q.front();
        q.pop();
        if (!cheak(head.y, head.x, head.isV)) {
            continue;
        }
        m_adjList->push_back(vector <int>());
        if (head.prev != -1) {
            (*m_adjList)[head.prev].push_back(m_adjList->size() - 1);
            m_adjList->rbegin()->push_back(head.prev);
        }
        node nNode;
        nNode.isVerticale = head.isV;
        nNode.x = head.x;
        nNode.y = head.y;
        m_nodeList->push_back(nNode);
        if (head.isV) {
            int newY = head.y;
            for (int k = newY; k < newY + 5;k++) {
                if (k == newY + 1) {
                    delete (*m_adjMatrix)[k][head.x];
                    (*m_adjMatrix)[k][head.x] = new OperatorCell("X");
                } else if (k == newY + 3) {
                    delete (*m_adjMatrix)[k][head.x];
                    (*m_adjMatrix)[k][head.x] = new OperatorCell("=" , false);
                } else {
                    delete (*m_adjMatrix)[k][head.x];
                    (*m_adjMatrix)[k][head.x] = new NumericCell("");
                    head.list[head.ctr] = k;
                    head.ctr++;
                }
            }
        } else {
            int newX = head.x;
            for (int k = newX; k < newX + 5;k++) {
                if (k == newX + 1) {
                    delete (*m_adjMatrix)[head.y][k];
                    (*m_adjMatrix)[head.y][k] = new OperatorCell("X");
                } else if (k == newX + 3) {
                    delete (*m_adjMatrix)[head.y][k];
                    (*m_adjMatrix)[head.y][k] = new OperatorCell("=" , false);
                } else {
                    delete (*m_adjMatrix)[head.y][k];
                    (*m_adjMatrix)[head.y][k] = new NumericCell("");
                    head.list[head.ctr] = k;
                    head.ctr++;
                }
            }
        }
        for (int k = 0; k < 3; k++) {
            n[0].x = head.isV ? head.x : head.list[k];
            n[0].y = head.isV ? head.list[k] : head.y;
            n[0].isV = !head.isV;
            n[0].isMid = rand() % 2;
            n[0].dir = rand() % 2;
            n[0].prev = m_adjList->size() - 1;
            for (int i = 1; i < 3; i++) {
                n[i] = n[0];
            }
            n[1].isMid = !n[1].isMid;
            n[2].dir = !n[2].dir;
            for (int i = 0; i < 3; i++) {
                if(n[i].isMid) {
                    if (n[i].isV) {
                        n[i].y -= 2;
                    } else {
                        n[i].x -= 2;
                    }
                    n[i].dir = true;
                } else if (!n[i].dir) {
                    if (n[i].isV) {
                        n[i].y -= 4;
                    } else {
                        n[i].x -= 4;
                    }
                    n[i].dir = true;
                }
                if (n[i].y >= 0 && n[i].x >= 0) {
                    q.push(n[i]);
                }
            }
        }
    }
}

bool GridBuilder::cheak (int y, int x, bool isV) {
    if (isV) {
        if (y + 5 > (*m_adjMatrix).size()) {
            return false;
        }
        if (y + 6 < (*m_adjMatrix).size() && (typeid(BlockedCell) != typeid(*(*m_adjMatrix)[y + 5][x]))) {
            return false;
        }
        if (y > 0 && typeid(BlockedCell) != typeid(*(*m_adjMatrix)[y - 1][x])) {
            return false;
        }
        if (typeid(BlockedCell) != typeid(*(*m_adjMatrix)[y + 3][x])
            || typeid(BlockedCell) != typeid(*(*m_adjMatrix)[y + 1][x])) {
            return false;
        }
        if (typeid(*(*m_adjMatrix)[y + 1][x]) == typeid(OperatorCell)) {
            oCell = dynamic_cast<OperatorCell*>((*m_adjMatrix)[y + 1][x]);
            if (oCell->getValue() == "=") {
            return false;
            }
        }
    } else {
        if (x + 5 > (*m_adjMatrix)[y].size()) {
            return false;
        }
        if (x + 6  < (*m_adjMatrix)[y].size() && typeid(*(*m_adjMatrix)[y][x + 5]) != typeid(BlockedCell)) {
            return false;
        }
        if (x > 0 && typeid(*(*m_adjMatrix)[y][x - 1]) != typeid(BlockedCell)) {
            return false;
        }
        if (typeid(*(*m_adjMatrix)[y][x + 3]) != typeid(BlockedCell)
            || typeid(*(*m_adjMatrix)[y][x + 1]) != typeid(BlockedCell)) {
            return false;
        }
        if (typeid(*(*m_adjMatrix)[y][x + 1]) == typeid(OperatorCell)) {
            oCell = dynamic_cast<OperatorCell*>((*m_adjMatrix)[y + 1][x]);
            if (oCell->getValue() == "=") {
                return false;
            }
        }
    }
    return true;
}

void GridBuilder::creatAdjMatrix(vector< vector < CoreCell *> > **adjMatrix, vector< vector < int > > *adjList,
                        vector<node> *nodeList, int height, int width) {

    m_adjList = adjList;
    m_nodeList = nodeList;
    if (m_adjMatrix == nullptr) {
        m_adjMatrix = new  vector< vector < CoreCell *> >();
    } else {
        freeMem(m_adjMatrix);
    }
    m_adjMatrix->assign(height, vector<CoreCell * > (width));
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            (*m_adjMatrix)[i][j] = new BlockedCell();
        }
    }
    m_visited.assign(m_nodeList->size(), 0);
    creatPuzzle(0);
    *adjMatrix = m_adjMatrix;
}

void GridBuilder::creatPuzzle (int startNode) {
    queue<int> q;
    q.push(startNode);
    while (!q.empty()) {
        int front = q.front();
        q.pop();
        if (m_visited[front]) {
            continue;
        }
        m_visited[front] = true;
        for (int i = 0; i < (*m_adjList)[front].size(); i++) {
            q.push((*m_adjList)[front][i]);
        }
        if ((*m_nodeList)[front].isVerticale) {
            int newY = (*m_nodeList)[front].y;
            for (int k = newY; k < newY + 5;k++) {
                if (k == newY + 1) {
                    delete (*m_adjMatrix)[k][(*m_nodeList)[front].x];
                    (*m_adjMatrix)[k][(*m_nodeList)[front].x] = new OperatorCell("X");
                } else if (k == newY + 3) {
                    delete (*m_adjMatrix)[k][(*m_nodeList)[front].x];
                    (*m_adjMatrix)[k][(*m_nodeList)[front].x] = new OperatorCell("=" , false);
                } else {
                    delete (*m_adjMatrix)[k][(*m_nodeList)[front].x];
                    (*m_adjMatrix)[k][(*m_nodeList)[front].x] = new NumericCell("");
                }
            }
        } else {
            int newX = (*m_nodeList)[front].x;
            for (int k = newX; k < newX + 5;k++) {
                if (k == newX + 1) {
                    delete (*m_adjMatrix)[(*m_nodeList)[front].y][k];
                    (*m_adjMatrix)[(*m_nodeList)[front].y][k] = new OperatorCell("X");
                } else if (k == newX + 3) {
                    delete (*m_adjMatrix)[(*m_nodeList)[front].y][k];
                    (*m_adjMatrix)[(*m_nodeList)[front].y][k] = new OperatorCell("=" , false);
                } else {
                    delete (*m_adjMatrix)[(*m_nodeList)[front].y][k];
                    (*m_adjMatrix)[(*m_nodeList)[front].y][k] = new NumericCell("");
                }
            }
        }
    }
}
