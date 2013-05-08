#ifndef NODE_H
#define NODE_H

class Node
{
protected:
    Node *_next;	// связь к последующему узлу
    Node *_prev;	// связь к предшествующему узлу
public:
    Node (void);
    virtual ~Node (void);
    Node *next(void);
    Node *prev(void);
    Node *insert(Node*);  // вставить узел после текущего
    Node *remove(void);// удалить узел из списка, возвратить его указатель
    void splice(Node*); // слияние/разделение кольцевых списков
};

#endif // NODE_H
