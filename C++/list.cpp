#include "list.h"

using namespace std;

Node::Node(string s)
{  
   data = s;
   previous = NULL;
   next = NULL;
}

List::List()
{  
   first = NULL;
   last = NULL;
}

void List::push_back(string data)
{  
   Node* new_node = new Node(data);
   if (last == NULL) // List is empty
   {  
      first = new_node;
      last = new_node;
   }
   else
   {  
      new_node->previous = last; 
      last->next = new_node;
      last = new_node;
   }
}

void List::insert(Iterator iter, string s)
{  
   if (iter.position == NULL)
   {  
      push_back(s);
      return;
   }

   Node* after = iter.position;
   Node* before = after->previous;
   Node* new_node = new Node(s);
   new_node->previous = before;
   new_node->next = after;
   after->previous = new_node;
   if (before == NULL) // Insert at beginning
      first = new_node;
   else
      before->next = new_node;
}

Iterator List::erase(Iterator iter)
{  
   assert(iter.position != NULL);
   Node* remove = iter.position;
   Node* before = remove->previous;
   Node* after = remove->next;

   if (remove == first)
      first = after;
   else
      before->next = after;
   if (remove == last)
      last = before;
   else
      after->previous = before;
   delete remove;
   Iterator r;
   r.position = after;
   r.container = this;
   return r;
}

Iterator List::begin()
{  
   Iterator iter;
   iter.position = first;
   iter.container = this;
   return iter;
}

Iterator List::end()
{  
   Iterator iter;
   iter.position = NULL;
   iter.container = this;
   return iter;
}

Iterator::Iterator()
{  
   position = NULL;
   container = NULL;
}

string Iterator::get() const
{  
   assert(position != NULL);
   return position->data;
}

void Iterator::next()
{  
   assert(position != NULL);
   position = position->next;
}

void Iterator::previous()
{  
   assert(position != container->first);
   if (position == NULL)
      position = container->last;
   else 
      position = position->previous;
}

bool Iterator::equals(Iterator b) const
{  
   return position == b.position;
}

