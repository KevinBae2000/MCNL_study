#include <string>
#include <iostream>
using namespace std;

enum Color
{
    RED,
    BLACK
};
template <typename T1,typename T2>
struct treenode
{
    /* data */
    Color color;
    T1 key;
    T2 value;
    treenode *llink, *rlink, *plink;


    treenode(){
        color=RED;
        llink=nullptr;
        rlink=nullptr;
        plink=nullptr;
    }
    ~treenode(){}
};

template <typename T1,typename T2>
class MyIterator
    {
    private:
        treenode<T1,T2>* cur;
    public:
        MyIterator(treenode<T1,T2> *p=nullptr)
            :cur(p){}
        MyIterator& operator++(){
            if(cur->rlink){
                //for(cur=cur->rlink;cur->llink;cur=cur->llink);
                cur=cur->rlink;
                while(cur->llink){
                    cur=cur->llink;
                }
            }
            else{
                if(cur->plink&&cur==cur->plink->llink){
                    cur=cur->plink;
                }
                else if(cur==cur->plink->rlink){
                    T1 v=cur->key;
                    // for(cur;cur->key<=v&&cur->plink!=nullptr;cur=cur->plink){
                    //     cout << cur->key << ": " << cur->value << endl;
                    // }
                    while(cur->key<=v&&cur->plink!=nullptr){
                        cur=cur->plink;
                    }
                    if(cur->key<=v)cur=nullptr;
                }
            }

            return *this;
        }
        bool operator==(const MyIterator &ref){
            return cur==ref.cur;
        }
        bool operator!=(const MyIterator &ref){
            return cur!=ref.cur;
        }
        treenode<T1,T2>* operator->(){
            return cur;
        }
    };


    

template <typename T1,typename T2>
class RBT{
    
private:
    /* data */
    treenode<T1,T2>*root;
    treenode<T1,T2>* head;
public:
    RBT(){root=nullptr;head=nullptr;};
    ~RBT();
    void delete_node(treenode<T1,T2>* ptr);
    void insert(pair<T1,T2>p);//노드를 생성 및 넣는다 
    void find_insert(treenode<T1,T2>* ctree, treenode<T1,T2>* ntree);//binary tree방식으로 넣기
    void balance(treenode<T1,T2>* ntree);// red black tree로 조절
    void print();
    void print(treenode<T1,T2>* iter);
    void print_n();
    
    treenode<T1,T2>* find(T1 key, treenode<T1,T2>* b);
    treenode<T1,T2>* left_max(treenode<T1,T2>* b);
    treenode<T1,T2>* right_min(treenode<T1,T2>* b);
    void delete_choose(T1 key);
    
    typedef MyIterator<T1,T2> iterator;
    iterator begin(){
        for(head=root;head->llink;head=head->llink);
        return iterator(head);
    } 

    iterator end(){
        return iterator(nullptr);
    }
    T2& operator[](T1 key){
        treenode<T1,T2>* ttree = find(key,root);
        if(ttree){
            return ttree->value;
        }
        else{
        treenode<T1,T2>* ntree = new treenode<T1,T2>;    
        ntree->key=key;
        if(!root){
            root=ntree;
            root->color=BLACK;
            return ntree->value;
        }

        find_insert(root,ntree);
        if(ntree->color==RED && ntree->plink->color==RED)
            balance(ntree);
        return ntree->value;   
        }
    }


    
};
template <typename T1,typename T2>
RBT<T1,T2>::~RBT()
{
    delete_node(root);
}

template <typename T1,typename T2>
void RBT<T1,T2>::delete_node(treenode<T1,T2>* ptr){
    if(!ptr)return;
    delete_node(ptr->rlink);
    delete_node(ptr->llink);
    delete ptr;
}

template <typename T1,typename T2>
void RBT<T1,T2>::insert(pair<T1,T2>p){
    treenode<T1,T2>* ntree = new treenode<T1,T2>();
    ntree->key=p.first;
    ntree->value=p.second;

    if(!root){
        root=ntree;
        root->color=BLACK;
        return;
    }

    find_insert(root,ntree);
    if(ntree->color==RED && ntree->plink->color==RED)
        balance(ntree);

}

