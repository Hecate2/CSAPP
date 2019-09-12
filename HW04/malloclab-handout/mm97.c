//用红黑树查找内存中空闲位置。红黑树节点为Node 

#include <stdio.h>  
#include <stdlib.h>  
#include <assert.h>  
#include <unistd.h>  
#include <string.h>  
  
#include "mm.h"  
#include "memlib.h"  
  
/********************************************************* 
 * NOTE TO STUDENTS: Before you do anything else, please 
 * provide your team information in the following struct. 
 ********************************************************/  
team_t team =  
{  
    /* Team name */  
    "16307130006",  
    /* First member's full name */  
    "陈幸豪",  
    /* First member's email address */  
    "16307130006@fuadn.edu.cn",  
    /* Second member's full name (leave blank if none) */  
    "",  
    /* Second member's email address (leave blank if none) */  
    ""  
};  
  
//int split_parity = 1;  
  
/*  Compilation Options - to be removed eventually   */  
#define _DEBUG_          0     /* Prints out lots of crap */  
  
  
/*  Typedefs for Heap Data Structures  */  
typedef struct List {  //链表节点。链表记录最近刚被free的节点。又称为blob 
//例如realloc中，不需要在free和malloc维护红黑树两次。只要在链表中先作临时记录即可
//realloc全部完成后，去维护红黑树 
    void** BLAH;  
    struct List* Prev;  
    struct List* Next;  
    void** BLAH2;  
} List;  
  
typedef struct Node {  //红黑树节点。红黑树记录所有空闲块 
    struct Node* parent;  
    struct Node* left;  
    struct Node* right;  
    int color;  
} Node;  
  
  
/*  Named Constants  */  
#define HEAP_INITSIZE    8232   /* multiple of 8 */  
#define HEAP_GROWSIZE    4096   /* multiple of 8 */  
#define REALLOC_GROWSIZE   2048  /* multiple of 8 */  
//#define REALLOC_BUBBLE   128     /* blocksize     */  
//#define REALLOC_BUBBLEINCREMENT 128  
#define RED              1  
#define BLACK            0  
#define FALSE            0  
  
  
/*  Masks for Boundary Tags  */  
#define SIZE_MASK        0xFFFFFFF8 //111...1000
#define TREE_MASK        4  		//000...0100
#define BLOB_MASK        2   		//000...0010
#define FREE_MASK        1   		//000...0001
//以上可以用一个字节表示很多信息。节约空间 
#define IN_TREE          (FREE_MASK|TREE_MASK)  
#define IN_BLOB          (FREE_MASK|BLOB_MASK)  
#define ALLOCATED        0  
  
  
/*  Named Heap Locations  */  
#define treeroot         ((Node**)(char *)mem_heap_lo())  //mem_heap_lo为堆的初始地址 
#define blobroot         ((List**)(char *)mem_heap_lo()+1)  
#define boundtag_lo      (((int*)((char *)mem_heap_lo()+16)))  
#define boundtag_hi      (((int*)((char *)mem_heap_hi()-3 )))   //mem_heap_hi为堆的最高地址 
#define split_parity     (*((int*)(char *)mem_heap_lo()+12))  
  
/*  Macros for Boundary Tags  */  
  
//  The following 2 are just for use in definitions  
#define __LowTag(p)      (*((int*)(p)-1))  
#define __HiPrevTag(p)   (*((int*)(p)-2))  
  
