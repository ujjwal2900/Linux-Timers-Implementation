#include "LinkedListApi.h"
#include <stddef.h>
#include <string.h>
#include <stdio.h>  


ll_t *intit_singly_ll()
{
    ll_t *ll = (ll_t *)malloc(sizeof(ll_t));
    ll->node_count = 0;
    ll->head = NULL;
    return ll;
}

singly_ll_node_t *singly_ll_init_node(void *data)
{
    singly_ll_node_t *node = (singly_ll_node_t *)malloc(sizeof(singly_ll_node_t));
    node->data = data;
    node->next = NULL;
    return node;
}

int singly_ll_add_node(ll_t *ll, singly_ll_node_t *node)
{
    if (ll == NULL || node == NULL) {
        return -1; // Error: Invalid input
    }

    if (ll->head == NULL) {
        ll->head = node;
    } else {
        singly_ll_node_t *current = ll->head;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = node;
    }
    ll->node_count++;
    return 0; // Success
}

int singly_ll_add_node_by_val(ll_t *ll, void *data){
    singly_ll_node_t *node = singly_ll_init_node(data);
    return singly_ll_add_node(ll, node);
}

int singly_ll_remove_node(ll_t *ll, singly_ll_node_t *node){
    if (ll == NULL || node == NULL) {
        return -1; // Error: Invalid input
    }

    singly_ll_node_t *current = ll->head;
    singly_ll_node_t *previous = NULL;

    while (current != NULL) {

        if (current == node) {
            if (previous == NULL) {
                ll->head = current->next; // Removing head
            } else {
                previous->next = current->next; // Bypass the current node
            }
            free(current);
            ll->node_count--;
            return 0; // Success
        }
        previous = current;
        current = current->next;
    }
    return -1; // Error: Node not found
}

unsigned int singly_ll_remove_node_by_val(ll_t *ll, void *data, int size)
{
    if (ll == NULL || data == NULL) {
        return -1; // Error: Invalid input
    }

    singly_ll_node_t *current = ll->head;
    singly_ll_node_t *previous = NULL;
    unsigned int removed_count = 0;

    while (current != NULL) {
        if (memcmp(current->data, data, size) == 0) {
            if (previous == NULL) {
                ll->head = current->next; // Removing head
            } else {
                previous->next = current->next; // Bypass the current node
            }
            singly_ll_node_t *temp = current;
            current = current->next; // Move to next before freeing
            free(temp);
            ll->node_count--;
            removed_count++;
        } else {
            previous = current;
            current = current->next;
        }
    }
    return removed_count; // Return the number of nodes removed
}

bool_t is_singly_ll_empty(ll_t *ll){
    if(ll == NULL || ll->node_count == 0){
        return LL_TRUE;
    }
    return LL_FALSE;
}

void print_singly_ll(ll_t *ll){
    if (ll == NULL) {
        printf("Linked list is NULL\n");
        return;
    }

    singly_ll_node_t *current = ll->head;
    printf("Linked List (Node Count: %u): ", ll->node_count);
    while (current != NULL) {
        printf("%p -> ", current->data); // Print the data pointer
        current = current->next;
    }
    printf("NULL\n");
}

void reverse_singly_ll(ll_t *ll){
    if (ll == NULL || ll->head == NULL) {
        return; // Nothing to reverse
    }

    singly_ll_node_t *previous = NULL;
    singly_ll_node_t *current = ll->head;
    singly_ll_node_t *next = NULL;

    while (current != NULL) {
        next = current->next; // Store next node
        current->next = previous; // Reverse the link
        previous = current; // Move previous to current
        current = next; // Move to next node
    }
    ll->head = previous; // Update head to the new first node
}

void delete_singly_ll(ll_t *ll){
    if (ll == NULL) {
        return; // Nothing to delete
    }

    singly_ll_node_t *current = ll->head;
    while (current != NULL) {
        singly_ll_node_t *temp = current;
        current = current->next;
        free(temp);
    }
    free(ll); // Free the linked list structure itself
}

int singly_ll_delete_node(ll_t *ll, singly_ll_node_t *node){
    return singly_ll_remove_node(ll, node);
}

unsigned int singly_ll_delete_node_by_val(ll_t *ll, void *data, int size){
    return singly_ll_remove_node_by_val(ll, data, size);
}

singly_ll_node_t *singly_ll_get_node_by_data_ptr(ll_t *ll, void *data){
    if (ll == NULL || data == NULL) {
        return NULL; // Error: Invalid input
    }

    singly_ll_node_t *current = ll->head;
    while (current != NULL) {
        if (current->data == data) { // Compare pointers
            return current; // Node found
        }
        current = current->next;
    }
    return NULL; // Node not found
}