template <typename T1,typename T2>
void RBT<T1,T2>::find_insert(treenode<T1,T2>* ctree, treenode<T1,T2>* ntree){
    if(ctree==nullptr){
        ctree=ntree;
        treenode<T1,T2>* parent = ntree->plink;
        if(ntree->key<parent->key)parent->llink=ntree;
        else parent->rlink=ntree;
        return;
    }
    ntree->plink=ctree;
    if(ctree->key>ntree->key){
        find_insert(ctree->llink,ntree);
    }
    else if(ctree->key==ntree->key){
        delete ntree;
    }
    else{
        find_insert(ctree->rlink,ntree);
    }

}

template <typename T1,typename T2>
void RBT<T1,T2>::balance(treenode<T1,T2>* ntree){
    treenode<T1,T2>* gp = ntree->plink->plink;
    treenode<T1,T2>* p = ntree->plink;
    treenode<T1,T2>* op = ntree->plink->plink->plink;
    
    
    if(ntree->plink == gp->rlink && (gp -> llink==nullptr || gp -> llink->color == BLACK)){
        if(ntree == p->rlink){
            if(op){
                if(op->rlink == gp)op->rlink=p;
                else if(op->llink == gp)op->llink=p;
            }
            else root=p;
            p->plink=op;
            if(p->llink)p->llink->plink=gp;
            gp->rlink=p->llink;
            p->llink=gp;
            gp->plink=p;
            p->color=BLACK;
            gp->color=RED;
        }
        else if(ntree == p->llink){
            if(op){
                if(op->rlink == gp)op->rlink=ntree;
                else if(op->llink == gp)op->llink=ntree;
            }
            else root=ntree;
            ntree->plink=op;
            if(ntree->llink)ntree->llink->plink=gp;
            gp->rlink=ntree->llink;
            ntree->llink=gp;
            gp->plink=ntree;
            ntree->color=BLACK;
            gp->color=RED;
            p->llink=ntree->rlink;
            if(ntree->rlink)ntree->rlink->plink=p;
            ntree->rlink=p;
            p->plink=ntree;
        }

    }
    else if(ntree->plink == gp->llink && (gp -> rlink==nullptr || gp -> rlink->color == BLACK)){
       if(ntree == p->llink){
            if(op){
                if(op->rlink == gp)op->rlink=p;
                else if(op->llink == gp)op->llink=p;
            }
            else root=p;
            p->plink=op;
            if(p->rlink)p->rlink->plink=gp;
            gp->llink=p->rlink;
            p->rlink=gp;
            gp->plink=p;
            p->color=BLACK;
            gp->color=RED;
        }
        else if(ntree == p->rlink){
            if(op){
                if(op->rlink == gp)op->rlink=ntree;
                else if(op->llink == gp)op->llink=ntree;
            }
            else root=ntree;
            ntree->plink=op;
            if(ntree->rlink)ntree->rlink->plink=gp;
            gp->llink=ntree->rlink;
            ntree->rlink=gp;
            gp->plink=ntree;
            ntree->color=BLACK;
            gp->color=RED;
            p->rlink=ntree->llink;
            if(ntree->llink)ntree->llink->plink=p;
            ntree->llink=p;
            p->plink=ntree;
        }
    }
    else if(gp->llink->color==RED&&gp->rlink->color==RED){
        gp->llink->color=BLACK;
        gp->rlink->color=BLACK;
        if(gp != root)gp->color=RED;


        if(gp->color==RED&&gp->plink->color==RED)balance(gp);
    }
}

template <typename T1,typename T2>
treenode<T1,T2>* RBT<T1,T2>::find(T1 key, treenode<T1,T2>* b){
    if(b==nullptr)return nullptr;
    if(key<b->key)return find(key,b->llink);
    else if(key==b->key)return b;
    else return find(key,b->rlink);
}

template <typename T1,typename T2>
treenode<T1,T2>* RBT<T1,T2>::left_max(treenode<T1,T2>* b){

    treenode<T1,T2>* c=b->llink;
    if(!c)return nullptr;
    while (c->rlink)
    {
        c=c->rlink;
    }
    
    return c;
}