//  The next few can be safely used with all block pointers  
#define Size(p)          (__LowTag(p) & SIZE_MASK)  //输入指针p，检查它占了多少内存空间 
#define IsFree(p)        (__LowTag(p) & FREE_MASK)  
#define IsInBlob(p)      (__LowTag(p) & BLOB_MASK)  
#define IsInTree(p)      (__LowTag(p) & TREE_MASK)  
//  PrevBlock(p) is undefined for the first addressable block.  
#define PrevBlock(p)     ((char *)(p) - (__HiPrevTag(p) & SIZE_MASK))  
#define NextBlock(p)     ((char *)(p) + Size(p))  
#define PrevFree(p)      (__HiPrevTag(p) & FREE_MASK)  
#define NextFree(p)      (IsFree(NextBlock(p)))  
  
  
//红黑树可查找空闲块 
/*  Tree Node Macros  */  
#define IS_RED(p)        ((p!=NULL) && (((Node*)p)->color == RED))  
#define IS_BLACK(p)        ((p==NULL) || (((Node*)p)->color == BLACK))  
  
/* 
 * Invariants: 
 *  - The word starting at (mem_heap_lo()) is a pointer to the root 
 *    node of the freetree. 
 *    If this value is null, then there are no free blocks. 
 *  - The word starting at (mem_heap_lo()+8) is a fake upper boundary 
 *    tag, with size 0, and flagged as allocated. 
 *  - The word starting at (mem_heap_hi()-3) is a fake lower boundary 
 *    tag, with size 0, and flagged as allocated. 
 *  - Every block has a "front" and "rear" tag.  This tag is 4 bytes 
 *    (1 word) being an integer of the size of the block. 
 *    The semantics of these tags are as follows: 
 *      - By masking off the least significant 3 bits, we get 
 *        the size of the block in bytes. 
 *      - Combinations of the last 3 bits are interpreted as follows: 
 *          0 0 0  =>  内存块被占用 
 *          1 1 1  =>  内存块为红色，在红黑树中 
 *          1 0 1  =>  内存块为黑色，在红黑树中 
 *          0 0 1  =>  不在红黑树中，也不在链表中 
 *          0 1 1  =>  内存块空闲，但不在红黑树中 
 *                     it is in the doubly-linked list of recently 
 *                     freed nodes (the blob). 
 *  - Free blocks have front and rear tags as described above, and 
 *    the front tag is followed by three pointers.  These pointers are 
 *    (in order):  Left Child, Right Child, List Next.  Each of these 
 *    is one word in length (4 bytes).  All Free blocks will be 
 *    maintained within a Red-Black tree with blocks of the same 
 *    size being in an address ordered linked list. 
 *  - Allocated Blocks will have front and rear tags as described 
 *    previously with block data between these tags. 
 * 
 *  - At a 
 * 
 * Policies: 
 * 
 *  - Free blocks are coalesced(合并) immediately and added to the freetree. 
 *  - Allocation is done best-fit(找最小的但大于等于要求的块). 
 *    When there are several candidates for 
 *    the best-fit, then the lowest-addressed block is chosen. 
 * 
 * Conventions: 
 * 
 *  - Block pointers are treated as (int*)'s 
 */  
  
void setTags (void* block, size_t size, int flags)  //把一块内存标记为在链表中，红黑树中 ，或被分配了 
{  
    int* tag1 = (int*)block - 1;  
    int* tag2 = (int*)((char *)block+size)-2;  
    *tag1 = *tag2 = (size | flags);  
}  
  
  
/* 对红黑树节点x左旋(Node* x) 。此时x的右孩子不可为空 
 *       x        -->        y 
 *      / \                 / \ 
 *     T1  y      -->      x  T3 
 *        / \             / \ 
 *       T2 T3    -->    T1 T2 
 * 这个漂亮的图是抄来的 
 */  
void left_rotate (Node* x)
{  
    Node* y = x->right;  
  
    x->right = y->left;  
    if ( y->left != NULL )  
    y->left->parent = x;  
  
    y->parent = x->parent;  
    if ( x->parent == NULL )  
    *treeroot = y;  
    else  
    if ( x == x->parent->left )  
        x->parent->left = y;  
    else  
        x->parent->right = y;  
  
    y->left = x;  
    x->parent = y;  
}  
  
