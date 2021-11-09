#pragma once

#include "SpatialImageBase.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <vector>
#include <unordered_map>
#include <cstdlib>
using namespace std;

namespace utec {
namespace spatial {

struct Node{
    pair<int, int> startCoord;
    pair<int, int> endCoord;
    Node* children[4];
    int color;
    Node() {}
    Node(pair<int, int> startCoord, pair<int, int> endCoord, int color){
        this->startCoord = startCoord;
        this->endCoord = endCoord;
        this->color = color;
        for(int i = 0; i < 4; i++){
            children[i] = nullptr;
        }
    }
    Node(pair<int, int> startCoord, pair<int, int> endCoord){
        this->startCoord = startCoord;
        this->endCoord = endCoord;
        this->color = -1;
        for(int i = 0; i < 4; i++){
            children[i] = nullptr;
        }
    }
    
    bool checkChildrenColor();

    void filter();

};

bool Node::checkChildrenColor(){
    for(int i = 0; i < 4; i++){
        if(children[i] && children[i]->color == -1){ 
            return false;
        }
    }
        
    int childColor = children[0]->color;
    for(int i = 1; i < 4; i++){
        if(children[i] && children[i]->color != childColor){ 
            return false;
        }
    }

    return true;
}

void Node::filter(){
    if(checkChildrenColor()){
        this->color = this->children[0]->color;
        for(int i = 0; i < 4; i++){
            if(children[i]){
                delete children[i];
            }
        }

        for(int i = 0; i < 4; i++){
            children[i] = nullptr;
        }
    }
}

class QuadTree {
private:
    Node* root;
    int heightSpace;
    int widthSpace;
    int ScaleImage;
    vector<vector<int>> matrix;

public:
    QuadTree(){}

    Node* insert(pair<int, int> startCoord, pair<int, int> endCoord);

    void load(const string& filename);

    Node* getRoot();

    int getHeight();

    int getWidth();

    int getmaxGrayScale();
};


Node* QuadTree::insert(pair<int, int> startCoord, pair<int, int> endCoord){
        Node* node = new Node(startCoord, endCoord);
        if(startCoord.first == endCoord.first){
            if(startCoord.second == endCoord.second){
                node->color = matrix[startCoord.first][startCoord.second];
                return node;
            }
            else{
                node->children[0] = insert(startCoord, {startCoord.first, startCoord.second+((endCoord.second - startCoord.second)/2)});
                node->children[2] = insert({startCoord.first, startCoord.second+((endCoord.second - startCoord.second)/2)+1}, endCoord);
            }
        }
        else if(startCoord.second == endCoord.second){
            node->children[0] = insert(startCoord, {startCoord.first+((endCoord.first - startCoord.first)/2), startCoord.second});
            node->children[1] = insert({startCoord.first+((endCoord.first - startCoord.first)/2)+1, startCoord.second}, endCoord);
        }
        else{
            node->children[0] = insert(startCoord, {startCoord.first+((endCoord.first - startCoord.first)/2), startCoord.second+((endCoord.second - startCoord.second)/2)});
            node->children[1] = insert({startCoord.first, startCoord.second+((endCoord.second - startCoord.second)/2)+1}, {startCoord.first+((endCoord.first - startCoord.first)/2), endCoord.second});
            node->children[2] = insert({startCoord.first+((endCoord.first - startCoord.first)/2)+1, startCoord.second}, {endCoord.first, startCoord.second+((endCoord.second - startCoord.second)/2)});
            node->children[3] = insert({startCoord.first+((endCoord.first - startCoord.first)/2)+1, startCoord.second+((endCoord.second - startCoord.second)/2)+1}, endCoord);
        }
        node->filter();
        return node;
}

void QuadTree::load(const string& filename){
    fstream stream(filename, ios::in);
    if(stream.is_open()){
        string comment;
        getline(stream, comment);
        getline(stream, comment);
        stream >> widthSpace >> heightSpace >> ScaleImage;
        matrix =  vector<vector<int>>(heightSpace, vector<int>(widthSpace));
        for(int i = 0; i < heightSpace; i++){
            for(int j = 0; j < widthSpace; j++){
                stream >> matrix[i][j];
            }
        }
        stream.close();
        this->root = insert({0,0}, {heightSpace-1, widthSpace-1});
    }else cout << "Error File Load" << endl;
}

Node* QuadTree::getRoot(){
    return this->root;
}

int QuadTree::getHeight(){
    return this->heightSpace;
}

int QuadTree::getWidth(){
    return this->widthSpace;
}

int QuadTree::getmaxGrayScale(){
    return this->ScaleImage;
}

struct Quadrant{
    vector<pair<short, short>> points;
    vector<pair<pair<short, short>, pair<short, short>>> pointsSection;
    Quadrant(pair<short, short> startCoord, pair<short, short> endCoord){
        insert(startCoord, endCoord);
    }
    void insert(pair<short, short> startCoord, pair<short, short> endCoord){
        if(startCoord == endCoord){
            points.push_back(startCoord);
        } else{
            pointsSection.emplace_back(startCoord, endCoord);
        }
    }

