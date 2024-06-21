// TaskList.h
#ifndef TASKLIST_H
#define TASKLIST_H

#include <iostream>
#include <string>
#include <QMainWindow>

class Node {
public:
    int stime;
    int etime;
    std::string task;
    Node* next;

    Node(int s, int e, const std::string& t);
};

class LinkedList {
private:
    Node* head;

public:
    LinkedList();

    void insert(int stime, int etime, const std::string& task);
    void deleteTask(const std::string& task);
    void display() const;
    Node* getHead() const {
        return head;
    }
};

#endif // TASKLIST_H