/* 对红黑树节点x右旋(Node* x)。此时x的左孩子不可为空 
 *           x      -->       y 
 *          / \              / \ 
 *         y  T3    -->     T1  x 
 *        / \                  / \    
 *       T1 T2      -->       T2 T3 
 * 这个漂亮的图是抄来的 
 */  
void right_rotate (Node* x) {  
    Node* y = x->left;  
      
    x->left = y->right;  
    if (y->right != NULL)  
    	y->right->parent = x;  
      
    y->parent = x->parent;  
      
    if (x->parent == NULL)   
    	*treeroot = y;  
    else  
	    if (x == x->parent->left)  
	        x->parent->left = y;  
	    else  
	        x->parent->right = y;  
      
    y->right = x;  
    x->parent = y;  
}  
  
//红黑树涂色（这些函数现在用宏代替！） 
int isRed (Node* x) {  
    return (x != NULL) && (x->color == RED);  
}  
  
int isBlack (Node* x) {  
    return (x == NULL) || (x->color == BLACK);  
}  
  
void setblack (Node* x) {  
    if (x!=NULL)  
    x->color = BLACK;  
}  
  
void setred (Node* x) {  
    // NULL nodes are never red.  
    x->color = RED;  
}  
  
/* 
 * int isLess(Node* x, Node* y) 
 * 
 * This expression defines the order by which 
 * the tree is constructed.  x and y may not be NULL. 
 * 
 */  
int isLess (Node* x, Node *y) {  
    return (Size(x) < Size(y)) || ((Size(x) == Size(y)) && x<y);  
}  
  
/* 
 * int tree_insert(Node* x) 
 * 
 * Preconditions: 
 *     - x should be an established node 
 *       (i.e. size flags, IN_TREE set) that 
 *       does not appear already in the tree. 
 * 
 * Postconditions: 
 *     Inserts the node into the tree as follows: 
 *     - If the tree is currently empty, then we set x 
 *       to be the root, color it black, and return 0. 
 *     - Otherwise, we insert x as a leaf, color it red, 
 *       and return 1. 
 * 
 */  
int tree_insert (Node* x) {  
  Node* current = *treeroot;  
  
  //Empty tree --> update root pointer  
  if (current == NULL) {  
      *treeroot = x;  
      x->parent = NULL;  
      x->right = x->left = NULL;  
      x->color = BLACK;  
      return 0;  
  }  
  
  //Non-empty tree --> insert as leaf  
  while(1) {  
      if (isLess(x,current)) {  
      //x belongs in the left child of current node  
      if (current->left != NULL)  
          current = current->left;  
      else {  
          current->left = x;  
          x->parent = current;  
          x->right = x->left = NULL;  
          x->color = RED;  
          return 1;   
      }  
      }  
      else {  
      //x belongs in the right child of current node  
      if (current->right != NULL)  
          current = current->right;  
      else {  
          current->right = x;  
          x->parent = current;  
          x->right = x->left = NULL;  
          x->color = RED;  
          return 1;  
      }  
      }  
  }  
}  
  