template <typename T1,typename T2>
treenode<T1,T2>* RBT<T1,T2>::right_min(treenode<T1,T2>* b){

    treenode<T1,T2>* c=b->rlink;
    if(!c)return nullptr;
    while (c->llink)
    {
        c=c->llink;
    }
    
    return c;
}

template <typename T1,typename T2>
void RBT<T1,T2>::delete_choose(T1 key){    
    treenode<T1,T2>* target=find(a,root);
    if(!target)return;

    if(target->color==RED){
        if(!target->llink&&!target->rlink){
            if(target->plink->llink==target)target->plink->llink=nullptr;
            else target->plink->rlink=nullptr;
            delete target;
        }
        else if(!target->llink&&target->rlink){
            target->key=right_min(target)->key;
            target->value=right_min(target)->value;
            delete_choose(right_min(target));
        }
        else{
            target->key=left_max(target)->key;
            target->value=left_max(target)->value;
            delete_choose(left_max(target));
        }
    }
    else{
        if(!target->llink&&target->rlink&&right_min(target)->color==RED){
            target->key=right_min(target)->key;
            target->value=right_min(target)->value;
            delete_choose(right_min(target));
        }
        else if(target->llink&&left_max(target)->color==RED){
            target->key=right_min(target)->key;
            target->value=right_min(target)->value;
            delete_choose(right_min(target));
        }
        else{
            if(target->plink){
                if(target==target->plink->llink&&target->plink->rlink->color==RED){//1-1
                    treenode<T1,T2>* p=target->plink;
                    treenode<T1,T2>* s=target->plink->rlink;
                    p->rlink=s->llink;
                    s->llink->plink=p;
                    s->plink=p->plink;
                    if(p->plink->rlink==p)p->plink->rlink=s;
                    else p->plink->llink=s;
                    s->llink=p;
                    p->plink=s;

                }
                else if(target==target->plink->rlink&&target->plink->llink->color==RED){//1-2
                    treenode<T1,T2>* p=target->plink;
                    treenode<T1,T2>* s=target->plink->llink;
                    p->llink=s->rlink;
                    s->rlink->plink=p;
                    s->plink=p->plink;
                    if(p->plink->rlink==p)p->plink->rlink=s;
                    else p->plink->llink=s;
                    s->rlink=p;
                    p->plink=s;

                }
                else if(target==target->plink->llink&&target->plink->rlink->color==BLACK&&target->plink->color==BLACK
                &&(!target->plink->rlink->llink||target->plink->rlink->llink->color==BLACK)
                &&(!target->plink->rlink->rlink||target->plink->rlink->rlink->color==BLACK)){//2-1
                    treenode<T1,T2>* s=target->plink->rlink;
                    s->color=RED;
                }
                else if(target==target->plink->rlink&&target->plink->llink->color==BLACK&&target->plink->color==BLACK
                &&(!target->plink->llink->llink||target->plink->llink->llink->color==BLACK)
                &&(!target->plink->llink->rlink||target->plink->llink->rlink->color==BLACK)){//2-2
                    treenode<T1,T2>* s=target->plink->llink;
                    s->color=RED;
                }
                else if(target==target->plink->llink&&target->plink->rlink->color==BLACK&&target->plink->color==RED
                &&(!target->plink->rlink->llink||target->plink->rlink->llink->color==BLACK)
                &&(!target->plink->rlink->rlink||target->plink->rlink->rlink->color==BLACK)){//3-1
                    treenode<T1,T2>* p=target->plink;
                    treenode<T1,T2>* s=target->plink->rlink;
                    p->color=BLACK;
                    s->color=RED;
                }
                else if(target==target->plink->rlink&&target->plink->llink->color==BLACK&&target->plink->color==RED
                &&(!target->plink->llink->llink||target->plink->llink->llink->color==BLACK)
                &&(!target->plink->llink->rlink||target->plink->llink->rlink->color==BLACK)){//3-2
                    treenode<T1,T2>* p=target->plink;
                    treenode<T1,T2>* s=target->plink->llink;
                    p->color=BLACK;
                    s->color=RED;
                }
                else if(target==target->plink->llink&&target->plink->rlink->llink&&target->plink->rlink->llink->color==RED){//4-1
                    treenode<T1,T2>* p=target->plink;
                    treenode<T1,T2>* s=target->plink->rlink;
                    treenode<T1,T2>* sl=target->plink->rlink->llink;
                    p->rlink=sl;
                    sl->plink=p;
                    s->llink=s->llink->rlink;
                    if(s->llink->rlink)s->llink->rlink->plink=s;
                    sl->rlink=s;
                    s->plink=sl;
                    sl->color=BLACK;
                    s->color=RED;
                }
                else if(target==target->plink->rlink&&target->plink->llink->rlink&&target->plink->llink->rlink->color==RED){//4-2
                    treenode<T1,T2>* p=target->plink;
                    treenode<T1,T2>* s=target->plink->llink;
                    treenode<T1,T2>* sr=target->plink->llink->rlink;
                    p->llink=sr;
                    sr->plink=p;
                    s->rlink=s->rlink->llink;
                    if(s->rlink->llink)s->rlink->llink->plink=s;
                    sr->llink=s;
                    s->plink=sr;
                    sr->color=BLACK;
                    s->color=RED;
                }
                else if(target==target->plink->llink&&target->plink->rlink->rlink&&target->plink->rlink->rlink->color==RED){//5-1
                    treenode<T1,T2>* p=target->plink;
                    treenode<T1,T2>* s=target->plink->rlink;
                    treenode<T1,T2>* sr=target->plink->rlink->rlink;
                    s->plink=p->plink;
                    if(s->plink){
                        if(s->plink->rlink==p)s->plink->rlink=s;
                        else s->plink->llink=s;
                    }
                    p->rlink=s->llink;
                    if(p->rlink)p->rlink->plink=p;
                    s->llink=p;
                    p->plink=s;
                    s->color=p->color;
                    p->color=BLACK;
                    sr->color=BLACK;
                }
                else if(target==target->plink->rlink&&target->plink->llink->llink&&target->plink->llink->llink->color==RED){//5-2
                    treenode<T1,T2>* p=target->plink;
                    treenode<T1,T2>* s=target->plink->llink;
                    treenode<T1,T2>* sl=target->plink->llink->llink;
                    s->plink=p->plink;
                    if(s->plink){
                        if(s->plink->rlink==p)s->plink->rlink=s;
                        else s->plink->llink=s;
                    }
                    p->llink=s->rlink;
                    if(p->llink)p->llink->plink=p;
                    s->rlink=p;
                    p->plink=s;
                    s->color=p->color;
                    p->color=BLACK;
                    sl->color=BLACK;
                }
            }
        }
    }
}

