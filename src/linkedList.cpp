#include "linkedList.h"

linkedList::linkedList(){}

void linkedList::append(int num)
{
     node *q,*t;

   if( p == NULL )
   {
      p = new node;
      p->data = num;
      p->link = NULL;
   }
   else
   {
      q = p;
      while( q->link != NULL )
           q = q->link;

      t = new node;
      t->data = num;
      t->link = NULL;
      q->link = t;
   }
}
void linkedList::sort_add(int num)
{
    int count=0;
    node *q;

    if(p==NULL)
        add_as_first(num);
    else
    {
        q=p;
        while(q->data<num)
        {
            count++;
            if(q->link==NULL)
             break;
            q = q->link;
        }
        if(count==0)
            add_as_first(num);
        else
            addafter( count-1, num);
    }
}

void linkedList::add_as_first(int num)
{
   node*q;
   q = new node;
   q->data = num;
   q->link = p;
   p = q;
}

void linkedList::addafter( int c, int num)
{
     node *q,*t;
   int i;
   for(i=0,q=p;i<c;i++)
   {
        q = q->link;
      if( q == NULL )
      {
           cout<<"\nThere are less than "<<c<<" elements.";
         return;
      }
   }

   t = new node;
   t->data = num;
   t->link = q->link;
   q->link = t;
}

void linkedList::del( int num )
{
     node *q,*r;
   q = p;
   if( q->data == num )
   {
        p = q->link;
      delete q;
      return;
   }

   r = q;
   while( q!=NULL )
   {
        if( q->data == num )
      {
           r->link = q->link;
         delete q;
         return;
      }

      r = q;
      q = q->link;
   }
   cout<<"\nElement "<<num<<" not Found.";
}

void linkedList::display()
{
     node *q;
   cout<<endl;

   for( q = p ; q != NULL ; q = q->link )
        cout<<endl<<q->data;

}

int linkedList::count()
{
     node *q;
   int c=0;
   for( q=p ; q != NULL ; q = q->link )
        c++;

   return c;
}

void linkedList::deleteFirstElement()
{
    node *q;
   if( p == NULL )
        return;

    q = p->link;
    delete p;
    p = q;
}

int linkedList::getValueAt(int position)
{
   node *q;
   int counter=0;
   if( p == NULL )
        return NULL;
   for( q=p ; q != NULL,counter!=position ; q = q->link )
        counter++;
    return q->data;
}

linkedList::~linkedList()
{

     node *q;
   if( p == NULL )
        return;

   while( p != NULL )
   {
     q = p->link;
      delete p;
      p = q;
   }
}