void freetree_insert (void* ptr, size_t size) {  
    Node* x;  
    Node* y;  
  
    //Write the tags for the block  
    setTags(ptr, size, IN_TREE);  
    x = (Node*)ptr;  
  
    //Do a tree insertion  
    if (tree_insert(x) == 0){  
    // No further work needed.  
    return;  
    }  
      
    /* Move up the tree to restore the red/black property.       */  
    /* Invariant: x is red, and red/black properties are every-  */  
    /*            where satisfied, except maybe between x and    */  
    /*            x->parent.                                     */  
    while ( (x->parent != NULL)  
        && (IS_RED(x->parent))  
        && (x->parent->parent != NULL)) {  
    if ( x->parent == x->parent->parent->left ) {  
        /* If x's parent is a left, y is x's right 'uncle' */  
        y = x->parent->parent->right;  
        if ( IS_RED(y) ) {  
        /* case 1 - change the colours */  
        setblack(x->parent);    
        setblack(y);  
        setred(x->parent->parent);  
        /* Move x up the tree */  
        x = x->parent->parent;  
        }  
        else {  
        /* y is a black node */  
        if ( x == x->parent->right ) {  
            /* and x is to the right */   
            /* double-rotate . . .  */  
            left_rotate( x->parent );  
            right_rotate( x->parent );  
            setblack( x->left );  
        }  
        else  
        {  
            /* single-rotate */  
            setblack(x);  
            x = x->parent;  
            right_rotate( x->parent );  
        }  
        }  
    }  
    else {  
        /* If x's parent is a right, y is x's left 'uncle' */  
        y = x->parent->parent->left;  
        if ( IS_RED(y) ) {  
        /* case 1 - change the colours */  
        setblack(x->parent);  
        setblack(y);  
        setred(x->parent->parent);  
        /* Move x up the tree */  
        x = x->parent->parent;  
        }  
        else {  
        /* y is a black node */  
        if ( x == x->parent->left ) {  
            /* and x is to the left */  
            /* double rotate */  
            right_rotate( x->parent );  
            left_rotate( x->parent );  
            setblack(x->right);  
        }  
        else {  
            /* single rotate */  
            setblack(x);  
            x = x->parent;  
            left_rotate( x->parent );  
        }  
        }  
    }  
    }  
    /* Colour the root black */  
    setblack(*treeroot);  
}  
  
  
Node* freetree_locate(int size)
{  
    Node* best = NULL;  
    Node* current = *treeroot;  
  
    //Find the smallest (with respect to tree-order) element  
    //for which size <= Size(current), assuming that size-  
    //insufficiency is preserved by the tree-order.  
    while(current != NULL) {  
    if (size <= Size(current)) {  
        best = current;  
        current = current->left;  
    }  
    else  
        current = current->right;  
    }  
    return best;  
}  
  
int freetree_locatemax()  
{  
    Node* n = *treeroot;  
    if (n == NULL)  
    return 0;  
    else  
    {  
    while (n->right)  
        n = n->right;  
    }  
    return Size(n);  
}  
  
  
void left_child_is2x(Node* x);  
void right_child_is2x(Node* x);  
  
//left child is a double-black node.  Fix it.  
void left_child_is2x(Node* x){  
    Node* sis = x->right;  
  
    if (sis->color == RED)  
    {  
    left_rotate(x);  
    x->color = !(x->color);  
    sis->color = !(sis->color);  
    sis = x->right;  
    }  
  
    //Now sis is black.  Let's check its children.  
    if (IS_BLACK(sis->right) && IS_BLACK(sis->left))  
    {  
    sis->color = RED;  
    if (x->color == RED)  
    {  
        x->color = BLACK;  
        return;  //done!  
    }  
    else  
    {  
        //move violation up to parent, if any.  
        //if node is root, it's already black, and we're done.  
        if (x->parent != NULL)  
        {  
        if (x->parent->left == x)  
            left_child_is2x(x->parent);  
        else  
            right_child_is2x(x->parent);  
        }  
        return;  
    }  
    }  
  
    if (IS_BLACK(sis->right))  //farther child is black  
    {  
    //make it so that the farther child is red  
    right_rotate(sis);  
    sis->color = RED;   //used to be black, old sis  
    sis = x->right;  
    sis->color = BLACK;  //used to be red.  New sis  
    }  
  
    //now we know that sis->right is red. This is fixable.  
    left_rotate(x);  
    sis->color = x->color;      //just to copy.  
    x->color = BLACK;           //was indeterminate.  
    sis->right->color = BLACK;  //was red.  
    return;  
}  
  
  
void right_child_is2x(Node* x){  
    Node* sis = x->left;  
  
    if (sis->color == RED)  
    {  
    right_rotate(x);  
    x->color = !(x->color);  
    sis->color = !(sis->color);  
    sis = x->left;  
    }  
  
    //Now sis is black.  Let's check its children.  
    if (IS_BLACK(sis->right) && IS_BLACK(sis->left))  
    {  
    sis->color = RED;  
    if (x->color == RED)  
    {  
        x->color = BLACK;  
        return;  //done!  
    }  
    else  
    {  
        //move violation up to parent, if any.  
        //if node is root, it's already black, and we're done.  
        if (x->parent != NULL)  
        {  
        if (x->parent->left == x)  
            left_child_is2x(x->parent);  
        else  
            right_child_is2x(x->parent);  
        }  
        return;  
    }  
    }  
  
    if (IS_BLACK(sis->left))  //farther child is black  
    {  
    //make it so that the farther child is red  
    left_rotate(sis);  
    sis->color = RED;   //used to be black, old sis  
    sis = x->left;  
    sis->color = BLACK;  //used to be red.  New sis  
    }  
  
    //now we know that sis->left is red. This is fixable.  
    right_rotate(x);  
    sis->color = x->color;      //just to copy.  
    x->color = BLACK;           //was indeterminate.  
    sis->left->color = BLACK;   //was red.  
    return;  
}  
  