template <typename T1,typename T2>
void RBT<T1,T2>::print(){
    print(root);
}

template <typename T1,typename T2>
void RBT<T1,T2>::print(treenode<T1,T2>* iter){
    if(!iter)return;
    print(iter->llink);
    if(iter->plink)cout << iter->plink->key << ": " << iter->plink->value << endl;
    print(iter->rlink);
}

template <typename T1,typename T2>
void RBT<T1,T2>::print_n(){
cout<<"root"<< root->key << ": " << root->value << endl;
}

template<typename T1,typename T2>
void print_map(RBT<T1, T2> m)
{
    RBT<string, int>::iterator iter;

    for(iter=m.begin(); iter != m.end(); ++iter){
        cout << iter->key << ": " << iter->value << endl;
    }
}

int main(){
    RBT<string,int> m;
   cout << "** First Step **\n";
    m.insert(make_pair("Global", 10));
    m.insert(make_pair("Handong", 30));
    m.insert(make_pair("CSEE", 20));
    m.insert(make_pair("MCNL", 15));
    //m.print_n();
    print_map(m);

    cout<<"\n** Second Step **\n";
    m["Pohang"] = 50;
    m["Korea"] = 60;
    print_map(m);

    cout<<"\n** Third Step **\n";
    m["CSEE"] = 100;
    //m.erase("Global");
    print_map(m);
    return 0;
}