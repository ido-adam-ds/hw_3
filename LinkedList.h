#ifndef LINKED_LIST
#define LINKED_LIST

#include "Exceptions.h"

namespace DS{

    template<class T>
    struct LinkedListNode{
        LinkedListNode* next;
        LinkedListNode* prev;
        T data;
        LinkedListNode(const T& data): next(nullptr), prev(nullptr),
             data(data){}
        ~LinkedListNode() = default;
    };

    template<class T>
    class LinkedList{
    private:
        typedef LinkedListNode<T>* Node;
        Node head;
    public:
        LinkedList(): head(nullptr){}
        ~LinkedList(){
            Node ptr = head;
            while(ptr){
                Node to_delete = ptr;
                ptr = ptr->next;
                delete to_delete;
            }
        }
        void insertNewNode(const T& data);
        void deleteNode(Node node_to_remove);
        Node findNode(const T& data);
        Node getHead() const;
    };

    template<class T>
    LinkedListNode<T>* LinkedList<T>::getHead() const{
        return head;
    }

    template<class T>
    void LinkedList<T>::insertNewNode(const T& data){
        LinkedListNode<T>* node_to_insert(new LinkedListNode<T>(data));
        if(head == nullptr){
            head = node_to_insert;
        }
        else{
            node_to_insert->next = head;
            head->prev = node_to_insert;
            head = node_to_insert;
        }
    }

    template<class T>
    void LinkedList<T>::deleteNode(LinkedListNode<T>* node_to_remove){
        if(node_to_remove == head){
            if(head->next){
                LinkedListNode<T>* temp = head;
                head->next->prev = nullptr;
                head = head->next;
                delete temp;
            }
            else{
                delete head;
                head = nullptr;
            }
        }
        else{
            if(node_to_remove->next){
                node_to_remove->next->prev = node_to_remove->prev;
                node_to_remove->prev->next = node_to_remove->next;
            }
            else{
                node_to_remove->prev->next = nullptr;
            }
            delete node_to_remove;
        }
    }

}

#endif