    void save(fstream& stream){
        int size = points.size();
        stream.write((char *)& size, sizeof(size));
        for(int i = 0; i < size; i++){
            stream.write((char *)& points[i], sizeof(points[i]));
        }
        size = pointsSection.size();
        stream.write((char *)& size, sizeof(size));
        for(int i = 0; i < size; i++){
            stream.write((char *)& pointsSection[i], sizeof(pointsSection[i]));
        }
    }
};

class ObjectFile{
private:
    unordered_map<short, Quadrant*> map_;
    QuadTree quadTree;

public:
    ObjectFile(){};

    ObjectFile(QuadTree quadTree_) : quadTree(quadTree_) {}

    void saveLeaves(Node* node);

    void add(short color, pair<short, short> startCoord, pair<short, short> endCoord);

    void compress(const string& filename);

    void decompress(const string& filename);

    void convertToPGM(string filename);

};

void ObjectFile::saveLeaves(Node* node){
    if(node->color != -1){
        this->add(node->color, node->startCoord, node->endCoord);
        return;
    }
    for(int i = 0; i < 4; i++){
        if(node->children[i]) saveLeaves(node->children[i]);
    }
}

void ObjectFile::add(short color, pair<short, short> startCoord, pair<short, short> endCoord){
    if(map_.find(color) == map_.end()){
        map_[color] = new Quadrant(startCoord, endCoord);
    }else{
        map_[color]->insert(startCoord, endCoord);
    }
}

void ObjectFile::compress(const string& filename){
    saveLeaves(quadTree.getRoot());
    fstream stream(filename, ios::out | ios::binary);
    int quadWith = quadTree.getWidth();
    int quadHeight = quadTree.getHeight();
    int quadMaxGrayScale = quadTree.getmaxGrayScale();
    stream.write((char*)& quadWith, sizeof(quadWith));
    stream.write((char*)& quadHeight, sizeof(quadHeight));
    stream.write((char*)& quadMaxGrayScale, sizeof(quadMaxGrayScale));
    stream.close();
}

void ObjectFile::decompress(const string& filename){
    fstream stream;

    stream.open(filename, ios::in | ios::out | ios::binary | ios::app);
    stream.seekg(0, ios::end);

    short size = map_.size();
    stream.write((char*)& size, sizeof(size));

    for(auto it : map_){
        stream.write((char *)& it.first, sizeof(it.first));
        it.second->save(stream);
    }

    stream.close();
}

void ObjectFile::convertToPGM(string filename){
    fstream stream("prueba.qt", ios::in | ios::binary);
    if(stream.is_open()){
        fstream pgmFile(filename, ios::out);
        int width, height, maxGrayScale_;
        stream.read((char*)& width, sizeof(width));
        stream.read((char*)& height, sizeof(height));
        stream.read((char*)& maxGrayScale_, sizeof(maxGrayScale_));
        pgmFile << "P2\n# feep.pgm\n";
        pgmFile << width << " " << height << '\n';
        pgmFile << maxGrayScale_ << '\n';
        vector<vector<int>> matrix(height, vector<int>(width, 0));
        
        short total;
        stream.read((char *)& total, sizeof(total));
        short color;
        int i = 0;
        while(i != total){
            stream.read((char *)& color, sizeof(color));
            int size1, size2;
            stream.read((char *)& size1, sizeof(size1));
            if(size1 != 0){
                pair<short, short> points[size1];
                stream.read((char *)& points, size1*sizeof(pair<short, short>));
                vector<pair<short, short>> pointsVector(points, points+size1);
                for(int i = 0; i < pointsVector.size(); i++){
                    matrix[pointsVector[i].first][pointsVector[i].second] = color;
                }
            }
            stream.read((char *)& size2, sizeof(size2));
            if(size2 != 0){
                pair<pair<short, short>,pair<short, short>> pointsSection[size2];
                stream.read((char *)& pointsSection, size2*sizeof(pair<pair<short, short>, pair<short, short>>));
                vector<pair<pair<short, short>, pair<short, short>>> sectionVector(pointsSection, pointsSection+size2);
                for(int m = 0; m < sectionVector.size(); m++){
                    pair<int, int> start = sectionVector[m].first;
                    pair<int, int> end = sectionVector[m].second;
                    for(int i = start.first; i <= end.first; i++){
                        for(int j = start.second; j <= end.second; j++){
                            matrix[i][j] = color;
                        }
                    }
                }
            }
            i++;
        }

        for(int i = 0; i < matrix.size(); i++){
            for(int j = 0; j < matrix[i].size(); j++){
                pgmFile << matrix[i][j];
                if(j != matrix[i].size() - 1){ 
                    pgmFile << " ";
                }
            }
            if(i != matrix.size() - 1){
                pgmFile << '\n';
            }
        }

        stream.close();
        pgmFile.close();
    }else cout << "Error File Convert" << endl;
}

class PRQuadTreeImage : public SpatialImageBase {
 private:
  QuadTree quadTree;
  ObjectFile stream;
 public:

  PRQuadTreeImage(){};
  
  void load(const std::string& filename) override {
    quadTree.load(filename);
  }

  void compress(const std::string& filename) override {
    stream = ObjectFile(quadTree);
    stream.compress(filename);
  }

  void decompress(const std::string& filename) override {
    stream.decompress(filename);
  }

  void convertToPGM(const std::string& filename) override {
    stream.convertToPGM(filename);
  }

};

}  // namespace spatial
}  // namespace utec
