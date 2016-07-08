
#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include <iostream>

using namespace std;

class linkedList
{
    public:
    linkedList();
    void deleteFirstElement();
    void append( int num);
    void sort_add( int num );
    void add_as_first( int num );
    void addafter( int c, int num );
    void del( int num );
    void display();
    int count();
    int getValueAt(int position);
    ~linkedList();

    protected:

    private:
     struct node
         {
              int data;
            node *link;
         }*p;


};

#endif // LINKEDLIST_H