void freetree_delete( Node *z )
{  
    if (z == NULL)  
    return;  
      
    if ((z->left == NULL || z->right == NULL) && z->color == RED)  
    {  
    Node* child = z->left ? z->left : z->right;  //is black  
  
    if (child)  
        child->parent = z->parent;  
      
    if (z->parent == NULL)  
        *treeroot = child;  
    else if (z->parent->left == z)  
        z->parent->left = child;  
    else  
        z->parent->right = child;  
    return;  
    }  
    else if ((z->left == NULL || z->right == NULL) && z->color == BLACK)  
    {  
    Node* child = z->left ? z->left : z->right;  
    if (child)  
        child->parent = z->parent;  
  
    if (z->parent == NULL)  
    {  
        *treeroot = child;  
        setblack(child);  
        return;  
    }  
    else if (z->parent->left == z)  
    {  
        z->parent->left = child;  
        left_child_is2x(z->parent);  
        return;  
    }  
    else  
    {  
        z->parent->right = child;  
        right_child_is2x(z->parent);  
        return;  
    }  
    }  
    else if (z->right->left == NULL && z->right->color == RED)  
    {  
    //We know that z->left is non-null  
    z->right->left = z->left;  
    z->left->parent = z->right;  
    z->right->parent = z->parent;  
    z->right->color = BLACK;  
  
    if (z->parent == NULL)  
        *treeroot = z->right;  
    else if (z->parent->left == z)  
        z->parent->left = z->right;  
    else  
        z->parent->right = z->right;  
    return;  
    }  
    else if (z->right->left == NULL && z->right->color == BLACK)  
    {  
    z->right->left = z->left;  
    z->left->parent = z->right;  
    z->right->parent = z->parent;  
    z->right->color = z->color;  
  
    if (z->parent == NULL)  
        *treeroot = z->right;  
    else if (z->parent->left == z)  
        z->parent->left = z->right;  
    else  
        z->parent->right = z->right;  
  
    right_child_is2x(z->right);  
    return;  
    }  
    else  
    {  
    Node* y = z->right->left;  
    Node  y2;  
    while (y->left)  
        y = y->left;  
  
    y2 = *y;  
    *y = *z;  
    if (z->parent == NULL)  
        *treeroot = y;  
    else if (z->parent->left == z)  
        z->parent->left = y;  
    else  
        z->parent->right = y;  
    y->left->parent = y;  
    y->right->parent = y;  
  
    //now y has replaced z.  Clean up y2, where y used to be.  
    y2.parent->left = y2.right;  
    if (y2.right)  
        y2.right->parent = y2.parent;  
    if (y2.color == RED)  
        return;  
    else  
    {  
        left_child_is2x(y2.parent);  
        return;  
    }  
    }  
}  
  
