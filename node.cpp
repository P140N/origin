#include "node.h"

Node::Node():
    next ( this ) , _prev ( this ){
}

Node *Node::next(void)
{
    return _next;
}

Node *Node::prev(void)
{
    return _prev;
}

Node *Node::insert(Node *b)
{
    Node *c = _next;
      b->__next = c;
      b->_prev = this ;
      _next = b;
      c->_prev = b;
      return b;
}

Node *Node::remove(void)
{
    _prev->_next = _next;
    _next->_prev = _prev;
    _next = _prev = this;
    return  this;
}


