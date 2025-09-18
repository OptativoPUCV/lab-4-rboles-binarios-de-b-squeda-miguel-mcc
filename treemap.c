#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap* mapa = (TreeMap*) malloc (sizeof(TreeMap));
    if (mapa == NULL) return NULL;

    mapa->root = NULL;
    mapa->current = NULL;
    mapa->lower_than = lower_than;
    return mapa;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree->root == NULL) {
        tree->root = createTreeNode(key, value);
        tree->current = tree->root;
        return;
    }

    TreeNode* aux = tree->root;
    TreeNode* parent = NULL;

    while (aux != NULL){
        if (is_equal(tree, key, aux->pair->key)) return;
        parent = aux;
        if (tree->lower_than(key, aux->pair->key))
            aux = aux->left;
        else
            aux = aux->right;
    }

    TreeNode* newNode = createTreeNode(key, value);
    newNode->parent = parent;

    if (tree->lower_than(key, parent->pair->key))
        parent->left = newNode;
    else
        parent->right = newNode;

    tree->current = newNode;    
}

TreeNode * minimum(TreeNode * node){
    while (node != NULL && node->left != NULL){
        node = node->left;
    }
    return node;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node == NULL) return;

    if (node->left == NULL && node->right == NULL){
        if (node->parent == NULL){
            tree->root = NULL;
        } else {
            if (node->parent->left == node) node->parent->left = NULL;
            else node->parent->right = NULL;
        }
        free(node->pair);
        free(node);
    }
    else if (node->left == NULL || node->right == NULL) {
        TreeNode* child = (node->left != NULL) ? node->left : node->right;
        if (node->parent == NULL) {
            tree->root = child;
            child->parent = NULL;
        } else {
            if (node->parent->left == node) node->parent->left = child;
            else node->parent->right = child;
            child->parent = node->parent;
        }
        free(node->pair);
        free(node);
    }
    else {
        TreeNode* minNode = minimum(node->right);
        node->pair->key = minNode->pair->key;
        node->pair->value = minNode->pair->value;
        removeNode(tree, minNode);
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}




Pair* searchTreeMap(TreeMap* tree, void* key) {
    TreeNode* aux = tree->root;

    while (aux != NULL) {
        if (is_equal(tree, key, aux->pair->key)) {
            tree->current = aux;
            return aux->pair;
        }
        if (tree->lower_than(key, aux->pair->key)) {
            aux = aux->left;
        } else {
            aux = aux->right;
        }
    }
    return NULL;
}


Pair* upperBound(TreeMap* tree, void* key){
    TreeNode* aux = tree->root;
    TreeNode* candidate = NULL;
    
    while (aux != NULL) {
        if (is_equal(tree, key, aux->pair->key)) {
            tree->current = aux;
            return aux->pair;
        }
        if (tree->lower_than(key, aux->pair->key)) {
            candidate = aux;
            aux = aux->left;
        } else {
            aux = aux->right;
        }
    }

    if (candidate != NULL) {
        tree->current = candidate;
        return candidate->pair;
    }
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    if (tree->root == NULL) return NULL;
    TreeNode* min = minimum(tree->root);
    tree->current = min;
    return min->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    return NULL;
}