/* 
 * Invariant: ptr points to a block that is free. 
 * Afterwards, the pointer will be deleted from 
 * the relevant data structures, but its tags 
 * will not reflect the change. 
 */  
void delFromWherever (void *ptr)  //从树里或者链表里删除ptr 
{  
    if (IsInTree(ptr))  
    	freetree_delete(ptr);  
    else if (IsInBlob(ptr))  
    {  
	    //The node is in the blob.  Remove it in O(1) time.  
	    List* L = ptr;  
	    if (L->Next != NULL)  
	        L->Next->Prev = L->Prev;  
	    if (L->Prev)  
	        L->Prev->Next = L->Next;  
	    else  
	        *blobroot = L->Next;  
    }  
}  
  
//用链表标记被分配的内存块(首地址ptr，大小size) 
void queueNewFreeBlock(void* ptr, int size)  
{  
    //Mark and insert into the blob.  
    List* L = ptr;  
      
    setTags(ptr, size, IN_BLOB);  
    L->Prev = NULL;  
    L->Next = *blobroot;  
    if (L->Next)  
    	L->Next->Prev = L;  
    *blobroot = L;  
}  
  
//takes all items from the blob and inserts into the freetree  
void emptyblob()  
{  
    /*  Move all blob-blocks into the tree  */  
    List* N = *blobroot;  
    while (N!=NULL)  
    {  
        List* temp = N->Next;  
        freetree_insert(N,Size(N));  
        N = temp;  
    }  
    *blobroot = NULL;  
}  
  
int mm_init (void)  
{  
    if (mem_sbrk(HEAP_INITSIZE) == NULL)  
        return -1;  
    //开出初始空间。mem_sbrk是memlib.c中的函数
	//会使堆空间增加HEAP_INITSIZE，并返回新的堆顶指针 
  
    //红黑树的根 
    *treeroot = NULL;  
  
    //记录哪些内存空着的链表  
    *blobroot = NULL;  
  
    //有效载荷的堆顶和堆底  
    *boundtag_lo = 0;  
    *boundtag_hi = 0;  
  
    split_parity = 0;  
      
    //在链表中记录：堆底+2到堆顶-24全是空着的 
    queueNewFreeBlock(boundtag_lo+2,HEAP_INITSIZE-24);  
  
      
    return 0;  
}  
  
