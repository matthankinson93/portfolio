#ifndef _BST_H_
#define _BST_H_

struct Bst;
enum TraverseOrder {pre, in, post};
typedef int BstCompare(const void *item1, const void *item2);
typedef void BstPrint(const void *item);

Bst *BstConstruct(BstCompare *fn);  
bool BstInsert(Bst *tree, const void *item);
const void *BstSearch(const Bst *tree, const void *item);  
void BstDestroy(Bst *tree);
void BstTraverse(Bst *tree, BstPrint *fn, TraverseOrder order);
#endif

