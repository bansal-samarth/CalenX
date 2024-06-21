// TaskList.cpp
#include "taklist.h"
#include <iostream>

// Node class implementation
Node::Node(int s, int e, const std::string& t) : stime(s), etime(e), task(t), next(nullptr) {}

// LinkedList class implementation
LinkedList::LinkedList() : head(nullptr) {}

void LinkedList::insert(int stime, int etime, const std::string& task) {
    Node* newNode = new Node(stime, etime, task);

    if (!head) {
        head = newNode;
    } else {
        Node* temp = head;
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

void LinkedList::deleteTask(const std::string& task) {
    Node* current = head;
    Node* previous = nullptr;

    // Traverse the list to find the node to delete
    while (current && current->task != task) {
        previous = current;
        current = current->next;
    }

    // If the node is found, delete it
    if (current) {
        if (previous) {
            previous->next = current->next;
        } else {
            // If the node to be deleted is the head
            head = current->next;
        }

        delete current;
    } else {
        std::cout << "Task not found: " << task << std::endl;
    }
}

void LinkedList::display() const {
    Node* temp = head;
    while (temp) {
        qDebug() << "Start Time:" << temp->stime << ", End Time:" << temp->etime
                 << ", Task:" << QString::fromStdString(temp->task);
        temp = temp->next;
    }
}