void mm_free (void *ptr)  
{  
    //与前后(可能)空着的块合并 
    int new_size     = Size(ptr);  
    void *new_block  = ptr;  
    void *prev_block;  
    void *next_block;  
    if (NextFree(ptr))  
    {  
	    next_block = NextBlock(ptr);  
	    new_size += Size(next_block);  
	    delFromWherever(next_block);  
    }  
    if (PrevFree(ptr))  
    {  
	    prev_block = PrevBlock(ptr);  
	    new_size += Size(prev_block);  
	    new_block = prev_block;  
	    delFromWherever(prev_block);  
    }  
    //链表记录：被free的块，以及它前后的块是完整空着的 
    queueNewFreeBlock(new_block,new_size);  
}  
  
  
void *mm_realloc(void *ptr,size_t size)  
{  
    if (ptr == NULL)  
    {  
        return mm_malloc(size);  
    }  
      
    if (size == 0)  
    {  
        mm_free(ptr);  
        return NULL;  
    }  
  
    if (Size(ptr) >= (size+8))  
    {  
        return ptr;  
    }  
          
    void* next_block = NextBlock(ptr);  
    void* ptr2;  
      
    //是否可以把后续空闲的块(如果存在)全部包含进来？ 
    if (IsFree(next_block) && (Size(next_block)+Size(ptr) >= size+8))  
    {  
        size = (int)(Size(next_block))+(int)(Size(ptr));   //add sizes  
        delFromWherever(next_block);                       //  
        setTags(ptr,size,ALLOCATED);  
        return ptr;  
    }  
    //Can we expand past the next block?  
    if (IsFree(next_block) && (NextBlock(next_block) > (char *)boundtag_hi))  
    {  
        //We can do a combination of heap expansion  
        //and coalescing.  So just claim the next block  
        //and add it to this block. . . this should flow  
        //into the next if automatically.  
        delFromWherever(next_block);  
        setTags(ptr,Size(ptr)+Size(next_block),ALLOCATED);  
        next_block = NextBlock(next_block);  
    }  
    //当前ptr所指的块是最后一个（即地址最高的）块吗？ 
    if (next_block > (void *)boundtag_hi)
    {  
        //此时需要新开内存（增大堆） 
        int min_size = ((size+15)&-8)+8;  //-8==111..11000，考虑内存对齐 
        int grow_size = min_size - Size(ptr);  
      
        grow_size += REALLOC_GROWSIZE;  
        grow_size &= REALLOC_GROWSIZE;  
  
        if (mem_sbrk(grow_size) == NULL)  
        {  
            //开不出新的内存了 
            return NULL;  
        }  
        *boundtag_hi = 0;  
  
	    setTags(ptr,Size(ptr)+grow_size,ALLOCATED);  
	    return ptr;  
    }  
      
    ptr2 = mm_malloc(size);  
    memcpy(ptr2,ptr,Size(ptr)-8);  //Size(ptr) should not be size!  
    mm_free(ptr);  
  
#if _DEBUG_  
    printf("REALLOC: I had %p and got %p . . . \n",ptr,ptr2);  
    printAllBlocks();  
#endif //_DEBUG_  
    return ptr2;  
}  
  
  
void *mm_malloc (size_t size)  
{  
    void* block;  
    int block_size;  
    void* leftover_block = NULL;  
    int leftover_size;  
    size = ((size+7)&-8)+8;  //-8==111..11000，考虑内存对齐 
    if (size < 24)  
        size = 24;  //min size  
    //printf("malloc(0x%x)\n",size);  
  
    //clears the blob.  Broken b/c of faulty Tinsert  
    emptyblob();  
      
      
    block = freetree_locate(size);  
      
    if (block == NULL)  
    {  
        //Grow the heap.  
        block = (char *)mem_heap_hi()+1;  
        block_size = size;  
        block_size += HEAP_GROWSIZE;  
        block_size &= -HEAP_GROWSIZE;  //mangle so that we get something bigger than  
                                 //we need.  
        if (mem_sbrk(block_size) == NULL)  
        {  
            //no more memory.  Request cannot be satisfied.  
            return NULL;  
        }  
        *boundtag_hi = 0; //*((int*)(mem_heap_hi()-3)) = NULL;     // removed int* cast VMF  
        if (IsFree(PrevBlock(block)))  
        {  
            block = PrevBlock(block);  
            block_size += Size(block);  
            delFromWherever(block);  
        }  
    }  
    else  
    {  
        block_size = Size(block);  
        delFromWherever(block);  
    }  
  
    if ((block_size - size)>24)  // can we split it?  
    {  
        //NOTE: this always splits to the left.  We want a different policy.  
        //split the block  
        //re-insert the remainder  
        if (split_parity)  
        {  
            leftover_size  = block_size - size;  
            block_size     = size;   //cut is just right  
            leftover_block = (char *)block + block_size;  
            queueNewFreeBlock(leftover_block,leftover_size);  
        }  
        else  
        {  
            leftover_size = block_size - size;  
            block_size    = size;  
            leftover_block = (char *)block + leftover_size;  
            queueNewFreeBlock(block,leftover_size);  
            block = leftover_block;  
        }  
        split_parity = !split_parity;  
    }  
      
    setTags(block,block_size,ALLOCATED);
    //printBlock(block);  
#if _DEBUG_  
    if (printHeap());  
    printAllBlocks();  
    isRedBlack();  
#endif //_DEBUG_  
    return block;  
} 

