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
    void delete_choose(int a);
    
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
void RBT<T1,T2>::delete_choose(int a){    
    treenode<T1,T2>* target=find(a,root);
    if(!target)return;

    treenode<T1,T2>* instead=left_max(target);

    treenode<T1,T2>* tp = target->plink;
    treenode<T1,T2>* ip = instead->plink;
    
    if(!instead){
        if(tp->llink==tp)tp->llink=target->rlink;
        else if(tp->rlink==tp)tp->rlink=target->rlink;
        if(target->rlink)target->rlink->plink=tp;
        delete target;
        return;
    }

    if(ip->rlink==instead)ip->rlink=instead->llink;
    else if(ip->llink==instead)ip->llink=instead->llink;
    if(instead->llink)instead->llink->plink=ip;
    target->k=instead->k;
    delete instead;

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