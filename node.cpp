#include "node.h"

Node::Node(void):
    _next(this), _prev(this)
{
}

Node::~Node(void){

}


Node *Node::next(void){
    return _next;
}

Node *Node::prev(void){
    return _prev;
}

Node *Node::remove(void){
    _prev->_next = _next;
    _next->_prev = _prev;
    _next = _prev = this;
    return this;
}

void Node::splice(Node *b){
    Node *a = this;
    Node *an = a->_next;
    Node *bn = b->_prev;
    a->_next = bn;
    b->_next = an;
    an->_prev = b;
    bn->_prev = a;
}

